#include "util.h"

void Grid::print_grid()
{
	for (int i = 0; i < height; i++) {
		cout << "i:" << i << endl;
		for (int j = 0; j < width; j++) {
			int temp_w = get_weight(j, i);
			std::cout << "_" << temp_w << "_";
		}
		std::cout << std::endl;
	}
}

void Grid::read_weight_from_cin()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int weight_temp;
			std::cin >> weight_temp;
			set_weight(j, i, weight_temp);
			set_reached(j, i, false);
		}
	}
}

Grid::Grid(int width_i, int height_i)
{
	width = width_i;
	height = height_i;
	cout << "Grid: width:" << width << "Grid: height:" << height << endl;
	reached = new bool[width * height];
	weight = new int[width * height];
}

Grid::~Grid()
{
	delete[] reached;
	delete[] weight;
}

void Grid::set_weight(int x, int y, int weight_i)
{
	assert(x < width);
	assert(y < height);
	weight[x + y * width] = weight_i;
}

void Grid::set_reached(int x, int y, bool if_reached)
{
	assert(x < width);
	assert(y < height);
	reached[x + y * width] = if_reached;
}

const int Grid::get_weight(int x, int y)
{
	assert(x < width);
	assert(y < height);
	return weight[x + y * width];
}

const bool Grid::get_reached(int x, int y)
{
	assert(x < width);
	assert(y < height);
	return reached[x + y * width];
}

void Interface_Property::get_ops(int argc, char **argv)
// void get_ops(int argc, char **argv, int &verbose_flag, std::string
// &method_name)
{
	int c;

	while (1) {
		static struct option long_options[] = {
		    /* These options set a flag. */
		    //{"verbose", no_argument, &verbose_flag, 1},
		    {"verbose", no_argument, 0, 'v'},
		    /* These options don’t set a flag.
		       We distinguish them by their indices. */
		    {"implementation", required_argument, 0, 'i'},
		    {0, 0, 0, 0}};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "vi:", long_options, &option_index);

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
			case 'v':
				// printf("option -i with value `%s'\n",
				// optarg);
				verbose_flag = 1;
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
