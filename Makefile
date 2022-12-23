CPPFLAGS=-I/usr/include/eigen3

all: main bench

%: %.cpp predicates.hpp
	g++ -std=c++20 $(CPPFLAGS) -o $@ $<
