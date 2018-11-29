#include "ut.h"

struct Pool {
	std::map<std::string, int> curr_e_names;

	std::vector<Argu_Order> va;
	std::vector<Equity> ve;
	int commission = 0;
};

void put_in_ve(Pool &pool, One_Line_Order &olo)
{
	auto it = pool.curr_e_names.find(olo.e_name);
	if (it == pool.curr_e_names.end()) {
		// new equity
		pool.ve.emplace_back(std::move(Equity()));
		pool.curr_e_names.emplace(
		    std::make_pair(olo.e_name, pool.ve.size() - 1));
	}
	it = pool.curr_e_names.find(olo.e_name);

	Equity &the_equity = pool.ve[it->second];

	if (olo.is_buy) {
		the_equity.buy_pq.emplace(
		    std::move(Order_for_pq(olo.ID, olo.price)));
	} else {
		the_equity.sell_pq.emplace(
		    std::move(Order_for_pq(olo.ID, olo.price)));
	}
}

void pay_commission(Pool &pool, const int &number, const int &price)
{
	int commission = price * number / 100;
	commission *= 2;
	pool.commission += commission;
}

void trans_msg(One_Line_Order buyer, One_Line_Order seller, int num, int price)
{
	std::cout << buyer.client_name << " purchased " << num << " shares of "
		  << buyer.e_name << " from " << seller.client_name << " for $"
		  << price << "/share\n";
}

bool find_buyer_and_trade(Pool &pool, One_Line_Order &olo,
			  const int &cur_time_stamp)
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
			pay_commission(pool, trade_num, trade_price);
			trans_msg(olo_pq, olo, trade_num, trade_price);
			olo.number -= trade_num;
			olo_pq.number -= trade_num;
			the_pq.pop();
			continue;
		} else {
			// DONE: trade event
			int trade_price = olo_pq.price;
			int trade_num = olo.number;
			pay_commission(pool, trade_num, trade_price);
			trans_msg(olo_pq, olo, trade_num, trade_price);
			olo_pq.number -= trade_num;
			olo.number -= trade_num;
			return true;
		}
	}
	// sell to all pq, still have some left
	return false;
}

bool find_seller_and_trade(Pool &pool, One_Line_Order &olo,
			   const int &cur_time_stamp)
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
			pay_commission(pool, trade_num, trade_price);
			trans_msg(olo, olo_pq, trade_num, trade_price);
			olo.number -= trade_num;
			olo_pq.number -= trade_num;
			the_pq.pop();
			continue;
		} else {
			// DONE: trade event
			int trade_price = olo_pq.price;
			int trade_num = olo.number;
			pay_commission(pool, trade_num, trade_price);
			trans_msg(olo, olo_pq, trade_num, trade_price);
			olo_pq.number -= trade_num;
			olo.number = trade_num;
			return true;
		}
	}
	// buy all pq, still have some left
	return false;
}

void print_all(const Pool &pool)
{
	int size = pool.va.size();
	for (int i = 0; i < size; i++) {
		cout << "line num:" << pool.va[i].olo.ID + 1 << " expire time "
		     << pool.va[i].olo.expire_time << std::endl;
	}
}

void print_end_of_day(Pool pool, Flags flag)
{
	cout << "---End of Day---\n";
	cout << "Commission Earnings: $" << pool.commission << std::endl;
}

int main(int argc, char **argv)
{
	Flags flags;
	get_ops(argc, argv, flags);

	std::cout << flags.g_num;
	for (auto a : flags.g_e_names) {
		std::cout << a << std::endl;
	}

	cout << "======\n";

	int order_id = 0;

	Pool pool;
	int current_time_stamp = 0;

	string line;
	while (getline(cin, line)) {
		One_Line_Order olo;
		olo.read(line, order_id);
		// olo.print();
		if (current_time_stamp != olo.time_stamp) {
			// print median

			// print midpoint

			current_time_stamp = olo.time_stamp;

			// check expired order
		}

		// print_all(pool);

		// match it
		bool trade_success =
		    (olo.is_buy)
			? find_seller_and_trade(pool, olo, current_time_stamp)
			: find_buyer_and_trade(pool, olo, current_time_stamp);

		// put in va
		pool.va.emplace_back(std::move(Argu_Order(olo, 1)));
		if (trade_success) {
			continue;
		}

		if (olo.expire_time == 0) {
			continue;
		}

		put_in_ve(pool, olo);
	}

	print_end_of_day(pool, flags);

	exit(0);
}