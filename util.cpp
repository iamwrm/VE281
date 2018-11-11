#include "util.h"

void Grid::trace_back_path(const int start_id, const int end_id)
{
	// for brief
	cout << "The shortest path from (";
	cout << start_id % width << ", " << start_id / width << ") to (";
	cout << end_id % width << ", " << end_id / width << ") is ";
	cout << get_pathcost(end_id) << "." << endl;
	cout << "Path:" << endl;
	std::vector<std::string> v1;
	int c_id = end_id;
	while (1) {
		stringstream s1;
		int c_x = c_id % width;
		int c_y = c_id / width;
		s1 << "(" << c_x << ", " << c_y << ")" << endl;
		c_id = get_pre(c_id);
		v1.emplace_back(std::move(s1.str()));
		if (c_id == start_id) {
			stringstream s1;
			int c_x = c_id % width;
			int c_y = c_id / width;
			s1 << "(" << c_x << ", " << c_y << ")" << endl;
			c_id = get_pre(c_id);
			v1.emplace_back(std::move(s1.str()));
			break;
		}
	}
	for (int it = v1.size() - 1; it >= 0; it--) {
		cout << v1[it];
	}
}

void Grid::diki(priority_queue<Point_Ptr, compare_t> &PQ, const int start_id,
		const int end_id)
{
	int counter = 0;
	set_pathcost(start_id, get_weight(start_id));
	set_reached(start_id, true);

	PQ.enqueue(get_ptr(start_id));
	while (!PQ.empty()) {
		// print_grid();
		Point_Ptr C_ptr = PQ.dequeue_min();
		int C = C_ptr.id;
		if (verbose_wr) {
			cout << "Step " << counter;
			cout << "\nChoose cell (" << C % width << ", "
			     << C / width << ") with accumulated length "
			     << get_pathcost(C) << "." << endl;
		}
		counter++;
		// four neighbors
		int Cx, Cy;
		id_to_xy(Cx, Cy, C);
		int r, d, l, u;
		r = xy_to_id(Cx + 1, Cy);
		d = xy_to_id(Cx, Cy + 1);
		l = xy_to_id(Cx - 1, Cy);
		u = xy_to_id(Cx, Cy - 1);
		int neighbors[] = {r, d, l, u};
		// cout << "C:" << C << endl;
		for (int i = 0; i < 4; i++) {
			if (neighbors[i] < 0) {
				continue;
			}
			if (get_reached(neighbors[i])) {
				continue;
			}
			int N = neighbors[i];
			set_pathcost(N, get_pathcost(C) + get_weight(N));
			set_reached(N, true);
			set_pre(N, C);
			// cout << "N:" << N << endl;
			if (end_id == N) {
				trace_back_path(start_id, end_id);
				return;
			} else {
				PQ.enqueue(get_ptr(N));
				if (verbose_wr) {
					cout << "Cell (" << N % width << ", "
					     << N / width
					     << ") with accumulated length "
					     << get_pathcost(N)
					     << " is added into the queue."
					     << endl;
				}
			}
		}
	}
}

inline const int Grid::xy_to_id(const int x, const int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return x + y * width;
	} else {
		return -1;
	}
}

inline void Grid::id_to_xy(int &x, int &y, const int id)
{
	assert(id < height * width);
	x = id % width;
	y = id / width;
}

void Grid::print_grid()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int id = xy_to_id(j, i);
			std::cout << "  "
				  << "|" << get_weight(id) << "="
				  << get_reached(id) << "|" << get_pathcost(id)
				  << "|" << get_pre(id) << "|";
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
			set_weight(xy_to_id(j, i), weight_temp);
			set_reached(xy_to_id(j, i), false);
			set_pathcost(xy_to_id(j, i), 0);
			set_pre(xy_to_id(j, i), -1);
			ptrs[xy_to_id(j, i)].id = xy_to_id(j, i);
			ptrs[xy_to_id(j, i)].pathcost =
			    pathcost + xy_to_id(j, i);
			ptrs[xy_to_id(j, i)].pre = pre + xy_to_id(j, i);
			ptrs[xy_to_id(j, i)].reached = reached + xy_to_id(j, i);
			ptrs[xy_to_id(j, i)].weight = weight + xy_to_id(j, i);
		}
	}
}

Grid::Grid(int width_i, int height_i, bool verbose_input)
{
	verbose_wr = verbose_input;
	width = width_i;
	height = height_i;
	// cout << "Grid: width:" << width << "Grid: height:" << height << endl;
	reached = new bool[width * height];
	weight = new int[width * height];
	pathcost = new int[width * height];
	pre = new int[width * height];
	ptrs = new Point_Ptr[width * height];
}

Grid::~Grid()
{
	delete[] reached;
	delete[] weight;
	delete[] pre;
	delete[] pathcost;
	delete[] ptrs;
}

void Grid::set_pathcost(int id, int cost)
{
	assert(id >= 0 && id < height * width);
	pathcost[id] = cost;
}

void Grid::set_weight(int id, int weight_i)
{
	assert(id >= 0 && id < height * width);
	weight[id] = weight_i;
}

void Grid::set_pre(int id, int id_pre)
{
	assert(id >= 0 && id < height * width);
	pre[id] = id_pre;
}

void Grid::set_reached(int id, bool reach)
{
	assert(id >= 0 && id < height * width);
	reached[id] = reach;
}

//===========================================

const int Grid::get_weight(int id)
{
	assert(id >= 0 && id < height * width);
	return weight[id];
}

const bool Grid::get_reached(int id)
{
	assert(id >= 0 && id < height * width);
	return reached[id];
}

const int Grid::get_pathcost(int id)
{
	assert(id >= 0 && id < height * width);
	return pathcost[id];
}

const int Grid::get_pre(int id)
{
	assert(id >= 0 && id < height * width);
	return pre[id];
}

void Interface_Property::get_ops(int argc, char **argv)
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
