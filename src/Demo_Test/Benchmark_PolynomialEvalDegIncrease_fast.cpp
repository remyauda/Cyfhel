/*
#   Benchmark_PolynomialEvalDegIncrease_fast
#   --------------------------------------------------------------------
#   Perform tests on operation PolynomialEval with degree increase.
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
#include <numeric>

/* The vector size of the plaintext that we will use for the demo. The effective size of evaluation points vector will be: (VECTOR_SIZE+1).*/
#define VECTOR_SIZE 99

/* The maximum degree that we will use for the demo.*/
#define DEGREE_MAX 10

/* Define the max value of an element in the vector when the user choose the random vectors (value will be choosen between 0 and RANGEOFRANDOM).*/
#define RANGEOFRANDOM 10

/* Define if the polynome is monic or not during the tests.*/
#define isMonic 0

/* Define the number of execution of Benchmark*/
#define NB_BENCHMARK 3


int main(int argc, char *argv[])
{
    // Remove all data in Result_Benchmark_PolynomialEvalDegIncrease and ResultVerbose_Benchmark_PolynomialEvalDegIncrease file to have clear text file.
    LibMatrix::removeAllDataInFile("Result_Benchmark_PolynomialEvalDegIncrease_fast");
    LibMatrix::removeAllDataInFile("ResultVerbose_Benchmark_PolynomialEvalDegIncrease_fast");

    // Boolean to know if the encrypted evaluation is the same that the unencrypted one.
    bool isEncryptedEvaluationTrue = true;

    // Display the title of the program.
    std::cout <<"" <<endl;
    std::cout <<"     ************Benchmark_PolynomialEvalDegIncrease_fast************" <<endl;
    std::cout <<"" <<endl;

    // Create object Cyfhel and enable print for all functions.
    // Cyfhel is an object that create keys for homeomorphism encryption with the parameter used in its constructor.
    // If no parameter are provided, uses default values for the generation of the keys.
    // Cyfhel is an object that allow the user to encrypt and decrypt vectors in a homeomorphism way.
    std::cout <<"******Generation of the keys for encryption******"<<endl<<endl;

    // Use this initialization for strong encryption. However, the computation time will be greater (takes several minutes at least).
    Cyfhel cy(true, 2, 8, 2, 1, 128, 64, 26);

    // Definition of a vector of points for polynomial evaluation.
    vector<long> vectorPtsEval;

    // Initialization of the vector of points for polynomial evaluation.
    
	for(int i=0; i<=VECTOR_SIZE; i++){
		vectorPtsEval.push_back(i);
	}
    
    // Counter for the degree of polynome.
    int degree = 0; 

    // Definition of the coefficients of the polynome.
    vector<long> coeffPoly;

    // Initialization of the vector of coefficients for polynome.
    coeffPoly.push_back(1);
        
    // While the degree of the polynome has not reach the max value...
    while(degree<=DEGREE_MAX)
    {
        vector<double> vectorBenchmark;// Vector for store execution time.
        std::cout <<endl<<endl<<"     ******Degree of polynome equal to "<< degree <<"******";

        // With the current number of evaluation points and with the current degree for the polynome, do NB_BENCHMARK benchmarks, store each computation time and take the average.
        for(int k=0; k<NB_BENCHMARK; k++)
        {
            std::cout <<endl<<endl<<"******Perform polynomialEval operation "<< k+1 <<"******"<<endl<<endl;

            // Begin the chrono.
            Timer timerDemo(true);
            timerDemo.start();

            // Polynomial evaluation.
            CyCtxt cEvalPoly_cyfhel = cy.polynomialEval(vectorPtsEval, coeffPoly);

            // Stop the chrono and display the execution time.
            timerDemo.stop();
            timerDemo.benchmarkInSeconds();
            timerDemo.benchmarkInHoursMinutesSecondsMillisecondes(true);
            timerDemo.benchmarkInYearMonthWeekHourMinSecMilli(true);

            vectorBenchmark.push_back(timerDemo.getm_benchmarkSecond());//Push in the vector the execution time in seconds.

            // ***Decrypt the CyCtxt which contain the polynomial evaluation.***
            vector<long> vect_polynomialEval = cy.decrypt(cEvalPoly_cyfhel);
            // The user can then verify if the result of the polynomial evaluation is the same that the polynomial evaluation without encryption.
            std::cout <<endl<<"Decrypt(P(encrypt(x))) mod("<< cy.getp2r() <<") -> "<< vect_polynomialEval<<endl;

            // Verify if the result of the Polynomial evaluation of the encrypted vector is the same that the polynomial evaluation of the vector without encryption.
            vector<long> plainTextPolyEval;
            // Polynomial evaluation on plaintext vector.
            ZZX polynome = cy.createPolynomeWithCoeff(coeffPoly); // Create a ZZX polynome with the coefficients provide by the user.
            // Perform the polynomial evaluation for all the elements of the plaintext evalauation points vector and put it in the vector plainTextPolyEval.
            for (unsigned long j=0;j<vectorPtsEval.size(); j++)
            {
                long elementPolyEval = polyEvalMod(polynome, vectorPtsEval[j], cy.getp2r()); // Polynomial evaluation of the ieme element of the plaintext vector.
                plainTextPolyEval.push_back(elementPolyEval); // Push the polynomial evaluation of the ieme element of the plaintext vector within the vector plainTextPolyEval.
            }
            // Display the plaintext vector that contain the polynomial evaluation of vectorPtsEval ie P(x).
            std::cout <<"P(x) mod("<< cy.getp2r() <<") -> "<< plainTextPolyEval<<endl;
            // If Decrypt(P(encrypt(x))) equal to P(x), the homeomorphic operation works and so it is a success. Else, it is a fail.
            if(vect_polynomialEval == plainTextPolyEval)
            {
                std::cout <<"Homeomorphic operation polynomeEval is a success: Decrypt(P(encrypt(x))) equal to P(x).\n"<<endl;
                isEncryptedEvaluationTrue = true;
            }
            else if(vect_polynomialEval != plainTextPolyEval)
            {
                std::cout <<"Homeomorphic operation polynomeEval is a fail: Decrypt(P(encrypt(x))) not equal to P(x)."<<endl;
                isEncryptedEvaluationTrue = false;
                break;
            }
            else
            {
                std::cout <<"Error: unexpected result during the comparison of vect_polynomialEval and plainTextPolyEval."<<endl;
                isEncryptedEvaluationTrue = false;
                break;
            }
        }//end for nb benchmark.

        // If Decrypt(P(encrypt(x))) equal to P(x), the homeomorphic operation don't works, stop the benchmark.
        if(!isEncryptedEvaluationTrue)
        {
            break;
         }
            
         // Compute the average of execution time.
         double averageOfExecutionTime = std::accumulate(vectorBenchmark.begin(), vectorBenchmark.end(), 0.0)/vectorBenchmark.size();
            
         // Write the double averageOfExecutionTime in the file Result_Benchmark_PolynomialEvalDegIncrease in the directory ResultOfBenchmark.
         LibMatrix::writeDoubleInFileWithoutEraseData("Result_Benchmark_PolynomialEvalDegIncrease_fast", averageOfExecutionTime);

         // Write the string verbose to transform the average of execution time in seconds to string verbose Years, Months, Weeks, Hours, Minutes, Seconds, Milliseconds in the file ResultVerbose_Benchmark_PolynomialEvalDegIncrease in the directory ResultOfBenchmark.
         LibMatrix::writeStringInFileWithoutEraseData("ResultVerbose_Benchmark_PolynomialEvalDegIncrease_fast", LibMatrix::transformSecondToYearMonthWeekHourMinSecMilli(averageOfExecutionTime));

         //Increase counter. The counter is equal to degree of the polynome.
         degree++;

         // Increase vector of coefficients for polynome. Therefore, the degree of polynome increase.
         coeffPoly.push_back(1);
    }
        
// Write a separator in the benchmark file.
string separator = "******Test with " + std::to_string(VECTOR_SIZE+1) + " evaluation points******";

LibMatrix::writeStringInFileWithoutEraseData("Result_Benchmark_PolynomialEvalDegIncrease_fast", separator);

LibMatrix::writeStringInFileWithoutEraseData("ResultVerbose_Benchmark_PolynomialEvalDegIncrease_fast", separator);

// Skip a line.
std::cout <<"\n"<<endl;

// Display the end of the program.
std::cout <<"     ************End of Benchmark_PolynomialEvalDegIncrease_fast************" <<endl;

// Skip a line.
std::cout <<"\n"<<endl;

// If success, return 0.
return 0;
};

