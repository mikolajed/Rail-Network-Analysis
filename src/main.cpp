#include<iostream>
///#include<graph.hpp>
//#include <nlohmann/json.hpp>
#include <string> 
#include"declarations.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, const char * argv[]) {
	string path = "../data/train_schedule.csv";

	cout<<"Loading...\n";
	Railway r(path);
	cout<<"System loaded successfully\n";

	stringstream ss;
#if 0	
	r.show_diagnostics(ss);
	cout<<"DIAGNOSTICS\n"<<ss.str();
	ss.str("");
#endif
#if 0	
	r.show_stations(ss);
	cout<<"LIST OF STATIONS\n"<<ss.str();
	ss.str("");	
#endif
#if 0
	r.show_routes(ss);
	cout<<"ROUTES\n"<<ss.str();
	ss.str("");	
#endif
#if 1
	r.show_route_details(ss, 107);
	cout<<"ROUTE "<<107<<" DETAILS\n"<<ss.str();
	ss.str("");
#endif	
	return 0;
}
