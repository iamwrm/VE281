#ifndef UTIL_H
#define UTIL_H
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

class Grid {
       private:
       bool * reached;
       int * weight;
       int height;
       int width;

       public:
	Grid(int width, int height);
	~Grid();
	void set_weight(int x,int y,int weight);
	void set_reached(int x,int y,bool if_reached);
	const int get_weight(int x,int y);
	const bool get_reached(int x,int y);
	void read_weight_from_cin();
	void print_grid();
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