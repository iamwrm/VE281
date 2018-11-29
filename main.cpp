#include "ut.h"

int order_id = 0;

class A_Ptr {
};

class Pool {
	std::map<std::string, A_Ptr> curr_e_names;
};

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
	while (getline(cin, line)) {
		One_Line_Order olo;
		olo.read(line, order_id);
		olo.print();
	}

	exit(0);
}