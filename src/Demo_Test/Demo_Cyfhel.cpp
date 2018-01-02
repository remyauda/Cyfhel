/*
#   Demo_Cyfhel
#   --------------------------------------------------------------------
#   Perform tests on all the main operations of Cyfhel. 
#   --------------------------------------------------------------------
#   Author: Remy AUDA
#   Date: 31/12/2017  
#   --------------------------------------------------------------------
#   License: GNU GPL v3
#
#   Demo_Cyfhel is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Demo_Cyfhel is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#   --------------------------------------------------------------------
*/

/* Import all the packages useful for the Demo.*/
#include <NTL/ZZ.h>
#include <NTL/BasicThreadPool.h>
#include <FHE.h>
#include <timing.h>
#include <EncryptedArray.h>
#include <NTL/lzz_pXFactoring.h>

#include <Cyfhel.h>
#include "Timer.h"

#include <cassert>
#include <cstdio>

/* The vector size of the plaintext that we will use for the demo.*/
#define VECTOR_SIZE 5

int main()
{
    //    string fileName = "DemoCyfhelEnv";
     
    // Values for the modulus p (size of p):
    //   - 2 (Binary)
    //   - 257 (Byte)
    //   - 65537 (Word)
    //   - 4294967311 (Long)

    //long p = 2;
    //long r = 32;
    //long d = 1;
    //long c = 2;
    //long sec = 128;
    //long w = 64;
    //long L = 40;

    // Display the title of the program.
    std::cout <<"" <<endl;
    std::cout <<"     ************CYFHEL DEMO************" <<endl;
    std::cout <<"" <<endl;

    // Begin the chrono.
    Timer timerDemo(true);
    timerDemo.start();

    // Define two vectors that we will use for the tests (+=, -=, *=, ...).
    // Initialization of v1.
    vector<long> v1;
    for(int i=0; i<VECTOR_SIZE; i++)
	{
		v1.push_back(i);  
	}

    // Initialization of v2.
    vector<long> v2(VECTOR_SIZE, 2);


    // Define variables to count the number of succes and the number of fail.
    int number_success = 0;
    int number_fail = 0;
    int number_unexpeted_error = 0;


    std::cout <<"******Definition of the vectors used during the tests******"<<endl;
    std::cout <<"v1: "<< v1<<endl;
    std::cout <<"v2: "<< v2<<endl;


    // Skip a line.
    std::cout <<"" <<endl;

    // Create object Cyfhel and enable print for all functions.
    // Cyfhel is an object that create keys for homeomorphism encryption with the parameter used in its constructor. 
    // If no parameter are provided, uses default values for the generation of the keys.
    // Cyfhel is an object that allow the user to encrypt and decrypt vectors in a homeomorphism way.
    std::cout <<"******Generation of the keys for encryption******"<<endl;
    Cyfhel cy(true);


    std::cout <<"******Homeomorphic encryption of the two vectors used during the tests******"<<endl;
    // Encrypted the two plaintexts to have two Cypher texts that are encrypted in an homeomorphic way with the key generated during the construction of object Cyfhel. 
    // These two Cypher txt will be use for the test on the homeomorphic operation (+=, -=, *=, ...).
    CyCtxt c1 = cy.encrypt(v1);
    CyCtxt c2 = cy.encrypt(v2);


    std::cout <<"Encryption of v1..."<<endl;
    std::cout <<"Encryption of v2..."<<endl;

    /* If the user has specified false for the second parameter of encrypt ie ispPtxt_vectResize, the plaintext vectors have been modified by the encrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the plaintext vectors. So, we have to resize the plaintext vectors to obtain the original ones.
       Note: we recommand to doesn't specify the second parameter of encrypt because the resize will then be done automatically.*/
    //v1.resize(VECTOR_SIZE);
    //v2.resize(VECTOR_SIZE);

    std::cout << "Encrypted v1: Encrypt(" << v1 << ")"<<endl;
    std::cout << "Encrypted v2: Encrypt(" << v2 << ")"<<endl;


    // Skip a line.
    std::cout <<"\n"<<endl;


    // Perform homeomorphic operations on the encrypted vectors.
    std::cout <<"******Test of the homeomorphic operations******"<<endl;

    // Skip a line.
    std::cout <<"\n"<<endl;

    // Perform homeomorphic addition with operator += .
    std::cout <<"*** Test of the homeomorphic addition with operator += ***"<<endl;
    std::cout <<"Encrypted v1: Encrypt("<< v1<< ")"<<endl;
    std::cout <<"Encrypted v2: Encrypt("<< v2<< ")"<<endl;
    std::cout <<"Performing Encrypt(v1) + Encrypt(v2)..."<<endl;
    // Sum of the two cypher text.
    c1 += c2;
    // Decrypt the result of the addition of the two encrypted vectors.
    vector<long> v_add_v1_v2 = cy.decrypt(c1);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    // TODO: The resize must be done in an override of the decrypt methods.
    v_add_v1_v2.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of the two encrypted vectors is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v1) + Encrypt(v2)) -> "<< v_add_v1_v2<<endl;
    // Perform the sum += on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v1Plusv2(VECTOR_SIZE, 0);
    std::transform (v1.begin(), v1.end(), v2.begin(), v1Plusv2.begin(), std::plus<long>());
    std::cout <<"v3 = v1 + v2 -> "<< v1Plusv2<<endl;
    // If Decrypt(Encrypt(v1) + Encrypt(v2)) equal to v1 + v2, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_add_v1_v2 == v1Plusv2){
   	std::cout <<"Homeomorphic operation add with operator += is a success: Decrypt(Encrypt(v1) + Encrypt(v2)) equal to v1 + v2."<<endl;
   	number_success += 1;
    }
    else if (v_add_v1_v2 != v1Plusv2){
   	std::cout <<"Homeomorphic operation add with operator += is a fail: Decrypt(Encrypt(v1) + Encrypt(v2)) not equal to v1 + v2."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_add_v1_v2 and v1Plusv2."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;



    // Perform homeomorphic substraction with operator -= .
    std::cout <<"*** Test of the homeomorphic substraction with operator -= ***"<<endl;
    std::cout <<"Encrypted v3: Encrypt("<< v_add_v1_v2<< ")"<<endl;
    std::cout <<"Encrypted v2: Encrypt("<< v2<< ")"<<endl;
    std::cout <<"Performing Encrypt(v3) - Encrypt(v2)..."<<endl;
    // Substraction of the two cypher text.
    c1 -= c2;
    // Decrypt the result of the substraction of the two encrypted vectors.
    vector<long> v_minus_v3_v2 = cy.decrypt(c1);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    // TODO: The resize must be done in an override of the decrypt methods.
    v_minus_v3_v2.resize(VECTOR_SIZE);
    // The user can then verify if the result of the substraction of the two encrypted vectors is the same that the substraction of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v3) - Encrypt(v2)) -> "<< v_minus_v3_v2<<endl;
    // Perform the substraction -= on the unencrypted vectors. 
    // std::minus substracts together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v3Minusv2(VECTOR_SIZE, 0);
    std::transform (v_add_v1_v2.begin(), v_add_v1_v2.end(), v2.begin(), v3Minusv2.begin(), std::minus<long>());
    std::cout <<"v4 = v3 - v2 -> "<< v3Minusv2<<endl;
    // If Decrypt(Encrypt(v3) - Encrypt(v2)) equal to v3 - v2, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_minus_v3_v2 == v3Minusv2){
   	std::cout <<"Homeomorphic operation substraction with operator -= is a success: Decrypt(Encrypt(v3) - Encrypt(v2)) equal to v3 - v2."<<endl;
   	number_success += 1;
    }
    else if (v_minus_v3_v2 != v3Minusv2){
   	std::cout <<"Homeomorphic operation substraction with operation -= is a fail: Decrypt(Encrypt(v3) - Encrypt(v2)) not equal to v3 - v2."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_minus_v3_v2 and v3Minusv2."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic multiplication with operator *= .
    std::cout <<"*** Test of the homeomorphic multiplication with operator *= ***"<<endl;
    std::cout <<"Encrypted v4: Encrypt("<< v_minus_v3_v2<< ")"<<endl;
    std::cout <<"Encrypted v2: Encrypt("<< v2<< ")"<<endl;
    std::cout <<"Performing Encrypt(v4) * Encrypt(v2)..."<<endl;
    // Multiplication of the two cypher text.
    c1 *= c2;
    // Decrypt the result of the multiplication of the two encrypted vectors.
    vector<long> v_mult_v4_v2  = cy.decrypt(c1);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    // TODO: The resize must be done in an override of the decrypt methods.
    v_mult_v4_v2.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of the two encrypted vectors is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v4) * Encrypt(v2)) -> "<< v_mult_v4_v2<<endl;
    // Perform the multiplication *= on the unencrypted vectors. 
    // std::multiplies multiplies together its two arguments elements by elements.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v4Multv2(VECTOR_SIZE, 0);
    std::transform (v_minus_v3_v2.begin(), v_minus_v3_v2.end(), v2.begin(), v4Multv2.begin(), std::multiplies<long>());
    std::cout <<"v5 = v4 * v2 -> "<< v4Multv2<<endl;
    // If Decrypt(Encrypt(v4) * Encrypt(v2)) equal to v4 * v2, The homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_mult_v4_v2 == v4Multv2){
   	std::cout <<"Homeomorphic operation mult with operator *= is a success: Decrypt(Encrypt(v4) * Encrypt(v2)) equal to v4 * v2."<<endl;
   	number_success += 1;
    }
    else if (v_mult_v4_v2 != v4Multv2){
   	std::cout <<"Homeomorphic operation mult with operator *= is a fail: Decrypt(Encrypt(v4) * Encrypt(v2)) not equal to v4 * v2."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_mult_v4_v2 and v4Multv2."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic addition with operator + .
    std::cout <<"*** Test of the homeomorphic addition with operator + ***"<<endl;
    std::cout <<"Encrypted v5: Encrypt("<< v_mult_v4_v2<< ")"<<endl;
    std::cout <<"Encrypted v2: Encrypt("<< v2<< ")"<<endl;
    std::cout <<"Performing Encrypt(v5) + Encrypt(v2)..."<<endl;
    // Sum of the two cypher text.
    CyCtxt cAdd1_2 = c1 + c2;
    // Decrypt the result of the addition of the two encrypted vectors.
    vector<long> v_add_v5_v2 = cy.decrypt(cAdd1_2);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    // TODO: The resize must be done in an override of the decrypt methods.
    v_add_v5_v2.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of the two encrypted vectors is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v5) + Encrypt(v2)) -> "<< v_add_v5_v2<<endl;
    // Perform the sum += on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v5Plusv2(VECTOR_SIZE, 0);
    std::transform (v_mult_v4_v2.begin(), v_mult_v4_v2.end(), v2.begin(), v5Plusv2.begin(), std::plus<long>());
    std::cout <<"v3 = v1 + v2 -> "<< v5Plusv2<<endl;
    // If Decrypt(Encrypt(v5) + Encrypt(v2)) equal to v5 + v2, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_add_v5_v2 == v5Plusv2){
   	std::cout <<"Homeomorphic operation add with operator + is a success: Decrypt(Encrypt(v1) + Encrypt(v2)) equal to v1 + v2."<<endl;
   	number_success += 1;
    }
    else if (v_add_v5_v2 != v5Plusv2){
   	std::cout <<"Homeomorphic operation add with operator + is a fail: Decrypt(Encrypt(v1) + Encrypt(v2)) not equal to v1 + v2."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_add_v5_v2 and v5Plusv2."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    

    // Scalar product
    //k1 = cy.encrypt(v1);
    //k2 = cy.encrypt(v2);
    //cy.scalarProd(k1, k2);
    //vector<long> vRes3 = cy.decrypt(k1);

    // Square
    //k1 = cy.encrypt(v1);
    //cy.square(k1);
    //vector<long> vRes4 = cy.decrypt(k1);


   // Skip a line.
std::cout <<"\n"<<endl;
std::cout <<"Number of successful tests: "<< number_success<<endl;
std::cout <<"Number of fail tests: "<< number_fail<<endl;
std::cout <<"Number of unexpected errors during the tests: "<< number_unexpeted_error<<endl;
std::cout <<"\n"<<endl;


    // Skip a line.
    std::cout <<"\n"<<endl;

    // Stop the chrono and display the execution time.
    timerDemo.stop();
    timerDemo.benchmarkInSeconds();
    timerDemo.benchmarkInHoursMinutesSecondsMillisecondes(true);

    // Store and retrieve environment.
    // cy.saveEnv(fileName);
    //std::cout << "Saved env with values: m=" << cy.getm_global_m() <<", p=" << cy.getm_global_p() << ", r=" << cy.getm_global_r() << endl;

    // Skip a line.
    std::cout <<"\n"<<endl;

    // Display the end of the program.
    std::cout <<"     ************END OF CYFHEL DEMO************" <<endl;

    // If success, return 0.
    return 0;
};

