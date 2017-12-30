#ifndef TIMER_H
#define TIMER_H

#include "Cyfhel.h"

class Timer{

 private:
    /******ATTRIBUTES******/
	double m_start;
	double m_stop;
	bool m_isVerbose;
	double m_benchmarkSecond;
	double m_benchmarkHourMinuteSecondMillisecond[4];

    /******PROTOTYPES OF PRIVATE METHODS******/
	double getTime();

 public:

    /******CONSTRUCTOR BY DEFAULT******/
	Timer();

    /******CONSTRUCTOR WITH PARAMETERS******/
	Timer(bool isVerbose=false);

    /******DESTRUCTOR BY DEFAULT******/
	virtual ~Timer();

    /******GETTERS******/
	double getm_benchmarkSecond();//Getter of attribute m_benchmarkSecond

    /******PROTOTYPES OF PUBLIC METHODS******/
	void start();
	void stop();
	double benchmarkInSeconds();
	double benchmarkInSeconds(bool const& isVerbose);
	void benchmarkInHoursMinutesSecondsMillisecondes(bool const& isVerbose);
};

#endif
