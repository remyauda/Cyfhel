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

/* Define the max value of an element in the vector when the user choose the random vectors (value will be choosen between 0 and RANGEOFRANDOM).*/
#define RANGEOFRANDOM 10

/* Define if the polynome is monic or not during the tests.*/
#define isMonic 0




/*
	@name: vectMod
	@description: Take a vector v and a long m. each element of the vector will undergo the operation modulo m. The vector v is modified by this function.

	@param: The method vectMod takes two mandatory parameter: a vector and a long.
	-param1: the vector that will undergo the modulo operator for each of its elements.
    -param2: the value of the modulo.
*/
vector<long> vectMod(vector<long>& v, long const& modulo){
    for(unsigned long i=0; i<v.size(); i++)
	    {
            // The thing is that the % operator isn't the "modulo operator" but the "division remainder".
            // So if you do a % b, if a is negative, you will have problem has a will remain negative.
            // To perform a real modulo, you have to apply the formula: (a%b+b)%b.
		    v[i] = (v[i] % modulo + modulo) % modulo; 
	    }
// Return the modified vector which has undergo the modulo operation on each of its elements.
return v;
}


void initVectorFixe(vector<long>& v1, vector<long>& v2, vector<long>& v12, vector<long>& v22){
	// Initialization of v1.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v1.push_back(i);  
	}

	// Initialization of v2.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v2.push_back(2);  
	}

	// Initialization of v12.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v12.push_back(i+2);  
	}

	// Initialization of v22.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v22.push_back(2);  
	}
}

void initVectorRandom(vector<long>& v1, vector<long>& v2, vector<long>& v12, vector<long>& v22){
	// Initialization of v1.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v1.push_back(RandomBnd(RANGEOFRANDOM+1));  
	}

	// Initialization of v2.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v2.push_back(RandomBnd(RANGEOFRANDOM+1));
	}

	// Initialization of v12.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v12.push_back(RandomBnd(RANGEOFRANDOM+1)+2);  
	}

	// Initialization of v22.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v22.push_back(RandomBnd(RANGEOFRANDOM+1));  
	}
}

int main(int argc, char *argv[])
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

    // Define variables to count the number of succes and the number of fail.
    int number_success = 0;
    int number_fail = 0;
    int number_unexpeted_error = 0;

    // Define two vectors that we will use for the tests (+=, -=, *=, ...).
    vector<long> v1; // Initialization of v1.
    vector<long> v2; // Initialization of v2.

    // Define two vectors that we will use for the tests (+, -, *, ...).
    vector<long> v12; // Initialization of v12.
    vector<long> v22; // Initialization of v22.

	// If the user gives some options at execution time.
	if(argc > 1)
	{
		// If the user gives the option -f or --fixe at execution time, we initialize vector by default instead of random.
		if(!strcmp(argv[1], "-f") || !strcmp(argv[1], "--fixe"))
		{
			initVectorFixe(v1, v2, v12, v22);
		}
		// If the user gives the option -r or --random at execution time, we initialize vector in random.
		else if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "--random"))
		{
			initVectorRandom(v1, v2, v12, v22);
		}
		// If the user gives other options at execution time that the arguments expected.
		else
		{
			std::cout <<"ERROR: cannot determine the use or not of random vectors. Use the fixe vectors instead."<<endl;

			initVectorFixe(v1, v2, v12, v22);
		}
	}
	//Otherwise if the user don't put option in compilation (i.e: argc <= 1), we initialize vector in random.
	else
	{
		initVectorRandom(v1, v2, v12, v22);
	}


    std::cout <<"******Definition of the vectors used during the tests +=, -=, *=, etc...******"<<endl;
    std::cout <<"v1: "<< v1<<endl;
    std::cout <<"v2: "<< v2<<endl;

    // Skip a line.
    std::cout <<"" <<endl;

    std::cout <<"******Definition of the vectors used during the tests +, -, *, etc...******"<<endl;
    std::cout <<"v12: "<< v12<<endl;
    std::cout <<"v22: "<< v22<<endl;


    // Skip a line.
    std::cout <<"" <<endl;

    // Create object Cyfhel and enable print for all functions.
    // Cyfhel is an object that create keys for homeomorphism encryption with the parameter used in its constructor. 
    // If no parameter are provided, uses default values for the generation of the keys.
    // Cyfhel is an object that allow the user to encrypt and decrypt vectors in a homeomorphism way.
    std::cout <<"******Generation of the keys for encryption******"<<endl;
	// Use this initialization for strong encryption. However, the computation time will be greater (takes several minutes at least).
	//Cyfhel cy(true);
	// Use this initialization to go fast through the demo the computation time will be very low (takes several second only). However, the encryption will be very weak.
    Cyfhel cy(true, 257, 1, 2, 1, 80, 64, 10); // Comment this line if you want to test the copy constructor.

    // Uncomment the following lines if you want to test the copy constructor.
    /*Cyfhel cy_0(true);
    // Use the copy constructor to construct a Cyfhel object called cy.
    Cyfhel cy(cy_0);*/

    // Test of Cyfhel getp2r() method.
    std::cout <<"Value of p^r -> "<< cy.getp2r() <<endl;

    // Skip a line.
    std::cout <<"\n"<<endl;


    std::cout <<"******Homeomorphic encryption of the two vectors used during the tests +=, -=, *=, etc...******"<<endl;
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


    std::cout <<"******Homeomorphic encryption of the two vectors used during the tests +, -, *, etc...******"<<endl;
    // Encrypted the two plaintexts to have two Cypher texts that are encrypted in an homeomorphic way with the key generated during the construction of object Cyfhel. 
    // These two Cypher txt will be use for the test on the homeomorphic operation (+, -, *, ...).
    CyCtxt c12 = cy.encrypt(v12);
    CyCtxt c22 = cy.encrypt(v22);

    std::cout <<"Encryption of v12..."<<endl;
    std::cout <<"Encryption of v22..."<<endl;

    /* If the user has specified false for the second parameter of encrypt ie ispPtxt_vectResize, the plaintext vectors have been modified by the encrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the plaintext vectors. So, we have to resize the plaintext vectors to obtain the original ones.
       Note: we recommand to doesn't specify the second parameter of encrypt because the resize will then be done automatically.*/
    //v12.resize(VECTOR_SIZE);
    //v22.resize(VECTOR_SIZE);

    std::cout << "Encrypted v12: Encrypt(" << v12 << ")"<<endl;
    std::cout << "Encrypted v22: Encrypt(" << v22 << ")"<<endl;


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
    std::cout <<"Performing Encrypt(v1) += Encrypt(v2)..."<<endl;
    // Sum of the two cypher text.
    c1 += c2;
    // Decrypt the result of the addition of the two encrypted vectors.
    vector<long> v_add_v1_v2 = cy.decrypt(c1);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v1_v2.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of the two encrypted vectors is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v1) += Encrypt(v2)) -> "<< v_add_v1_v2<<endl;
    // Perform the sum += on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v1Plusv2(VECTOR_SIZE, 0);
    std::transform (v1.begin(), v1.end(), v2.begin(), v1Plusv2.begin(), std::plus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v1Plusv2.
    vectMod(v1Plusv2, cy.getp2r());
    std::cout <<"v3 = v1 + v2 mod("<< cy.getp2r() <<") -> "<< v1Plusv2<<endl;
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
    //v_minus_v3_v2.resize(VECTOR_SIZE);
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
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v3Minusv2.
    vectMod(v3Minusv2, cy.getp2r());
    std::cout <<"v4 = v3 - v2 mod("<< cy.getp2r() <<") -> "<< v3Minusv2<<endl;
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
    //v_mult_v4_v2.resize(VECTOR_SIZE);
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
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v4Multv2.
    vectMod(v4Multv2, cy.getp2r());
    std::cout <<"v5 = v4 * v2 mod("<< cy.getp2r() <<") -> "<< v4Multv2<<endl;
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

    // Perform homeomorphic operation returnNegate (doesn't test on unencrypted vector because the negate operation are done modulo ...).
    std::cout <<"*** Test of the homeomorphic operation returnNegate (doesn't test on unencrypted vector because the negate operation are done modulo ...)***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing -Encrypt(v12)..."<<endl;
    // Negate the cypher text.
    CyCtxt cNegate12 = c12.returnNegate();
    // Decrypt the result of the negate of the encrypted vector.
    vector<long> v_negate_v12 = cy.decrypt(cNegate12);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the negate of the encrypted vector is the same that the negate of the vector without encryption.
    std::cout <<"Decrypt(-Encrypt(v12)) -> "<< v_negate_v12<<endl;


   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic addition with operator + .
    std::cout <<"*** Test of the homeomorphic addition with operator + ***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Encrypted v22: Encrypt("<< v22<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) + Encrypt(v22)..."<<endl;
    // Sum of the two cypher text.
    CyCtxt cAdd12_22 = c12 + c22;
    // Decrypt the result of the addition of the two encrypted vectors.
    vector<long> v_add_v12_v22 = cy.decrypt(cAdd12_22);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of the two encrypted vectors is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) + Encrypt(v22)) -> "<< v_add_v12_v22<<endl;
    // Perform the sum + on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v12Plusv22(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), v22.begin(), v12Plusv22.begin(), std::plus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Plusv22.
    vectMod(v12Plusv22, cy.getp2r());
    std::cout <<"v32 = v12 + v22 mod("<< cy.getp2r() <<") -> "<< v12Plusv22<<endl;
    // If Decrypt(Encrypt(v12) + Encrypt(v22)) equal to v12 + v22, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_add_v12_v22 == v12Plusv22){
   	std::cout <<"Homeomorphic operation add with operator + is a success: Decrypt(Encrypt(v12) + Encrypt(v22)) equal to v12 + v22."<<endl;
   	number_success += 1;
    }
    else if (v_add_v12_v22 != v12Plusv22){
   	std::cout <<"Homeomorphic operation add with operator + is a fail: Decrypt(Encrypt(v12) + Encrypt(v22)) not equal to v12 + v22."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_add_v12_v22 and v12Plusv22."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic addition with operator + between a long and a CyCtxt.
    std::cout <<"*** Test of the homeomorphic addition with operator + between a long and a CyCtxt.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing 2 + Encrypt(v12)..."<<endl;
    // Sum of a long and a CyCtxt.
    CyCtxt cAdd2_12 = 2 + c12;
    // Decrypt the result of the addition of a long and a CyCtxt.
    vector<long> v_add_2_v12 = cy.decrypt(cAdd2_12);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of a long and a CyCtxt is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(2 + Encrypt(v12)) -> "<< v_add_2_v12<<endl;
    // Perform the sum + on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> vect_2(VECTOR_SIZE, 2);
    vector<long> twoPlusv12(VECTOR_SIZE, 0);
    std::transform (vect_2.begin(), vect_2.end(), v12.begin(), twoPlusv12.begin(), std::plus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector twoPlusv12.
    vectMod(twoPlusv12, cy.getp2r());
    std::cout <<"vAdd_2 = vect(2) + v12 mod("<< cy.getp2r() <<") -> "<< twoPlusv12<<endl;
    // If Decrypt(2 + Encrypt(v12)) equal to vect(2) + v12, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_add_2_v12 == twoPlusv12){
   	std::cout <<"Homeomorphic operation add with operator + between a long and a CyCtxt is a success: Decrypt(2 + Encrypt(v12)) equal to vect(2) + v12."<<endl;
   	number_success += 1;
    }
    else if (v_add_2_v12 != twoPlusv12){
   	std::cout <<"Homeomorphic operation add with operator + between a long and a CyCtxt is a fail: Decrypt(2 + Encrypt(v12)) not equal to vect(2) + v12."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_add_2_v12 and twoPlusv12."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


// Perform homeomorphic addition with operator + between a CyCtxt and a long.
    std::cout <<"*** Test of the homeomorphic addition with operator + between a CyCtxt and a long.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) + 2..."<<endl;
    // Sum of a CyCtxt and a long.
    CyCtxt cAdd12_2 = c12 + 2;
    // Decrypt the result of the addition of a CyCtxt and a long.
    vector<long> v_add_v12_2 = cy.decrypt(cAdd12_2);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of a CyCtxt and a long is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) + 2) -> "<< v_add_v12_2<<endl;
    // Perform the sum + on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
	vector<long> vectOf2(VECTOR_SIZE, 2);
    vector<long> v12Plustwo(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), vectOf2.begin(), v12Plustwo.begin(), std::plus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Plustwo.
    vectMod(v12Plustwo, cy.getp2r());
    std::cout <<"vAdd_2 = v12 + vect(2) mod("<< cy.getp2r() <<") -> "<< v12Plustwo<<endl;
    // If Decrypt(Encrypt(v12) + 2) equal to v12 + vect(2), the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_add_v12_2 == v12Plustwo){
   	std::cout <<"Homeomorphic operation add with operator + between a CyCtxt and a long is a success: Decrypt(Encrypt(v12) + 2) equal to v12 + vect(2)."<<endl;
   	number_success += 1;
    }
    else if (v_add_v12_2 != v12Plustwo){
   	std::cout <<"Homeomorphic operation add with operator + between a CyCtxt and a long is a fail: Decrypt(Encrypt(v12) + 2) not equal to v12 + vect(2)."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_add_v12_2 and v12Plustwo."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic addition with operator - .
    std::cout <<"*** Test of the homeomorphic addition with operator - ***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Encrypted v22: Encrypt("<< v22<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) - Encrypt(v22)..."<<endl;
    // Sum of the two cypher text.
    CyCtxt cMinus12_22 = c12 - c22;
    // Decrypt the result of the addition of the two encrypted vectors.
    vector<long> v_minus_v12_v22 = cy.decrypt(cMinus12_22);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_minus_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the addition of the two encrypted vectors is the same that the addition of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) - Encrypt(v22)) -> "<< v_minus_v12_v22<<endl;
    // Perform the substract - on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v12Minusv22(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), v22.begin(), v12Minusv22.begin(), std::minus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Minusv22.
    vectMod(v12Minusv22, cy.getp2r());
    std::cout <<"v42 = v12 - v22 mod("<< cy.getp2r() <<") -> "<< v12Minusv22<<endl;
    // If Decrypt(Encrypt(v12) - Encrypt(v22)) equal to v12 - v22, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_minus_v12_v22 == v12Minusv22){
   	std::cout <<"Homeomorphic operation minus with operator - is a success: Decrypt(Encrypt(v12) - Encrypt(v22)) equal to v12 - v22."<<endl;
   	number_success += 1;
    }
    else if (v_minus_v12_v22 != v12Minusv22){
   	std::cout <<"Homeomorphic operation minus with operator - is a fail: Decrypt(Encrypt(v12) - Encrypt(v22)) not equal to v12 - v22."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_minus_v12_v22 and v12Minusv22."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic substraction with operator - between a long and a CyCtxt.
    std::cout <<"*** Test of the homeomorphic substraction with operator - between a long and a CyCtxt.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing 6 - Encrypt(v12)..."<<endl;
    // substraction of a long and a CyCtxt.
    CyCtxt cMinus2_12 = 6 - c12;
    // Decrypt the result of the substraction of a long and a CyCtxt.
    vector<long> v_minus_2_v12 = cy.decrypt(cMinus2_12);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the substraction of a long and a CyCtxt is the same that the substraction of the two vectors without encryption.
    std::cout <<"Decrypt(6 - Encrypt(v12)) -> "<< v_minus_2_v12<<endl;
    // Perform the substraction - on the unencrypted vectors. 
    // std::plus minus together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> vect_2_forMinus(VECTOR_SIZE, 6);
    vector<long> twoMinusv12(VECTOR_SIZE, 0);
    std::transform (vect_2_forMinus.begin(), vect_2_forMinus.end(), v12.begin(), twoMinusv12.begin(), std::minus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector twoMinusv12.
    vectMod(twoMinusv12, cy.getp2r());
    std::cout <<"vMinus_2 = vect(6) - v12 mod("<< cy.getp2r() <<") -> "<< twoMinusv12<<endl;
    // If Decrypt(6 - Encrypt(v12)) equal to vect(6) - v12, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_minus_2_v12 == twoMinusv12){
   	std::cout <<"Homeomorphic operation substraction with operator - between a long and a CyCtxt is a success: Decrypt(6 - Encrypt(v12)) equal to vect(6) - v12."<<endl;
   	number_success += 1;
    }
    else if (v_minus_2_v12 != twoMinusv12){
   	std::cout <<"Homeomorphic operation substraction with operator - between a long and a CyCtxt is a fail: Decrypt(6 - Encrypt(v12)) not equal to vect(6) - v12."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_minus_2_v12 and twoMinusv12."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


// Perform homeomorphic substraction with operator - between a CyCtxt and a long.
    std::cout <<"*** Test of the homeomorphic substraction with operator - between a CyCtxt and a long.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) - 2..."<<endl;
    // Substraction of a CyCtxt and a long.
    CyCtxt cMinus12_2 = c12 - 2;
    // Decrypt the result of the substraction of a CyCtxt and a long.
    vector<long> v_minus_v12_2 = cy.decrypt(cMinus12_2);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the substraction of a CyCtxt and a long is the same that the substraction of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) - 2) -> "<< v_minus_v12_2<<endl;
    // Perform the substraction - on the unencrypted vectors. 
    // std::plus minus together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> vectOf2_1(VECTOR_SIZE, 2);
    vector<long> v12Minustwo_1(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), vectOf2_1.begin(), v12Minustwo_1.begin(), std::minus<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector vectOf2_1.
    vectMod(vectOf2_1, cy.getp2r());
    std::cout <<"vMinus_2 = v12 - vect(2) mod("<< cy.getp2r() <<") -> "<< v12Minustwo_1<<endl;
    // If Decrypt(Encrypt(v12) - 2) equal to v12 - vect(2), the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_minus_v12_2 == v12Minustwo_1){
   	std::cout <<"Homeomorphic operation substraction with operator - between a CyCtxt and a long is a success: Decrypt(Encrypt(v12) - 2) equal to v12 - vect(2)."<<endl;
   	number_success += 1;
    }
    else if (v_minus_v12_2 != v12Minustwo_1){
   	std::cout <<"Homeomorphic operation substraction with operator - between a CyCtxt and a long is a fail: Decrypt(Encrypt(v12) - 2) not equal to v12 - vect(2)."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_minus_v12_2 and v12Minustwo_1."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic multiplication with operator * .
    std::cout <<"*** Test of the homeomorphic multiplication with operator * ***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Encrypted v22: Encrypt("<< v22<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) * Encrypt(v22)..."<<endl;
    // Multiplication of the two cypher text.
    CyCtxt cMult12_22 = c12 * c22;
    // Decrypt the result of the multiplication of the two encrypted vectors.
    vector<long> v_mult_v12_v22 = cy.decrypt(cMult12_22);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_mult_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the multiplication of the two encrypted vectors is the same that the multiplication of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) * Encrypt(v22)) -> "<< v_mult_v12_v22<<endl;
    // Perform the multiplication * on the unencrypted vectors. 
    // std::plus adds together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v12Multv22(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), v22.begin(), v12Multv22.begin(), std::multiplies<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Multv22.
    vectMod(v12Multv22, cy.getp2r());
    std::cout <<"v52 = v12 * v22 mod("<< cy.getp2r() <<") -> "<< v12Multv22<<endl;
    // If Decrypt(Encrypt(v12) * Encrypt(v22)) equal to v12 * v22, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_mult_v12_v22 == v12Multv22){
   	std::cout <<"Homeomorphic operation multiplication with operator * is a success: Decrypt(Encrypt(v12) * Encrypt(v22)) equal to v12 * v22."<<endl;
   	number_success += 1;
    }
    else if (v_mult_v12_v22 != v12Multv22){
   	std::cout <<"Homeomorphic operation multiplication with operator * is a fail: Decrypt(Encrypt(v12) * Encrypt(v22)) not equal to v12 * v22."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_mult_v12_v22 and v12Multv22."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic multiplication with operator * between a long and a CyCtxt.
    std::cout <<"*** Test of the homeomorphic multiplication with operator * between a long and a CyCtxt.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing 2 * Encrypt(v12)..."<<endl;
    // Multiplication of a long and a CyCtxt.
    CyCtxt cMult2_12 = 2 * c12;
    // Decrypt the result of the multiplication of a long and a CyCtxt.
    vector<long> v_mult_2_v12 = cy.decrypt(cMult2_12);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the multiplication of a long and a CyCtxt is the same that the multiplication of the two vectors without encryption.
    std::cout <<"Decrypt(2 * Encrypt(v12)) -> "<< v_mult_2_v12<<endl;
    // Perform the multiplication * on the unencrypted vectors. 
    // std::plus multiplies together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> vect_2_forMult1(VECTOR_SIZE, 2);
    vector<long> twoMultv12(VECTOR_SIZE, 0);
    std::transform (vect_2_forMult1.begin(), vect_2_forMult1.end(), v12.begin(), twoMultv12.begin(), std::multiplies<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector twoMultv12.
    vectMod(twoMultv12, cy.getp2r());
    std::cout <<"vMult_2 = vect(2) * v12 mod("<< cy.getp2r() <<") -> "<< twoMultv12<<endl;
    // If Decrypt(2 * Encrypt(v12)) equal to vect(2) * v12, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_mult_2_v12 == twoMultv12){
   	std::cout <<"Homeomorphic operation multiplication with operator * between a long and a CyCtxt is a success: Decrypt(2 * Encrypt(v12)) equal to vect(2) * v12."<<endl;
   	number_success += 1;
    }
    else if (v_mult_2_v12 != twoMultv12){
   	std::cout <<"Homeomorphic operation multiplication with operator * between a long and a CyCtxt is a fail: Decrypt(2 * Encrypt(v12)) not equal to vect(2) * v12."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_mult_2_v12 and twoMultv12."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


   // Perform homeomorphic multiplication with operator * between a CyCtxt and a long.
    std::cout <<"*** Test of the homeomorphic multiplication with operator * between a CyCtxt and a long.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) * 2..."<<endl;
    // Multiplication of a CyCtxt and a long.
    CyCtxt cMult12_2 = c12 * 2;
    // Decrypt the result of the multiplication of a CyCtxt and a long.
    vector<long> v_mult_v12_2 = cy.decrypt(cMult12_2);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_add_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the multiplication of a CyCtxt and a long is the same that the multiplication of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) * 2) -> "<< v_mult_v12_2<<endl;
    // Perform the multiplication * on the unencrypted vectors. 
    // std::multiplies multiplies together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> vect_2_forMult2(VECTOR_SIZE, 2);
    vector<long> v12Multtwo(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), vect_2_forMult2.begin(), v12Multtwo.begin(), std::multiplies<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Multtwo.
    vectMod(v12Multtwo, cy.getp2r());
    std::cout <<"vMult_2 = v12 * vect(2) mod("<< cy.getp2r() <<") -> "<< v12Multtwo<<endl;
    // If Decrypt(Encrypt(v12) * 2) equal to v12 * vect(2), the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_mult_v12_2 == v12Multtwo){
   	std::cout <<"Homeomorphic operation multiplication with operator * between a CyCtxt and a long is a success: Decrypt(Encrypt(v12) * 2) equal to v12 * vect(2)."<<endl;
   	number_success += 1;
    }
    else if (v_mult_v12_2 != v12Multtwo){
   	std::cout <<"Homeomorphic operation multiplication with operator * between a CyCtxt and a long is a fail: Decrypt(Encrypt(v12) * 2) not equal to v12 * vect(2)."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_mult_v12_2 and v12Multtwo."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;

    // Perform homeomorphic scalar product with returnScalarProd method.
    std::cout <<"*** Test of the homeomorphic scalar product with returnScalarProd method.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Encrypted v22: Encrypt("<< v22<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12).returnScalarProd(Encrypt(v22))..."<<endl;
    // Scalar product with returnScalarProd method of the two cypher text.
    CyCtxt cScalarProd12_22 = c12.returnScalarProd(c22);
    // Decrypt the result of the scalar product with returnScalarProd method of the two encrypted vectors.
    vector<long> v_scalarProd_v12_v22 = cy.decrypt(cScalarProd12_22);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_mult_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the scalar product with returnScalarProd method of the two encrypted vectors is the same that the scalar product with returnScalarProd method of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12).returnScalarProd(Encrypt(v22)) -> "<< v_scalarProd_v12_v22<<endl;
    // Perform the scalar product with returnScalarProd method on the unencrypted vectors. 
    // Use of std::inner_product preform scalar product together its two arguments.
    vector<long> v12ScalarProdv22(VECTOR_SIZE, std::inner_product(std::begin(v12), std::end(v12), std::begin(v22), 0.0));
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12ScalarProdv22.
    vectMod(v12ScalarProdv22, cy.getp2r());
    std::cout <<"vScalarProd = v12.returnScalarProd(v22) mod("<< cy.getp2r() <<") -> "<< v12ScalarProdv22<<endl;
    // If Decrypt(Encrypt(v12).returnScalarProd(Encrypt(v22))) equal to v12.returnScalarProd(v22), the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_scalarProd_v12_v22 == v12ScalarProdv22){
   	std::cout <<"Homeomorphic operation scalar product with returnScalarProd method is a success: Decrypt(Encrypt(v12).returnScalarProd(Encrypt(v22)) equal to v12.returnScalarProd(v22)."<<endl;
   	number_success += 1;
    }
    else if (v_scalarProd_v12_v22 != v12ScalarProdv22){
   	std::cout <<"Homeomorphic operation scalar product with returnScalarProd method is a fail: Decrypt(Encrypt(v12).returnScalarProd(Encrypt(v22)) not equal to v12.returnScalarProd(v22)."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_scalarProd_v12_v22 and v12ScalarProdv22."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;

    // Perform homeomorphic scalar product with operator %.
    std::cout <<"*** Test of the homeomorphic scalar product with operator %.***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Encrypted v22: Encrypt("<< v22<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12) % Encrypt(v22)..."<<endl;
    // Scalar product with operator % of the two cypher text.
    CyCtxt cScalarProd12_22_op = c12 % c22;
    // Decrypt the result of the scalar product with operator % of the two encrypted vectors.
    vector<long> v_scalarProd_v12_v22_op = cy.decrypt(cScalarProd12_22_op);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_mult_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the scalar product with operator % of the two encrypted vectors is the same that the scalar product with operator % of the two vectors without encryption.
    std::cout <<"Decrypt(Encrypt(v12) % Encrypt(v22) -> "<< v_scalarProd_v12_v22_op<<endl;
    // Perform the scalar product on the unencrypted vectors. 
    // Use of std::inner_product preform scalar product together its two arguments.
    vector<long> v12ScalarProdv22_op(VECTOR_SIZE, std::inner_product(std::begin(v12), std::end(v12), std::begin(v22), 0.0));
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12ScalarProdv22_op.
    vectMod(v12ScalarProdv22_op, cy.getp2r());
    std::cout <<"vScalarProd_operator = v12 % v22 mod("<< cy.getp2r() <<") -> "<< v12ScalarProdv22_op<<endl;
    // If Decrypt(Encrypt(v12) % Encrypt(v22)) equal to v12 % v22, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_scalarProd_v12_v22_op == v12ScalarProdv22_op){
   	std::cout <<"Homeomorphic operation scalar product with operator % is a success: Decrypt(Encrypt(v12) % Encrypt(v22) equal to v12 % v22."<<endl;
   	number_success += 1;
    }
    else if (v_scalarProd_v12_v22_op != v12ScalarProdv22_op){
   	std::cout <<"Homeomorphic operation scalar product with operator % is a fail: Decrypt(Encrypt(v12) % Encrypt(v22) not equal to v12 % v22."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_scalarProd_v12_v22_op and v12ScalarProdv22_op."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;


    // Perform homeomorphic returnSquare.
    std::cout <<"*** Test of the homeomorphic returnSquare with operator returnSquare ***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12)²..."<<endl;
    // Square of the first cypher text.
    CyCtxt cSquare12_22 = c12.returnSquare();
    // Decrypt the result of the square of the encrypted vectors.
    vector<long> v_square_v12_v22 = cy.decrypt(cSquare12_22);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_square_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the square of the encrypted vector is the same that the square of the vector without encryption.
    std::cout <<"Decrypt(Encrypt(v12)²) -> "<< v_square_v12_v22<<endl;
    // Perform the square ² on the unencrypted vector. 
    // std::multiplies multiplies together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v12Squarev22(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), v12.begin(), v12Squarev22.begin(), std::multiplies<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Squarev22.
    vectMod(v12Squarev22, cy.getp2r());
    std::cout <<"v62 = v12² mod("<< cy.getp2r() <<") -> "<< v12Squarev22<<endl;
    // If Decrypt(Encrypt(v12)²) equal to v12², the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_square_v12_v22 == v12Squarev22){
   	std::cout <<"Homeomorphic operation square with operator ² is a success: Decrypt(Encrypt(v12)²) equal to v12²."<<endl;
   	number_success += 1;
    }
    else if (v_square_v12_v22 != v12Squarev22){
   	std::cout <<"Homeomorphic operation square with operator ² is a fail: Decrypt(Encrypt(v12)²) not equal to v12²."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_square_v12_v22 and v12Squarev22."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;

    // Perform homeomorphic returnCube.
    std::cout <<"*** Test of the returnCube returnSquare with operator returnCube ***"<<endl;
    std::cout <<"Encrypted v12: Encrypt("<< v12<< ")"<<endl;
    std::cout <<"Performing Encrypt(v12)³..."<<endl;
    // Cube of the first cypher text.
    CyCtxt cCube12_22 = c12.returnCube();
    // Decrypt the result of the cube of the encrypted vectors.
    vector<long> v_cube_v12_v22 = cy.decrypt(cCube12_22);
    /* If the user has specified false for the second parameter of decrypt ie isDecryptedPtxt_vectResize, the decrypted plaintext vectors have been modified by the
       decrypt method.
       Indeed, (m_numberOfSlots - vector_size) zeros has been added to the decrypted plaintext vectors. So, we have to resize the decrypted plaintext vectors to obtain
       the original ones.
       Note: we recommand to doesn't specify the second parameter of decrypt because the resize will then be done automatically.*/
    //v_cube_v12_v22.resize(VECTOR_SIZE);
    // The user can then verify if the result of the cube of the encrypted vector is the same that the cube of the vector without encryption.
    std::cout <<"Decrypt(Encrypt(v12)³) -> "<< v_cube_v12_v22<<endl;
    // Perform the cube ³ on the unencrypted vector. 
    // std::multiplies multiplies together its two arguments.
    /* This form of std::transform takes 5 arguments: Two first are input iterators to the initial and final positions of the first sequence.
       The third is an input iterator to the initial position of the second range.
       The fourth is an output iterator of the initial position of the range where the operation results are stored.
       The last argument is a binary function that accepts two elements as argument (one of each of the two sequences),
       and returns some result value convertible to the type pointed by OutputIterator.*/
    vector<long> v12Cubev22_0(VECTOR_SIZE, 0);
    vector<long> v12Cubev22(VECTOR_SIZE, 0);
    std::transform (v12.begin(), v12.end(), v12.begin(), v12Cubev22_0.begin(), std::multiplies<long>());
    std::transform (v12Cubev22_0.begin(), v12Cubev22_0.end(), v12.begin(), v12Cubev22.begin(), std::multiplies<long>());
    // All the operations are done modulo p^r. So we apply the operation mod(p^r) on each elements of the vector v12Cubev22.
    vectMod(v12Cubev22, cy.getp2r());
    std::cout <<"v72 = v12³ mod("<< cy.getp2r() <<") -> "<< v12Cubev22<<endl;
    // If Decrypt(Encrypt(v12)³) equal to v12³, the homeomorphic operation works and so it is a success. Else, it is a fail.
    if (v_cube_v12_v22 == v12Cubev22){
   	std::cout <<"Homeomorphic operation cube with operator ³ is a success: Decrypt(Encrypt(v12)³) equal to v12³."<<endl;
   	number_success += 1;
    }
    else if (v_cube_v12_v22 != v12Cubev22){
   	std::cout <<"Homeomorphic operation cube with operator ³ is a fail: Decrypt(Encrypt(v12)³) not equal to v12³."<<endl;
   	number_fail += 1;
    }
    else{
   	std::cout <<"Error: unexpected result during the comparison of v_cube_v12_v22 and v12Cubev22."<<endl;
	number_unexpeted_error += 1;
    }

   // Skip a line.
   std::cout <<"\n"<<endl;

   // Powers.
   /*std::cout <<"c12 -> "<< cy.decrypt(c12)<<endl;
   DynamicCtxtPowers dp(c12, 2);
   Ctxt cypower_4 = dp.getPower(2);
   vector<long> v12_power_4 = cy.decrypt(cypower_4);
   std::cout <<"Decrypt(Encrypt(v12)⁴) -> "<< v12_power_4<<endl;*/


   std::cout <<"******Homomorphic Polynomial Evaluation******"<<endl;
   /*In this test, we choose n points (choosen randomly or choosen by the user) and put them in a vector x = [x1, x2, ..., xn]. 
     Then, we define a polynome Poly of degree d and the coefficients of the polynome are choosen either randomly or are choosen by the user. 
     Then, we cyphered the vector of points x to obtain cx = [c1, c2, ..., cn].
     Then, we evaluate the n cyphered points with the polynome we have defined previously ie P(cx) = [P(c1), P(c2), ..., P(cn)].
     Then, we decrypt the previous vector: Decrypt(P(cx)) = Decrypt([P(c1), P(c2), ..., P(cn)]) = [P(x1), P(x2), ..., P(xn)].
     Then, we perform the polynomiale evaluation on the plaintext vector x: P(x) = [P(x1), P(x2), ..., P(xn)].
     Finally, we verify if Decrypt(P(cx)) = P(x).
   */


   const long p2r = cy.getp2r(); // Value of p power r.

   // Variables useful to define the vector points evaluation in which we will evaluate the polynome.
   const long sizeVectorPtsEval = 10; // We evaluate the polynome in ten points.
   const bool isVectorPtsEvalRandom = true; // Is the vector of evaluation points random?
   vector<long> x; // Create an empty vector that will contain the evaluation points.

   // Variables useful to define the polynome.
   const long d = 3; // Degree of the polynome.
   const bool isPolynomeRandom = true; // Is the polynome coefficients are random?


   // ***Initialization of the vector that contains the evaluations points.***

   // If we choose random points for evaluation...
   if(isVectorPtsEvalRandom){
       // ***evaluate at random points (at least one co-prime with p)***
       cy.random(x); // Initialize the vector with m_encryptedArray.size() random values.
       // (Facultatif) Replace the first random element of vector x with a value which is co-prime with p ie gcd(x[0], p)=1. 
       // This step is not mandatory but we want to test a point with this property.
       while (GCD(x[0], cy.getm_global_p())!=1) { x[0] = RandomBnd(p2r); }
       // As x has m_encryptedArray.size() random values, we resize the vector of evaluation points to has only a size of sizeVectorPtsEval.
       // For exemple, if m_encryptedArray.size()=1000 and sizeVectorPtsEval=10, we have 1000 random value points to evaluate the polynome but we only want to evaluate
       // 10 points. So we resize the vector so that the evaluation points vector only contains the first 10 random value points to evalauate the polynome.  
       x.resize(sizeVectorPtsEval);
       std::cout <<"vector X of random points -> "<< x <<endl;
   }
   // Else if we choose fixed points to test the evalauation...
   else if(!isVectorPtsEvalRandom){
       // Initialize the evaluation points vectors with values like: [0, ..., sizeVectorPtsEval-1].
       for(int i=0; i<sizeVectorPtsEval; i++)
	   {
		    x.push_back(i);  
	   }
       std::cout <<"vector X of fixed points -> "<< x <<endl;
   }
   // In case of error, we initialize with fixed points.
   else{
       std::cout <<"Error: unable to evaluate isVectorPtsEvalRandom. Initalize evaluation points vector with fixed values."<<endl;
       // Initialize the evaluation points vectors with values like: [0, ..., sizeVectorPtsEval-1].
       for(int i=0; i<sizeVectorPtsEval; i++)
	   {
		    x.push_back(i);  
	   }
       std::cout <<"vector X of fixed points -> "<< x <<endl;
   }
   


   // ***Definition of the polynome.***
   ZZX poly;
   if(isPolynomeRandom){
      for (long i=d; i>=0; i--){
          SetCoeff(poly, i, RandomBnd(p2r)); // coefficients are random.
      }
      if (isMonic) SetCoeff(poly, d);    // set top coefficient to 1.

      // Print the polynome.
      std::cout <<"Polynome P(X) with random coefficients of degree " << d <<" -> ";
      for (int i=deg(poly); i>0; i--){
      std::cout << poly[i] <<"X^"<<i<<" + ";
      }
      std::cout << poly[0] <<endl;
   }
   else if(!isPolynomeRandom){
       vector<long> coeff(d+1, 2);
       std::cout <<coeff<<endl;
       for (long i=d; i>=0; i--){
          SetCoeff(poly, i, coeff[i]); // coefficients are fixed.
      }
      if (isMonic) SetCoeff(poly, d);    // set top coefficient to 1

      // Print the polynome.
      std::cout <<"Polynome P(X) with fixed coefficients of degree " << d <<" -> ";
      for (int i=deg(poly); i>0; i--){
      std::cout << poly[i] <<"X^"<<i<<" + ";
      }
      std::cout << poly[0] <<endl;
   }
   else{
        std::cout <<"Error: unable to evaluate isPolynomeRandom. Initalize coefficients of polynome with fixed values."<<endl;
        vector<long> coeff(d+1, 2);
        for (long i=d; i>=0; i--){
          SetCoeff(poly, i, coeff[i]); // coefficients are fixed.
        }
        if (isMonic) SetCoeff(poly, d);    // set top coefficient to 1

        // Print the polynome.
        std::cout <<"Polynome P(X) with fixed coefficients of degree " << d <<" -> ";
        for (int i=deg(poly); i>0; i--){
        std::cout << poly[i] <<"X^"<<i<<" + ";
        }
        std::cout << poly[0] <<endl;
   }


   // ***Encrypt the vectors of points to evaluate the polynome.***
   CyCtxt cx = cy.encrypt(x);
   std::cout <<"Encryption of vector x..."<<endl;
   std::cout << "Encrypted x: Encrypt(" << x << ")"<<endl;

   // ***Evaluate the polynome by each cypher elements of the CyCtxt containing the cypher evaluation points.***
   // Creation of a CyCtxt to contain the result CyCtxt of the polynomial evaluation.
   CyCtxt cEvalPoly = cx; 
   // Evaluate poly on the ciphertext.
   polyEval(cEvalPoly, poly, cx, 0);


  // ***Decrypt the CyCtxt which contain the polynomial evaluation.***
  vector<long> vect_polyEval = cy.decrypt(cEvalPoly);
  // The user can then verify if the result of the polynomial evaluation is the same that the polynomial evaluation without encryption.
  std::cout <<"Decrypt(P(encrypt(x))) -> "<< vect_polyEval<<endl;

  // Verify if the result of the Polynomial evaluation of the encrypted vector is the same that the polynomial evaluation of the vector without encryption.
  vector<long> plainTextEval;
  for (long i=0; i<sizeVectorPtsEval; i++) {
     long ret = polyEvalMod(poly, x[i], p2r);
     plainTextEval.push_back(ret);
     if (ret != vect_polyEval[i]) {
       std::cout << "Decrypt(P(encrypt(x))) != P(x). Plaintext poly MISMATCH\n";
       number_fail += 1;
       break; // Break the for loop to count only one fail.
    }
  }
   std::cout <<"P(x) -> "<< plainTextEval<<endl;
   std::cout << "Decrypt(P(encrypt(x))) == P(x). Plaintext poly match\n" << std::flush;
   number_success += 1;




   // Skip a line.
   std::cout <<"\n"<<endl;

   std::cout <<"******Homomorphic Polynomial Evaluation with random values for points evaluation and random polynome (testPolynomialEvalAllRandom)******"<<endl;
   // Test poly eval with 10 points of evaluation and a random polnome of degree 10.
   const bool isPolyEvalSuccess = cy.testPolynomialEvalAllRandom(10, 10);
   if(isPolyEvalSuccess){
       std::cout <<"testPolynomialEvalAllRandom -> true" <<endl;
       number_success += 1;
   }
   else if(!isPolyEvalSuccess){
       std::cout <<"testPolynomialEvalAllRandom -> false" <<endl;
       number_fail += 1;
   }
   else{
       std::cout <<"testPolynomialEvalAllRandom -> unexpected error" <<endl;
       number_unexpeted_error += 1;
   }

   // Skip a line.
   std::cout <<"\n"<<endl;

   std::cout <<"******Homomorphic Polynomial Evaluation with Cyfhel method polynomeEval (ZZX version)******"<<endl;
   /*
	Choose a vector of n points x = [x1, x2, ..., xn]. 
    Then, define a polynome Poly of degree d. 
    Then, we cyphered the vector of points x to obtain cx = [c1, c2, ..., cn].
    Then, we evaluate the n cyphered points with the polynome we have defined previously ie P(cx) = [P(c1), P(c2), ..., P(cn)].
    Then, we decrypt the previous vector: Decrypt(P(cx)) = Decrypt([P(c1), P(c2), ..., P(cn)]) = [P(x1), P(x2), ..., P(xn)].
    Then, we perform the polynomiale evaluation on the plaintext vector x: P(x) = [P(x1), P(x2), ..., P(xn)].
    Finally, we verify if Decrypt(P(cx)) = P(x) or not.
    */

   vector<long> vectorPtsEval0;// Definition of a vector of points for polynomial evaluation.
   vector<long> coeffPoly0;// Definition of the coefficients of the polynome.

   // Initialization of the vector of points for polynomial evaluation.
   for(unsigned long i=0; i<VECTOR_SIZE; i++)
   {
       vectorPtsEval0.push_back(i);
   }

   // Initialization of the vector of coefficients for polynome.
   for(unsigned long i=0; i<VECTOR_SIZE; i++)
   {
       coeffPoly0.push_back(i+2);
   }

   // Create a polynome.
   ZZX poly0 = cy.createPolynomeWithCoeff(coeffPoly0);
   
   // Polynomial evaluation.
   CyCtxt cEvalPoly_cyfhel0 = cy.polynomialEval(vectorPtsEval0, poly0);

   // ***Decrypt the CyCtxt which contain the polynomial evaluation.***
   vector<long> vect_polynomialEval0 = cy.decrypt(cEvalPoly_cyfhel0);
   // The user can then verify if the result of the polynomial evaluation is the same that the polynomial evaluation without encryption.
   std::cout <<"Decrypt(P(encrypt(x))) mod("<< cy.getp2r() <<") -> "<< vect_polynomialEval0<<endl;

   // Verify if the result of the Polynomial evaluation of the encrypted vector is the same that the polynomial evaluation of the vector without encryption.
   vector<long> plainTextPolyEval0;
   // Polynomial evaluation on plaintext vector.
   ZZX polynome0 = cy.createPolynomeWithCoeff(coeffPoly0); // Create a ZZX polynome with the coefficients provide by the user.
   // Perform the polynomial evaluation for all the elements of the plaintext evalauation points vector and put it in the vector plainTextPolyEval.
   for (unsigned long i=0; i<vectorPtsEval0.size(); i++) {
       long elementPolyEval0 = polyEvalMod(polynome0, vectorPtsEval0[i], p2r); // Polynomial evaluation of the ieme element of the plaintext vector.
       plainTextPolyEval0.push_back(elementPolyEval0); // Push the polynomial evaluation of the ieme element of the plaintext vector within the vector plainTextPolyEval.
   }
   // Display the plaintext vector that contain the polynomial evaluation of vectorPtsEval ie P(x).
   std::cout <<"P(x) mod("<< cy.getp2r() <<") -> "<< plainTextPolyEval0<<endl;
   // If Decrypt(P(encrypt(x))) equal to P(x), the homeomorphic operation works and so it is a success. Else, it is a fail.
   if(vect_polynomialEval0 == plainTextPolyEval0){
       std::cout <<"Homeomorphic operation polynomeEval is a success: Decrypt(P(encrypt(x))) equal to P(x).\n"<<endl;
       number_success += 1;
   }
   else if(vect_polynomialEval0 != plainTextPolyEval0){
       std::cout <<"Homeomorphic operation polynomeEval is a fail: Decrypt(P(encrypt(x))) not equal to P(x)."<<endl;
       number_fail += 1;
   }
   else{
       std::cout <<"Error: unexpected result during the comparison of vect_polynomialEval and plainTextPolyEval."<<endl;
	   number_unexpeted_error += 1;
   }


   // Skip a line.
   std::cout <<"\n"<<endl;

   std::cout <<"******Homomorphic Polynomial Evaluation with Cyfhel method polynomeEval******"<<endl;
   /*In this test, we choose n points (choosen by the user) and put them in a vector vectorPtsEval = [x1, x2, ..., xn]. 
     Then, we define a polynome poly of degree d by choosing d+1 coefficients of the polynome in a vector coeffPoly. 
     Then, call the method polynomeEval to evaluate the n cyphered points corresponding to vectorPtsEval with the polynome we have defined previously ie
     P(cx) = [P(c1), P(c2), ..., P(cn)].
     Then, we decrypt the previous vector: Decrypt(P(cx)) = Decrypt([P(c1), P(c2), ..., P(cn)]) = [P(x1), P(x2), ..., P(xn)].
     Then, we perform the polynomiale evaluation on the plaintext vector x: P(x) = [P(x1), P(x2), ..., P(xn)].
     Finally, we verify if Decrypt(P(cx)) = P(x) or not.
   */

   vector<long> vectorPtsEval;// Definition of a vector of points for polynomial evaluation.
   vector<long> coeffPoly;// Definition of the coefficients of the polynome.

   // Initialization of the vector of points for polynomial evaluation.
   for(unsigned long i=0; i<VECTOR_SIZE; i++)
   {
       vectorPtsEval.push_back(i);
   }

   // Initialization of the vector of coefficients for polynome.
   for(unsigned long i=0; i<VECTOR_SIZE; i++)
   {
       coeffPoly.push_back(i+2);
   }
   
   // Polynomial evaluation.
   CyCtxt cEvalPoly_cyfhel = cy.polynomialEval(vectorPtsEval, coeffPoly);

   // ***Decrypt the CyCtxt which contain the polynomial evaluation.***
   vector<long> vect_polynomialEval = cy.decrypt(cEvalPoly_cyfhel);
   // The user can then verify if the result of the polynomial evaluation is the same that the polynomial evaluation without encryption.
   std::cout <<"Decrypt(P(encrypt(x))) mod("<< cy.getp2r() <<") -> "<< vect_polynomialEval<<endl;

   // Verify if the result of the Polynomial evaluation of the encrypted vector is the same that the polynomial evaluation of the vector without encryption.
   vector<long> plainTextPolyEval;
   // Polynomial evaluation on plaintext vector.
   ZZX polynome = cy.createPolynomeWithCoeff(coeffPoly); // Create a ZZX polynome with the coefficients provide by the user.
   // Perform the polynomial evaluation for all the elements of the plaintext evalauation points vector and put it in the vector plainTextPolyEval.
   for (unsigned long i=0; i<vectorPtsEval.size(); i++) {
       long elementPolyEval = polyEvalMod(polynome, vectorPtsEval[i], p2r); // Polynomial evaluation of the ieme element of the plaintext vector.
       plainTextPolyEval.push_back(elementPolyEval); // Push the polynomial evaluation of the ieme element of the plaintext vector within the vector plainTextPolyEval.
   }
   // Display the plaintext vector that contain the polynomial evaluation of vectorPtsEval ie P(x).
   std::cout <<"P(x) mod("<< cy.getp2r() <<") -> "<< plainTextPolyEval<<endl;
   // If Decrypt(P(encrypt(x))) equal to P(x), the homeomorphic operation works and so it is a success. Else, it is a fail.
   if(vect_polynomialEval == plainTextPolyEval){
       std::cout <<"Homeomorphic operation polynomeEval is a success: Decrypt(P(encrypt(x))) equal to P(x).\n"<<endl;
       number_success += 1;
   }
   else if(vect_polynomialEval != plainTextPolyEval){
       std::cout <<"Homeomorphic operation polynomeEval is a fail: Decrypt(P(encrypt(x))) not equal to P(x)."<<endl;
       number_fail += 1;
   }
   else{
       std::cout <<"Error: unexpected result during the comparison of vect_polynomialEval and plainTextPolyEval."<<endl;
	   number_unexpeted_error += 1;
   }
   

   // Skip a line.
   std::cout <<"\n"<<endl;
   std::cout <<"Number of successful tests: "<< number_success<<endl;
   std::cout <<"Number of fail tests: "<< number_fail<<endl;
   std::cout <<"Number of unexpected errors during the tests: "<< number_unexpeted_error<<endl;
   std::cout <<"\n"<<endl;


    // Skip a line.
    std::cout <<"\n"<<endl;

    // Test the output stream of Cyfhel.
    std::cout <<"******Test of the stream operators.******"<<endl;
    std::cout <<"Test the << operator. Output stream of Cyfhel object used in this demo: " << cy<<endl;
    // Uncomment the following lines to test the cin operator. Be careful, it takes time because you regenerate two Cyfhel objects!
    /*std::cout <<"Test the >> operator: "<<endl;
    Cyfhel cy_in(true);
    std::cin >> cy_in;*/

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

    // Skip a line.
    std::cout <<"\n"<<endl;

    // If success, return 0.
    return 0;
};

