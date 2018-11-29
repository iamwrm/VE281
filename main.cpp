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
	while (getline(cin, line)) {
		One_Line_Order olo;
		olo.read(line);
		olo.print();
		// ss1.clear();
	}

	exit(0);
}