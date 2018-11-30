#include "ut.h"

void print_end_of_day(Pool &pool, Flags &flag)
{
	cout << "---End of Day---\n";
	cout << "Commission Earnings: $" << pool.commission << std::endl;
	cout << "Total Amount of Money Transferred: $"
	     << pool.total_money_transferred << std::endl;
	cout << "Number of Completed Trades: " << pool.completed_trades_num
	     << std::endl;
	cout << "Number of Shares Traded: " << pool.num_of_shares_traded
	     << std::endl;
	// print final transfer
	for (auto it : pool.client_names) {
		auto &c = pool.vc[it.second];
		// BluthCorp bought 50 and sold 22 for a net transfer of $-3900
		cout << c.C_name << " bought " << c.buy_num << " and sold "
		     << c.sell_num << " for a net transfer of $" << c.net_trans
		     << "\n";
	}

	// print ttt
	for (auto it : pool.ve_ttt_names) {
		auto &e = pool.ve_ttt[it.second];
		int bt, st;
		e.end_ttt_result(bt, st);
		cout << "Time travelers would buy " << e.E_t_name
		     << "at time: " << bt << " and sell it at time: " << st
		     << "\n";
	}
}
void put_in_vettt(Pool &pool, const One_Line_Order &olo, Flags &flags)
{
	for (auto ge_name : flags.g_e_names) {
		// std::cerr << ge_name << std::endl;

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
		// std::cerr << olo.e_name << std::endl;
	}
}

int main(int argc, char **argv)
{
	Flags flags;
	get_ops(argc, argv, flags);

	// std::cerr << "flags ge num:" << flags.g_e_names.size() << std::endl;

	int order_id = 0;

	Pool pool;
	int current_time_stamp = 0;

	string line;
	while (getline(cin, line)) {
		One_Line_Order olo;
		olo.read(line, order_id);

		put_in_vettt(pool, olo, flags);

		if (current_time_stamp != olo.time_stamp) {
			// print median
			if (flags.m_flag) {
				print_all_equity_median(pool,
							current_time_stamp);
			}

			// print midpoint
			if (flags.p_flag) {
				print_all_equity_midpoint(pool,
							  current_time_stamp);
			}

			current_time_stamp = olo.time_stamp;

			// check expired order DONE:
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

	// print median
	if (flags.m_flag) {
		print_all_equity_median(pool, current_time_stamp);
	}

	// print midpoint
	if (flags.p_flag) {
		print_all_equity_midpoint(pool, current_time_stamp);
	}

	print_end_of_day(pool, flags);

	exit(0);
}