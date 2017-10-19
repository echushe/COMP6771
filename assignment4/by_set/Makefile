##
## Makefile for COMP6771 Assignment 4
##

## compiler
CXX = g++

## compiler flags
CXXFLAGS = -Wall -Werror -O2 -std=c++14 -fsanitize=address
## enable this for debugging
#CXXFLAGS = -Wall -g

SOURCES = $(wildcard *.cpp)
OBJECTS = $(subst .cpp,,$(SOURCES))

default: test01

## using this target will automagically compile all the *.cpp
## files (hopefully tests) found in the current directory into
## individual binaries
all: $(OBJECTS)

%: %.cpp btree.h btree_iterator.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean: 
	rm -f *.o a.out core out? $(OBJECTS)
