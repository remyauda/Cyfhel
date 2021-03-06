##############################################################################################
##############################################################################################
###########################                                     ##############################
###########################             MAIN MAKEFILE           ##############################
###########################                                     ##############################
##############################################################################################
##############################################################################################
#
# DESCRIPTION: Main Makefile that acts as installer for HElib and Cyfhel. With it, there
# 			   is no need to travel across directories unless some particular configuration is
# 			   desired. Just type the next command and wait for everything to be finished.
# 			   		> sudo make
#
# 			   You can also select which library to build&install (e.g.:sudo make Cyfhel), and
# 			   clean all the subdirectories all at once with 'make clean'. Finally, 'make
# 			   uninstall' allows the fast removal of all libraries
#
#
# AUTHOR: Remy AUDA & Alexandre AUDA
# DATE: 17/12/2017
#   
#   
# LICENSE: GNU GPL v3
#  
#  This file is part of Cyfhel project
#
#  Cyfhel is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Cyfhel is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

$(info -------------------HELIB, CYFHEL MAKEFILE----------------------)
$(info Dependencies=Cyfhel -> HElib -> NTL & GMP)
$(info If you get compilation errors, try to add/remove -std=c++11 in Makefile)
$(info For a list If all the available commands, run >make info)



##############################################################################################
#                                   MAKEFILE VARIABLES                                       #
##############################################################################################

#.................................. COMPILER & LINKER ........................................
CC = g++
CFLAGS = -g -O2 -std=c++11 -I/usr/local/include \
         -pthread -DFHE_THREADS -DFHE_DCRT_THREADS -DFHE_BOOT_THREADS \
         -Wfatal-errors -Wshadow -Wall -fmax-errors=2

# useful flags:
#   -std=c++11
#   -DNO_HALF_SIZE_PRIME  tells helib to not use the half size prime
#                  in the prime chain
#
#   -DFHE_THREADS  tells helib to enable generic multithreading capabilities;
#                  must be used with a thread-enabled NTL and the -pthread
#                  flag should be passed to gcc
#
#   -DFHE_DCRT_THREADS  tells HElib to use a multithreading strategy at the
#                  DoubleCRT level; requires -DFHE_THREADS (see above)
#
#   -DFHE_BOOT_THREADS  tells helib to use a multithreading strategy for
#                  bootstrapping; requires -DFHE_THREADS (see above)
#
#   -DEVALMAP_CACHED=1 Cache some constants for bootstrapping
#
#        * EVALMAP_CACHED=0 caches these constants as ZZX'es, this takes
#                  less space but it means that they still need to be converted
#                  to DoubleCRT's with every recryption operation. 
#
#        * EVALMAP_CACHED=1 caches these constants directly as DoubleCRT's,
#                  this may take a significant amount of memeory.
#
#  If you get compilation errors, you may need to add -std=c++11 or -std=c++0x
# 


#.................................. LIBRARY ATTRIBUTES .......................................
LDLIBS = -lntl -lgmp -lm -lfhe -lcyfhel




##############################################################################################
#                                         EASY INSTALL                                       #
##############################################################################################


all: requirements install
.PHONY: packages libdownloads pypackages HElib Cyfhel


##############################################################################################
#                                     INSTALL REQUIREMENTS                                   #
##############################################################################################


requirements: packages libdownloads

packages:
	sudo apt-get install libboost-all-dev
	sudo apt-get install libtool-bin
	sudo apt-get install m4

libdownloads:
	mkdir -p .installers
	cp .Makefiles/Makefile_Inst_GMP_NTL .installers/Makefile
	$(MAKE) -C .installers
	rm -rf .installers

##############################################################################################
#                                     INSTALL ALL LIBRARIES                                  #
##############################################################################################

install: HElib Cyfhel

Upgrade: HElib Cyfhel CyfhelUpgrade

HElib:
	$(MAKE) -C HElib/src/
	$(MAKE) install -C HElib/src/	

Cyfhel:
	$(MAKE) -C src/Cyfhel/
	$(MAKE) install -C src/Cyfhel/


CyfhelUpgrade:
	$(MAKE) upgrade -C src/Cyfhel


##############################################################################################
#                                  CREATE BINARIES WITH CYFHEL                                #
##############################################################################################

./%_x: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)



##############################################################################################
#                                       UNINSTALL & CLEAN                                    #
##############################################################################################

clean:
	$(MAKE) clean -C HElib/src/
	$(MAKE) clean -C src/Cyfhel/

uninstall:
	$(MAKE) uninstall -C HElib/src/
	$(MAKE) uninstall -C src/Cyfhel/

cleanexe:
	-rm -f *.aenv *_x




##############################################################################################
#                                        ADDITIONAL INFO                                     #
##############################################################################################


info:
	: Dependencies = Cyfhel -> HElib -> NTL>=10.0.0 & GMP
	: Compilation flags are 'CFLAGS=$(CFLAGS)'
	: Commands Available:
	: * sudo make - build & install all three libraries. Requires root
	: * sudo make HElib|Cyfhel - build&instlal a single library. Requires root
	: * make fileName_x - Compile & Link binary filename.cpp with Cyfhel and its dependencies
	: * make clean - remove all library files from the folder
	: * sudo make uninstall - remove libraries from host. Requires root
	: If errors occur, try adding/removing '-std=c++11' in Makefile

