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

    Timer timerDemo(true);
    timerDemo.start();
    Cyfhel cy(true);// Create object Cyfhel and enable print for all functions 
    vector<long> v1;
    vector<long> v2;
    
    // Initialization of v1.
    for(int i=0; i<cy.nslots; i++){
        if(i<VECTOR_SIZE){ 
		v1.push_back(i);  
	}
    }
    // Initialization of v2.
    for(int i=0; i<cy.nslots; i++){
        if(i<VECTOR_SIZE){ 
		v2.push_back(2);  
	}
    }

    // Sum
    string c1 = cy.encrypt(v1);
    string c2 = cy.encrypt(v2);
    cy.add(c1, c2);
    vector<long> vRes = cy.decrypt(c1);
 

    // Multiplication
    //k1 = cy.encrypt(v1);
    //k2 = cy.encrypt(v2);
    //cy.mult(k1, k2);
    //vector<long> vRes2 = cy.decrypt(k1);

    // Scalar product
    //k1 = cy.encrypt(v1);
    //k2 = cy.encrypt(v2);
    //cy.scalarProd(k1, k2);
    //vector<long> vRes3 = cy.decrypt(k1);

    // Square
    //k1 = cy.encrypt(v1);
    //cy.square(k1);
    //vector<long> vRes4 = cy.decrypt(k1);

    timerDemo.stop();
    timerDemo.benchmarkInSeconds();
    timerDemo.benchmarkInHoursMinutesSecondsMillisecondes(true);

    // Store & retrieve environment
//    cy.saveEnv(fileName);
    std::cout << "Saved env with values: m=" << cy.getM() <<", p=" << cy.getP() << ", r=" << cy.getR() << endl;
    std::cout << "END OF DEMO" << endl;
};

