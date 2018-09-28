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

       public:
	void debug_print()
	{
		cout << "Head p addr:\t" << Head_p << endl;
		cout << "length_limit:\t" << length_limit << endl;
		cout << "curr_length:\t" << curr_length << endl;
		print_all();
	}
	My_Array()
	{
		length_limit = 8;
		Head_p = new T[length_limit];
		curr_length = 0;
	}
	T At(my_array_index_type index)
	{
		if (index < curr_length) {
			return *(Head_p + index);
		} else {
			throw 1;
		}
	}
	void extend_array()
	{
		T* new_array = new T[length_limit * 2];
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
			extend_array();
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

	~My_Array()
	{
		delete[] Head_p;
	}
};

int main()
{
	string temp_line;

	My_Array<my_int_type> my_array_1;

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

	int num = 10;
	for (int i = 0; i < num; i++) {
		my_array_1.push_back(i);
		my_array_1.debug_print();
	}

	my_array_1.debug_print();

	return 0;
}