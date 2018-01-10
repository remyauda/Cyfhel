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

    static int loadIntInFile(std::string const& stringLevel);//Permet de retourner un int lu dans dans un fichier.

    static int loadIntInFileWithDirectory(std::string const& stringLevel, std::string const& stringNameDirectory="ResultOfBenchmark/");//Permet de retourner un int lu dans un fichier qui se trouve dans un ou dans de multiples repertoires.

    static void writeIntInFileWithEraseData(std::string const& stringNameFile, double const& valueWrite, std::string const& stringNameDirectory="ResultOfBenchmark/");//Permet d'ecrire un int pris en parametre dans un fichier en effacant au prealable son contenu.

};

#endif // LIBMATRIX_H_INCLUDED
