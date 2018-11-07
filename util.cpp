#include "util.h"

void get_ops(int argc, char **argv, int &verbose_flag, std::string &method_name)
{
	int c;

	while (1) {
		static struct option long_options[] = {
		    /* These options set a flag. */
		    {"verbose", no_argument, &verbose_flag, 1},
		    /* These options don’t set a flag.
		       We distinguish them by their indices. */
		    {"implementation", required_argument, 0, 'i'},
		    {0, 0, 0, 0}};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "i:", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
			case 0:
				/* If this option set a flag, do nothing else
				 * now. */
				if (long_options[option_index].flag != 0) break;
				/*
				printf("option %s",
				       long_options[option_index].name);
				if (optarg) printf(" with arg %s", optarg);
				printf("\n");
				*/
				break;
			case 'i':
				// printf("option -i with value `%s'\n",
				// optarg);
				method_name = optarg;
				break;
			case '?':
				/* getopt_long already printed an error message.
				 */
				break;

			default:
				abort();
		}
	}

	/* Print any remaining command line arguments (not options). */
	if (optind < argc) {
		// printf("non-option ARGV-elements: ");
		// while (optind < argc) printf("%s ", argv[optind++]);
		// putchar('\n');
	}
}
