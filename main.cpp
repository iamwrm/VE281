#include "ut.h"

int main(int argc, char **argv)
{
	Flags flags;
	get_ops(argc, argv, flags);

	std::cout << flags.g_num;
	for (auto a : flags.g_e_names) {
		std::cout << a << std::endl;
	}

	cout << "======\n";

	string line;
	// read from stdin
	while (getline(cin, line)) {
		One_Line_Order olo;
		std::stringstream ss1(line);
		int int_buffer;
		string string_buffer;

		ss1 >> olo.time_stamp;
		ss1 >> olo.client_name;

		ss1 >> string_buffer;
		if (string_buffer == "BUY") {
			olo.is_buy = true;
		} else {
			olo.is_buy = false;
		}

		ss1 >> olo.e_name;

		char em;
		ss1 >> em;
		ss1 >> olo.price;

		ss1 >> em;
		ss1 >> olo.number;

		ss1 >> olo.expire_time;
		olo.print();
		// ss1.clear();
	}

	exit(0);
}