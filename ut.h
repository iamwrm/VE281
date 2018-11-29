#ifndef UT_H
#define UT_H
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
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
	int ID;

       public:
	void read(std::string line, int &order_id)
	{
		ID = order_id++;
		std::stringstream ss1(line);
		string string_buffer;

		ss1 >> time_stamp >> client_name >> string_buffer;

		is_buy = (string_buffer == "BUY") ? true : false;

		char em;
		ss1 >> e_name >> em >> price >> em >> number >> expire_time;
	}
	void print()
	{
		std::cout << ID << " " << time_stamp << " " << client_name
			  << " " << is_buy << " " << e_name << " " << price
			  << " " << number << " " << expire_time << " "
			  << std::endl;
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