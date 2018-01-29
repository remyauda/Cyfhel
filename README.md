# Cyfhel

Abstraction of Helib in C++ for homeomorphic encryption.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

1. **INSTALLATION Linux Ubuntu (computer or virtual machine) :**

Install a Linux Ubuntu virtual machine or use your own computer if you are running Linux Ubuntu (tested version: Ubuntu 14.04.3 64 bit) -> [Install a Linux Ubuntu virtual machine](https://openclassrooms.com/courses/reprenez-le-controle-a-l-aide-de-linux/installez-linux-dans-une-machine-virtuelle)

To enable the share clipboard etc... on virtual machine -> [https://forums.virtualbox.org/viewtopic.php?f=3&t=15679](https://forums.virtualbox.org/viewtopic.php?f=3&t=15679)

If git is not install yet:
```
sudo apt-get install git
```

If git is not configured yet:
```
git config --global user.email "you@example.com" and git config --global user.name "Your UserName"
```

2. **INSTALLATION Cyfhel:**

In a terminal, perform the following command (the installation should take a moment: about half an hour):
```
git clone https://github.com/remyauda/Cyfhel
cd Cyfhel/
./configure
sudo make all
(sudo make Cyfhel)->facultatif
```

3. **EXECUTION OF THE DEMO:** 

To test if Cyfhel works, you can run the main demo of the project called Demo_Cyfhel. To do this, go in the folder Demo_Test and do:
```
cd src/Demo_Test/
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```
More generally, you can run every Demo which are in the folder DemoTest. To do it you have just to go in the folder DemoTest and then run the following command:
```
make demoYouWantToRun_x
```
The previous command will compile the demoYouWantToRun.cpp you want to run and will create an executable called demoYouWantToRun_x. Then, just run the following command to run your demo:
```
./demoYouWantToRun_x
```

**Important note:** To make your life easier, we have provided two batch files:

- removeAllExecutable.sh: this batch removed all the executable_x in the directory DemoTest.
- compileAll.sh: this batch compile all the cpp file in the directory DemoTest to create executable_x for every Demo presents in the folder DemoTest.

The first batch could be run with:
```
./removeAllExecutable.sh
```

The second batch could be run with:
```
./compileAll.sh
```

**WARNING:**

It is very important to do:

make program_x et ./program_x

The _x is very important!
If you don't put it, you will encounter some errors at the execution time (segmentation fault, etc...)

4. **COMPILATION:**

**If you change the source code of the project Cyfhel**, you must recompile it. Otherwise your changes will be ignored. To do this, go to the root of the project and do.
```
sudo make Cyfhel
```

**If you change the source code of the project HElib**, you must recompile it. Otherwise your changes will be ignored. To do this, go to the root of the project and do.
```
sudo make HElib
```

## Prerequisites

What things you need to install the software and how to install them: **No prerequisites**.

## Installing

A step by step series of examples that tell you have to get a development env running

1. **INSTALLATION Cyfhel:**

In a terminal, perform the following command:
```
git clone https://github.com/remyauda/Cyfhel
cd Cyfhel/
./configure
sudo make all
(sudo make Cyfhel)->facultatif
```


2. **EXECUTION OF THE DEMO:**

To test if Cyfhel works, you can run the main demo of the project called Demo_Cyfhel. To do this, go in the folder Demo_Test and do:
```
cd src/Demo_Test/
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```
More generally, you can run every Demo which are in the folder DemoTest. To do it you have just to go in the folder DemoTest and then run the following command:
```
make demoYouWantToRun_x
```
The previous command will compile the demoYouWantToRun.cpp you want to run and will create an executable called demoYouWantToRun_x. Then, just run the following command to run your demo:
```
./demoYouWantToRun_x
```

**Important note:** To make your life easier, we have provided two batch files:

- removeAllExecutable.sh: this batch removed all the executable_x in the directory DemoTest.
- compileAll.sh: this batch compile all the cpp file in the directory DemoTest to create executable_x for every Demo presents in the folder DemoTest.

The first batch could be run with:
```
./removeAllExecutable.sh
```

The second batch could be run with:
```
./compileAll.sh
```

**WARNING:**

It is very important to do:

make program_x et ./program_x

The _x is very important!
If you don't put it, you will encounter some errors at the execution time (segmentation fault, etc...)

3. **COMPILATION:**

**If you change the source code of the project Cyfhel**, you must recompile it. Otherwise your changes will be ignored. To do this, go to the root of the project and do.
```
sudo make Cyfhel
```

**If you change the source code of the project HElib**, you must recompile it. Otherwise your changes will be ignored. To do this, go to the root of the project and do.
```
sudo make HElib
```

## Running the tests

Explain how to run the automated tests for this system:

To test if Cyfhel works, you can run the main demo of the project called Demo_Cyfhel. To do this, go in the folder Demo_Test and do:
```
cd src/Demo_Test/
make Demo_Cyfhel_x
./Demo_Cyfhel_x
```
More generally, you can run every Demo which are in the folder DemoTest. To do it you have just to go in the folder DemoTest and then run the following command:
```
make demoYouWantToRun_x
```
The previous command will compile the demoYouWantToRun.cpp you want to run and will create an executable called demoYouWantToRun_x. Then, just run the following command to run your demo:
```
./demoYouWantToRun_x
```

**Important note:** To make your life easier, we have provided two batch files:

- removeAllExecutable.sh: this batch removed all the executable_x in the directory DemoTest.
- compileAll.sh: this batch compile all the cpp file in the directory DemoTest to create executable_x for every Demo presents in the folder DemoTest.

The first batch could be run with:
```
./removeAllExecutable.sh
```

The second batch could be run with:
```
./compileAll.sh
```

**WARNING:**

It is very important to do:

make program_x et ./program_x

The _x is very important!
If you don't put it, you will encounter some errors at the execution time (segmentation fault, etc...)

### Break down into Benchmark

Appart from traditional Demo in DemoTest, we have provided some benchmarks of Cyfhel. In these Benchmark, we measure the time that Cyfhel takes to perform each homomorphic operation:
- Time for the creation of an object Cyfhel (generation of the keys).
- Time to perform an homomorphic addition.
- Time to perform an homomorphic substraction.
- Time to perform an homomorphic multiplication.
- etc...
The results of these Benchmarks are stored in txt files within the folder ResultOfBenchmark.
You can then export them into a cvs file and perform some visualisation with some graphical softwares:
SAP Lumira, Qlik, Webi, Tableau etc...

![BasicOperation3](https://github.com/remyauda/Cyfhel/blob/master/src/Demo_Test/ResultOfBenchmark/Benchmark_graph/BasicOperation3.PNG)

![PoynomialDegIncrease1](https://github.com/remyauda/Cyfhel/blob/master/src/Demo_Test/ResultOfBenchmark/Benchmark_graph/PoynomialDegIncrease1.PNG)



## Deployment

Add additional notes about how to deploy this on a live system:

**Nothing to add: just perform the installation as described**.

## Built With

* [HElib](https://github.com/shaih/HElib) - Software library that implements homomorphic encryption (HE)
* [GMP](https://gmplib.org/) - The GNU Multiple Precision Arithmetic Library
* [NTL](http://www.shoup.net/ntl/download.html) - NTL: A Library for doing Number Theory

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.


## Authors

* **Rémy AUDA** - [GitHub](https://github.com/remyauda)

See also the list of [contributors](https://github.com/remyauda/Cyfhel/graphs/contributors) who participated in this project.

## License

This project is licensed under the GNU GPLv3 License - see the [LICENSE](https://github.com/remyauda/Cyfhel/tree/master/License) file for details.

## Acknowledgments

* The SW is based on HElib by Shai Halevi, HEIDE by Grant Frame, analysis of addition by Matheus S.H. Cruz. In compliance with their respective Licenses, I name all of them in this section. This project could not be possible without them.
