#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "util.h"


int main(int argc, char **argv)
{
	// for I/O speedup ============================
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	// ============================================

	// get verbose_flag and method_name ===========
	int verbose_flag = 0;
	std::string method_name;
	get_ops(argc, argv, verbose_flag, method_name);
	// ============================================

	if (verbose_flag) puts("verbose flag is set");
	std::cout << "method chosen:" << method_name << std::endl;

return 0;
}
