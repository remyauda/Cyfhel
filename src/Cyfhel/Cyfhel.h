#ifndef DEF_CYFHEL
#define DEF_CYFHEL

#include <unistd.h>
#include <iostream>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <cstdlib>
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
	bool flagPrint = false;// Flag to print messages on console
        long nslots;// Nº of slots in scheme


	/******PROTOTYPES OF PRIVATE METHODS******/        
        string store(Ctxt* ctxt);//Store the ciphertext in the unordered map and return key where * it was stored


    public:

	/******CONSTRUCTOR BY DEFAULT******/
        Cyfhel();

	/******DESTRUCTOR BY DEFAULT******/
        virtual ~Cyfhel();

       
	/******PROTOTYPES OF PUBLIC METHODS******/
        void keyGen(long p, long r, long c, long d, long sec, long w = 64,
                    long L = -1, long m = -1, long R = 3, long s = 0, 
                    const vector<long>& gens = vector<long>(),
                    const vector<long>& ords = vector<long>());

        
        string encrypt(vector<long> ptxt_vect);//Encryption
        
        vector<long> decrypt(string id1);//Decryption
        
        //******OPERATORS OVERLOAD******/
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

        
        // -------------------------------- I/O -------------------------------
        //SAVE ENVIRONMENT
        /**
         * @brief Saves the context, SecretKey and G polynomial in a .aenv file
         * @param fileName name of the file without the extention
         * @return BOOL 1 if all ok, 0 otherwise
         */
        bool saveEnv(string fileName);

        //RESTORE ENVIRONMENT OVERLOAD
        /**
         * @brief Restores the context, SecretKey and G polynomial from a .aenv file.
         *  Then it reconstucts publicKey and ea (EncriptedArray) with SecretKey & G.
         * @param fileName name of the file without the extention
         * @return BOOL 1 if all ok, 0 otherwise
         */
        bool restoreEnv(string fileName);


        // ----------------------------- AUXILIARY ----------------------------
        /**
         * @brief Number of plaintext slots 
         * @return number of plaintext slots
         */
        long numSlots();

        /**
         * @brief Getters for global parameters of the class
         */
        long getM();
        long getP();
        long getR();


        /**
        * @brief Create a new ciphertext and set it equal to the ciphertext 
        * stored in unordered map under ID id1
        * @param id1 ID of ctxt in unordered map ctxtMap
        * @return ID corresponding to new ciphertext
        */
        string set(string id1);

        /**
        * @brief Retrieve the ciphertext object from the unordered map
        * @param id1 ID of ctxt in unordered map ctxtMap
        * @return the ciphertext corresponding to the one stored with ID id1
        */
        Ctxt retrieve(string id1);
        
        /**
        * Replace the ciphertext at id1 with the new one provided
        * @param id1 ID of ctxt in unordered map ctxtMap
        * @param new_ctxt new Ctxt object to store in the unordered map
        */
        void replace(string id1, Ctxt new_ctxt);
        
        /**
        * @brief Delete from the unordered map the entry at key
        * @param id1 ID of ctxt in unordered map ctxtMap
        */
        void erase(string id1);

};

#endif


#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <cstddef>

class Timer{
    private:
    double m_start;
    double m_stop;
    double my_clock();

    public:
    Timer(bool print=false);
    virtual ~Timer();
    void start();
    void stop();
    double elapsed_time();
    bool flagPrint=false;
};

#endif

