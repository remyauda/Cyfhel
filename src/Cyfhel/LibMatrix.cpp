/*
 * LibMatrix
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

#include "LibMatrix.h"

using namespace std;

/******IMPLEMENTATION DES METHODES******/

/*
@description:
La methode loadIntInFile permet de retourner un int lu dans dans un fichier.
@param: La methode loadIntInFile prend un parametre obligatoire: un string.
-param1: le string permet de specifier le chemin du fichier contenant le int que l'on veut lire et retourner.
*/
int LibMatrix::loadIntInFile(string const& stringLevel) {

    ifstream flux(stringLevel.c_str(),ios::app);//ouverture du flux de lecture
    int valueInt;

    if(flux)//si l'ouverture du fichier reussi
    {
        while(flux>>valueInt){//on lit le fichier mot par mot jusqu'a la fin de celui-ci
            return valueInt;
        }

    }
    else// En cas de probleme lors de l'ouverture du fichier
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;// on affiche un message d'erreur
		return -1;
    }
	return -1;
}


/*
@description:
La methode loadIntInFileWithDirectory permet de retourner un int lu dans dans un fichier qui se trouve dans un ou dans de multiples repertoires.
@param: La methode loadIntInFileWithDirectory prend un parametre obligatoire et un parametre facultatif: un string et un string facultatif.
-param1: Le string pris en premier parametre permet de specifier le fichier contenant le int que l'on veut lire et retourner.
-param2: Le string facultatif pris en second parametre permet de specifier le nom du repertoire ou des multiples repertoires ou est stocke le fichier contenant le int que l'on veut lire et retourner. Par defaut, le repertoire se nomme Files.
*/
int LibMatrix::loadIntInFileWithDirectory(string const& stringNameFile, string const& stringNameDirectory) {

    //On construit petit a petit l'arborescence du chemin pour trouver les fichiers
    string stringPathFile=stringNameDirectory;
    stringPathFile+=stringNameFile;
    stringPathFile+=".txt";

    ifstream flux(stringPathFile.c_str(),ios::app);//ouverture du flux de lecture
    int valueInt;

    if(flux)//si l'ouverture du fichier reussi
    {
        while(flux>>valueInt){//on lit le fichier mot par mot jusqu'a la fin de celui-ci
            return valueInt;
        }

    }
    else// En cas de probleme lors de l'ouverture du fichier
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;// on affiche un message d'erreur
		return -1;
    }
	return -1;
}


/*
@description:
La methode writeDoubleInFileWithEraseData permet d'ecrire un double pris en parametre dans dans un fichier en effacant au prealable son contenu.
@param: La methode writeDoubleInFileWithEraseData prend deux parametres obligatoires et un parametre facultatif: un string, un double et un string facultatif.
-param1: Le string permet de specifier le nom du ficher dont on souhaite ecrire le int passee en second parametre.
-param2: Le double permet de specifier le double que l'on souhaite ecrire dans le ficher en ayant efface au prealable son contenu.
-param3: Le string facultatif permet de specifier le nom du repertoire ou des multiples repertoires ou est stocke le fichier dont on souhaite ecrire le double passee en second parametre. Par defaut, le repertoire se nomme ResultOfBenchmark/.
*/
void LibMatrix::writeDoubleInFileWithEraseData(string const& stringNameFile, double const& valueWrite, string const& stringNameDirectory){

    //On construit petit a petit l'arborescence du chemin pour trouver les fichiers
    string stringPathFile=stringNameDirectory;
    stringPathFile+=stringNameFile;
    stringPathFile+=".txt";

    ofstream flux(stringPathFile.c_str());//ouverture en mode ecriture avec ecrasement des donnees
    //si l'ouverture du fichier reussi
    if(flux){
        flux<<valueWrite;//Ecriture du double dans le fichier
    }
    //En cas de probleme lors de l'ouverture du fichier
    else{
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;// on affiche un message d'erreur
    }
}

/*
@description:
La methode writeStringInFileWithEraseData permet d'ecrire un double pris en parametre dans dans un fichier en effacant au prealable son contenu.
@param: La methode writeStringInFileWithEraseData prend deux parametres obligatoires et un parametre facultatif: un string, un string et un string facultatif.
-param1: Le string permet de specifier le nom du ficher dont on souhaite ecrire le int passee en second parametre.
-param2: Le string permet de specifier le string que l'on souhaite ecrire dans le ficher en ayant efface au prealable son contenu.
-param3: Le string facultatif permet de specifier le nom du repertoire ou des multiples repertoires ou est stocke le fichier dont on souhaite ecrire le double passee en second parametre. Par defaut, le repertoire se nomme ResultOfBenchmark/.
*/
void LibMatrix::writeStringInFileWithEraseData(string const& stringNameFile, string const& stringWrite, string const& stringNameDirectory){

    //On construit petit a petit l'arborescence du chemin pour trouver les fichiers
    string stringPathFile=stringNameDirectory;
    stringPathFile+=stringNameFile;
    stringPathFile+=".txt";

    ofstream flux(stringPathFile.c_str());//ouverture en mode ecriture avec ecrasement des donnees
    //si l'ouverture du fichier reussi
    if(flux){
        flux<<stringWrite;//Ecriture du string dans le fichier
    }
    //En cas de probleme lors de l'ouverture du fichier
    else{
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;// on affiche un message d'erreur
    }
}


/*
@description:
The transformSecondToYearMonthWeekHourMinSecMilli method allows to transform seconds to string verbose Years, Months, Weeks, Hours, Minutes, Seconds, Milliseconds.
@param: The transformSecondToYearMonthWeekHourMinSecMilli method takes one mandatory parameter: a double.
-param1: A double allows to specify the seconds to transform.
*/
string LibMatrix::transformSecondToYearMonthWeekHourMinSecMilli(double const& seconds){
	double benchmarkYearMonthWeekDayHourMinSecMilli[8];
	int intBenchmarkSecond = seconds;
	int intBenchmarkMillisecond = ((seconds - intBenchmarkSecond) * 1000);
	intBenchmarkMillisecond %= 1000;//Milliseconds
	benchmarkYearMonthWeekDayHourMinSecMilli[7] = intBenchmarkMillisecond;
	benchmarkYearMonthWeekDayHourMinSecMilli[6] = intBenchmarkSecond % 60;//Seconds
	benchmarkYearMonthWeekDayHourMinSecMilli[5] = (intBenchmarkSecond / 60) % 60;//Minutes
	benchmarkYearMonthWeekDayHourMinSecMilli[4] = (intBenchmarkSecond / 3600) % 24;//Hours
	benchmarkYearMonthWeekDayHourMinSecMilli[3] = (intBenchmarkSecond / 86400) % 7;//Days
	benchmarkYearMonthWeekDayHourMinSecMilli[2] = (intBenchmarkSecond / 604800) % 4;//Weeks
	benchmarkYearMonthWeekDayHourMinSecMilli[1] = (intBenchmarkSecond / 2419200) % 24;//Months
	benchmarkYearMonthWeekDayHourMinSecMilli[0] = (intBenchmarkSecond / 58060800);//Years

	std::string stringVerboseYearMonthWeekHourMinSecMilli = "Elapsed time: " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[0]) + " years " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[1]) + " months " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[2]) + " weeks " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[3]) + " days " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[4]) + " hours " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[5]) + " minutes " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[6]) + " seconds " + std::to_string((int) benchmarkYearMonthWeekDayHourMinSecMilli[7]) + " milliseconds.\n";

	return stringVerboseYearMonthWeekHourMinSecMilli;
}
