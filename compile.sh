#!/bin/sh
# 
##################################################################################################
# This script does the compilation of all the packages required by d-jon. 
# 
# All the project files are under the terms of the license explain in LICENSE, please
# be sure to read it first.
# 
# This script will use qmake in order to create all the makefiles required for the compilation
# 
# 
# author: Juan Pablo Crossley (Cross) 
# twitter: @CrossDevelop
# email: crossleyjuan@gmail.com
# 
##################################################################################################

# configures and compiles the djon-util module
echo Creating the makefile for djon-util module
cd djon-util
qmake djon-util.pro
make clean
make
cd ..


# configure and compiles the djon-data module
echo Creating the makefile for djon-data module
cd djon-data
qmake djon-data.pro
make clean
make
cd ..


# configure and compiles the djon-plugins module
echo Creating the makefile for djon-plugins module
cd djon-plugins
qmake djon-plugins.pro
make clean
make
cd ..


# configure and compiles the djon
echo Creating the makefile for djon
cd djon
qmake djon.pro
make clean
make
cd ..

