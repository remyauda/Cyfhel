#ifndef LIBMATRIX_H_INCLUDED
#define LIBMATRIX_H_INCLUDED

#include "Cyfhel.h"


class LibMatrix{

private:

    /******ATTRIBUT******/
    int m_lm1;

public:

    /******CONSTRUCTEUR PAR DEFAUT******/
	LibMatrix():m_lm1(0){}

    /******PROTOTYPES DES METHODES******/

    static int loadIntInFile(std::string const& stringLevel);//Allow to return a int read in a file.

    static int loadIntInFileWithDirectory(std::string const& stringLevel, std::string const& stringNameDirectory="ResultOfBenchmark/");//Allow to return a int read in a file which is located in one in one or in multiple directories.

    static void writeDoubleInFileWithEraseData(std::string const& stringNameFile, double const& valueWrite, std::string const& stringNameDirectory="ResultOfBenchmark/");//Allows you to write a double taken as parameter in a file by erasing its contents beforehand.

    static void writeDoubleInFileWithoutEraseData(std::string const& stringNameFile, double const& valueWrite, std::string const& stringNameDirectory="ResultOfBenchmark/");//Allows you to write a double taken as parameter in a file without first deleting its contents.

	static void writeStringInFileWithEraseData(std::string const& stringNameFile, std::string const& stringWrite, std::string const& stringNameDirectory="ResultOfBenchmark/");//Allows you to write a string taken as parameter in a file by first deleting its contents.

	static void writeStringInFileWithoutEraseData(std::string const& stringNameFile, std::string stringWrite, std::string const& stringNameDirectory="ResultOfBenchmark/");//Allows you to write a string taken as parameter in a file without first erasing its contents.

	static void removeAllDataInFile(std::string const& stringNameFile, std::string const& stringNameDirectory="ResultOfBenchmark/");//Erases all the contents of a file.


	static std::string transformSecondToYearMonthWeekHourMinSecMilli(double const& seconds);//Transform seconds to string verbose Years, Months, Weeks, Hours, Minutes, Seconds, Milliseconds

};

#endif // LIBMATRIX_H_INCLUDED
