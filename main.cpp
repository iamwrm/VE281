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
}

int main(int argc, char **argv)
{
	Flags flags;
	get_ops(argc, argv, flags);

	int order_id = 0;

	Pool pool;
	int current_time_stamp = 0;

	string line;
	while (getline(cin, line)) {
		One_Line_Order olo;
		olo.read(line, order_id);

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