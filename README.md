# Cyfhel

Abstraction of Helib in C++ for homeomorphic encryption.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

INSTALLATION Linux Ubuntu (computer or virtual machine) :

Install a Linux Ubuntu virtual machine or use your own computer if you are running Linux Ubuntu (tested version: Ubuntu 14.04.3 64 bit). -> https://openclassrooms.com/courses/reprenez-le-controle-a-l-aide-de-linux/installez-linux-dans-une-machine-virtuelle

To enable the share clipboard etc... on virtual machine -> https://forums.virtualbox.org/viewtopic.php?f=3&t=15679

If git is not install yet:
```
sudo apt-get install git
```

If git is not configured yet:
```
git config --global user.email "you@example.com" and git config --global user.name "Your UserName"
```

INSTALLATION Cyfhel:
```
git clone https://github.com/remyauda/Cyfhel
cd Cyfhel/
./configure
sudo make all
(sudo make Cyfhel)->facultatif
cd src/Demo_Test/
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```

COMPILATION:
Se placer à la racine du projet.
```
sudo make Cyfhel
```

EXECUTION DE LA DEMO:
Se placer dans le répertoire Demo_Test
```
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```

ATTENTION: 
Il est très important de faire:
make program_x et ./program_x
Le _x est très important!
Ne pas le mettre provoquera des erreurs dans l'execution de votre code (segmentation fault, etc...)

### Prerequisites

What things you need to install the software and how to install them:

INSTALLATION Linux Ubuntu (computer or virtual machine) :
Install a Linux Ubuntu virtual machine or use your own computer if you are running Linux Ubuntu (tested version: Ubuntu 14.04.3 64 bit). -> https://openclassrooms.com/courses/reprenez-le-controle-a-l-aide-de-linux/installez-linux-dans-une-machine-virtuelle

To enable the share clipboard etc... on virtual machine -> https://forums.virtualbox.org/viewtopic.php?f=3&t=15679

If git is not install yet:
```
sudo apt-get install git
```

If git is not configured yet:
```
git config --global user.email "you@example.com" and git config --global user.name "Your UserName"
```

### Installing

A step by step series of examples that tell you have to get a development env running

INSTALLATION Cyfhel:
```
git clone https://github.com/remyauda/Cyfhel
cd Cyfhel/
./configure
sudo make all
(sudo make Cyfhel)->facultatif
cd src/Demo_Test/
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```

COMPILATION:
Se placer à la racine du projet.
```
sudo make Cyfhel
```

You can test your installation by going within the folder Demo_Test and do:
```
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```
## Running the tests

Explain how to run the automated tests for this system:



### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
