/*
#   Benchmark_Square
#   --------------------------------------------------------------------
#   Perform tests on operation Square. 
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

#include "polyEval.h"

/* The vector size of the plaintext that we will use for the demo.*/
#define VECTOR_SIZE 5

/* Define the max value of an element in the vector when the user choose the random vectors (value will be choosen between 0 and RANGEOFRANDOM).*/
#define RANGEOFRANDOM 10

/* Define if the polynome is monic or not during the tests.*/
#define isMonic 0


int main(int argc, char *argv[])
{
	vector<long> v1; // Initialization of v1.

	// Initialization of v1.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v1.push_back(i);  
	}
	
    // Display the title of the program.
    std::cout <<"" <<endl;
    std::cout <<"     ************Benchmark_Square************" <<endl;
    std::cout <<"" <<endl;

    // Create object Cyfhel and enable print for all functions.
    // Cyfhel is an object that create keys for homeomorphism encryption with the parameter used in its constructor. 
    // If no parameter are provided, uses default values for the generation of the keys.
    // Cyfhel is an object that allow the user to encrypt and decrypt vectors in a homeomorphism way.
    std::cout <<"******Generation of the keys for encryption******"<<endl;

	// Use this initialization for strong encryption. However, the computation time will be greater (takes several minutes at least).
	Cyfhel cy(true);

    // Skip a line.
    std::cout <<"\n"<<endl;

	std::cout <<"******Homeomorphic encryption******"<<endl;

    // Encrypted the plaintext to have Cypher text that are encrypted in an homeomorphic way with the key generated during the construction of object Cyfhel. 
    CyCtxt c1 = cy.encrypt(v1);

	// Begin the chrono.
    Timer timerDemo(true);
    timerDemo.start();

	// Square of the first cypher text.
    CyCtxt cSquare1 = c1.returnSquare();

	// Stop the chrono and display the execution time.
    timerDemo.stop();
    timerDemo.benchmarkInSeconds();
    timerDemo.benchmarkInHoursMinutesSecondsMillisecondes(true);
	timerDemo.benchmarkInYearMonthWeekHourMinSecMilli(true);

    // Skip a line.
    std::cout <<"\n"<<endl;

    // Display the end of the program.
    std::cout <<"     ************End of Benchmark_Square************" <<endl;

    // Skip a line.
    std::cout <<"\n"<<endl;

    // If success, return 0.
    return 0;
};

