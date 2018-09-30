all: main.cpp 
	clang++ -O2 -std=c++11 -o main main.cpp
test:all
	python3 list_gen.py
	python3 list_run.py