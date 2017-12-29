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
	FHEcontext *m_context;// Required for key Generation
	FHESecKey *m_secretKey;// Secret key of the Public-Secret key pair
	FHEPubKey *m_publicKey;// Public key of the public-secret key pair
	ZZX m_G;// NTL Poly used to create m_encryptedArray
	EncryptedArray *m_encryptedArray;// Array used for encryption
	long m_global_m, m_global_p, m_global_r;
	long m_numberOfSlots;// Nº of slots in scheme
	bool m_isVerbose;// Flag to print messages on console
        


    /******PROTOTYPES OF PRIVATE METHODS******/
	void keyGen(long const& p, long const& r, long const& c, long const& d, long const& sec, long const& w = 64,
                    long L = -1, long m = -1, long const& R = 3, long const& s = 0,
                    const vector<long>& gens = vector<long>(),
                    const vector<long>& ords = vector<long>());


 public:

	/******CONSTRUCTOR BY DEFAULT******/


	/******CONSTRUCTOR WITH PARAMETERS******/
	Cyfhel(bool isVerbose = false, long p = 2, long r = 32, long c = 2, long d = 1, long sec = 128, long w = 64, long L = 40);

	Cyfhel(long p = 2, long r = 32, long c = 2, long d = 1, long sec = 128, long w = 64, long L = 40, bool isVerbose = false);

	Cyfhel(vector<long> cryptoParameters, bool isVerbose = false);

	/******DESTRUCTOR BY DEFAULT******/
	virtual ~Cyfhel();

    /******GETTERS******/
	long getm_numberOfSlots() const;//Getter of attribute m_numberOfSlots

	long getm_global_m() const;//Getter of attribute m_global_m

	long getm_global_p() const;//Getter of attribute m_global_p

	long getm_global_r() const;//Getter of attribute m_global_r

	bool getm_isVerbose() const;//Getter of attribute m_isVerbose

	/******SETTERS******/
	void setm_numberOfSlots(long numberOfSlots);//Setter of attribute m_numberOfSlots

	void setm_global_m(long global_m);//Setter of attribute m_global_m

	void setm_global_p(long global_p);//Setter of attribute m_global_p

	void setm_global_r(long global_r);//Setter of attribute m_global_r

       
    /******PROTOTYPES OF PUBLIC METHODS******/
	//------ENCRYPTION------
	Ctxt encrypt(vector<long> &ptxt_vect) const;//Encryption
        
	vector<long> decrypt(Ctxt& ctxt_vect) const;//Decryption


	//------AUXILIARY------

    
	//------I/O------
	bool saveEnv(string const& fileName) const;//Save environment

	bool restoreEnv(string const& fileName);//Restore environment

        
    /******OPERATORS OVERLOAD******/
	void add(Ctxt& id1, Ctxt& id2, bool negative=false) const;//Addition overload
};

#endif
