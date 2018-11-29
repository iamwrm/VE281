#include "ut.h"

struct Pool {
	std::map<std::string, int> curr_e_names;

	std::vector<Argu_Order> va;
	std::vector<Equity> ve;
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
		    olo_pq.expire_time + olo_pq.time_stamp < cur_time_stamp) {
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
			olo.number -= olo_pq.number;
			olo_pq.number = 0;
			the_pq.pop();
			continue;
		} else {
			// DONE: trade event
			olo_pq.number -= olo.number;
			olo.number = 0;
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
		    olo_pq.expire_time + olo_pq.time_stamp < cur_time_stamp) {
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
			olo.number -= olo_pq.number;
			olo_pq.number = 0;
			the_pq.pop();
			continue;
		} else {
			// DONE: trade event
			olo_pq.number -= olo.number;
			olo.number = 0;
			return true;
		}
	}
	// buy all pq, still have some left
	return false;
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
		olo.print();
		if (current_time_stamp != olo.time_stamp) {
			// print median

			// print midpoint

			current_time_stamp = olo.time_stamp;

			// check expired order
		}

		// match it
		bool trade_success;
		if (olo.is_buy) {
			trade_success = find_seller_and_trade(
			    pool, olo, current_time_stamp);
		} else {
			trade_success =
			    find_buyer_and_trade(pool, olo, current_time_stamp);
		}
		if (trade_success) {
			continue;
		}
		if (olo.expire_time == 0) {
			continue;
		}

		// put in va
		pool.va.emplace_back(std::move(Argu_Order(olo, 1)));
		// put in ve
		put_in_ve(pool, olo);
	}

	cout << "ve size " << pool.ve.size() << std::endl;

	auto a = pool.ve[0];
	while (!a.buy_pq.empty()) {
		auto b = a.buy_pq.top();
		auto c = pool.va[b.ID];
		auto d = c.olo;
		d.print();
		a.buy_pq.pop();
	}

	cout << "====\n";
	while (!a.sell_pq.empty()) {
		auto b = a.sell_pq.top();
		auto c = pool.va[b.ID];
		auto d = c.olo;
		d.print();
		a.sell_pq.pop();
	}

	exit(0);
}