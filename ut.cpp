#include "ut.h"

void get_ops(int argc, char **argv, Flags &flags)
{
	int c;

	while (1) {
		static struct option long_options[] = {
		    {"verbose", no_argument, 0, 'v'},
		    {"transfers", no_argument, 0, 't'},
		    {"median", no_argument, 0, 'm'},
		    {"midpoint", no_argument, 0, 'p'},
		    {"ttt", required_argument, 0, 'g'},
		    {0, 0, 0, 0}};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "vtmpg:", long_options,
				&option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
			case 'v':
				// puts("option -v\n");
				flags.v_flag = 1;
				break;
			case 't':
				// puts("option -t\n");
				flags.t_flag = 1;
				break;
			case 'm':
				flags.m_flag = 1;
				break;
			case 'p':
				flags.p_flag = 1;
				break;

			case 'g':
				flags.g_num++;
				flags.g_e_names.push_back(std::string(optarg));
				break;
			default:
				abort();
		}
	}
	return;
}

bool find_buyer_and_trade(Pool &pool, One_Line_Order &olo,
			  const int &cur_time_stamp, Flags flags)
{
	std::string e_name = olo.e_name;
	auto it = pool.curr_e_names.find(e_name);
	if (it == pool.curr_e_names.end()) {
		return false;
	}
	// find the equity
	int id_of_e = it->second;
	auto &the_pq = pool.ve[id_of_e].buy_pq;

	if (olo.number == 0) {
		return true;
	}

	while (!the_pq.empty()) {
		if (olo.number == 0) {
			return true;
		}
		int top_order_id = the_pq.top().ID;

		One_Line_Order &olo_pq = pool.va[top_order_id].olo;

		if (olo_pq.expire_time != -1 &&
		    olo_pq.expire_time + olo_pq.time_stamp <= cur_time_stamp) {
			// expired
			the_pq.pop();
			continue;
		}

		if (olo_pq.price < olo.price) {
			// the highest price is lower than wanted
			return false;
		}

		if (olo.number >= olo_pq.number) {
			// DONE: trade event
			int trade_price = olo_pq.price;
			int trade_num = olo_pq.number;
			trans_msg(pool, olo_pq, olo, trade_num, trade_price,
				  flags);
			olo.number -= trade_num;
			olo_pq.number -= trade_num;
			the_pq.pop();
			continue;
		} else {
			// DONE: trade event
			int trade_price = olo_pq.price;
			int trade_num = olo.number;
			trans_msg(pool, olo_pq, olo, trade_num, trade_price,
				  flags);
			olo_pq.number -= trade_num;
			olo.number -= trade_num;
			return true;
		}
	}
	// sell to all pq, still have some left
	return false;
}

bool find_seller_and_trade(Pool &pool, One_Line_Order &olo,
			   const int &cur_time_stamp, Flags flags)
{
	std::string e_name = olo.e_name;
	auto it = pool.curr_e_names.find(e_name);
	if (it == pool.curr_e_names.end()) {
		return false;
	}
	// find the equity
	int id_of_e = it->second;
	auto &the_pq = pool.ve[id_of_e].sell_pq;

	if (olo.number == 0) {
		return true;
	}

	while (!the_pq.empty()) {
		if (olo.number == 0) {
			return true;
		}
		int top_order_id = the_pq.top().ID;

		One_Line_Order &olo_pq = pool.va[top_order_id].olo;

		if (olo_pq.expire_time != -1 &&
		    olo_pq.expire_time + olo_pq.time_stamp <= cur_time_stamp) {
			// expired
			the_pq.pop();
			continue;
		}

		if (olo_pq.price > olo.price) {
			// the lowest price is higher than wanted
			return false;
		}

		if (olo.number >= olo_pq.number) {
			// DONE: trade event
			int trade_price = olo_pq.price;
			int trade_num = olo_pq.number;
			trans_msg(pool, olo, olo_pq, trade_num, trade_price,
				  flags);
			olo.number -= trade_num;
			olo_pq.number -= trade_num;
			the_pq.pop();
			continue;
		} else {
			// DONE: trade event
			int trade_price = olo_pq.price;
			int trade_num = olo.number;
			trans_msg(pool, olo, olo_pq, trade_num, trade_price,
				  flags);
			olo_pq.number -= trade_num;
			olo.number = trade_num;
			return true;
		}
	}
	// buy all pq, still have some left
	return false;
}

void Equity::median_push_back(const int &i)
{
	median_e.push_back(i);
}

void Equity::median_print(int tm)
{
	int median = median_e.get_m();
	if (median != -1) {
		std::cout << "Median match price of " << E_name << " at time "
			  << tm << " is $" << median << "\n";
	}
}

void Median_E::push_back(int i)
{
	data.push_back(i);
}
int Median_E::get_m()
{
	std::sort(data.begin(), data.end());
	int size = data.size();
	if (size > 0) {
		int median = (size % 2 == 1)
				 ? (data[size / 2])
				 : ((data[size / 2] + data[size / 2 - 1]) / 2);
		return median;
	} else {
		return -1;
	}
}