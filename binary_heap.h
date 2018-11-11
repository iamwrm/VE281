#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <algorithm>
#include <cassert>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a binary
//           heap.
template <typename TYPE, typename COMP = std::less<TYPE> >
class binary_heap : public priority_queue<TYPE, COMP> {
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
	size_t size_p;

       private:
	void percolateDown(int id);
	// Add any additional member functions or data you require here.
};

template <typename TYPE, typename COMP>
binary_heap<TYPE, COMP>::binary_heap(COMP comp)
{
	compare = comp;
	data.emplace_back(TYPE());
	size_p = 0;
	// Fill in the remaining lines if you need.
}

template <typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::enqueue(const TYPE &val)
{
	data.emplace_back(std::move(val));
	int pos = size_p++;
	// per up
	while (pos > 1 && (compare(data[pos], data[pos >> 1]))) {
		std::swap(data[pos], data[pos >> 1]);
		pos = pos >> 1;
	}
}

template <typename TYPE, typename COMP>
TYPE binary_heap<TYPE, COMP>::dequeue_min()
{
	auto datum_poped = data[1];
	data[1] = data[size_p--];
	data.pop_back();
	percolateDown(1);
	return datum_poped;
}

template <typename TYPE, typename COMP>
const TYPE &binary_heap<TYPE, COMP>::get_min() const
{
	if (!this->empty()) {
		assert(-1);
	}
	auto &datum_poped = data[1];
	return datum_poped;
}

template <typename TYPE, typename COMP>
bool binary_heap<TYPE, COMP>::empty() const
{
	return (size_p < 1);
}

template <typename TYPE, typename COMP>
unsigned binary_heap<TYPE, COMP>::size() const
{
	return size_p;
}

template <typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::percolateDown(int id)
{
	auto size = data.size();
	auto &heap = data;
	for (auto j = 2 * id; j <= size; j = 2 * id) {
		if (j < size && compare(heap[j + 1], heap[j])) {
			j++;
		}
		// if (!compare(heap[j], heap[id])) break;
		if (compare(heap[id], heap[j])) break;
		std::swap(heap[id], heap[j]);
		id = j;
	}
	/*
	unsigned p = 1;
	while ((p <<= 1) <= size_p) {
		if (p < size_p && compare(data[p ^ 1], data[p])) p ^= 1;
		if (compare(data[p >> 1], data[p])) break;
		std::swap(data[p >> 1], data[p]);
	}
	*/
}

#endif  // BINARY_HEAP_H
