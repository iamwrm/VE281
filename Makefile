all:	main.cpp ut.cpp ut.h
	clang++ -o main -std=c++11 main.cpp ut.cpp -g