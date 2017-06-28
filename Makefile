main: main.o makefile.o target.o
	g++-5 -std=c++14 main.o makefile.o target.o -o a3q2
makefile.o: makefile.cc makefile.h target.h
	g++-5 -std=c++14 -c makefile.cc
target.o: target.cc target.h
	g++-5 -std=c++14 -c target.cc
main.o: makefile.h main.cc
	g++-5 -std=c++14 -c main.cc
