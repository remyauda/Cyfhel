#ifndef DEF_CYFHEL
#define DEF_CYFHEL

#include <unistd.h>
#include <iostream>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <string.h>

#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <NTL/BasicThreadPool.h>
#include <NTL/lzz_pXFactoring.h>
#include <cassert>
#include <cstdio>

#include <timing.h>

#include "FHE.h"
#include "EncryptedArray.h"
#include "PAlgebra.h"

//The Cyfhel Class
class Cyfhel {

 private:

	/******ATTRIBUTES******/
        FHEcontext *context;// Required for key Generation
        FHESecKey *secretKey;// Secret key of the Public-Secret key pair
        FHEPubKey *publicKey;// Public key of the public-secret key pair
        ZZX G;// NTL Poly used to create ea
        EncryptedArray *ea;// Array used for encryption
        boost::unordered_map<string, Ctxt> ctxtMap; // Unordered map which stores the ciphertexts
        long global_m, global_p, global_r;
	bool m_isVerbose;// Flag to print messages on console
        


	/******PROTOTYPES OF PRIVATE METHODS******/
        void keyGen(long p, long r, long c, long d, long sec, long w = 64,
                    long L = -1, long m = -1, long R = 3, long s = 0, 
                    const vector<long>& gens = vector<long>(),
                    const vector<long>& ords = vector<long>());

        string store(Ctxt* ctxt);//Store the ciphertext in the unordered map and return key where * it was stored


 public:
	
        long nslots;// Nº of slots in scheme

	/******CONSTRUCTOR BY DEFAULT******/


	/******CONSTRUCTOR WITH PARAMETERS******/
	Cyfhel(bool isVerbose = false, long p = 2, long r = 32, long c = 2, long d = 1, long sec = 128, long w = 64, long L = 40);

        Cyfhel(long p = 2, long r = 32, long c = 2, long d = 1, long sec = 128, long w = 64, long L = 40, bool isVerbose = false);

        Cyfhel(vector<long> cryptoParameters, bool isVerbose = false);

	/******DESTRUCTOR BY DEFAULT******/
        virtual ~Cyfhel();

    	/******GETTERS******/
        long numSlots();

        long getM();
        long getP();
        long getR();

    	/******SETTERS******/

       
	/******PROTOTYPES OF PUBLIC METHODS******/
        //------ENCRYPTION------
        
        string encrypt(vector<long> ptxt_vect);//Encryption
        
        vector<long> decrypt(string id1);//Decryption


        //------AUXILIARY------

    
        //------I/O------
        bool saveEnv(string fileName);//Save environment

        bool restoreEnv(string fileName);//Restore environment

        
    	/******OPERATORS OVERLOAD******/
    	void add(string id1, string id2, bool negative=false);//Addition overload
            
    	void mult(string id1, string id2);//Multiplication overload
           
    	void mult3(string id1, string id2, string id3);//Multiplication by 2 overload

    	void cumSum(string id1);//Cumulative sum overload
            
    	void scalarProd(string id1, string id2, int partitionSize=0);//Scalar product overload

    	void square(string id1);//Square overload

    	void cube(string id1);//Cube overload

    	void negate(string id1);//Negate overload
            
    	bool equalsTo(string id1, string id2, bool comparePkeys=true);//Compare equals overload

    	void rotate(string id1, long c);//Rotate overload
            
    	void shift(string id1, long c);//Shift overload
};

#endif

