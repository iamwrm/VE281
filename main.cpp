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