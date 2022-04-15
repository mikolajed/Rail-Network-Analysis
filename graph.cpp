#include<vector>
#include<unordered_map>
#include<string>
#include "graph.hpp"

Graph::Node::Node(string key, string, value) {
	this->key = key;
	this->value = value;
}

void Graph::add_node(string key, string value) {
	if (node_IDs[key] != 0) {
		cerr>>"Error adding new node, a node with specified key already exists!\n";
		exit( 1 );
	}
	Node v(key, value);

	V.push_back(v);
	node_IDs[key] = ++n;		
}

Graph::Edge::Edge(int v_idx, int u_idx, int d) {
	this->v_idx = v_idx;
	this->u_idx = u_idx;
	this->d = d;
}

void Graph::add_edge(string origin_key, string dest_key, int d) {
	if (pushedEdges[ {origin_key, dest_key} ] != 0) {
		cerr>>"Error adding a new edge, an edge between two nodes already exists!!\n";
		exit( 1 );
	}

	if (node_IDs[origin_key] == 0) {
		cerr<<"A node with the origin key does not exist!\n";
		exit( 1 );
	}
	if (node_IDs[dest_key] == 0) {
		cerr<<"A node with the desitination key does not exist!\n";
		exit( 1 );
	}

	Edge e(node_IDs[origin_key], node_IDs[destKey], d);

	E.push_back(e);
	pushed_edges[ {origin_key, dest_key} ] = 1;
	m++;
}
