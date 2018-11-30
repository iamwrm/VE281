all:	main.cpp ut.cpp ut.h
	clang++ -O2 -Wall -o main -std=c++11 main.cpp ut.cpp -g