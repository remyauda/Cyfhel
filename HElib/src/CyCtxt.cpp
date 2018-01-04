/*
 * CyCtxt
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

#include "CyCtxt.h"

using namespace std;

/******CONSTRUCTOR BY DEFAULT******/


/******CONSTRUCTOR WITH PARAMETERS******/


/******DESTRUCTOR BY DEFAULT******/


/******IMPLEMENTATION OF GETTERS******/
/*
	@name: getm_sizeOfPlaintext
	@description: Getter of attribute m_sizeOfPlaintext. It corresponds to the original size of the plaintext vector coresponding to the CyCtxt.

	@param: null.
*/
long CyCtxt::getm_sizeOfPlaintext() const {
	return m_sizeOfPlaintext;
}

/******IMPLEMENTATION OF SETTERS******/
/*
	@name: setm_sizeOfPlaintext
	@description: Setter of attribute m_sizeOfPlaintext.

	@param: The method setm_sizeOfPlaintext takes one mandatory parameter: a long.
	-param1: the new size of the plaintext vector coresponding to the CyCtxt.
*/
void CyCtxt::setm_sizeOfPlaintext(long sizeOfPlaintext) {
	this->m_sizeOfPlaintext = sizeOfPlaintext;
}

/*
	@name: setm_publicKey
	@description: Setter of attribute m_publicKey.

	@param: The method setm_publicKey takes one mandatory parameter: a FHEPubKey.
	-param1: the new public key for m_publicKey.
*/
void CyCtxt::setm_publicKey(FHEPubKey *publicKey) {
	this->m_publicKey = publicKey;
}

/*
	@name: setm_encryptedArray
	@description: Setter of attribute m_encryptedArray.

	@param: The method setm_encryptedArray takes one mandatory parameter: a EncryptedArray.
	-param1: the new encrypted array for m_encryptedArray.
*/
void CyCtxt::setm_encryptedArray(EncryptedArray *encryptedArray) {
	this->m_encryptedArray = encryptedArray;
}

/*
	@name: setm_numberOfSlots
	@description: Setter of attribute m_numberOfSlots.

	@param: The method setm_numberOfSlots takes one mandatory parameter: a long.
	-param1: the new number of slots for m_numberOfSlots.
*/
void CyCtxt::setm_numberOfSlots(long numberOfSlots) {
	this->m_numberOfSlots = numberOfSlots;
}


/******IMPLEMENTATION OF PRIVATE METHODS******/



/******IMPLEMENTATION OF PUBLIC METHODS******/

//ENCRYPTION
/*
	@name: encrypt
	@description: Public method which allow to encrypt a provided vector which corresponds to vector to encrypt, creates the corresponding CyCtxt and return it.

	@param: The method encrypt takes one mandatory parameter: a vector of long.
	-param1: a mandatory vector of long which corresponds to vector to encrypt.

	@return: Return a CyCtxt which corresponds to encrypted vector.
*/
CyCtxt CyCtxt::encrypt(vector<long> &ptxt_vect) const {
	// Create a vector of size nddSlots and fill it first with values from plaintext, then with zeros.
	long vector_size = ptxt_vect.size();
	// Empty cyphertext object.
	CyCtxt ctxt_vect(*m_publicKey, vector_size);
	// If the user try to encrypt a vector with a size greater than the maximum slots we can encrypt, then return an error.
	if(vector_size>m_numberOfSlots){
		cerr<<"Error: the size of the plaintext vector to encrypt cannot be greater than the number of slot"<<m_numberOfSlots<<"of the CyCtxt object."<<endl;
	}
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

	// Set the encryption informations in the CyCtxt
	ctxt_vect.setm_publicKey(m_publicKey);// Set the public key of Cyfhel object used to encrypt in the CyCtxt
	ctxt_vect.setm_encryptedArray(m_encryptedArray);// Set the encrypted array of Cyfhel object used to encrypt in the CyCtxt
	ctxt_vect.setm_numberOfSlots(m_numberOfSlots);// Set the number of slots of Cyfhel object used to encrypt in the CyCtxt
	// Return the homeomorphic cypher vector of ptxt_vect: the CyCtxt ctxt_vect.
	return ctxt_vect;
}

// Scalar product: [1, 2, 3].[4, 5, 6] = [32, 32, 32] (because (1 * 4) + (2 * 5) + (3 * 6) = 32).
void CyCtxt::scalarProd(CyCtxt const& cy){
    // Called the multiplyBy method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy. Multiply the two CyCtxt.
    this->multiplyBy(cy);
    // Sum the elements of the resulting CyCtxt.
    totalSums(*m_encryptedArray, *this);
}

// Negate (what does it do?).
CyCtxt CyCtxt::returnNegate() const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Called the square method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy.
    this_copy.negate();
    // Return the result ie the square of the initial CyCtxt.
    return this_copy;
}

// Scalar product: [1, 2, 3].[4, 5, 6] = [32, 32, 32] (because (1 * 4) + (2 * 5) + (3 * 6) = 32).
CyCtxt CyCtxt::returnScalarProd(CyCtxt const& cy) const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Called the multiplyBy method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy. Multiply the two CyCtxt.
    this_copy.multiplyBy(cy);
    // Sum the elements of the resulting CyCtxt.
    totalSums(*m_encryptedArray, this_copy);
    // Return the result ie the square of the initial CyCtxt.
    return this_copy;
}

// Cumulative sum: returnCumSum([1, 2, 3]) = [6, 6, 6] (because 6 = 1 + 2 + 3).
CyCtxt CyCtxt::returnCumSum() const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Sum the elements of the resulting CyCtxt.
    totalSums(*m_encryptedArray, this_copy);
    // Return the result ie the square of the initial CyCtxt.
    return this_copy;
}

// Return the square: returnSquare([1, 2, 3]) = [1, 4, 9] (because 1² = 1; 2² = 4; 3² = 9).
CyCtxt CyCtxt::returnSquare() const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Called the square method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy.
    this_copy.square();
    // Return the result ie the square of the initial CyCtxt.
    return this_copy;
}

// Return the cube: returnCube([1, 2, 3]) = [1, 8, 27] (because 1³ = 1; 2³ = 8; 3³ = 27).
CyCtxt CyCtxt::returnCube() const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Called the cube method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy.
    this_copy.cube();
    // Return the result ie the cube of the initial CyCtxt.
    return this_copy;
}



/******IMPLEMENTATION OF PUBLIC METHODS: COMPARISON OPERATORS OVERLOAD******/


/******IMPLEMENTATION OF PUBLIC METHODS: SHORTCUT OPERATORS OVERLOAD******/



/******IMPLEMENTATION OF PUBLIC METHODS: ARITHMETIC OPERATORS OVERLOAD******/

// Sum of two CyCtxt.
CyCtxt operator+ (CyCtxt const& cy1, CyCtxt const& cy2){
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy1_copy(cy1);
	// Called the operator += of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	cy1_copy += cy2;
	// Return the result ie the sum of the two CyCtxt.
	return cy1_copy;
}

// Substraction of two CyCtxt.
CyCtxt operator- (CyCtxt const& cy1, CyCtxt const& cy2){
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy1_copy(cy1);
	// Called the operator -= of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	cy1_copy -= cy2;
	// Return the result ie the substraction of the two CyCtxt.
	return cy1_copy;
}

// Multiplication of two CyCtxt.
CyCtxt operator* (CyCtxt const& cy1, CyCtxt const& cy2){
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy1_copy(cy1);
	// Called the operator *= of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	cy1_copy *= cy2;
	// Return the result ie the multiplication of the two CyCtxt.
	return cy1_copy;
}

// Scalar product of two CyCtxt.
CyCtxt operator% (CyCtxt const& cy1, CyCtxt const& cy2){
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy1_copy(cy1);
	// Called the operator scalarProd of class CyCtxt to modify the copy of cy1: cy1_copy.
	cy1_copy.scalarProd(cy2);
	// Return the result ie the multiplication of the two CyCtxt.
	return cy1_copy;
}



// Sum of a long and a CyCtxt.
CyCtxt operator+ (long a, CyCtxt const& cy){
	// Create a vector of size the original size of plaintext corresponding to cy, and with value a.
	vector<long> vect_a(cy.getm_sizeOfPlaintext(), a);
	// Encrypt the vector vect_a.
	CyCtxt c_a = cy.encrypt(vect_a);
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy_copy(cy);
	// Called the operator += of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	 c_a += cy_copy;
	// Return the result ie the sum of a long and a CyCtxt.
	return c_a;
}

// Substraction of a long and a CyCtxt.
CyCtxt operator- (long a, CyCtxt const& cy){
	// Create a vector of size the original size of plaintext corresponding to cy, and with value a.
	vector<long> vect_a(cy.getm_sizeOfPlaintext(), a);
	// Encrypt the vector vect_a.
	CyCtxt c_a = cy.encrypt(vect_a);
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy_copy(cy);
	// Called the operator -= of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	 c_a -= cy_copy;
	// Return the result ie the substraction of a long and a CyCtxt.
	return c_a;
}

// Multiplication of a long and a CyCtxt.
CyCtxt operator* (long a, CyCtxt const& cy){
	// Create a vector of size the original size of plaintext corresponding to cy, and with value a.
	vector<long> vect_a(cy.getm_sizeOfPlaintext(), a);
	// Encrypt the vector vect_a.
	CyCtxt c_a = cy.encrypt(vect_a);
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy_copy(cy);
	// Called the operator *= of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	 c_a *= cy_copy;
	// Return the result ie the multiplication of a long and a CyCtxt.
	return c_a;
}


// Sum of a CyCtxt and a long.
CyCtxt operator+ (CyCtxt const& cy, long a){
	// Create a vector of size the original size of plaintext corresponding to cy, and with value a.
	vector<long> vect_a(cy.getm_sizeOfPlaintext(), a);
	// Encrypt the vector vect_a.
	CyCtxt c_a = cy.encrypt(vect_a);
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy_copy(cy);
	// Called the operator += of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	cy_copy += c_a;
	// Return the result ie the sum of the two CyCtxt.
	return cy_copy;
}

// Substraction of a CyCtxt and a long.
CyCtxt operator- (CyCtxt const& cy, long a){
	// Create a vector of size the original size of plaintext corresponding to cy, and with value a.
	vector<long> vect_a(cy.getm_sizeOfPlaintext(), a);
	// Encrypt the vector vect_a.
	CyCtxt c_a = cy.encrypt(vect_a);
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy_copy(cy);
	// Called the operator -= of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	 cy_copy -= c_a;
	// Return the result ie the sum of the two CyCtxt.
	return cy_copy;
}

// Multiplication of a CyCtxt and a long.
CyCtxt operator* (CyCtxt const& cy, long a){
	// Create a vector of size the original size of plaintext corresponding to cy, and with value a.
	vector<long> vect_a(cy.getm_sizeOfPlaintext(), a);
	// Encrypt the vector vect_a.
	CyCtxt c_a = cy.encrypt(vect_a);
	// Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
	CyCtxt cy_copy(cy);
	// Called the operator *= of class CyCtxt inherit from class Ctxt to modify the copy of cy1: cy1_copy.
	 cy_copy *= c_a;
	// Return the result ie the sum of the two CyCtxt.
	return cy_copy;
}


/******IMPLEMENTATION OF PUBLIC METHODS: STREAM OPERATORS OVERLOAD******/





