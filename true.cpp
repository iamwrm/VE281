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

#define my_int_type long long int
#define mai_type long long int

bool myfunction(int i, int j)
{
	return (i < j);
}

int main()
{
	std::clock_t start;
	double duration;

	start = std::clock();

	/* Your algorithm here */

	int algo_code;
	cin >> algo_code;
	mai_type line_num;
	cin >> line_num;

	mai_type i = 0;
	// NOTE: need delete 1
	my_int_type* my_array = new my_int_type[line_num];
	my_int_type temp_int;
	while (cin >> temp_int) {
		my_array[i] = temp_int;
		i++;
		if (i >= line_num) {
			break;
		}
	}

	for (mai_type i = 0; i < line_num; i++) {
		cout << "myarray[" << i << "]: " << my_array[i] << endl;
	}

	// sorting begins
	std::sort(my_array, my_array + line_num);

	for (mai_type i = 0; i < line_num; i++) {
		if (i == 0) cout << "sorted\n";
		cout << "myarray[" << i << "]: " << my_array[i] << endl;
	}

	// NOTE: deleted 1
	delete[] my_array;
	// deleted 1

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	// std::cout << "printf: " << duration << '\n';

	return 0;
}
