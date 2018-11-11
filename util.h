#ifndef UTIL_H
#define UTIL_H
#include <getopt.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "binary_heap.h"

using std::cin;
using std::cout;
using std::endl;

class Point_Ptr {
       public:
	int id;
	bool *reached;
	int *weight;
	int *pathcost;
	int *pre;
};

struct compare_t {
	bool operator()(Point_Ptr a, Point_Ptr b) const
	{
		return *(a.pathcost) < *(b.pathcost);
	}
};

class Grid {
       private:
	bool *reached;
	int *weight;
	int *pathcost;
	int *pre;
	Point_Ptr *ptrs;
	int height;
	int width;
	bool verbose_wr;

       public:
	Grid(int width, int height, bool verbose_wr);
	~Grid();

	void set_weight(int id, int weight);
	void set_reached(int id, bool if_reached);
	void set_pathcost(int id, int pathcost);
	void set_pre(int id, int id_of_pre);

	const int get_weight(int id);
	const bool get_reached(int id);
	const int get_pathcost(int id);
	const int get_pre(int id);

	void read_weight_from_cin();

	void print_grid();

	const int xy_to_id(const int x, const int y);
	void id_to_xy(int &x, int &y, const int id);
	void diki(priority_queue<Point_Ptr, compare_t> &pq, int const,
		  int const);
	void trace_back_path(const int start, const int end);
	Point_Ptr get_ptr(const int id)
	{
		return ptrs[id];
	}
};

class Interface_Property {
       private:
	int verbose_flag = 0;
	std::string method_name;
	void get_ops(int argc, char **argv);

       public:
	Interface_Property(int argc, char **argv)
	{
		get_ops(argc, argv);
	}
	const int &get_verbose_flag()
	{
		return verbose_flag;
	}
	const std::string &get_method_name()
	{
		return method_name;
	}
};

#endif  // !UTIL_H