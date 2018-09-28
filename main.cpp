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

int main()
{
	string temp_line;

	my_int_type array1[80];

	while (getline(cin, temp_line)) {
		if (temp_line.length() == 0) {
			break;
		}
		stringstream ss1(temp_line);
		my_int_type i1;
		ss1 >> i1;
	}
	return 0;
}