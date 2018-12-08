all:	main.cpp
	clang++ -std=c++11 -o main main.cpp 
test: all
	./main < g1.in 