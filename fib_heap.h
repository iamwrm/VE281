#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
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

	struct Node {
		int degree = 0;
		TYPE key;
		bool mark = false;
		std::list<Node *> child;
	};

	Node *min_node = NULL;
	typename std::list<Node *>::iterator min;

	std::vector<Node *> root_list;
	std::list<Node *> roots;

	void consolidate();
	void link(const decltype(min) &y, Node *&x);
	void recursive_delete(std::list<Node *>);

	void link(Node *y, Node *x);
	void enqueue_(const TYPE &val);
	TYPE dequeue_min_();
	void consolidate_();
	void make_son(Node *new_father, Node *new_son);
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::recursive_delete(std::list<Node *> vic)
{
	if (vic.empty()) return;
	for (auto &it : vic) {
		recursive_delete(it->child);
		delete it;
	}
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::enqueue(const TYPE &val)
{
	Node *x = new Node;
	x->key = std::move(val);
	roots.emplace_front(std::move(x));
	if (min == roots.end()) {
		min = roots.begin();
	} else if (compare(x->key, (*min)->key)) {
		min = roots.begin();
	}
	size_p++;
}

template <typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min()
{
	TYPE res = (*min)->key;
	auto &z = *min;
	if (!z) {
		return res;
	}
	if (!z->child.empty()) {
		roots.splice(roots.begin(), std::move(z->child));
	}

	delete z;
	roots.erase(min);
	size_p--;

	if (size_p == 0) {
		min = roots.end();
	} else {
		consolidate();
	}
	return res;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::consolidate()
{
	const unsigned Dn = (unsigned)(log(size_p) / log(1.618));
	auto roots_end = roots.end();
	// std::vector<decltype(min)> A(Dn + 1, roots_end);
	decltype(min) *A = new decltype(min)[Dn + 1];
	for (int i = 0; i < Dn + 1; i++) {
		A[i] = roots_end;
	}
	if (!roots.empty()) {
		for (auto w = roots.begin(); w != roots.end();) {
			auto x = w++;
			unsigned d = (*x)->degree;
			while (A[d] != roots.end()) {
				auto y = A[d];
				if (compare((*y)->key, (*x)->key)) {
					std::swap(x, y);
				}
				link(y, *x);
				A[d] = roots.end();
				d++;
			}
			A[d] = x;
		}
	}
	min = roots.begin();
	for (auto it = roots.begin(); it != roots.end(); ++it) {
		if (compare((*it)->key, (*min)->key)) {
			min = it;
		}
	}
	delete[] A;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::link(const decltype(min) &y, Node *&x)
{
	Node *inserted = (*y);
	roots.erase(y);
	x->child.emplace_front(std::move(inserted));
	++x->degree;
}

// =======================================

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::link(Node *y, Node *x)
{
	auto it = find(root_list.begin(), root_list.end(), y);
	root_list.erase(it);
	make_son(x, y);
	y->mark = false;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::make_son(Node *new_father, Node *new_son)
{
	if (new_father->child == NULL) {
		new_father->child = new std::list<Node *>;
	}
	new_father->child->emplace_back(new_son);
	new_father->degreeree++;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::consolidate_()
{
	std::vector<Node *> A(size_p + 1);
	for (int i = 0; i < size_p; i++) {
		A[i] = NULL;
	}
	if (!root_list.empty()) {
		for (int w = 0; w < root_list.size(); w++) {
			Node *x = root_list[w];
			// int &d = x->degree;
			int d;
			if (x->child == NULL) {
				d = 0;
			} else {
				d = x->child->size();
			}
			while (A[d] != NULL) {
				Node *y = A[d];
				if (compare(x->key, y->key)) {
					std::swap(x, y);
				}
				link(y, x);
				A[d] = NULL;
				x->degree++;
			}
			A[d] = x;
		}
	}
	min_node = NULL;
	for (int i = 0; i < size_p; i++) {
		if (A[i] != NULL) {
			if (min_node == NULL) {
				root_list.emplace_back(A[i]);
				min_node = A[i];
			} else {
				root_list.emplace_back(A[i]);
				if (!compare(A[i]->key, min_node->key)) {
					min_node = A[i];
				}
			}
		}
	}
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::enqueue_(const TYPE &val)
{
	Node *x = new Node;
	x->degree = 0;
	x->child = NULL;
	x->key = val;
	x->mark = false;

	if (min_node == NULL) {
		root_list.clear();
		root_list.emplace_back(x);
		min_node = x;
	} else {
		root_list.emplace_back(x);
		if (!compare(x->key, min_node->key)) {
			min_node = x;
		}
	}
	size_p++;
	std::cout << min_node->key << std::endl;
}

template <typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min_()
{
	Node *z = min_node;
	if (min_node != NULL) {
		if (z->child != NULL) {
			for (auto x = z->child->begin(); x != z->child->end();
			     x++) {
				root_list.emplace_back(*x);
				z->degree--;
			}
			delete z->child;
			z->child = NULL;
		}

		auto itz = find(root_list.begin(), root_list.end(), z);
		auto itzr = itz + 1;
		if (itzr == root_list.end()) {
			itzr = root_list.begin();
		}
		int flag = 0;

		auto d_z = itz - root_list.begin();
		auto d_zr = itzr - root_list.begin();
		if (d_z == d_zr) {
			flag = 1;
		}
		root_list.erase(itz);
		if (size_p <= 1) {
			min_node = NULL;
		} else {
			min_node = *itzr;
			consolidate();
		}
		size_p--;
	}

	auto z_key = z->key;
	delete z;
	return z_key;
}

// DONE:
template <typename TYPE, typename COMP>
const TYPE &fib_heap<TYPE, COMP>::get_min() const
{
	return (*min)->key;
}

// DONE:
template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::fib_heap(COMP comp)
{
	compare = comp;
	size_p = 0;
	min = roots.end();
	// Fill in the remaining lines if you need.
}

// DONE:
template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::~fib_heap()
{
	recursive_delete(roots);
	// Fill in the remaining lines if you need.
}

// DONE:
template <typename TYPE, typename COMP>
bool fib_heap<TYPE, COMP>::empty() const
{
	return (size_p < 1);
}

// DONE:
template <typename TYPE, typename COMP>
unsigned fib_heap<TYPE, COMP>::size() const
{
	return size_p;
}

#endif  // FIB_HEAP_H
