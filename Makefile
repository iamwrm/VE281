ds_headers= binary_heap.h fib_heap.h priority_queue.h unsorted_heap.h
my_headers= util.h
GXX=clang++ -std=c++11 -O2
all: main.cpp
	${GXX} -o main main.cpp util.cpp
tar: all
	tar -cvzf oh3.tar Makefile main.cpp util.cpp util.h binary_heap.h fib_heap.h priority_queue.h unsorted_heap.h
