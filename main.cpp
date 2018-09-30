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

void bubble_sort_0(my_int_type* array_input, mai_type length);
void insertion_sort_1(my_int_type* array_input, mai_type length);
void selection_sort_2(my_int_type* array_input, mai_type length);
void merge_sort_3(my_int_type* array_input, mai_type length);
void quick_sort_ex_4(my_int_type* array_input, mai_type length);
void quick_sort_in_5(my_int_type* array_input, mai_type length);

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

	switch (algo_code) {
		case 0: {
			bubble_sort_0(my_array, line_num);
			break;
		}
		case 1: {
			insertion_sort_1(my_array, line_num);
			break;
		}
		case 2: {
			selection_sort_2(my_array, line_num);
			break;
		}
		case 3: {
			merge_sort_3(my_array, line_num);
			break;
		}
		case 4: {
			quick_sort_ex_4(my_array, line_num);
			break;
		}
		case 5: {
			quick_sort_in_5(my_array, line_num);
			break;
		}
	}

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

void bubble_sort_0(my_int_type* array_input, mai_type length)
{
	for (mai_type i = 0; i < length; i++) {
		for (mai_type j = 0; j < length - 1; j++) {
			if (array_input[j] > array_input[j + 1]) {
				my_int_type temp = array_input[j];
				array_input[j] = array_input[j + 1];
				array_input[j + 1] = temp;
			}
		}
	}
}
void insertion_sort_1(my_int_type* array_input, mai_type length)
{
	mai_type i = 1;
	while (i < length) {
		mai_type j = i;
		while ((j > 0) && (array_input[j - 1] > array_input[j])) {
			my_int_type temp = array_input[j];
			array_input[j] = array_input[j - 1];
			array_input[j - 1] = temp;
			j--;
		}
		i++;
	}
}
void selection_sort_2(my_int_type* array_input, mai_type length)
{
	bubble_sort_0(array_input, length);
}
void merge_sort_3(my_int_type* array_input, mai_type length)
{
	bubble_sort_0(array_input, length);
}
void quick_sort_ex_4(my_int_type* array_input, mai_type length)
{
	bubble_sort_0(array_input, length);
}
void quick_sort_in_5(my_int_type* array_input, mai_type length)
{
	bubble_sort_0(array_input, length);
}