/*
 * Timer
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

#include "Timer.h"

using namespace std;

/******CONSTRUCTOR BY DEFAULT******/
Timer::Timer(){
	m_start = 0;
	m_stop = 0;
	m_isVerbose = false;
	m_benchmarkSecond = 0.0;
}

/******CONSTRUCTOR WITH PARAMETERS******/
Timer::Timer(bool isVerbose){
	m_start = 0;
	m_stop = 0;
	m_isVerbose = isVerbose;
	m_benchmarkSecond = 0.0;
}

/******DESTRUCTOR BY DEFAULT******/
Timer::~Timer(){}


/******GETTERS******/
double Timer::getm_benchmarkSecond(){
	return m_benchmarkSecond;
}


/******IMPLEMENTATION OF PRIVATE METHODS******/
double Timer::getTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 1e-6;
}

/******IMPLEMENTATION OF PUBLIC METHODS******/

void Timer::start() {
	m_start = getTime();
}

void Timer::stop() {
	m_stop = getTime();
}

double Timer::benchmarkInSeconds() {
	m_benchmarkSecond = m_stop - m_start;
	if(m_isVerbose)
	{
		std::cout << "Elapsed time: " << m_benchmarkSecond << endl;
	}
	return m_benchmarkSecond;
}

double Timer::benchmarkInSeconds(bool const& isVerbose) {
	m_benchmarkSecond = m_stop - m_start;
	if(isVerbose)
	{
		std::cout << "Elapsed time: " << m_benchmarkSecond << endl;
	}
	return m_benchmarkSecond;
}


void Timer::benchmarkInHoursMinutesSecondsMillisecondes(bool const& isVerbose) {
	m_benchmarkSecond = m_stop - m_start;
	int intBenchmarkSecond = m_benchmarkSecond;
	int intBenchmarkMillisecond = ((m_benchmarkSecond - intBenchmarkSecond) * 1000);
	intBenchmarkMillisecond %= 1000;//Milliseconds
	m_benchmarkHourMinuteSecondMillisecond[3] = intBenchmarkMillisecond;
	m_benchmarkHourMinuteSecondMillisecond[2] = intBenchmarkSecond % 60;//Seconds
	m_benchmarkHourMinuteSecondMillisecond[1] = (intBenchmarkSecond / 60) % 60;//Minutes
	m_benchmarkHourMinuteSecondMillisecond[0] = (intBenchmarkSecond / 3600);//Hours
	if(isVerbose)
	{
		std::cout << "Elapsed time: " << m_benchmarkHourMinuteSecondMillisecond[0] << " hours " << m_benchmarkHourMinuteSecondMillisecond[1] << " minutes " << m_benchmarkHourMinuteSecondMillisecond[2] << " seconds " << m_benchmarkHourMinuteSecondMillisecond[3] << " millisecondes." << endl;
	}
}


void Timer::benchmarkInYearMonthWeekHourMinSecMilli(bool const& isVerbose) {
	m_benchmarkSecond = m_stop - m_start;
	int intBenchmarkSecond = m_benchmarkSecond;
	int intBenchmarkMillisecond = ((m_benchmarkSecond - intBenchmarkSecond) * 1000);
	intBenchmarkMillisecond %= 1000;//Milliseconds
	m_benchmarkYearMonthWeekDayHourMinSecMilli[7] = intBenchmarkMillisecond;
	m_benchmarkYearMonthWeekDayHourMinSecMilli[6] = intBenchmarkSecond % 60;//Seconds
	m_benchmarkYearMonthWeekDayHourMinSecMilli[5] = (intBenchmarkSecond / 60) % 60;//Minutes
	m_benchmarkYearMonthWeekDayHourMinSecMilli[4] = (intBenchmarkSecond / 3600) % 24;//Hours
	m_benchmarkYearMonthWeekDayHourMinSecMilli[3] = (intBenchmarkSecond / 86400) % 7;//Days
	m_benchmarkYearMonthWeekDayHourMinSecMilli[2] = (intBenchmarkSecond / 604800) % 4;//Weeks
	m_benchmarkYearMonthWeekDayHourMinSecMilli[1] = (intBenchmarkSecond / 2419200) % 24;//Months
	m_benchmarkYearMonthWeekDayHourMinSecMilli[0] = (intBenchmarkSecond / 58060800);//Years
	if(isVerbose)
	{
		std::cout << "Elapsed time: " << m_benchmarkYearMonthWeekDayHourMinSecMilli[0] << " years " << m_benchmarkYearMonthWeekDayHourMinSecMilli[1] << " months " << m_benchmarkYearMonthWeekDayHourMinSecMilli[2] << " weeks " << m_benchmarkYearMonthWeekDayHourMinSecMilli[3] << " days " << m_benchmarkYearMonthWeekDayHourMinSecMilli[4] << " hours " << m_benchmarkYearMonthWeekDayHourMinSecMilli[5] << " minutes " << m_benchmarkYearMonthWeekDayHourMinSecMilli[6] << " seconds " << m_benchmarkYearMonthWeekDayHourMinSecMilli[7] << " milliseconds." << endl;
	}
}





