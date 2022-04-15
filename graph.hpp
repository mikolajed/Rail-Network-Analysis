#include<vector>
#include<unordered_map>
#include<string>

class Graph {
	class Node {
		int id;
		string key;
		string value;
		// adj store indexes of edges
		vector<int> adj_in;
		vector<int> adj_out;
		Node(string key, string, value);
	};
	class Edge {
		int v_idx, u_idx; 	// source and destination node
		int d;				// distance

		Edge(int v_idx, int u_idx);
	};

	int n;	// number of ndoes
	int m;	// number of edges
	unordered_map<string, int> node_IDs;
	unordered_map< pair<string, string>, bool > pushed_edges;
	vector<Node> V;
	vector<Edge> E;

	void add_node(string key, string value);
	void add_edge(string origin_key, string dest_key, int d);
	//vector<string> shortestPath(string origin, string destination);
};
