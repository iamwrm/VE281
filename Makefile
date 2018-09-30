all: main.cpp true.cpp
	clang++ -O2 -std=c++11 -o main main.cpp -g 
	clang++ -O2 -std=c++11 -o true true.cpp 



test: all
	python3 massive_test/src/list_gen1.py