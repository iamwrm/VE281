#include <iostream>
#include <limits>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Graph {
	vector<vector<int> > arr;
	int num;

       public:
	Graph(int size)
	    : num(size),
	      arr(size, vector<int>(size, std::numeric_limits<int>::max()))
	{
	}
	void print_graph()
	{
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				cout << arr[i][j] << " ";
			}
			cout << "\n";
		}
	}
};

int main()
{
	int size;
	cin >> size;
	// cout << size;
	Graph g1(size);
	g1.print_graph();

	string line;
	while (getline(cin, line)) {
		// cout << line << endl;
	}

	return 0;
}