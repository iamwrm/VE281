#include "ut.h"

int main(int argc, char **argv)
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);

	Flags flags;
	get_ops(argc, argv, flags);

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
		}

		add_to_midpoint_listen_list(pool, olo, flags);

		// match trade
		bool trade_success =
		    (olo.is_buy) ? find_seller_and_trade(
				       pool, olo, current_time_stamp, flags)
				 : find_buyer_and_trade(
				       pool, olo, current_time_stamp, flags);

		// put in va
		pool.va.emplace(std::make_pair(order_id, olo));
		order_id++;

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