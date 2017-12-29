/*
 * Cyfhel
 * --------------------------------------------------------------------
 *  Cyfhel is a C++ library that creates an abstraction over the basic
 *  functionalities of HElib as a Homomorphic Encryption library, such as
 *  addition, multiplication, scalar product and others.
 *
 *  Cyfhel implements a higher level of abstraction than HElib, and handles
 *  Cyphertexts using an unordered map (key-value pairs) that is accessed
 *  via keys of type string. This is done in order to manage Cyphertext
 *  using references (the keys), which will allow Cyfhel to work only
 *  using strings (keeping the Cyphertexts in C++). Cyfhel also compresses
 *  the Context setup and Key generation into one single KeyGen function
 *  with multiple parameter selection.
 *  --------------------------------------------------------------------
 *  Author: Remy AUDA
 *  Date: 17/12/2017  
 *  --------------------------------------------------------------------
 *  License: GNU GPL v3
 *  
 *  Cyfhel is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Cyfhel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  --------------------------------------------------------------------
 */

#include "Cyfhel.h"

using namespace std;

/******CONSTRUCTOR BY DEFAULT******/


/******CONSTRUCTOR WITH PARAMETERS******/
Cyfhel::Cyfhel(bool isVerbose, long p, long r, long c, long d, long sec, long w, long L){
	m_isVerbose = isVerbose;
	keyGen(p, r, c, d, sec, w, L);
}

Cyfhel::Cyfhel(long p, long r, long c, long d, long sec, long w, long L, bool isVerbose){
    m_isVerbose = isVerbose;
    keyGen(p, r, c, d, sec, w, L);
}

// TODO: MUST be tested.
Cyfhel::Cyfhel(vector<long> cryptoParameters, bool isVerbose){
	// TODO: We should be able to provide just some parameters and the rest will be initialize by default.
	if(cryptoParameters.size() < 7)
	{
		std::cout << "Error: Not enough parameters were given for the key Geneneration." << endl;
        std::cout << "Use the default parameters." << endl;
        long p = 2;
        long r = 32;
        long c = 2;
        long d = 1;
        long sec = 128;
        long w = 64;
        long L = 40;
        m_isVerbose = isVerbose;
        std::cout << "p=" << p << ", r=" << r
        << ", d=" << d << ", c=" << c
        << ", sec=" << sec << ", w=" << w
        << ", L=" << L <<  endl;

        keyGen(p, r, c, d, sec, w, L);
    }
    // TODO: We should be able to provide other parameters like m etc...
    else if(cryptoParameters.size() > 7)
	{
		std::cout << "Error: Too many parameters were given for the key Geneneration." << endl;
        std::cout << "Use the default parameters." << endl;
        long p = 2;
        long r = 32;
        long c = 2;
        long d = 1;
        long sec = 128;
        long w = 64;
        long L = 40;
        m_isVerbose = isVerbose;
        std::cout << "p=" << p << ", r=" << r
        << ", d=" << d << ", c=" << c
        << ", sec=" << sec << ", w=" << w
        << ", L=" << L <<  endl;

        keyGen(p, r, c, d, sec, w, L);
    }
	else if(cryptoParameters.size() == 7)
	{
        m_isVerbose = isVerbose;
        long p = cryptoParameters[0];
        long r = cryptoParameters[1];
        long c = cryptoParameters[2];
        long d = cryptoParameters[3];
        long sec = cryptoParameters[4];
        long w = cryptoParameters[5];
		long L = cryptoParameters[6];
    
        keyGen(p, r, c, d, sec, w, L);
    }
    else
	{
		std::cout << "Error: Unexpected number of parameters were given for the key Geneneration. Unable to completed the task." << endl;
    }
}

/******DESTRUCTOR BY DEFAULT******/
Cyfhel::~Cyfhel(){}

/******GETTERS******/

/**
  * @brief Number of plaintext slots
  * @return number of plaintext slots
  */
long Cyfhel::getm_numberOfSlots() {
	return m_numberOfSlots;
}

/**
  * @brief Getters for global parameters of the class
  */
long Cyfhel::getm_global_m(){
	return m_global_m;
}

/**
  * @brief Getters for global parameters of the class
  */
long Cyfhel::getm_global_p(){
	return m_global_p;
}

/**
  * @brief Getters for global parameters of the class
  */
long Cyfhel::getm_global_r(){
	return m_global_r;
}

/**
  * @brief Getter of attribute m_isVerbose
  */
bool Cyfhel::getm_isVerbose(){
	return m_isVerbose;
}

/******SETTERS******/
/**
  * @brief Setters for attribute m_numberOfSlots
  */
void Cyfhel::setm_numberOfSlots(long numberOfSlots){
	m_numberOfSlots = numberOfSlots;
}

/**
  * @brief Setters for attribute m_global_m
  */
void Cyfhel::setm_global_m(long global_m){
	m_global_m = global_m;
}

/**
  * @brief Setters for attribute m_global_p
  */
void Cyfhel::setm_global_p(long global_p){
	m_global_p = global_p;
}


/******IMPLEMENTATION OF PRIVATE METHODS******/
// KEY GENERATION
/**
  * @brief Performs Key Generation using HElib functions
  * @param p plaintext base
  * @param r lifting
  * @param c # of columns in key switching matrix
  * @param d degree of field extension
  * @param sec security parameter
  * @param w Hamming weight of secret key
  * @param L # of levels in modulus chain
  * @param m (optional) use m'th cyclotomic polynomial
  * @param R (=3) number of expected rounds of multiplication
  * @param s (=0) minimum number of slots for vectors.
  * @param gens (optional) Vector of Generators
  * @param ords (optional) Vector of Orders
  */
void Cyfhel::keyGen(long p, long r, long c, long d, long sec, long w, long L, long m, long R, long s, const vector<long>& gens, const vector<long>& ords){
	if(m_isVerbose)
	{
		std::cout << "Cyfhel::keyGen START" << endl;
	}
        
	// Initializing possible empty parameters for context
	//  - L -> Heuristic computation
	if(L==-1)
	{
		L=3*R+3;
		if(p>2 || r>1)
		{
			L += R * 2*ceil(log((double)p)*r*3)/(log(2.0)*FHE_p2Size) +1;
		}
		if(m_isVerbose)
		{
			std::cout << "  - calculated L: " << L <<endl;
		}
	}
	//  - m -> use HElib method FindM with other parameters
	if(m==-1)
	{
            m = FindM(sec, L, c, p, d, 0, 0);
            if(m_isVerbose)
			{
				std::cout << "  - Calculated m: " << m <<endl;
			}
	}

	// Context creation
	m_global_m = m;
	m_global_p = p;
	m_global_r = r;
	m_context = new FHEcontext(m, p, r, gens, ords);  // Initialize context
	buildModChain(*m_context, L, c);                  // Add primes to modulus chain
	if(m_isVerbose)
	{
		std::cout << "  - Created Context: "
		<< "p="   << p        << ", r=" << r
		<< ", d=" << d        << ", c=" << c
		<< ", sec=" << sec    << ", w=" << w
		<< ", L=" << L        << ", m=" << m
		<< ", gens=" << gens  << ", ords=" << ords <<  endl;
	}

	// ZZX Polynomial creation
	if (d == 0)
	{
		m_G = m_context->alMod.getFactorsOverZZ()[0];
	}
	else
	{
		m_G = makeIrredPoly(p, d);
   	}
	if(m_isVerbose)
	{
		std::cout << "  - Created ZZX poly from NTL lib" <<endl;
	}

	// Secret/Public key pair creation
	m_secretKey = new FHESecKey(*m_context);// Initialize object
	m_publicKey = (FHEPubKey*) m_secretKey;// Upcast: FHESecKey to FHEPubKey
	m_secretKey->GenSecKey(w);// Hamming-weight-w secret key
	if(m_isVerbose)
	{
		std::cout << "  - Created Public/Private Key Pair" << endl;
	}

	// Additional initializations
	addSome1DMatrices(*m_secretKey);// Key-switch matrices for relin.
	m_encryptedArray = new EncryptedArray(*m_context, m_G);// Object for packing in subfields
	m_numberOfSlots = m_encryptedArray->size();

	if(m_isVerbose)
	{
		std::cout << "Cyfhel::keyGen COMPLETED" << endl;
	}
}



/******IMPLEMENTATION OF PUBLIC METHODS******/

//------ENCRYPTION------
//ENCRYPTION
/**
  * @brief Enctypts a provided plaintext vector and stores the cyphertext
  * in the unordered map, returning the key(string) used to access it.
  * The encryption is carried out with HElib.
  * @param ptxt_vect plaintext vector to encrypt
  * @return id (string) used to access ciphertext in the ctxtMap.
  */
Ctxt Cyfhel::encrypt(vector<long> &ptxt_vect) {
	Ctxt ctxt_vect(*m_publicKey);// Empty cyphertext object
	// Create a vector of size nddSlots and fill it first with values from plaintext, then with zeros
	long vector_size = ptxt_vect.size();
	for(int i=0; i<m_numberOfSlots; i++)
	{
		if(i>=vector_size)
		{
			ptxt_vect.push_back(0);
		}
	}
	m_encryptedArray->encrypt(ctxt_vect, *m_publicKey, ptxt_vect);// Encrypt plaintext
	if(m_isVerbose)
	{
		std::cout << "  Cyfhel::encrypt(" << ptxt_vect <<  ")" << endl;
	}
	return ctxt_vect;
}

//DECRYPTION
/**
  * @brief Decrypts the cyphertext accessed in the ctxtMap using the id.
  * The decryption is carried out with HElib.
  * @param id (string) used to access ciphertext in the ctxtMap.
  * @return plaintext, the result of decrypting the ciphertext
  */
vector<long> Cyfhel::decrypt(Ctxt ctxt_vect) {
	vector<long> ptxt_vect(m_numberOfSlots, 0);// Empty vector of values
	m_encryptedArray->decrypt(ctxt_vect, *m_secretKey, ptxt_vect);// Decrypt cyphertext
	if(m_isVerbose)
	{
		std::cout << "  Cyfhel::decrypt(" << ptxt_vect << ")" << endl;
	}
	return ptxt_vect;
}

//------AUXILIARY------


//------I/O------
//SAVE ENVIRONMENT
/**
  * @brief Saves the context, m_secretKey and m_G polynomial in a .aenv file
  * @param fileName name of the file without the extention
  * @return BOOL 1 if all ok, 0 otherwise
  */
bool Cyfhel::saveEnv(string fileName){
	bool res=1;
	try
	{
		fstream keyFile(fileName+".aenv", fstream::out|fstream::trunc);
		assert(keyFile.is_open());

		writeContextBase(keyFile, *m_context);    // Write m, p, r, gens, ords
		keyFile << *m_context << endl;            // Write the rest of the context
		keyFile << *m_secretKey << endl;          // Write Secret key
		keyFile << m_G <<endl;                    // Write m_G poly (m_encryptedArray can't be written, we save
		                                          //  m_G in order to reconstruct m_encryptedArray in restoreEnv)
		keyFile.close();
	}
	catch(exception& e)
	{
		res=0;
	}
	return res;// 1 if all OK, 0 otherwise
}

//RESTORE ENVIRONMENT
/**
  * @brief Restores the context, m_secretKey and m_G polynomial from a .aenv file.
  *  Then it reconstucts m_publicKey and m_encryptedArray (EncriptedArray) with m_secretKey & m_G.
  * @param fileName name of the file without the extention
  * @return BOOL 1 if all ok, 0 otherwise
  */
bool Cyfhel::restoreEnv(string fileName){
	bool res=1;
	unsigned long m1, p1, r1;
	vector<long> gens, ords;
	try
	{
		fstream keyFile(fileName+".aenv", fstream::in);
        assert(keyFile.is_open());

        readContextBase(keyFile, m1, p1, r1, gens, ords);   
                                                            // Read m, p, r, gens, ords
        m_context = new FHEcontext(m1, p1, r1, gens, ords);   
                                                            // Prepare empty context object
        m_secretKey = new FHESecKey(*m_context);                // Prepare empty FHESecKey object
        
        keyFile >> *m_context;                    // Read the rest of the context
        keyFile >> *m_secretKey;                  // Read Secret Key
        keyFile >> m_G;                           // Read m_G Poly
        m_encryptedArray = new EncryptedArray(*m_context, m_G);   // Reconstruct m_encryptedArray using m_G
        m_publicKey = (FHEPubKey*) m_secretKey;     // Reconstruct Public Key from Secret Key
        m_numberOfSlots = m_encryptedArray->size();                    // Refill m_numberOfSlots
        m_global_m = m1;
        m_global_p = p1;
        m_global_r = r1;
    }
    catch(exception& e)
	{
		res=0;
    }
    return res;                                 // 1 if all OK, 0 otherwise
}


//******OPERATORS OVERLOAD******/
//ADDITION OVERLOAD
/**
  * @brief Add ciphertext at key to ciphertext at other_key and store result
  * back in unordered map at key
  * @param id1 ID of ctxt1 in unordered map
  * @param id2 ID of ctxt2 in unordered map
  * @param negative if True then perform subtraction
  */
void Cyfhel::add(Ctxt& c1, Ctxt& c2, bool negative){
	c1.addCtxt(c2, negative);
}

