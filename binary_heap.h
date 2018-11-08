#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <algorithm>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a binary
//           heap.
template<typename TYPE, typename COMP = std::less<TYPE> >
class binary_heap: public priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  // EFFECTS: Construct an empty heap with an optional comparison functor.
  //          See test_heap.cpp for more details on functor.
  // MODIFIES: this
  // RUNTIME: O(1)
  binary_heap(COMP comp = COMP());

  // EFFECTS: Add a new element to the heap.
  // MODIFIES: this
  // RUNTIME: O(log(n))
  virtual void enqueue(const TYPE &val);

  // EFFECTS: Remove and return the smallest element from the heap.
  // REQUIRES: The heap is not empty.
  // MODIFIES: this
  // RUNTIME: O(log(n))
  virtual TYPE dequeue_min();

  // EFFECTS: Return the smallest element of the heap.
  // REQUIRES: The heap is not empty.
  // RUNTIME: O(1)
  virtual const TYPE &get_min() const;

  // EFFECTS: Get the number of elements in the heap.
  // RUNTIME: O(1)
  virtual size_type size() const;

  // EFFECTS: Return true if the heap is empty.
  // RUNTIME: O(1)
  virtual bool empty() const;

private:
  // Note: This vector *must* be used in your heap implementation.
  std::vector<TYPE> data;
  // Note: compare is a functor object
  COMP compare;

private:
void percolateDown(int id);
  // Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP>
binary_heap<TYPE, COMP> :: binary_heap(COMP comp) {
    compare = comp;
    // Fill in the remaining lines if you need.
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP> :: enqueue(const TYPE &val) {
	// Fill in the body.
	data.push_back(val);
	int pos = data.size()-1;
	//for (; (pos > 1) && (this->compare(data[pos / 2], data[pos]) < 0);
	     //pos = pos / 2) {
	while (pos > 0 && (compare(data[pos / 2], data[pos]))) {
		std::swap(data[pos], data[pos / 2]);
        pos=pos/2;
		// data[pos]= data[pos / 2];
	}
	using namespace std;
	int a = 0;
	int b = 1;
	for (auto it = data.begin(); it != data.end(); it++) {
		cout << *(it) << "\t";
		a++;
		if (a >= b) {
			cout << endl;
			a = 0;
			b = b << 1;
		}
	}
	cout << endl;
	cout << endl;
}

template<typename TYPE, typename COMP>
TYPE binary_heap<TYPE, COMP> :: dequeue_min() {
    // Fill in the body.
    if (!this->empty()) {
	    auto datum_poped = data[0];
	    auto datum_last = *(data.end());
        data[0]=datum_last;
	    data.erase(data.end());
        // TODO: perculate down
        percolateDown(0);

	    return datum_poped;
    } else {
	    assert(-1);
    }
}

template<typename TYPE, typename COMP>
const TYPE &binary_heap<TYPE, COMP> :: get_min() const {
    // Fill in the body.
    if (!this->empty()) {
	    auto datum_poped = data[0];
	    // data.erase(data.begin());
	    return datum_poped;
    } else {
	    assert(-1);
    }
}

template<typename TYPE, typename COMP>
bool binary_heap<TYPE, COMP> :: empty() const {
    // Fill in the body.
    return (data.size()<1);
}

template<typename TYPE, typename COMP>
unsigned binary_heap<TYPE, COMP> :: size() const { 
    // Fill in the body.
    return data.size();
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE,COMP>::percolateDown(int id)
{
	auto size = data.size();
	auto & heap  = data;
	for (auto j = 2 * id; j <= size; j = 2 * id) {
		if (j < size && heap[j] > heap[j + 1]) j++;
		if (heap[id] <= heap[j]) break;
		std::swap(heap[id], heap[j]);
		id = j;
	}
}

#endif //BINARY_HEAP_H
