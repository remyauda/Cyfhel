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


/******IMPLEMENTATION OF SETTERS******/



/******IMPLEMENTATION OF PRIVATE METHODS******/


/******IMPLEMENTATION OF PUBLIC METHODS******/

CyCtxt CyCtxt::square() const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Called the square method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy.
    this_copy = this->square();
    // Return the result ie the square of the initial CyCtxt.
    return this_copy;
}

CyCtxt CyCtxt::cube() const{
    // Empty cyphertext object. Use of the copy constructor of class CyCtxt inherit from class Ctxt.
    CyCtxt this_copy(*this);
    // Called the cube method inherit from class Ctxt to modify the copy of current CyCtxt: this_copy.
    this_copy = this->cube();
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


/******IMPLEMENTATION OF PUBLIC METHODS: STREAM OPERATORS OVERLOAD******/





