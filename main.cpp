#include "ut.h"

static int verbose_flag = 0;
static int transfers_flag = 0;

int main(int argc, char **argv)
{
	Flags flags;
	get_ops(argc, argv, flags);

	std::cout<<flags.g_num;
	for (auto a : flags.g_e_names) {
		std::cout << a;
	}

	exit(0);
}