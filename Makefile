CPPFLAGS=-I/usr/include/eigen3

main: main.cpp predicates.hpp
	g++ -std=c++20 $(CPPFLAGS) -o main main.cpp
