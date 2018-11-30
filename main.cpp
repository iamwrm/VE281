#include "ut.h"

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
	if (flags.t_flag) {
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
}

void print_all_equity_median(Pool &pool, int tm, Flags flags)
{
	// interate through all equity
	for (auto it : pool.curr_e_names) {
		auto &that_ve = pool.ve[it.second];
		if (flags.m_flag) {
			that_ve.median_print(tm);
		}
		// print if not empty
	}
}

void print_end_of_day(Pool pool, Flags flag)
{
	cout << "---End of Day---\n";
	cout << "Commission Earnings: $" << pool.commission << std::endl;
	cout << "Total Amount of Money Transferred: $"
	     << pool.total_money_transferred << std::endl;
	cout << "Number of Completed Trades: " << pool.completed_trades_num
	     << std::endl;
	cout << "Number of Shares Traded: " << pool.num_of_shares_traded
	     << std::endl;
}

int main(int argc, char **argv)
{
	Flags flags;
	get_ops(argc, argv, flags);

	// std::cout << flags.g_num;
	// for (auto a : flags.g_e_names) {
	//  std::cout << a << std::endl;
	// }

	int order_id = 0;

	Pool pool;
	int current_time_stamp = 0;

	string line;
	while (getline(cin, line)) {
		One_Line_Order olo;
		olo.read(line, order_id);

		if (current_time_stamp != olo.time_stamp) {
			// print median
			print_all_equity_median(pool, current_time_stamp,flags);

			// print midpoint

			current_time_stamp = olo.time_stamp;

			// check expired order
		}

		// match trade
		bool trade_success =
		    (olo.is_buy) ? find_seller_and_trade(
				       pool, olo, current_time_stamp, flags)
				 : find_buyer_and_trade(
				       pool, olo, current_time_stamp, flags);

		// put in va
		pool.va.emplace_back((Argu_Order(olo, 1)));

		if (trade_success || !olo.expire_time) {
			continue;
		}

		put_in_ve(pool, olo);
	}

	print_end_of_day(pool, flags);

	exit(0);
}