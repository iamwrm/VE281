#include "bot.h"

void get_ops(int argc, char **argv, Flags &flags)
{
	int c;

	while (1) {
		static struct option long_options[] = {
		    {"verbose", no_argument, 0, 'v'},
		    {"transfers", no_argument, 0, 't'},
		    {"median", no_argument, 0, 'm'},
		    {"midpoint", no_argument, 0, 'p'},
		    {"ttt", required_argument, 0, 'g'},
		    {0, 0, 0, 0}};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "vtmpg:", long_options,
				&option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
			case 'v':
				flags.v_flag = 1;
				break;
			case 't':
				flags.t_flag = 1;
				break;
			case 'm':
				flags.m_flag = 1;
				break;
			case 'p':
				flags.p_flag = 1;
				break;

			case 'g':
				flags.g_num++;
				flags.g_e_names.push_back(std::string(optarg));
				break;
			default:
				abort();
		}
	}
	return;
}
