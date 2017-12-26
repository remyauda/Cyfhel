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

/******DESTRUCTOR BY DEFAULT******/
Cyfhel::~Cyfhel(){}

/******GETTERS******/

/**
  * @brief Number of plaintext slots 
  * @return number of plaintext slots
  */
long Cyfhel::numSlots() {
  return ea->size();
}

/**
  * @brief Getters for global parameters of the class
  */
long Cyfhel::getM(){
  return global_m;
}

/**
  * @brief Getters for global parameters of the class
  */
long Cyfhel::getP(){
  return global_p; 
}

/**
  * @brief Getters for global parameters of the class
  */
long Cyfhel::getR(){ 
  return global_r; 
}

/******SETTERS******/
/**
  * @brief Create a new ciphertext and set it equal to the ciphertext 
  * stored in unordered map under ID id1
  * @param id1 ID of ctxt in unordered map ctxtMap
  * @return ID corresponding to new ciphertext
  */
string Cyfhel::set(string id1){
    Ctxt ctxt = ctxtMap.at(id1);
    return store(&ctxt);
}

/******IMPLEMENTATION OF PRIVATE METHODS******/
/**
  * @brief Store the ciphertext in the unordered map and return key where 
  * it was stored
  * @param ctxt Ciphertext to store in unordered map
  * @return the ID used to locate this ciphertext in the unordered map
*/
string Cyfhel::store(Ctxt* ctxt) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    string id1 = boost::lexical_cast<string>(ms);
    ctxtMap.insert(make_pair(id1, *ctxt));
    return id1;
}


/******IMPLEMENTATION OF PUBLIC METHODS******/

//------ENCRYPTION------
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
void Cyfhel::keyGen(long p, long r, long c, long d, long sec, long w,
                       long L, long m, long R, long s,
                       const vector<long>& gens,
                       const vector<long>& ords){
        if(m_isVerbose){std::cout << "Cyfhel::keyGen START" << endl;}
        
        // Initializing possible empty parameters for context
        //  - L -> Heuristic computation
        if(L==-1){
            L=3*R+3;
            if(p>2 || r>1){
                 L += R * 2*ceil(log((double)p)*r*3)/(log(2.0)*FHE_p2Size) +1;
            }
            if(m_isVerbose){std::cout << "  - calculated L: " << L <<endl;}
        }
        //  - m -> use HElib method FindM with other parameters
        if(m==-1){
            m = FindM(sec, L, c, p, d, 0, 0);
            if(m_isVerbose){std::cout << "  - Calculated m: " << m <<endl;}
        }

        // Context creation
        global_m = m;
        global_p = p;
        global_r = r;
        context = new FHEcontext(m, p, r, gens, ords);  // Initialize context
        buildModChain(*context, L, c);                  // Add primes to modulus chain
        if(m_isVerbose){
		std::cout << "  - Created Context: " 
		<< "p="   << p        << ", r=" << r
		<< ", d=" << d        << ", c=" << c
		<< ", sec=" << sec    << ", w=" << w
		<< ", L=" << L        << ", m=" << m
		<< ", gens=" << gens  << ", ords=" << ords <<  endl;
	}

        // ZZX Polynomial creation
        if (d == 0){
		G = context->alMod.getFactorsOverZZ()[0];
	}
        else{
		G = makeIrredPoly(p, d);
	}
        if(m_isVerbose){
		std::cout << "  - Created ZZX poly from NTL lib" <<endl;
	}

        // Secret/Public key pair creation
        secretKey = new FHESecKey(*context);// Initialize object
        publicKey = (FHEPubKey*) secretKey;// Upcast: FHESecKey to FHEPubKey
        secretKey->GenSecKey(w);// Hamming-weight-w secret key
        if(m_isVerbose){
		std::cout << "  - Created Public/Private Key Pair" << endl;
	} 

        // Additional initializations
        addSome1DMatrices(*secretKey);// Key-switch matrices for relin.
        ea = new EncryptedArray(*context, G);// Object for packing in subfields
        nslots = ea->size();


        if(m_isVerbose){
		std::cout << "Cyfhel::keyGen COMPLETED" << endl;
	}
}

//ENCRYPTION
/**
  * @brief Enctypts a provided plaintext vector and stores the cyphertext
  * in the unordered map, returning the key(string) used to access it.
  * The encryption is carried out with HElib. 
  * @param ptxt_vect plaintext vector to encrypt
  * @return id (string) used to access ciphertext in the ctxtMap.
  */
string Cyfhel::encrypt(vector<long> plaintext) {
        Ctxt cyphertext(*publicKey);// Empty cyphertext object
        //TODO: create a vector of size nddSlots and fill it first with values from plaintext, then with zeros
        ea->encrypt(cyphertext, *publicKey, plaintext);// Encrypt plaintext
        string id1 = store(&cyphertext);
        if(m_isVerbose){
            std::cout << "  Cyfhel::encrypt({ID" << id1 << "}[" << plaintext <<  "])" << endl;
        }
        return id1;
}

//DECRYPTION
/**
  * @brief Decrypts the cyphertext accessed in the ctxtMap using the id.
  * The decryption is carried out with HElib.
  * @param id (string) used to access ciphertext in the ctxtMap.
  * @return plaintext, the result of decrypting the ciphertext
  */
vector<long> Cyfhel::decrypt(string id1) {
        vector<long> res(nslots, 0);// Empty vector of values
        ea->decrypt(ctxtMap.at(id1), *secretKey, res);// Decrypt cyphertext
        if(m_isVerbose){
            std::cout << "  Cyfhel::decrypt({ID" << id1 << "}[" << res << "])" << endl;
        }
        return res;
}

//------AUXILIARY------
/**
  * @brief Retrieve the ciphertext object from the unordered map
  * @param id1 ID of ctxt in unordered map ctxtMap
  * @return the ciphertext corresponding to the one stored with ID id1
  */
Ctxt Cyfhel::retrieve(string id1) {
    return ctxtMap.at(id1);
}

/**
  * Replace the ciphertext at id1 with the new one provided
  * @param id1 ID of ctxt in unordered map ctxtMap
  * @param new_ctxt new Ctxt object to store in the unordered map
  */
void Cyfhel::replace(string id1, Ctxt new_ctxt) {
    boost::unordered_map<string, Ctxt>::const_iterator i = ctxtMap.find(id1);
    if(i != ctxtMap.end()) {
        ctxtMap.at(id1) = new_ctxt;
    }
}

/**
  * @brief Delete from the unordered map the entry at key
  * @param id1 ID of ctxt in unordered map ctxtMap
  */
void Cyfhel::erase(string id1) {
    if(ctxtMap.find(id1) != ctxtMap.end()) {
        ctxtMap.erase(id1);
    }
}

//------I/O------
//SAVE ENVIRONMENT
/**
  * @brief Saves the context, SecretKey and G polynomial in a .aenv file
  * @param fileName name of the file without the extention
  * @return BOOL 1 if all ok, 0 otherwise
  */
bool Cyfhel::saveEnv(string fileName){
    bool res=1;
    try{
        fstream keyFile(fileName+".aenv", fstream::out|fstream::trunc);
        assert(keyFile.is_open());

        writeContextBase(keyFile, *context);    // Write m, p, r, gens, ords
        keyFile << *context << endl;            // Write the rest of the context
        keyFile << *secretKey << endl;          // Write Secret key
        keyFile << G <<endl;                    // Write G poly (ea can't be written, we save
                                                //  G in order to reconstruct ea in restoreEnv)
        keyFile.close();
    }
    catch(exception& e){
        res=0;
    }
    return res;                                 // 1 if all OK, 0 otherwise
}

//RESTORE ENVIRONMENT
/**
  * @brief Restores the context, SecretKey and G polynomial from a .aenv file.
  *  Then it reconstucts publicKey and ea (EncriptedArray) with SecretKey & G.
  * @param fileName name of the file without the extention
  * @return BOOL 1 if all ok, 0 otherwise
  */
bool Cyfhel::restoreEnv(string fileName){
    bool res=1;
    unsigned long m1, p1, r1;
    vector<long> gens, ords;
    try{
        fstream keyFile(fileName+".aenv", fstream::in);
        assert(keyFile.is_open());

        readContextBase(keyFile, m1, p1, r1, gens, ords);   
                                                            // Read m, p, r, gens, ords
        context = new FHEcontext(m1, p1, r1, gens, ords);   
                                                            // Prepare empty context object
        secretKey = new FHESecKey(*context);                // Prepare empty FHESecKey object
        
        keyFile >> *context;                    // Read the rest of the context
        keyFile >> *secretKey;                  // Read Secret Key
        keyFile >> G;                           // Read G Poly
        ea = new EncryptedArray(*context, G);   // Reconstruct ea using G
        publicKey = (FHEPubKey*) secretKey;     // Reconstruct Public Key from Secret Key
        nslots = ea->size();                    // Refill nslots
        global_m = m1;
        global_p = p1; 
        global_r = r1;
    }
    catch(exception& e){
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
void Cyfhel::add(string id1, string id2, bool negative){
        ctxtMap.at(id1).addCtxt(ctxtMap.at(id2), negative);
}

//MULTIPLICATION OVERLOAD
/**
  * @breif Multiply ciphertext at key by ciphertext at other_key and store
  * result in unordered map at key
  * @param id1 ID of ctxt 1 in unordered map
  * @param id2 ID of ctxt 2 in unordered map
  */
void Cyfhel::mult(string id1, string id2){
        ctxtMap.at(id1).multiplyBy(ctxtMap.at(id2));
}

//MULTIPLICATION BY 2 OVERLOAD
/**
  * @breif Multiply ciphertext at key by ciphertext at other_key and store
  * result in unordered map at key
  * @param id1 ID of ctxt 1 in unordered map
  * @param id2 ID of ctxt 2 in unordered map
  * @param id3 ID of ctxt 3 in unordered map
  */
void Cyfhel::mult3(string id1, string id2, string id3){
        ctxtMap.at(id1).multiplyBy2(ctxtMap.at(id2), ctxtMap.at(id3));
}

//CUMULATIVE SUM OVERLOAD
/**
  * @brief sum all the values in the vector. 
  * @param id1 ID of ctxt1 in unordered map
  */
void Cyfhel::cumSum(string id1){
        totalSums(*ea, ctxtMap.at(id1));
}

//SCALAR PRODUCT OVERLOAD
/**
  * @brief Multiply ciphertext by ciphertext and perform cumulative sum
  * @param id1 ID of ctxt1 in unordered map
  * @param id2 ID of ctxt2 in unordered map
  */
void Cyfhel::scalarProd(string id1, string id2, int partitionSize){
        ctxtMap.at(id1).multiplyBy(ctxtMap.at(id2));
        totalSums(*ea, ctxtMap.at(id1));
}

//SQUARE OVERLOAD
/**
  * @brief Square ciphertext at id1 in ctxtMap
  * @param id1 ID of ctxt in unordered map
  */
void Cyfhel::square(string id1){
        ctxtMap.at(id1).square();
}

//CUBE OVERLOAD
/**
  * @brief Cube ciphertext at id1 in ctxtMap
  * @param id1 ID of ctxt in unordered map
  */
void Cyfhel::cube(string id1){
        ctxtMap.at(id1).cube();
}

//NEGATE OVERLOAD
/**
  * @brief Multiply ciphertext at id1 by -1
  * @param id1 ID of ctxt in unordered map ctxtMap
  */
void Cyfhel::negate(string id1){
        ctxtMap.at(id1).negate();
}

//COMPARE EQUALS OVERLOAD
/**
  * @brief Compare ciphertext at id1 and ciphertext at id2 
  * to see if they are equal
  * @param id1 ID of ctxt 1 in unordered map ctxtMap
  * @param id2 ID of ctxt 2 in unordered map ctxtMap
  * @param comparePkeys if true then pkeys will be compared
  * @return BOOL --> ctxt(id1) == ctxt(id2)
  */
bool Cyfhel::equalsTo(string id1, string id2, bool comparePkeys){
        return ctxtMap.at(id1).equalsTo(ctxtMap.at(id2), comparePkeys);
}

//ROTATE OVERLOAD
/**
  * @brief Rotate ciphertext at id1 by c spaces
  * @param id1 ID of ctxt in unordered map ctxtMap
  * @param c number of spaces to rotate
  */
void Cyfhel::rotate(string id1, long c){
        ea->rotate(ctxtMap.at(id1), c);
}

//SHIFT OVERLOAD
/**
  * @brief Shift ciphertext at id1 by c spaces
  * @param id1 ID of ctxt in unordered map ctxtMap
  * @param c number of spaces to shift
  */
void Cyfhel::shift(string id1, long c){
        ea->shift(ctxtMap.at(id1), c);
}
