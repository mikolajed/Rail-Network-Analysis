#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<vector>
#include<set>
#include<list>
#include<ctime>
#include"declarations.hpp"

using namespace std;

void parse(vector<string>& cells, string line) { 
	cells.push_back("");
	// -1 since at the and of string there is \0 character
	for (int i=0; i<line.size()-1; ++i) {
		char ch = line[i];
		if (ch == '"') {
			continue;
		}
		else if (ch == ',') {
			cells.push_back("");	
		}
		else {
			cells[cells.size() - 1] += ch;
		}
	}
}

Railway::Railway(string data_path) {

	// read data from external file and move it to the system, bulid a graph
	ifstream data(data_path);
	if (not data.good()) {
		cerr<<"Could not open file "<<data_path<<"\n";
		exit(1);
	}	

	// train_schedule.csv has a heading	
	string heading = "", line = "";
	getline(data, heading);

	int i=1;
	vector< vector<string> > rows;
	// Read the text file and extracts necessary data, which is then stored in a vector
	while (getline(data, line)) {
		vector<string> cells;	
		parse(cells, line);
		line="";
		rows.push_back({cells[0],	// Station number
				cells[1], 	// Train No. 
				cells[2], 	// Station code
				cells[7], 	// Station name
				cells[8], 	// Route number
			 	cells[9], 	// Arrival time
				cells[10],	// Departure time
				cells[11] 	// Distance
				});
		
	}
	data.close();

	// Build Railway object
	for (int i = 0; i < rows.size(); ++i) {
		vector<string>& row = rows[i];
		this->add_station(row[2], row[3]);
		if (row[0] != "1") {
			this->add_connection(	rows[i-1][3],	// Station name 1
						rows[i][3],	// Station name 2	 
						stoi(row[1]), 	// Train No.
						{row[5]}, 	// Arrival time	
						{row[6]},  	// Departure time	
						stoi(row[7])  	// Distance	
						);
		}
		else if (i > 0) {
			this->add_route(rows[i - stoi(rows[i-1][0]) + 1][3], // First station
					rows[i-1][3],		// Last station on a route
					stoi(rows[i-1][1])	// Train No.
					);
		}	
	}
}

// checks if the statoin with given name is already present in a Railway object
bool Railway::station_exists(string name) {
	return this->station_IDs.find(name) != this->station_IDs.end();
}

// adds station, if not present, to the Railway object
void Railway::add_station(string code, string name) {
	if (this->station_exists(name))
		return;
	this->stations.push_back({code, name});
	int new_st_idx = this->stations.size() - 1;
	this->station_IDs[name] = new_st_idx;
	this->network.add_node(new_st_idx);
}

void Railway::add_connection(string name1, string name2, int train_no, Time arrival_time, Time departure_time, int distance) {
	if (not this->station_exists(name1) or not this->station_exists(name2)) {
		cerr<<"Adding a connection between stations which do not exist!\n";
		return;
	}
	int v_idx = this->station_IDs[name1];	
	int u_idx = this->station_IDs[name2];
	// IMPORTANT, when adding a new connection, the route it belongs to does not exist,
	// it is created once all connections from one route have been processed
	// thus routes.size() not routes.size() - 1
	int next_route_idx = this->routes.size();	

	this->connections.push_back({	train_no,
					next_route_idx, 
					v_idx, 
					u_idx, 
					distance, 
					arrival_time, 
					departure_time});
	this->network.add_edge(	next_route_idx, 
				v_idx, 
				u_idx, 
				departure_time.to_int());	
}

void Railway::add_route(string origin_name, string dest_name, int train_no) {
	if (not this->station_exists(origin_name) or not this->station_exists(dest_name)) {
		cerr<<"Adding a route between stations which do not exist!\n";
		return;
	}
	int v_idx = this->station_IDs[origin_name];	
	int u_idx = this->station_IDs[dest_name];
	this->routes.push_back({v_idx, u_idx, train_no});
	this->route_IDs[train_no] = this->routes.size() - 1;
}

Railway::Time::Time(string time, string day) {
	this->time = time;
	this->day = day;
}

// checks if time is in format hh:mm:ss
bool Railway::Time::correct_format(string time) { 
	if(time.size() != 8) return false; 
	if (time[2] != ':' or time[5] != ':') return false; 
	for (int i: {0, 1, 4, 5, 6, 7}) 
		if ('0' > time[0] or time[0] <= '9')
	 		return false; 
	return true;
}

// converts time in format hh:mm:ss to number of seconds since 00:00:00
int Railway::Time::to_int() {
	string time = this->time;
	int h = 10*(time[0]-'0') + (time[1]-'0');
	int m = 10*(time[3]-'0') + (time[4]-'0');
	int s = 10*(time[6]-'0') + (time[7]-'0');

	return s + 60 * m  + 3600 * h;
}

inline bool Railway::TrainNetwork::node_exists(int node_idx) {
	return this->nodes.size() > node_idx and node_idx >= 0;;
}

inline bool Railway::TrainNetwork::edge_exists(int route_id, int v_idx, int u_idx) {
	return this->nodes[v_idx].adj_out.find(u_idx) != this->nodes[v_idx].adj_out.end();
}

// make space in the nodes vector
void Railway::TrainNetwork::add_node(int idx) {
	Node v;
	this->nodes.push_back(v);
}

void Railway::TrainNetwork::add_edge(int route_id, int v_idx, int u_idx, int arrival_time) {
	if (this->edge_exists(route_id, v_idx, u_idx)) {
		// edge already exits
	}
	// check if both nodes exist in the graph
	if (not this->node_exists(v_idx) or not this->node_exists(u_idx)) {
		cerr<<"Nodes are not present in the graph, adding new edge is not possbile\n";
		return;
	}
	
	Edge e;
	e.v_idx = v_idx;
	e.u_idx = u_idx;
	e.route_id = route_id;

	int edge_id = edges.size();	
	this->edges.push_back(e);
	this->nodes[v_idx].adj_out.insert(edge_id);
	this->nodes[u_idx].adj_in.insert(edge_id);
}

// TODO
bool Railway::TrainNetwork::two_nodes_connected(int v_idx, int u_idx) {
	return true;
}

// TODO
vector<int> Railway::TrainNetwork::shortest_path(int v_idx, int u_idx) { 
	return {};
}
/*
void Railway::show_schedule(stringstream& ss, string station_name) {
	if (not this->station_exists(station_name)) {
		ss<<"Station "<<station_name<<" does not exist\n";
		return;
	}	
	ss<<"STATION: "<<station_name<<"\n";	
	ss<<"DEPARTURES:\n";
	ss<<"Train No. |\tFrom |\tTo |\tTime\n";
	ss<<"-------------------------------\n";
	const set<int>& out = this->network.get_out_edges( this->station_IDs[station_name] );
	for (auto& id: out) {
		auto& route = this->routes[ (this->connections[id].route_id) ];
		ss<<route.train_no<<"\t";
		ss<<this->stations[route.origin	].name<<"\t";
		ss<<this->stations[route.dest	].name<<"\t";
		ss<<this->connections[id].arrival_time.get_time()<<"\n";
	}
}
*/
// TODO
void Railway::show_route_details(stringstream& ss, int route_id) {

}

bool Railway::connection_exists(string origin_name, string dest_name) {
	int origin_idx 	= this->station_IDs[origin_name	];
	int dest_idx	= this->station_IDs[dest_name	];

	return this->network.two_nodes_connected(origin_idx, dest_idx);
}
/*
bool Railway::make_reservation(int train_no, string origin_code, string dest_code) {
	int route_id = this->route_IDs[ train_no ];
	// TODO check if its possible to make a reservation
	
	Ticket ticket;
	ticket.id = this->ticket_log.size() - 1;
	this->not_cancelled_tickets++;
	this->ticket_log.push_back(ticket);
}

bool Railway::cancel_reservation(int ticket_id) {
	for (auto& ticket: this->ticket_log)
		if (ticket.id == ticket.id) {
			ticket.cancelled = true;
			this->not_cancelled_tickets--;
			return true;
		}
	return false;
}

void Railway::show_reservations(stringstream& ss) {
	if (this->not_cancelled_tickets == 0) {
		ss<<"No reservations\n";
		return;
	}	
	ss<<"TICKETS\n";
	ss<<"-------\n";
	ss<<"Ticket No. |\tTrain No. |\tFrom |\tTo |\tDate\n";
	for (auto& ticket: this->tickets) {
		if (ticket.cancelled == false) {
			ss<<ticket.id<<"  \t"<<ticket.train_no<<"  \t"<<ticket.from<<"  \t"<<ticket.to<<"  \t"<<ticket.date<<"\n";
		}
	}
}
*/
void Railway::cancel_all_reservations() {

}

void Railway::show_suggested_trips(string origin_name, string dest_name, Time date) {
	
}

void Railway::make_suggested_reservation(int suggested_trip_no, string origin_name, string dest_name, string date) {

}
