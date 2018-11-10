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
		Node<T> *father = NULL;
		Node<T> *son = NULL;
		Node<T> *left = this;
		Node<T> *right = this;
	};

	Node<TYPE> *min_node = NULL;

	virtual void print_root();
	virtual void print_all();

	virtual void consolidate();
	virtual void make_son(Node<TYPE> *new_father, Node<TYPE> *new_son);
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::make_son(Node<TYPE> *new_father, Node<TYPE> *new_son)
{
	new_father->degree++;
	if (new_son == min_node) {
		min_node = new_father;
	}
	if (!new_father->son) {
		new_father->son = new_son;
		new_son->left = new_son;
		new_son->right = new_son;
		new_son->father = new_father;
	} else {
		auto nfsr = new_father->son->right;

		new_father->son->right = new_son;

		new_son->left = new_father->son;
		new_son->right = nfsr;

		nfsr->left = new_father->son;
	}
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::consolidate()
{
	int v_size = (int)(((double)log(root_size)) / ((double)log(1.618)));
	std::vector<Node<TYPE> *> v1(root_size, NULL);

	Node<TYPE> *current_point_1 = min_node;
	if (1) {
		while (1) {
			while (1) {
				assert(current_point_1->father == NULL);

				int son_num = current_point_1->degree;
				if (!v1[son_num]) {
					v1[son_num] = current_point_1;
					break;
				} else {
					Node<TYPE> *that_node = v1[son_num];
					if (compare(that_node->key,
						    current_point_1->key)) {
						that_node->left->right =
						    that_node->right;
						that_node->right->left =
						    that_node->left;

						make_son(current_point_1,
							 that_node);
						v1[son_num] = NULL;

					} else {
						// current be son
						current_point_1->left->right =
						    current_point_1->right;
						current_point_1->right->left =
						    current_point_1->left;
						make_son(that_node,
							 current_point_1);
						v1[son_num] = NULL;
						current_point_1 = that_node;
					}
				}
			}

			if (current_point_1->right == min_node) {
				break;
			} else {
				current_point_1 = current_point_1->right;
			}
		}
	}

	// last step: decide min_node
	Node<TYPE> *min_temp_node = min_node;
	Node<TYPE> *current_point = min_node;

	while (1) {
		if (!compare(min_temp_node->key, current_point->key)) {
			min_temp_node = current_point;
		}
		if (current_point->right == min_node) {
			break;
		} else {
			current_point = current_point->right;
		}
	}
	min_node = min_temp_node;
}

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

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::enqueue(const TYPE &val)
{
	size_p++;
	root_size++;
	// if the fib_heap is empty
	if (!min_node) {
		min_node = new Node<TYPE>;
		min_node->key = val;
		return;
	}

	// if the fib_heap is not empty
	Node<TYPE> *node_inserted = new Node<TYPE>;
	node_inserted->key = val;

	auto *min_right_stored = min_node->right;

	min_node->right = node_inserted;

	node_inserted->left = min_node;
	node_inserted->right = min_right_stored;

	min_right_stored->left = node_inserted;

	consolidate();

	print_all();
}

template <typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min()
{
	assert(size_p > 0);
	TYPE min_cout = min_node->key;
	if (size_p == 1) {
		size_p--;
		root_size--;
		TYPE min = min_node->key;
		auto *to_be_deleted = min_node;
		min_node = NULL;
		delete to_be_deleted;
		return min;
	}
	// size_p >0
	size_p--;
	root_size--;
	if (!min_node->son) {
		auto *min_node_left = min_node->left;
		auto *min_node_right = min_node->right;
		min_node_left->right = min_node_right;
		min_node_right->left = min_node_left;
		TYPE min = min_node->key;
		delete min_node;
		min_node = min_node_left;
		consolidate();
		return min;
	} else {
		Node<TYPE> *min_son_1 = min_node->son;
		Node<TYPE> *cur_son = min_son_1;
		while (1) {
			if (cur_son->right == min_son_1) {
				break;
			}
			cur_son->father = NULL;
			cur_son = cur_son->right;
		}
		auto *leftest_son = cur_son;

		min_node->left->right = min_son_1;
		min_son_1->left = min_node->left;

		leftest_son->right = min_node->right;
		min_node->right->left = leftest_son;

		TYPE min = min_node->key;
		auto *to_delete = min_node;
		min_node = min_node->right;
		delete to_delete;
		consolidate();
		return min;
	}
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
