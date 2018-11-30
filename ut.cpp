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
void put_in_ve(Pool &pool, One_Line_Order &olo)
{
	auto it = pool.curr_e_names.find(olo.e_name);
	if (it == pool.curr_e_names.end()) {
		// new equity
		pool.ve.emplace_back(Equity());
		pool.curr_e_names.emplace(
		    std::make_pair(olo.e_name, pool.ve.size() - 1));
		pool.ve[pool.ve.size() - 1].E_name = olo.e_name;
	}
	it = pool.curr_e_names.find(olo.e_name);

	Equity &the_equity = pool.ve[it->second];

	if (olo.is_buy) {
		the_equity.buy_pq.emplace((Order_for_pq(olo.ID, olo.price)));
	} else {
		the_equity.sell_pq.emplace((Order_for_pq(olo.ID, olo.price)));
	}
}

void pay_commission(Pool &pool, const int &number, const int &price)
{
	int commission = price * number / 100;
	commission *= 2;
	pool.commission += commission;
}

void trans_msg(Pool &pool, One_Line_Order buyer, One_Line_Order seller, int num,
	       int price, Flags flags)
{
	pay_commission(pool, num, price);
	if (flags.v_flag) {
		std::cout << buyer.client_name << " purchased " << num
			  << " shares of " << buyer.e_name << " from "
			  << seller.client_name << " for $" << price
			  << "/share\n";
	}
	//  register number of trades
	pool.completed_trades_num++;
	pool.total_money_transferred += price * num;
	pool.num_of_shares_traded += num;

	// median
	auto &the_equity =
	    pool.ve[pool.curr_e_names.find(buyer.e_name)->second];
	the_equity.median_push_back(price);

	// register buyer and seller
	auto f = [](Pool &pool,
		    decltype(buyer) &that_client) -> decltype(pool.vc[0]) & {
		auto it = pool.client_names.find(that_client.client_name);
		if (it == pool.client_names.end()) {
			// no this name
			pool.vc.emplace_back(Client());
			pool.vc[pool.vc.size() - 1].C_name =
			    that_client.client_name;
			pool.client_names.emplace(std::make_pair(
			    that_client.client_name, pool.vc.size() - 1));
		}
		it = pool.client_names.find(that_client.client_name);

		auto &client_found = pool.vc[it->second];
		return client_found;
	};
	// register buyer
	{
		auto &client_found = f(pool, buyer);
		client_found.buy_num += num;
		client_found.net_trans -= num * price;
	}
	// register seller
	{
		auto &client_found = f(pool, seller);
		client_found.sell_num += num;
		client_found.net_trans += num * price;
	}
}

void print_all_equity_median(Pool &pool, int tm)
{
	// interate through all equity
	for (auto it : pool.curr_e_names) {
		auto &that_ve = pool.ve[it.second];
		that_ve.median_print(tm);
	}
}

void print_all_equity_midpoint(Pool &pool, int tm)
{
	// interate through all equity
	for (auto it : pool.curr_e_names) {
		auto &that_ve = pool.ve[it.second];
		int sell_price = -1;
		int buy_price = -1;
		{
			auto &the_pq = that_ve.sell_pq;
			while (!the_pq.empty()) {
				int top_order_id = the_pq.top().ID;

				One_Line_Order &olo_pq =
				    pool.va[top_order_id].olo;

				if (olo_pq.expire_time != -1 &&
				    olo_pq.expire_time + olo_pq.time_stamp <=
					tm) {
					// expired
					the_pq.pop();
					continue;
				}
				sell_price = olo_pq.price;
				break;
			}
		}
		{
			auto &the_pq = that_ve.buy_pq;
			while (!the_pq.empty()) {
				int top_order_id = the_pq.top().ID;

				One_Line_Order &olo_pq =
				    pool.va[top_order_id].olo;

				if (olo_pq.expire_time != -1 &&
				    olo_pq.expire_time + olo_pq.time_stamp <=
					tm) {
					// expired
					the_pq.pop();
					continue;
				}
				buy_price = olo_pq.price;
				break;
			}
		}
		cout << "Midpoint of " << that_ve.E_name << " at time " << tm
		     << " is ";

		if (sell_price != -1 && buy_price != -1) {
			cout << "$" << (buy_price + sell_price) / 2;
		} else {
			cout << "undefined";
		}
		cout << "\n";
	}
}

void Equity_ttt::end_ttt_result(int &buy_time, int &sell_time)
{
	buy_time = -1;
	sell_time = -1;

	int sell_price = -1;
	int buy_price = -1;

	int new_buy_time = -1;    // new buy time time stamp
	int new_buy_price = -1;   // new buy time time stamp
	int new_sell_time = -1;   // new buy time time stamp
	int new_sell_price = -1;  // new buy time time stamp

	int i = 0;

	// first seller
	for (i = 0; i < vo.size(); i++) {
		auto &olo = vo[i];
		if (!olo.is_buy) {
			buy_time = olo.time_stamp;
			buy_price = olo.price;
			i++;
			break;
		}
	}

	for (; i < vo.size(); i++) {
		auto &olo = vo[i];

		if (olo.is_buy && olo.price > sell_price) {
			// higher buyer
			sell_time = olo.time_stamp;
			sell_price = olo.price;
		}

		if (new_sell_time > -1) {
			if (olo.is_buy && olo.price > new_sell_price) {
				// higher buyer
				new_sell_time = olo.time_stamp;
				new_sell_price = olo.price;
			}
		}

		if (!olo.is_buy && olo.price < buy_price) {
			// lower seller
			new_buy_time = olo.time_stamp;
			new_buy_price = olo.price;
			new_sell_price = -1;
		}

		if (new_buy_price > 0 && new_buy_time > 0) {
			if ((new_sell_price - new_buy_price) >
			    sell_price - buy_price) {
				sell_time = new_sell_time;
				sell_price = new_sell_price;
			}
		}
	}
}