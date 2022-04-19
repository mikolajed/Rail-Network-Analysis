#include<iostream>
#include<graph.hpp>
#include <nlohmann/json.hpp>
#include <string> 
#include <fstream> 

using namespace std;
using json = nlohmann::json;

int main () {
/*	Graph G;
	G.add_node("a", "a");
	G.add_node("b", "b");
	G.add_edge("a", "b", 1);
	G.print();
*/
	
	ifstream i("data/stations.json");

	json j;
	i >> j;

	cout<<j;
	return 0;
}
