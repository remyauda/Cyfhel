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
Cyfhel::Cyfhel(bool isVerbose, long p, long r, long c, long d, long sec, long w, long L, long m, long const& R, long const& s, vector<long> const& gens, vector<long> const& ords):m_context(0), m_secretKey(0), m_publicKey(0), m_encryptedArray(0) {
	m_isVerbose = isVerbose;
	keyGen(p, r, c, d, sec, w, L, m, R, s, gens, ords);
}

Cyfhel::Cyfhel(long p, long r, long c, long d, long sec, long w, long L, long m, long const& R, long const& s, vector<long> const& gens, vector<long> const& ords, bool isVerbose):m_context(0), m_secretKey(0), m_publicKey(0), m_encryptedArray(0) {
	m_isVerbose = isVerbose;
	keyGen(p, r, c, d, sec, w, L, m, R, s, gens, ords);
}

// TODO: MUST be tested.
Cyfhel::Cyfhel(vector<long> cryptoParameters, bool isVerbose):m_context(0), m_secretKey(0), m_publicKey(0), m_encryptedArray(0) {
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

/******COPY CONSTRUCTOR******/
Cyfhel::Cyfhel(Cyfhel const& cyfhelToCopy):m_G(cyfhelToCopy.m_G), m_global_m(cyfhelToCopy.m_global_m), m_global_p(cyfhelToCopy.m_global_p), m_global_r(cyfhelToCopy.m_global_r), m_numberOfSlots(cyfhelToCopy.m_numberOfSlots), m_isVerbose(cyfhelToCopy.m_isVerbose) {
	if(m_isVerbose){
		std::cout << "Use the copy constructor. Begin the construction." << endl;
	}
	m_context = new FHEcontext(*(cyfhelToCopy.m_context));
	m_secretKey = new FHESecKey(*(cyfhelToCopy.m_secretKey));
	m_publicKey = new FHEPubKey(*(cyfhelToCopy.m_publicKey));
	m_encryptedArray = new EncryptedArray(*(cyfhelToCopy.m_encryptedArray));
	if(m_isVerbose){
		std::cout << "End of the construction." << endl;
	}
}


/******DESTRUCTOR BY DEFAULT******/
Cyfhel::~Cyfhel(){

}

/******IMPLEMENTATION OF GETTERS******/
/*
	@name: getm_numberOfSlots
	@description: Getter of attribute m_numberOfSlots. It corresponds to the number of plaintext slots.

	@param: null.
*/
long Cyfhel::getm_numberOfSlots() const {
	return m_numberOfSlots;
}

/*
	@name: getm_global_m
	@description: Getter of attribute m_global_m.

	@param: null.
*/
long Cyfhel::getm_global_m() const {
	return m_global_m;
}

/*
	@name: getm_global_p
	@description: Getter of attribute m_global_p.

	@param: null.
*/
long Cyfhel::getm_global_p() const {
	return m_global_p;
}

/*
	@name: getm_global_r
	@description: Getter of attribute m_global_r.

	@param: null.
*/
long Cyfhel::getm_global_r() const {
	return m_global_r;
}

/*
	@name: getm_isVerbose
	@description: Getter of attribute m_isVerbose.

	@param: null.
*/
bool Cyfhel::getm_isVerbose() const {
	return m_isVerbose;
}

/******IMPLEMENTATION OF SETTERS******/
/*
	@name: setm_numberOfSlots
	@description: Setter of attribute m_numberOfSlots.

	@param: The method setm_numberOfSlots takes one mandatory parameter: a long.
	-param1: the new number of slots for m_numberOfSlots.
*/
void Cyfhel::setm_numberOfSlots(long numberOfSlots) {
	this->m_numberOfSlots = numberOfSlots;
}

/*
	@name: setm_global_m
	@description: Setter of attribute m_global_m.

	@param: The method setm_global_m takes one mandatory parameter: a long.
	-param1: the new value for attribute m_global_m.
*/
void Cyfhel::setm_global_m(long global_m) {
	this->m_global_m = global_m;
}

/*
	@name: setm_global_p
	@description: Setter of attribute m_global_p.

	@param: The method setm_global_p takes one mandatory parameter: a long.
	-param1: the new value for attribute m_global_p.
*/
void Cyfhel::setm_global_p(long global_p) {
	this->m_global_p = global_p;
}

/*
	@name: setm_global_r
	@description: Setter of attribute m_global_r.

	@param: The method setm_global_r takes one mandatory parameter: a long.
	-param1: the new value for attribute m_global_r.
*/
void Cyfhel::setm_global_r(long global_r) {
	this->m_global_r = global_r;
}

/*
	@name: setm_isVerbose
	@description: Setter of attribute m_isVerbose.

	@param: The method setm_isVerbose takes one mandatory parameter: a bool.
	-param1: the new value for attribute m_isVerbose.
*/
void Cyfhel::setm_isVerbose(bool isVerbose) {
	this->m_isVerbose = isVerbose;
}


/******IMPLEMENTATION OF PRIVATE METHODS******/
// KEY GENERATION
/*
	@name: keyGen
	@description: Private method used in Cyfhel contructor which performs Key Generation using HElib functions.

	@param: The method keyGen takes five mandatory parameters and seven optional parameters: five mandatory long, five optional long and two optional vector of long.
	-param1: a long which corresponds to plaintext base.
	-param2: a long which corresponds to lifting.
	-param3: a long which corresponds to number of columns in key switching matrix.
	-param4: a long which corresponds to degree of field extension.
	-param5: a long which corresponds to security parameter.
	-param6 (optional)(Default: w = 64): a optional long which corresponds to Hamming weight of secret key. By default, this parameter is such as w = 64.
	-param7 (optional)(Default: L = -1): a optional long which corresponds to number of levels in modulus chain. By default, this parameter is such as L = -1.
	-param8 (optional)(Default: m = -1): a optional long which used to m'th cyclotomic polynomial. By default, this parameter is such as m = -1.
	-param9 (optional)(Default: R = 3): a optional long which corresponds to number of expected rounds of multiplication. By default, this parameter is such as R = 3.
	-param10(optional)(Default: s = 0): a optional long which corresponds to minimum number of slots for vectors. By default, this parameter is such as s = 0.
	-param11(optional)(Default: gens = []): a optional vector of long which corresponds to vector of generators. By default, this parameter is such as gens = [].
	-param12(optional)(Default: ords = []): a optional vector of long which corresponds to vector of orders. By default, this parameter is such as ords = [].

	@return: null.
*/
void Cyfhel::keyGen(long const& p, long const& r, long const& c, long const& d, long const& sec, long const& w, long L, long m, long const& R, long const& s, const vector<long>& gens, const vector<long>& ords) {
	if(m_isVerbose)
	{
		std::cout << "Cyfhel::keyGen START" << endl;
	}
        
	// Initializing possible empty parameters for context
	//  - L -> Heuristic computation
	if(L == -1)
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
	if(m == -1)
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
		<< "p=" << p << ", r=" << r
		<< ", d=" << d << ", c=" << c
		<< ", sec=" << sec << ", w=" << w
		<< ", L=" << L << ", m=" << m
		<< ", gens=" << gens << ", ords=" << ords <<  endl;
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
		std::cout << "Cyfhel::keyGen COMPLETED \n" << endl;
	}
}



/******IMPLEMENTATION OF PUBLIC METHODS******/

//------ENCRYPTION------
//ENCRYPTION
/*
	@name: encrypt
	@description: Public method which allow to encrypt a provided plaintext vector which corresponds to vector to encrypt, creates the corresponding CyCtxt and return it.

	@param: The method encrypt takes one mandatory parameter: a vector of long.
	-param1: a mandatory vector of long which corresponds to vector to encrypt.

	@return: Return a CyCtxt which corresponds to encrypted vector.
*/
CyCtxt Cyfhel::encrypt(vector<long> &ptxt_vect, bool isPtxt_vectResize) const {
	// Create a vector of size nddSlots and fill it first with values from plaintext, then with zeros.
	long vector_size = ptxt_vect.size();
	// Empty cyphertext object.
	CyCtxt ctxt_vect(*m_publicKey, vector_size);
	// If the user try to encrypt a vector with a size greater than the maximum slots we can encrypt, then return an error.
	if(vector_size>m_numberOfSlots){
		cerr<<"Error: the size of the plaintext vector to encrypt cannot be greater than the number of slot"<<m_numberOfSlots<<"of the Cyfhel object."<<endl;
	}
	/*if(m_isVerbose)
	{
		std::cout << "  Cyfhel::encrypt(" << ptxt_vect <<  ")\n" << endl;
	}*/
	// Add (m_numberOfSlots - vector_size) zeros after the original vector.
	for(int i=0; i<m_numberOfSlots; i++)
	{
		if(i>=vector_size)
		{
			ptxt_vect.push_back(0);
		}
	}
	// Encryption of the ptxt_vect with the public key m_publicKey. Initialize the CyCtxt ctxt_vect.
	m_encryptedArray->encrypt(ctxt_vect, *m_publicKey, ptxt_vect);// Encrypt plaintext
	// The vector ptxt_vect has been changed as we have add (m_numberOfSlots - vector_size) zeros after the original vector.
	// So if the user want to, we resize the vector ptxt_vect to have the original vector.
	if(isPtxt_vectResize){
		ptxt_vect.resize(vector_size);
	}
	// Set the encryption informations in the CyCtxt
	ctxt_vect.setm_publicKey(m_publicKey);// Set the public key of Cyfhel object used to encrypt in the CyCtxt
	ctxt_vect.setm_encryptedArray(m_encryptedArray);// Set the encrypted array of Cyfhel object used to encrypt in the CyCtxt
	ctxt_vect.setm_numberOfSlots(m_numberOfSlots);// Set the number of slots of Cyfhel object used to encrypt in the CyCtxt
	// Return the homeomorphic cypher vector of ptxt_vect: the CyCtxt ctxt_vect.
	return ctxt_vect;
}

//DECRYPTION
/*
	@name: decrypt
	@description: Public method which allow to decrypt a provided CyCtxt which corresponds to vector to decrypt, creates the corresponding vector of long and return it.

	@param: The method decrypt takes one mandatory parameter: a CyCtxt.
	-param1: a mandatory CyCtxt which corresponds to vector to decrypt.

	@return: Return a vector of long which corresponds to decrypted vector.
*/
vector<long> Cyfhel::decrypt(Ctxt& ctxt_vect, bool isDecryptedPtxt_vectResize) const {
	// The size of the original plaintext.
	long vector_size = ctxt_vect.getm_sizeOfPlaintext();
	vector<long> ptxt_vect(m_numberOfSlots, 0);// Empty vector of values
	m_encryptedArray->decrypt(ctxt_vect, *m_secretKey, ptxt_vect);// Decrypt cyphertext
	/*if(m_isVerbose)
	{
		std::cout << "  Cyfhel::decrypt(" << ptxt_vect << ")\n" << endl;
	}*/
	if(isDecryptedPtxt_vectResize){
		ptxt_vect.resize(vector_size);
	}
	return ptxt_vect;
}

//------AUXILIARY------


//------I/O------
//SAVE ENVIRONMENT
/*
	@name: saveEnv
	@description: Public method which allow to saves the context, m_secretKey and m_G polynomial in a .aenv file. The method return 1 if all ok and 0 otherwise.

	@param: The method saveEnv takes one mandatory parameter: a string.
	-param1: a mandatory string which corresponds to the name of the file without the extention.

	@return: Return a bool which is equal to 1 if all ok and 0 otherwise.
*/
bool Cyfhel::saveEnv(string const& fileName) const {
	bool res=1;
	try
	{
		fstream keyFile(fileName+".aenv", fstream::out|fstream::trunc);
		assert(keyFile.is_open());

		writeContextBase(keyFile, *m_context);    // Write m, p, r, gens, ords
		keyFile << *m_context << endl;            // Write the rest of the context
		keyFile << *m_secretKey << endl;          // Write Secret key
		keyFile << m_G <<endl;                    // Write m_G poly (m_encryptedArray can't be written, we save
		                                          // m_G in order to reconstruct m_encryptedArray in restoreEnv)
		keyFile.close();
	}
	catch(exception& e)
	{
		res=0;
	}
	return res;// 1 if all OK, 0 otherwise
}

//RESTORE ENVIRONMENT
/*
	@name: restoreEnv
	@description: Public method which allow to restores the context, m_secretKey and m_G polynomial from a .aenv file. The method return 1 if all ok and 0 otherwise.

	@param: The method restoreEnv takes one mandatory parameter: a string.
	-param1: a mandatory string which corresponds to the name of the file without the extention to restore the context, m_secretKey and m_G polynomial.

	@return: Return a bool which is equal to 1 if all ok and 0 otherwise.
*/
bool Cyfhel::restoreEnv(string const& fileName) {
	bool res=1;
	unsigned long m1, p1, r1;
	vector<long> gens, ords;
	try
	{
		fstream keyFile(fileName+".aenv", fstream::in);
        assert(keyFile.is_open());

        readContextBase(keyFile, m1, p1, r1, gens, ords);// Read m, p, r, gens, ords

        m_context = new FHEcontext(m1, p1, r1, gens, ords);// Prepare empty context object

        m_secretKey = new FHESecKey(*m_context);// Prepare empty FHESecKey object
        
        keyFile >> *m_context;// Read the rest of the context
        keyFile >> *m_secretKey;// Read Secret Key
        keyFile >> m_G;// Read m_G Poly
        m_encryptedArray = new EncryptedArray(*m_context, m_G);// Reconstruct m_encryptedArray using m_G
        m_publicKey = (FHEPubKey*) m_secretKey;// Reconstruct Public Key from Secret Key
        m_numberOfSlots = m_encryptedArray->size();// Refill m_numberOfSlots
        m_global_m = m1;
        m_global_p = p1;
        m_global_r = r1;
    }
    catch(exception& e)
	{
		res=0;
    }
    return res;//1 if all OK, 0 otherwise
}


//******OPERATORS OVERLOAD******/


/******STREAM OPERATORS OVERLOAD******/
ostream& operator<< (std::ostream& flux, Cyfhel const& cy){
flux << "Parameters used: m value: " << cy.m_global_m << ". p value: " << cy.m_global_p << ". r value: " << cy.m_global_r << ". Number of slots used in the scheme: " << cy.m_numberOfSlots << ". Is the Cyfhel object is verbose?: " << cy.m_isVerbose;
return flux;
}

istream& operator>> (std::istream& in, Cyfhel& cy){
bool isVerbose;
long p;
long r;
long c;
long d;
long sec;
//long const& w;
//long L;

std::cout<<"Do you want your Cyfhel object verbose? (0 if no, 1 if yes): isVerbose = ";
in >> isVerbose;
std::cout<<"A long which corresponds to plaintext base (ex: p = 2): p = ";
in >> p;
std::cout<<"A long which corresponds to lifting (ex: r = 32): r = ";
in >> r;
std::cout<<"A long which corresponds to number of columns in key switching matrix (ex: c = 2): c = ";
in >> c;
std::cout<<"A long which corresponds to degree of field extension (ex: d = 1): d = ";
in >> d;
std::cout<<"A long which corresponds to security parameter (ex: sec = 128): sec = ";
in >> sec;
/*std::cout<<"A optional long which corresponds to Hamming weight of secret key. By default, this parameter is such as w = 64: w = ";
in >> w;
std::cout<<"A optional long which corresponds to number of levels in modulus chain. By default, this parameter is such as L = -1: L = ";
in >> L;*/
cy = Cyfhel(isVerbose, p, r, c, d, sec);

return in;
}

