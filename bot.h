#ifndef BOT_H
#define BOT_H

#include <getopt.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Flags {
	int v_flag = 0;
	int t_flag = 0;
	int m_flag = 0;
	int p_flag = 0;
	int g_num = 0;
	std::vector<std::string> g_e_names;
};


void get_ops(int argc, char **argv, Flags &flags);


#endif  //