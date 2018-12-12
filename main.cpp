#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
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
	int weight;
	typedef std::shared_ptr<Edge> Ptr;
	Edge(int f, int t, int w) : from(f), to(t), weight(w)
	{
	}
};

class Graph {
	vector<vector<int> > arr;
	std::shared_ptr<vector<Edge> > edge_list_ptr;
	vector<std::shared_ptr<Edge> > edge_ptr_pool;
	vector<bool> if_spanned;
	int num;
	bool _has_mst = false;
	int mst_val = 0;
	struct greater_t {
		bool operator()(const Edge::Ptr x, const Edge::Ptr y) const
		{
			return x->weight > y->weight;
		}
	};
	std::priority_queue<Edge::Ptr, vector<Edge::Ptr>, greater_t> pq1;

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
		edge_list_ptr->push_back(Edge(from, to, weight));

		if (!edge_ptr_pool[from * num + to]) {
			edge_ptr_pool[from * num + to] =
			    std::make_shared<Edge>(from, to, weight);
		}
	}
	// DONE:
	void convert_to_undirected()
	{
		for (auto e : *edge_list_ptr) {
			int f = e.from;
			int t = e.to;
			if (arr[f][t] < MAX_INT) {
				if (arr[t][f] < arr[f][t]) {
					arr[f][t] = arr[t][f];
					edge_ptr_pool[f * num + t] =
					    std::make_shared<Edge>(f, t,
								   arr[t][f]);

				} else {
					arr[t][f] = arr[f][t];
					edge_ptr_pool[t * num + f] =
					    std::make_shared<Edge>(t, f,
								   arr[f][t]);
				}
			}
		}
	}

	void add_all_edge_tobe(int i)
	{
		if (if_spanned[i] == true) {
			for (int j = 0; j < num; j++) {
				if (arr[i][j] < MAX_INT) {
					if (if_spanned[j] == false) {
						if (edge_ptr_pool[i * num +
								  j]) {
							pq1.emplace(
							    edge_ptr_pool
								[i * num + j]);
						}
					}
				}
			}
		}
	}
	void prim()
	{
		_has_mst = false;

		if_spanned[0] = 1;

		int spanned_num = 1;

		for (int k = 0; k < num - 1; k++) {
			for (int i = 0; i < num; i++) {
				add_all_edge_tobe(i);
			}
			if (pq1.empty()) {
				continue;
			}
			Edge::Ptr shortest_edge = pq1.top();
			if (shortest_edge) {
				if_spanned[shortest_edge->to] = true;
				mst_val += shortest_edge->weight;
				spanned_num++;
			}
			while (!pq1.empty()) {
				pq1.pop();
			}
		}

		if (spanned_num != num) {
			_has_mst = false;
		} else {
			_has_mst = true;
		}
	}

       public:
	Graph(int size)
	{
		num = size;
		arr = vector<vector<int> >(size, vector<int>(size, MAX_INT));
		edge_list_ptr = std::make_shared<vector<Edge> >();
		edge_ptr_pool =
		    vector<std::shared_ptr<Edge> >(num * num, nullptr);
		if_spanned = vector<bool>(num, false);
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
			cout << "The total weight of MST is " << mst_val
			     << "\n";
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