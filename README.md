# Cyfhel
Abstraction of Helib in C++ for homeomorphic encryption.

INSTALLATION machine virtuelle:
(To enable the share clipboard etc...) -> https://forums.virtualbox.org/viewtopic.php?f=3&t=15679
(Si git n'est pas installé) -> sudo apt-get install git
(Si git n'est pas configuré) -> git config --global user.email "you@example.com" and git config --global user.name "Your UserName"

INSTALLATION Cyfhel:
git clone https://github.com/remyauda/Cyfhel
cd Cyfhel/
./configure
sudo make all
(sudo make Cyfhel)->facultatif
cd src/Demo_Test/
make Demo_Cyfhel_x
./Demo_Cyfhel_x

COMPILATION:
Se placer à la racine du projet.
sudo make Cyfhel

EXECUTION DE LA DEMO:
Se placer dans le répertoire Demo_Test
make Demo_Cyfhel_x
./Demo_Cyfhel_x

ATTENTION: 
Il est très important de faire:
make program_x et ./program_x
Le _x est très important!
Ne pas le mettre provoquera des erreurs dans l'execution de votre code (segmentation fault, etc...)
