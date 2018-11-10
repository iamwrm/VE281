#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a
//           Fibonacci heap.
template <typename TYPE, typename COMP = std::less<TYPE> >
class fib_heap : public priority_queue<TYPE, COMP> {
       public:
	typedef unsigned size_type;

	// EFFECTS: Construct an empty heap with an optional comparison functor.
	//          See test_heap.cpp for more details on functor.
	// MODIFIES: this
	// RUNTIME: O(1)
	fib_heap(COMP comp = COMP());

	// EFFECTS: Deconstruct the heap with no memory leak.
	// MODIFIES: this
	// RUNTIME: O(n)
	~fib_heap();

	// EFFECTS: Add a new element to the heap.
	// MODIFIES: this
	// RUNTIME: O(1)
	virtual void enqueue(const TYPE &val);

	// EFFECTS: Remove and return the smallest element from the heap.
	// REQUIRES: The heap is not empty.
	// MODIFIES: this
	// RUNTIME: Amortized O(log(n))
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
	// Note: compare is a functor object
	COMP compare;

       private:
	// Add any additional member functions or data you require here.
	// You may want to define a strcut/class to represent nodes in the heap
	// and a pointer to the min node in the heap.
	int size_p = 0;
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::fib_heap(COMP comp)
{
	compare = comp;
	size_p = 0;
	// Fill in the remaining lines if you need.
}

template <typename TYPE, typename COMP>
bool fib_heap<TYPE, COMP>::empty() const
{
	return (size_p < 1);
}

template <typename TYPE, typename COMP>
unsigned fib_heap<TYPE, COMP>::size() const
{
	return size_p;
}

#endif  // FIB_HEAP_H
