all: main.cpp true.cpp
	clang++ -O2 -std=c++11 -o main main.cpp -g 
	clang++ -O2 -std=c++11 -o true true.cpp 

test: all
	python3 massive_test/src/list_gen.py
	python3 massive_test/src/list_run.py
	python3 massive_test/src/list_diff.py
tar: all
	tar -cvzf oh1.tar Makefile main.cpp