#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
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
	int root_size = 0;

	template <typename T>
	struct Node {
		int degree = 0;
		TYPE key;
		bool mark = false;
		Node<T> *father = NULL;
		std::vector<Node<T> *> *son = NULL;
		Node<T> *left = this;
		Node<T> *right = this;
	};

	Node<TYPE> *min_node = NULL;
	std::vector<Node<TYPE> *> root_list;

	// virtual void print_root();
	// virtual void print_all();

	virtual void consolidate();
	virtual void link(Node<TYPE> *y, Node<TYPE> *x);
	virtual void make_son(Node<TYPE> *new_father, Node<TYPE> *new_son);
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.
template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::link(Node<TYPE> *y, Node<TYPE> *x)
{
	auto it = find(root_list.begin(), root_list.end(), y);
	root_list.erase(it);
	make_son(x, y);
	y->mark = false;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::make_son(Node<TYPE> *new_father, Node<TYPE> *new_son)
{
	if (new_father->son == NULL) {
		new_father->son = new std::vector<Node<TYPE> *>;
	}
	new_father->son->emplace_back(new_son);
	new_father->degree++;
	new_son->father = new_father;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::consolidate()
{
	std::vector<Node<TYPE> *> A(size_p, NULL);

	for (auto w = root_list.begin(); w != root_list.end(); w++) {
		Node<TYPE> *x = *w;
		auto &d = x->degree;
		while (A[d] != NULL) {
			Node<TYPE> *y = A[d];
			if (!compare(x->key, y->key)) {
				std::swap(x, y);
			}
			link(y, x);
			A[d] = NULL;
			d++;
		}
		A[d] = x;
	}
	min_node = NULL;
	for (int i = 0; i < size_p; i++) {
		if (A[i] != NULL) {
			if (min_node == NULL) {
				root_list.clear();
				root_list.emplace_back(A[i]);
				min_node = A[i];
			} else {
				root_list.emplace_back(A[i]);
				if (compare(A[i]->key, min_node->key)) {
					min_node = A[i];
				}
			}
		}
	}
}

/*
template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::print_all()
{
	struct Print_all_local {
		void print_all_helper(Node<TYPE> *start_point)
		{
			if (!start_point) {
				return;
			}
			Node<TYPE> *current_point = start_point;

			while (1) {
				std::cout << current_point->key;
				if (current_point->son) {
					std::cout << "[";
					print_all_helper(current_point->son);
					std::cout << "]\n";
				} else {
					std::cout << " ";
				}
				if (current_point->right == start_point) {
					break;
				} else {
					current_point = current_point->right;
				}
			}
		}
	};
	Print_all_local pal;
	std::cout << std::endl;
	pal.print_all_helper(min_node);
	std::cout << std::endl;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::print_root()
{
	int counter = root_size;
	std::cout << "\nprint root:";
	auto *this_node = min_node;
	while (counter > 0) {
		std::cout << this_node->key << " ";
		this_node = this_node->right;
		counter--;
	}
	std::cout << std::endl;
}
*/

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::enqueue(const TYPE &val)
{
	Node<TYPE> *x = new Node<TYPE>;
	x->degree = 0;
	x->father = NULL;
	x->son = NULL;
	x->key = val;
	x->mark = false;

	if (min_node == NULL) {
		root_list.clear();
		root_list.emplace_back(x);
		min_node = x;
	} else {
		root_list.emplace_back(x);
		if (compare(x->key, min_node->key)) {
			min_node = x;
		}
	}
	size_p++;
}

template <typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min()
{
	Node<TYPE> *z = min_node;
	if (min_node != NULL) {
		for (auto x = z->son->begin(); x != z->son->end(); x++) {
			root_list.emplace_back(*x);
			(*x)->father = NULL;
		}

		auto itz = find(root_list.begin(), root_list.end(), z);
		auto itzr = itz + 1;
		if (itzr == root_list.end()) {
			itzr = root_list.begin();
		}
		int flag = 0;
		if (*itz == *(itzr)) {
			flag = 1;
		}
		root_list.erase(itz);
		if (flag) {
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

template <typename TYPE, typename COMP>
const TYPE &fib_heap<TYPE, COMP>::get_min() const
{
	const TYPE &min = min_node->key;
	return min;
}

template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::fib_heap(COMP comp)
{
	compare = comp;
	// Fill in the remaining lines if you need.
}

template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::~fib_heap()
{
	while (!empty()) {
		dequeue_min();
	}
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
