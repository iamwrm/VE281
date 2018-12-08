#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
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
				cout << std::setw(10) << arr[i][j] << " ";
			}
			cout << "\n";
		}
	}
	void add_edge(int from, int to, int weight)
	{
		arr[from][to] = weight;
	}
};

int main()
{
	int size;
	cin >> size;
	// cout << size;
	Graph g1(size);

	string line;
	while (getline(cin, line)) {
		// cout << line << endl;
		std::stringstream ss1(line);
		int from, to, weight;
		ss1 >> from >> to >> weight;
		g1.add_edge(from, to, weight);
	}
	g1.print_graph();

	return 0;
}