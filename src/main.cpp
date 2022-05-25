#include<iostream>
#include<fstream>
///#include<graph.hpp>
//#include <nlohmann/json.hpp>
#include <string> 
#include"declarations.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, const char * argv[]) {
	string path = "../data/";
	cout<<"Loading...\n";
	Railway r(path);
	cout<<"System loaded successfully\n";

	stringstream ss;

	cout<<"Welcome to the India's railway purchasing system.\n";
	cout<<"Possible commands are:\n";
	cout<<"-> show stations\n";
	cout<<"-> show connections\n";
	cout<<"-> show routes\n";
	cout<<"-> show route details <train no>\n";
	cout<<"-> show schedule <station name>\n";
	cout<<"-> show reservations\n";
	cout<<"-> make reservation\n";
	cout<<"-> cancel reservation <ticket id>\n";
	cout<<"-> cancel all reservations\n";
	cout<<"-> help <- gives more detailes about how to use all above command\n";
	cout<<"-> exit\n";
	cout<<"----------------------------------------------------------------------------\n";
#define testing 0
#if testing
	std::ofstream out("tests/test8.txt");

	auto *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
#endif
	string command = "";
	getline(cin, command);
	while (1) {
		ss.str("");
		if (command == "show stations") {
			if (not testing) cout<<"STATIONS\n";
			r.show_stations(ss);
			cout<<ss.str()<<"\n";
		}
		else if (command ==  "show connections") {
			if (not testing) cout<<"CONNECTIONS\n";
			r.show_connections(ss);
			cout<<ss.str()<<"\n";
		}
		else if (command == "show routes") {
			if (not testing) cout<<"ROUTES\n";
			r.show_routes(ss);
			cout<<ss.str()<<"\n";
		}
		else if (command.find("show route details") == 0) {
			string a = "show route details";
			int route = stoi(command.substr(a.size()+1, command.size()-1));
			if (not testing) cout<<"ROUTE "<<route<<" DETAILS\n";
			r.show_route_details(ss, route);
			cout<<ss.str()<<"\n";
		}
		else if (command.find("show schedule") == 0) {
			string a = "show schedule";
			string station_name = command.substr(a.size()+1, command.size()-1);
			if (not testing) cout<<"STATION "<<station_name<<" SCHEDULE\n";
			r.show_schedule(ss, station_name);
			cout<<ss.str()<<"\n";
		}
		else if (command == "show reservations") {
			if (not testing) cout<<"RESERVATIONS\n";
			r.show_reservations(ss);
			cout<<ss.str()<<"\n";
		}
		else if (command.find("make reservation") == 0) {
			string train_no, origin, dest;
			if (not testing) cout<<"Train No.: ";
			getline(cin, train_no);
			if (not testing) cout<<"From: "; 
			getline(cin, origin);
			if (not testing) cout<<"To: "; 
			getline(cin, dest);
			r.make_reservation(stoi(train_no), origin, dest);
		}
		else if (command.find("cancel reservation") == 0) {
			string a = "cancel reservation";
			int ticket_id = stoi(command.substr(a.size()+1, command.size()-1));
			r.cancel_reservation(ticket_id);
		}
		else if (command == "cancel all reservations") {
			r.cancel_all_reservations();
		}
		else if (command == "help") {
			cout<<"-> show stations\n";
			cout<<"\t<- shows a list of all stations and number of departures and arrivals\n\n";
			cout<<"-> show connections\n";
			cout<<"\t<- shows all pairs of connected stations\n\n";
			cout<<"-> show routes\n";
			cout<<"\t<- shows a list of all routes\n\n";
			cout<<"-> show route details <train no>\n";
			cout<<"\t<- shows all stations on a route with Train No. equal to <train_no>, <train_no> is an integer values that has to be provided as input it can be obtained by running command \"show routes\"\n";
			cout<<"\t<- e.g \"show route details 22439\"\n\n";
			cout<<"-> show schedule <station name>\n";
			cout<<"\t<- shows all departures from a stations with <station name> which can be obtained with command \"show stations\"\n";
			cout<<"\t<- e.g. \"show schedule RAJAPUR ROAD\"\n\n";
			cout<<"-> show reservations\n";
	 		cout<<"\t<- shows all reservations that have been made with \"make reservation\" command\n\n";
			cout<<"-> make reservation\n";
 			cout<<"\t<- stars a process of making a reservation, then asks about reservation details (train_no, origin station name and destination stations name), all of those values can be obtained by running command \"show routes\", all three values must be provided\n\n";
			cout<<"-> cancel reservation <ticket id>\n";
			cout<<"\t<- cancels a reservation with <ticket_id> which can be obtained with command \"show reservations\"\n";
			cout<<"\t<- e.g. \"cancel reservation 0\"\n\n";
			cout<<"-> cancel all reservations\n";
			cout<<"\t<- cencels all reservations\n\n";
			cout<<"-> help\n";
			cout<<"\t<- gives more detailes about how to use all above command\n\n";
			cout<<"-> exit\n";
			cout<<"\t<-exits the program, does not save any information\n";
		}
		else if (command == "exit") {
			exit(0);
		}
		else {
			cout<<"Command not recognized, please try again\n";
		}
		command = "";
		getline(cin, command);
	}

	return 0;
}
