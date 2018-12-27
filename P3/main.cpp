#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <deque>
#include <list>
#include <vector>

#include "binary_heap.h"
#include "fib_heap.h"
#include "unsorted_heap.h"
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

	// if (interface_property.get_verbose_flag()) puts("verbose flag is
	// set");
	// std::cout << "method chosen:" << interface_property.get_method_name()
	//<< std::endl;

	// input
	int width, height, start_x, start_y, end_x, end_y;

	cin >> width >> height;
	cin >> start_x >> start_y;
	cin >> end_x >> end_y;

	width_g=width;

	Grid grid(width, height, interface_property.get_verbose_flag());
	grid.read_weight_from_cin();


	// cout << width << " " << height << endl;
	// grid.print_grid();

	priority_queue<Point_Ptr, compare_t> *pq;

	if (interface_property.get_method_name() == "BINARY") {
		pq = new binary_heap<Point_Ptr, compare_t>;
	}
	if (interface_property.get_method_name() == "UNSORTED") {
		pq = new unsorted_heap<Point_Ptr, compare_t>;
	}
	if (interface_property.get_method_name() == "FIBONACCI") {
		pq = new fib_heap<Point_Ptr, compare_t>;
	}
	// pq is tested

	auto &PQ = *pq;

	grid.diki(PQ, start_x + start_y * width, end_x + end_y * width);

	delete pq;
	return 0;
}
