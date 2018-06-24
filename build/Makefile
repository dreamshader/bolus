#################################################################################
# Makefile for bolus calculator and CLI
#
#################################################################################
# This Makefile is part of my bolus calculator project
#	git://github.com/dreamshader/smartTFT.git
#
#  Copyright (C) 2018 Dreamshader (aka Dirk Schanz)
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#      http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#################################################################################
# Note: you may copy the library to every location you want.
#       Hold in mind that, in case of you use the shared library, 
#       you have to set your LD_LIBRARY_PATH environment variable
#       to point to that location 
#       ( e.g.  export LD_LIBRARY_PATH=~/mylibs/:LD_LIBRARY_PATH} ).
#       In addition you have to tell the compiler where to find the
#       library using the -I and -L arguments (e.g. -L./ -I./ ) and
#       what's the name of the lib with the -l argument( -l smartTFT ).
#
#################################################################################
CXX = g++
CC  = gcc
AR  = ar
#
CCDEBUG  = -g
CXXDEBUG = -g
CXXOBJECT = -c
CXXPROGRAM = -o
#
CXXFLAGS = -Wall
CXXLIBS = -luuid
CXXLIBSOFLAGS = -fPIC -shared 
#
SOURCEDIR = ../src
SRCFILES = $(SOURCEDIR)/settings.cpp $(SOURCEDIR)/bolus.cpp $(SOURCEDIR)/datafile.cpp
INCFILES = $(SOURCEDIR)/settings.h $(SOURCEDIR)/bolus.h $(SOURCEDIR)/datafile.h
#
OBJFILES = settings.o bolus.o datafile.o
LIBSRC = 
CLISRC = $(SOURCEDIR)/bolus-cli.cpp
CLINAME = bolus-cli
EXAMPLFLAGS = -I. -L ../build -l rt
LIBOBJ = 
SOLIBNAME = 
STATLIBNAME =
#
#
all:	$(CLINAME)

$(CLINAME): compile $(CLISRC)
	$(CXX) $(CXXDEBUG) $(CXXPROGRAM) $(CLINAME) $(CLISRC) $(OBJFILES) $(CXXLIBS)

compile: $(SRCFILES) $(INCFILES)
	$(CXX) $(CXXOBJECT) $(CXXDEBUG) $(SRCFILES)

clean:
	rm -f *.o *~ 

