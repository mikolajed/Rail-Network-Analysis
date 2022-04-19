#include<iostream>
#include<graph.hpp>

using namespace std;

int main () {
	Graph G;
	G.add_node("a", "a");
	G.add_node("b", "b");
	G.add_edge("a", "b", 1);
	G.print();
	return 0;
}
