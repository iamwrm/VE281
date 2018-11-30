#include "ut.h"

bool find_buyer_and_trade(Pool &pool, One_Line_Order &olo,
			  const int &cur_time_stamp, Flags flags)
{
	std::string e_name = olo.e_name;
	auto it = pool.curr_e_names.find(e_name);
	if (it == pool.curr_e_names.end()) {
		return false;
	}
	// find the equity
	// int id_of_e = it->second;
	auto &the_pq = it->second->buy_pq;

	if (olo.number == 0) {
		return true;
	}

	while (!the_pq.empty()) {
		if (olo.number == 0) {
			return true;
		}
		int top_order_id = the_pq.top().ID;

		One_Line_Order &olo_pq = pool.va[top_order_id];

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
	// int id_of_e = it->second;
	auto &the_pq = it->second->sell_pq;

	if (olo.number == 0) {
		return true;
	}

	while (!the_pq.empty()) {
		if (olo.number == 0) {
			return true;
		}
		int top_order_id = the_pq.top().ID;

		One_Line_Order &olo_pq = pool.va[top_order_id];

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
		pool.curr_e_names.emplace(std::make_pair(
		    olo.e_name, std::make_shared<Equity>(olo.e_name)));
	}
	it = pool.curr_e_names.find(olo.e_name);

	Equity &the_equity = *(it->second);

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

	pool.curr_e_names.find(buyer.e_name)->second->median_push_back(price);

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
		auto &that_ve = *it.second;
		that_ve.median_print(tm);
	}
}

void print_all_equity_midpoint(Pool &pool, int tm)
{
	// interate through all equity
	for (auto it : pool.midpoint_listen_list.names) {
		auto jt = pool.curr_e_names.find(it);
		auto &that_ve = *jt->second;
		int sell_price = -1;
		int buy_price = -1;
		{
			auto &the_pq = that_ve.sell_pq;
			while (!the_pq.empty()) {
				int top_order_id = the_pq.top().ID;

				One_Line_Order &olo_pq = pool.va[top_order_id];

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

				One_Line_Order &olo_pq = pool.va[top_order_id];

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
		cout << "Midpoint of " << that_ve.get_ename() << " at time "
		     << tm << " is ";

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

	end_ttt_result_1(buy_time, sell_time);
	return;
}

void Equity_ttt::end_ttt_result_1(int &buy_time, int &sell_time)
{
	buy_time = -1;
	sell_time = -1;
	int buy_price = -1;
	int sell_price = -1;
	int profit = -1;
	int i = 0;
	for (i = 0; i < vo.size(); i++) {
		auto &olo = vo[i];
		if (!olo.is_buy) {
			// olo is a seller
			// int olo_sell_price = olo.price;

			int buyer_max_price = -1;
			int buyer_max_time = -1;
			for (int j = i; j < vo.size(); j++) {
				auto &olo1 = vo[j];
				if (olo1.is_buy) {
					// olo is a buyer
					if (olo1.price > buyer_max_price) {
						buyer_max_price = olo1.price;
						buyer_max_time =
						    olo1.time_stamp;
					}
				}
			}

			if (buyer_max_price > -1) {
				if (buyer_max_price - olo.price > profit) {
					buy_time = olo.time_stamp;
					buy_price = olo.price;
					sell_time = buyer_max_time;
					sell_price = buyer_max_price;
					profit = buyer_max_price - olo.price;
				}
			}
		}
	}
}

void add_to_midpoint_listen_list(Pool &pool, const One_Line_Order &olo,
				 Flags &flags)
{
	auto it = pool.midpoint_listen_list.names.find(olo.e_name);
	if (it == pool.midpoint_listen_list.names.end()) {
		// new listen name
		pool.midpoint_listen_list.names.emplace(olo.e_name);

		// add to ve
		auto it = pool.curr_e_names.find(olo.e_name);
		if (it == pool.curr_e_names.end()) {
			// new equity
			pool.curr_e_names.emplace(std::make_pair(
			    olo.e_name, std::make_shared<Equity>(olo.e_name)));
		}
	}
}

void print_end_of_day(Pool &pool, Flags &flag)
{
	cout << "---End of Day---\n";
	cout << "Commission Earnings: $" << pool.commission << "\n";
	cout << "Total Amount of Money Transferred: $"
	     << pool.total_money_transferred << "\n";
	cout << "Number of Completed Trades: " << pool.completed_trades_num
	     << "\n";
	cout << "Number of Shares Traded: " << pool.num_of_shares_traded
	     << "\n";
	// print final transfer
	if (flag.t_flag) {
		for (auto it : pool.client_names) {
			auto &c = pool.vc[it.second];
			cout << c.C_name << " bought " << c.buy_num
			     << " and sold " << c.sell_num
			     << " for a net transfer of $" << c.net_trans
			     << "\n";
		}
	}

	// print ttt
	for (auto ttt_name : flag.g_e_names) {
		auto it = pool.ve_ttt_names.find(ttt_name);
		auto &e = pool.ve_ttt[it->second];
		int bt, st;
		e.end_ttt_result(bt, st);
		cout << "Time travelers would buy " << e.E_t_name
		     << " at time: " << bt << " and sell it at time: " << st
		     << "\n";
	}
}

void put_in_vettt(Pool &pool, const One_Line_Order &olo, Flags &flags)
{
	for (auto ge_name : flags.g_e_names) {
		if (ge_name != olo.e_name) {
			continue;
		}

		auto it = pool.ve_ttt_names.find(ge_name);
		if (it == pool.ve_ttt_names.end()) {
			pool.ve_ttt.emplace_back(Equity_ttt(ge_name));
			pool.ve_ttt_names.emplace(
			    std::make_pair(ge_name, pool.ve_ttt.size() - 1));
		}
		it = pool.ve_ttt_names.find(ge_name);

		Equity_ttt &the_equity_ttt = pool.ve_ttt[it->second];
		the_equity_ttt.push_back(olo);
	}
}