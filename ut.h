#ifndef UT_H
#define UT_H
#include <getopt.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
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

struct Order_for_pq {
	int ID;
	int price;
	Order_for_pq(int ID, int price) : ID(ID), price(price)
	{
	}
};

class Median_E {
	vector<int> data;

       public:
	void push_back(int i);
	int get_m();
};

// TODO:

class Compare_s {
       public:
	bool operator()(Order_for_pq o1, Order_for_pq o2)
	{
		if (o1.price > o2.price) {
			return true;
		} else if (o1.price == o2.price) {
			if (o1.ID > o2.ID) {
				return true;
			}
		}
		return false;
	}
};
class Compare_b {
       public:
	bool operator()(Order_for_pq o1, Order_for_pq o2)
	{
		if (o1.price < o2.price) {
			return true;
		} else if (o1.price == o2.price) {
			if (o1.ID > o2.ID) {
				return true;
			}
		}
		return false;
	}
};

class Equity {
	vector<One_Line_Order> v_for_ttt;
	Median_E median_e;

       public:
	std::string E_name;
	void median_push_back(const int &i);
	void median_print(int tm);
	std::priority_queue<Order_for_pq, vector<Order_for_pq>, Compare_s>
	    sell_pq;
	std::priority_queue<Order_for_pq, vector<Order_for_pq>, Compare_b>
	    buy_pq;
};

struct Client {
	std::string C_name;
	int buy_num = 0;
	int sell_num = 0;
	int net_trans = 0;
};

class Equity_ttt {
	vector<One_Line_Order> vo;

       public:
	std::string E_t_name;
	Equity_ttt(std::string name)
	{
		E_t_name = name;
	}
	void push_back(const One_Line_Order olo)
	{
		vo.push_back((olo));
	}
	void end_ttt_result(int &buy_time, int &sell_time);
};

class Midpoint_Listen_List {
       public:
	std::map<std::string, int> names;
};

struct Pool {
	std::map<std::string, int> curr_e_names;
	std::map<std::string, int> client_names;

	std::vector<One_Line_Order> va;

	std::vector<Equity> ve;

	std::vector<Client> vc;

	int commission = 0;
	int completed_trades_num = 0;
	int total_money_transferred = 0;
	int num_of_shares_traded = 0;

	std::vector<Equity_ttt> ve_ttt;
	std::map<std::string, int> ve_ttt_names;

	Midpoint_Listen_List midpoint_listen_list;
};

// void get_ops(int argc,char ** argv int&, int&);

void get_ops(int argc, char **argv, Flags &flags);
bool find_seller_and_trade(Pool &pool, One_Line_Order &olo,
			   const int &cur_time_stamp, Flags flags);
bool find_buyer_and_trade(Pool &pool, One_Line_Order &olo,
			  const int &cur_time_stamp, Flags flags);
void trans_msg(Pool &pool, One_Line_Order buyer, One_Line_Order seller, int num,
	       int price, Flags flags);
void put_in_ve(Pool &pool, One_Line_Order &olo);
void pay_commission(Pool &pool, const int &number, const int &price);
void print_all_equity_median(Pool &pool, int tm);
void print_all_equity_midpoint(Pool &pool, int tm);

#endif  // UT_H