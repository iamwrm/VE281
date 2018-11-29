#ifndef UT_H
#define UT_H
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

struct One_Line_Order {
	int time_stamp;
	std::string client_name;
	bool is_buy;
	std::string e_name;
	int price;
	int number;
	int expire_time;
	void read(std::string line)
	{
		std::stringstream ss1(line);
		string string_buffer;

		ss1 >> time_stamp;
		ss1 >> client_name;

		ss1 >> string_buffer;
		if (string_buffer == "BUY") {
			is_buy = true;
		} else {
			is_buy = false;
		}

		ss1 >> e_name;

		char em;
		ss1 >> em;
		ss1 >> price;

		ss1 >> em;
		ss1 >> number;

		ss1 >> expire_time;
	}
	void print()
	{
		std::cout << time_stamp << " " << client_name << " " << is_buy
			  << " " << e_name << " " << price << " " << number
			  << " " << expire_time << " " << std::endl;
	}
};

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