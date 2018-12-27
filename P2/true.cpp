#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char const* argv[])
{
	std::clock_t start;
	srand(time(NULL));
	double duration;

	start = std::clock();

	/* Your algorithm here */

	int algo_code;
	cin >> algo_code;
	int line_num;
	cin >> line_num;
	int n_th;
	cin >> n_th;

	int i = 0;
	// NOTE: need delete 1
	long* my_array = new long[line_num];
	long temp_int;
	while (cin >> temp_int) {
		my_array[i] = temp_int;
		i++;
		if (i >= line_num) {
			break;
		}
	}

	// sorting begins

	sort(my_array, my_array + line_num);

	for (int i = 0; i < line_num; i++) {
		if (i == n_th) {
			cout << "The order-" << n_th << " item is "
			     << my_array[i] << endl;
			break;
		}
	}

	// NOTE: deleted 1
	delete[] my_array;
	// deleted 1

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	// std::cout << "printf: " << duration << '\n';

	return 0;
}
