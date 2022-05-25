#include<vector>
#include<unordered_map>
#include<string>
#include<set>

using namespace std;

class Graph {
	class Node {
		string value;
		// adjs store indexes of edges
		set<int> adj_in;
		set<int> adj_out;
		Node(string value);
	
		friend class Graph;
	};
	class Edge {
		int v_idx, u_idx; 	// source and destination node
		int d;				// distance

		// create an edges between two nodes (indexes)
		Edge(int v_idx, int u_idx, int d);
		
		friend class Graph;
	};

	unordered_map<string, int> node_IDs;
	vector<Node> V;
	vector<Edge> E;

public:
	void add_node(string key, string value);
	void add_edge(string origin_key, string dest_key, int d);
	//vector<string> shortestPath(string origin, string destination);
	void print();
};

