#ifndef UT_H
#define UT_H
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Flags {
	int v_flag = 0;
	int t_flag = 0;
	int m_flag = 0;
	int p_flag = 0;
	int g_num = 0;
	vector<string> g_e_names;
};

// void get_ops(int argc,char ** argv int&, int&);

void get_ops(int argc, char **argv, Flags &flags);

#endif  // UT_H