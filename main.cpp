#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
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
       public:
	int from;
	Node(int f) : from(f)
	{
	}
};
class Edge {
       public:
	int from;
	int to;
	Edge(int f, int t) : from(f), to(t)
	{
	}
};

class Graph {
	vector<vector<int> > arr;
	std::shared_ptr<vector<Edge> > edge_list_ptr;
	std::shared_ptr<vector<Node> > node_list_ptr;
	int num;
	bool _has_mst = false;

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
	bool has_mst()
	{
		return _has_mst;
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
	void add_edge(int from, int to, int weight)
	{
		arr[from][to] = weight;
		edge_list_ptr->push_back((Edge(from, to)));
	}
	void convert_to_undirected()
	{
		for (auto e : *edge_list_ptr) {
			int f = e.from;
			int t = e.to;
			arr[t][f] = arr[f][t];
		}
	}
	void add_node(int from)
	{
		node_list_ptr->push_back(Node(from));
	}
	void prim()
	{
		_has_mst = false;
	}

       public:
	Graph(int size) : num(size), arr(size, vector<int>(size, MAX_INT))
	//,node_list(size, Node())
	{
		edge_list_ptr = std::make_shared<vector<Edge> >();
		node_list_ptr = std::make_shared<vector<Node> >();
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
	void read_from_cin()
	{
		string line;
		while (getline(cin, line)) {
			if (line.empty()) continue;
			std::stringstream ss1(line);
			int from, to, weight;
			ss1 >> from >> to >> weight;
			add_edge(from, to, weight);
			add_node(from);
		}
	}
	void print_is_dag()
	{
		if (is_dag()) {
			cout << "The graph is a DAG\n";
		} else {
			cout << "The graph is not a DAG\n";
		}
	}
	void print_mst()
	{
		convert_to_undirected();
		prim();
		if (has_mst()) {
			cout << "The total weight of MST is " << 12 << "\n";
		} else {
			cout << "No MST exists!\n";
		}
	}
};

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int size;
	cin >> size;
	Graph g1(size);

	g1.read_from_cin();
	g1.print_is_dag();

	g1.print_mst();

	return 0;
}