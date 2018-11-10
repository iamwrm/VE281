#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <deque>
#include <list>
#include <vector>

#include "binary_heap.h"
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
	Interface_Property interface_property(argc, argv);
	// ============================================

	if (interface_property.get_verbose_flag()) puts("verbose flag is set");
	std::cout << "method chosen:" << interface_property.get_method_name()
		  << std::endl;

	// input
	int width, height, start_x, start_y, end_x, end_y;

	cin >> width >> height;
	cin >> start_x >> start_y;
	cin >> end_x >> end_y;

	Grid grid(width, height);
	grid.read_weight_from_cin();

	cout << width << " " << height << endl;
	grid.print_grid();

	priority_queue<int> *pq = new binary_heap<int>;
	auto &PQ = *pq;

	grid.diki(PQ, start_x + start_y * width, end_x + end_y * width);

	delete pq;
	return 0;
}
