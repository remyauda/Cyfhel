/*
#   Benchmark_Plus
#   --------------------------------------------------------------------
#   Perform tests on operation +. 
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
#include "LibMatrix.h"

#include <cassert>
#include <cstdio>

#include "polyEval.h"

/* The vector size of the plaintext that we will use for the demo.*/
#define VECTOR_SIZE 5

/* Define the max value of an element in the vector when the user choose the random vectors (value will be choosen between 0 and RANGEOFRANDOM).*/
#define RANGEOFRANDOM 10

/* Define if the polynome is monic or not during the tests.*/
#define isMonic 0

/* Define the number of execution of Benchmark*/
#define NB_BENCHMARK 1000


int main(int argc, char *argv[])
{
	vector<double> vectorBenchmark;// Vector for store execution time.

	vector<long> v1; // Initialization of v1.
	vector<long> v2; // Initialization of v2.

	// Initialization of v1.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v1.push_back(i);  
	}

	// Initialization of v2.
	for(int i=0; i<VECTOR_SIZE; i++)
	{
		v2.push_back(2);  
	}
	
    // Display the title of the program.
    std::cout <<"" <<endl;
    std::cout <<"     ************Benchmark_Plus************" <<endl;
    std::cout <<"" <<endl;

    // Create object Cyfhel and enable print for all functions.
    // Cyfhel is an object that create keys for homeomorphism encryption with the parameter used in its constructor. 
    // If no parameter are provided, uses default values for the generation of the keys.
    // Cyfhel is an object that allow the user to encrypt and decrypt vectors in a homeomorphism way.
    std::cout <<"******Generation of the keys for encryption******"<<endl<<endl;

	// Use this initialization for strong encryption. However, the computation time will be greater (takes several minutes at least).
	Cyfhel cy(true);

    // Encrypted the two plaintexts to have two Cypher texts that are encrypted in an homeomorphic way with the key generated during the construction of object Cyfhel. 
    // These two Cypher txt will be use for the test on the homeomorphic operation (+=, -=, *=, ...).
    CyCtxt c1 = cy.encrypt(v1);
    CyCtxt c2 = cy.encrypt(v2);

	for(int k=0; k<NB_BENCHMARK; k++)
	{

		std::cout <<"******Perform + operation "<< k+1 <<"******"<<endl<<endl;

		// Begin the chrono.
		Timer timerDemo(true);
		timerDemo.start();

		//Perform + operation
		// Sum of the two cypher text.
		CyCtxt cAdd1_2 = c1 + c2;

		// Stop the chrono and display the execution time.
		timerDemo.stop();
		timerDemo.benchmarkInSeconds();
		timerDemo.benchmarkInHoursMinutesSecondsMillisecondes(true);
		timerDemo.benchmarkInYearMonthWeekHourMinSecMilli(true);

		vectorBenchmark.push_back(timerDemo.getm_benchmarkSecond());//Push in the vector the execution time in seconds.
	}

	double averageOfExecutionTime = std::accumulate( vectorBenchmark.begin(), vectorBenchmark.end(), 0.0)/vectorBenchmark.size();// Compute the average of execution time.

	LibMatrix::writeDoubleInFileWithEraseData("Result_Benchmark_Plus", averageOfExecutionTime);// Write the double averageOfExecutionTime in the file Result_Benchmark_Plus in the directory ResultOfBenchmark.

	LibMatrix::writeStringInFileWithEraseData("ResultVerbose_Benchmark_Plus", LibMatrix::transformSecondToYearMonthWeekHourMinSecMilli(averageOfExecutionTime));// Write the string verbose to transform the average of execution time in seconds to string verbose Years, Months, Weeks, Hours, Minutes, Seconds, Milliseconds in the file ResultVerbose_Benchmark_Plus in the directory ResultOfBenchmark.

    // Skip a line.
    std::cout <<"\n"<<endl;

    // Display the end of the program.
    std::cout <<"     ************End of Benchmark_Plus************" <<endl;

    // Skip a line.
    std::cout <<"\n"<<endl;

    // If success, return 0.
    return 0;
};

