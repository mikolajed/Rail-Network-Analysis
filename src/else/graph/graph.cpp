#include<vector>
#include<unordered_map>
#include<string>
#include<iostream>
#include "graph.hpp"

using namespace std;

Graph::Node::Node(string value) {
	this->value = value;
}

void Graph::add_node(string key, string value) {
	// node already exists
	if (this->node_IDs.find(key) != this->node_IDs.end()) {
		cerr<<"Error adding new node, a node with specified key already exists!\n";
		exit( 1 );
	}

	Node v(value);
	this->V.push_back(v);
	this->node_IDs[key] = V.size()-1;	
}

Graph::Edge::Edge(int v_idx, int u_idx, int d) {
	this->v_idx = v_idx;
	this->u_idx = u_idx;
	this->d = d;
}

// TODO: checking if an edge already exists, implementation should not assume user does not make a mistake but it should not make inserting linear nor memory expensive using hashtables, similar with nodes
void Graph::add_edge(string origin_key, string dest_key, int d) {
	// at lest one node does not exist
	if (this->node_IDs.find(origin_key) == this->node_IDs.end()) {
		cerr<<"Error adding new edge, origin node does not exists!\n";
		exit( 1 );
	}
	if (this->node_IDs.find(dest_key) == this->node_IDs.end()) {
		cerr<<"Error adding new edge, destination node does not exists!\n";
		exit( 1 );
	}

	auto v = this->node_IDs[origin_key];
	auto u = this->node_IDs[dest_key];

	/*
	if (this->V[v].adj_out.contains(u) or this->V[u].adj_in.contains(v)) {
		cerr<<"An edge already exists!\n";
		exit( 1 );
	}
	*/
	Edge e(v, u, d);
	this->E.push_back(e);
	this->V[v].adj_out.insert( E.size()-1 );
	this->V[u].adj_in.insert( E.size()-1 );
}

void Graph::print() {
	for (const auto & [ key, value ] : node_IDs) {
		cout<<"(key: "<<key<<", ";
		cout<<"adj_out: [";
		for (const auto & e: this->V[value].adj_out) {
			cout<<this->V[ this->E[e].u_idx ].value<<" ";
		}
		cout<<"], ";
		cout<<"adj_in: [";
		for (const auto & e: this->V[value].adj_in) {
			cout<<this->V[ this->E[e].v_idx ].value<<" ";
		}
		cout<<"])\n";
	}
}

