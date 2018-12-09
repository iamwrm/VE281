#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#define MAX_INT std::numeric_limits<int>::max()

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Node {
	bool is_visited = false;

       public:
};

class Graph {
	vector<vector<int> > arr;
	vector<Node> node_list;
	int num;
	bool cyc_chk(int v, std::set<int> s)
	{
		if (s.find(v) != s.end()) {
			return true;
		}
		s.insert(v);
		for (int i = 0; i < num; i++) {
			if (arr[v][i] < MAX_INT) {
				if (cyc_chk(i, s)) {
					return true;
				} else {
					s.erase(i);
				}
			}
		}
		return false;
	}

       public:
	Graph(int size)
	    : num(size),
	      arr(size, vector<int>(size, MAX_INT)),
	      node_list(size, Node())
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
	bool is_dag()
	{
		for (int i = 0; i < num; i++) {
			if (cyc_chk(i, std::set<int>())) {
				return false;
			}
		}
		return true;
	}
};

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int size;
	cin >> size;
	Graph g1(size);

	string line;
	while (getline(cin, line)) {
		if (line.empty()) continue;
		std::stringstream ss1(line);
		int from, to, weight;
		ss1 >> from >> to >> weight;
		g1.add_edge(from, to, weight);
	}
	// g1.print_graph();

	if (g1.is_dag()) {
		cout << "The graph is a DAG\n";
	} else {
		cout << "The graph is not a DAG\n";
	}

	return 0;
}