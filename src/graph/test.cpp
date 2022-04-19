#include<iostream> 
#include<unordered_map> 
#include "graph.hpp"

using namespace std;

int main() {
	Graph G;

	for (int i=0; i<6; ++i) { 
		string key;
		cin>>key;
		G.add_node(key, key);
	}
	for (int i=0; i<6; ++i) {
		G.print();
		string a, b;
		cin>>a>>b;
		G.add_edge(a, b, i);
	}
	G.print();

	return 0;
}

