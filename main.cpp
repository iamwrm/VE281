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
#define my_array_index_type long long int

template <class T>
class My_Array {
	T* Head_p;
	long long int length_limit;
	long long int curr_length;
	int extend_multiple;

       public:
	void debug_print()
	{
		cout << "\n\n---------------------\n";
		cout << "Head p addr:\t" << Head_p << endl;
		cout << "length_limit:\t" << length_limit << endl;
		cout << "curr_length:\t" << curr_length << endl;
		print_all();
	}
	My_Array(my_array_index_type input_length_limit = 1024)
	{
		length_limit = input_length_limit;
		extend_multiple = 2;
		Head_p = new T[length_limit];
		curr_length = 0;
	}
	T at(my_array_index_type index)
	{
		if (index < curr_length) {
			return *(Head_p + index);
		} else {
			throw 1;
		}
	}
	void change_at(my_array_index_type index, T input_elt)
	{
		if (index < curr_length) {
			*(Head_p + index) = input_elt;
		} else {
			throw 1;
		}
	}
	void _extend_array()
	{
		int length_multiple = 2;
		T* new_array = new T[length_limit * length_multiple];
		for (my_array_index_type i = 0; i < curr_length; i++) {
			*(new_array + i) = *(Head_p + i);
		}
		delete[] Head_p;
		Head_p = new_array;
		length_limit *= 2;
	}
	void push_back(T elt)
	{
		if (curr_length < length_limit) {
			*(Head_p + curr_length++) = elt;
		} else {
			// TODO: enlarge
			_extend_array();
			push_back(elt);
		}
	}
	void print_all()
	{
		for (my_array_index_type i = 0; i < curr_length; i++) {
			cout << *(Head_p + i) << " ";
		}
		cout << endl;
	}
	void set_extend_multiple(int input)
	{
		if (input > 1) {
			extend_multiple = input;
		}
	}

	~My_Array()
	{
		delete[] Head_p;
	}
};

int main()
{
	std::clock_t start;
	double duration;

	start = std::clock();

	/* Your algorithm here */

	string temp_line;

	My_Array<my_int_type> my_array_1(8);
	my_array_1.set_extend_multiple(2);

	while (getline(cin, temp_line)) {
		if (temp_line.length() == 0) {
			break;
		}
		stringstream ss1(temp_line);
		my_int_type i1;
		ss1 >> i1;
		my_array_1.push_back(i1);
	}

	my_array_1.debug_print();

	long long num = 1000000;
	for (long long i = 0; i < num; i++) {
		my_array_1.push_back(i);
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "printf: " << duration << '\n';
	return 0;
}