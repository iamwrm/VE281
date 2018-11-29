#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static int verbose_flag=0;
static int transfers_flag=0;

int main(int argc, char **argv)
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
				//puts("option -v\n");
				verbose_flag = 1;
				break;
			case 't':
				//puts("option -t\n");
				transfers_flag = 1;
				break;
			case 'm':
				puts("option -m\n");
				break;
			case 'p':
				puts("option -p\n");
				break;

			case 'g':
				printf("option -g with value `%s'\n", optarg);
				break;
			default:
				abort();
		}
	}


	exit(0);
}