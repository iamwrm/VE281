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
void merge_sort_helper(my_int_type* a, mai_type left, mai_type right);
void merge_sort_merge(my_int_type* a, mai_type left, mai_type mid,
		      mai_type right);
void quick_sort_ex_4(my_int_type* array_input, mai_type length);
void quick_sort_ex_4_helper(my_int_type* a, mai_type left, mai_type right);
mai_type quick_sort_ex_4_partion(my_int_type* a, mai_type left, mai_type right);

void quick_sort_in_5(my_int_type* array_input, mai_type length);
void quick_sort_in_5_helper(my_int_type* a, mai_type left, mai_type right);
mai_type quick_sort_in_5_partion(my_int_type* a, mai_type left, mai_type right);

int main()
{
	std::clock_t start;
	srand(time(NULL));
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

	/*
	for (mai_type i = 0; i < line_num; i++) {
		if (i == 0) cout << "sorted\n";
		cout << "myarray[" << i << "]: " << my_array[i] << endl;
	}
	*/

	for (mai_type i = 0; i < line_num; i++) {
		cout << my_array[i] << endl;
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
	for (mai_type i = 0; i < length - 1; i++) {
		mai_type Min = i;
		for (mai_type j = i + 1; j < length; j++) {
			if (array_input[j] < array_input[Min]) {
				Min = j;
			}
		}
		if (Min != i) {
			swap(array_input[i], array_input[Min]);
		}
	}
}
void merge_sort_3(my_int_type* array_input, mai_type length)
{
	merge_sort_helper(array_input, 0, length - 1);
}

void merge_sort_helper(my_int_type* a, mai_type left, mai_type right)
{
	if (left >= right) return;
	my_int_type mid = (left + right) / 2;
	merge_sort_helper(a, left, mid);
	merge_sort_helper(a, mid + 1, right);
	merge_sort_merge(a, left, mid, right);
}
void merge_sort_merge(my_int_type* a, mai_type left, mai_type mid,
		      mai_type right)
{
	my_int_type* temp = new my_int_type[right - left + 1];
	mai_type i = 0;
	mai_type j = 0;
	mai_type k = 0;

	mai_type first_lengh = mid - left + 1;
	mai_type second_lengh = right - mid;
	while ((i < first_lengh) && (j < second_lengh)) {
		if (a[left + i] <= a[mid + 1 + j]) {
			temp[k++] = a[left + i++];
		} else {
			temp[k++] = a[mid + 1 + j++];
		}
	}

	while (i < first_lengh) {
		temp[k++] = a[left + i++];
	}

	while (j < second_lengh) {
		temp[k++] = a[mid + 1 + j++];
	}

	for (mai_type i = 0; i < k; i++) {
		a[left + i] = temp[i];
	}
	delete[] temp;
}

void quick_sort_ex_4(my_int_type* array_input, mai_type length)
{
	quick_sort_ex_4_helper(array_input, 0, length - 1);
}

void quick_sort_ex_4_helper(my_int_type* a, mai_type left, mai_type right)
{
	if (left >= right) return;
	mai_type pivot = quick_sort_ex_4_partion(a, left, right);
	quick_sort_ex_4_helper(a, left, pivot - 1);
	quick_sort_ex_4_helper(a, pivot + 1, right);
}

mai_type quick_sort_ex_4_partion(my_int_type* a, mai_type left, mai_type right)
{
	my_int_type* temp = new my_int_type[right - left + 1];

	mai_type chosen_one_pos = rand() % (right - left + 1) + left;

	my_int_type chosen_one_value = a[chosen_one_pos];

	mai_type temp_i = 0;
	mai_type temp_i_minus = right - left;

	for (mai_type i = left; i < right + 1; i++) {
		if (i == chosen_one_pos) {
			continue;
		}
		if (a[i] < chosen_one_value) {
			temp[temp_i++] = a[i];
		} else {
			temp[temp_i_minus--] = a[i];
		}
	}

	temp[temp_i] = chosen_one_value;

	assert(temp_i == temp_i_minus);

	for (mai_type i = 0; i < right - left + 1; i++) {
		a[left + i] = temp[i];
	}

	delete[] temp;

	return (left + temp_i);
}

void quick_sort_in_5(my_int_type* array_input, mai_type length)
{
	// quick_sort_in_5_helper(array_input, 0, length - 1);
	selection_sort_2(array_input, length);
}

void quick_sort_ex_5_helper(my_int_type* a, mai_type left, mai_type right)
{
}