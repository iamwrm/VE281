ds_headers= binary_heap.h fib_heap.h priority_queue.h unsorted_heap.h
my_headers= util.h
GXX=clang++ -std=c++11 -O2
all: main.cpp
	${GXX} -o main main.cpp util.cpp
test_heap: test_heap.cpp ${ds_headers}
	${GXX} -o test_heap test_heap.cpp 
memeory: all
	valgrind --leak-check=full ./main -i BINARY < test-1.in
