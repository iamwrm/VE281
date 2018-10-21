#include <cassert>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int my_partition1(int* a, int n, int chosen_one_pos)
{
	int left = 0;
	int right = n - 1;

	int chosen_one_value = a[chosen_one_pos];
	swap(a[left], a[chosen_one_pos]);

	auto i = left;
	auto j = left + 1;

	while (j < right + 1) {
		if (a[j] >= chosen_one_value) {
		} else {
			i++;
			swap(a[i], a[j]);
		}
		j++;
	}

	swap(a[left], a[i]);
	return i;
}

int rand_linear_sort(int* A, int n, int i)
{
	if (n == 1) return A[0];
	int p = rand() % n;
	int j = my_partition1(A, n, p);
	if (j == i) return A[j];
	if (j > i)
		return rand_linear_sort(A, j, i);
	else
		return rand_linear_sort(A + j + 1, n - j - 1, i - j - 1);
}

int deter_linear_sort(int* A, int n, int i)
{
	if (n == 1) return A[0];
	int j = rand() % (n);
	if (j == i) return A[j];
	if (j > i)
		return rand_linear_sort(A, j - 1, i);
	else
		return rand_linear_sort(A + j + 1, n - j, i - j);

	return 0;
}

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
	int* my_array = new int[line_num];
	int temp_int;
	while (cin >> temp_int) {
		my_array[i] = temp_int;
		i++;
		if (i >= line_num) {
			break;
		}
	}

	// sorting begins

	int index_find = 1;
	if (algo_code == 0) {
		index_find = rand_linear_sort(my_array, line_num, n_th);
	} else {
		index_find = deter_linear_sort(my_array, line_num, n_th);
	}

	cout << "The order-" << n_th << " item is " << index_find << endl;

	// NOTE: deleted 1
	delete[] my_array;
	// deleted 1

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	// std::cout << "printf: " << duration << '\n';

	return 0;
}
