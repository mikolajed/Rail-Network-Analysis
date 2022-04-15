#include <iostream> 
#include <string> 
#include <fstream> 
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

int main () {
	ifstream i("data/stations.json");

	json j;
	i >> j;

	cout<<j;
	return 0;
}
