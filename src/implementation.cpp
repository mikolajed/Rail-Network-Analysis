#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<vector>
#include<set>
#include<list>
#include<ctime>
#include <iomanip>
#include"declarations.hpp"

// checking memory used by a process is system dependent
#if __APPLE__
	#include<mach/mach.h>

	#include <mach/vm_statistics.h>
	#include <mach/mach_types.h>
	#include <mach/mach_init.h>
	#include <mach/mach_host.h>
#elif __linux__
	#include "sys/types.h"
	#include "sys/sysinfo.h"
#elif _WIN64 || _WIN32
	#include "windows.h"
#endif

using namespace std;

/*
	IMPLEMENTATION OF RAILWAY CLASS
*/
Railway::Railway(string data_path) {
	// read data from external file and move it to the system, bulid a graph
	ifstream data(data_path);
	if (not data.good()) {
		cerr<<"Could not open file "<<data_path<<"\n";
		exit(1);
	}	
	
	// define parser for a line of txt file
	auto parse = [](vector<string>& cells, string line) { 
		bool opening = false;
		cells.push_back("");
		// -1 since at the and of string there is \0 character
		for (int i=0; i<line.size()-1; ++i) {
			char ch = line[i];
			if (ch == '"') {
				opening ^= 1;
				continue;
			}
			if (ch == ',' and opening == true)
				continue;
			else if (ch == ',') {
				cells.push_back("");	
			}
			else {
				cells[cells.size() - 1] += ch;
			}
		}
	};

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
	this->add_station(rows[0][2], rows[0][3]);
	for (int i = 1; i < rows.size(); ++i) {
		vector<string>& row = rows[i];
		this->add_station(row[2], row[3]);
		if (row[0] != "1") {
			this->add_connection(	rows[i-1][3],		// Station name 1
						rows[i][3],		// Station name 2	 
						stoi(row[1]), 		// Train No.
						Time (rows[i-1][6], ""),// Departure time from 1	
						Time (rows[i][5], ""),  // Arrival time at 2
						stoi(row[7])  		// Distance	
						);
		}
		else if (row[0] == "1") {
			this->add_route(rows[i - stoi(rows[i-1][0])][3], // First station
					rows[i-1][3],		// Last station on a route
					stoi(rows[i-1][1])	// Train No.
					);
		}
	}
	int last = rows.size() - 1;	
	this->add_route(rows[last - stoi(rows[last][0]) + 1][3], // First station
			rows[last][3],		// Last station on a route
			stoi(rows[last][1])	// Train No.
			);
}

// checks if the statoin with given name is already present in a Railway object
bool Railway::station_exists(string name) {
	return this->station_IDs.find(name) != this->station_IDs.end();
}

bool Railway::route_exists(int train_no) {
	return this->route_IDs.find(train_no) != this->route_IDs.end()
		and this->route_IDs[train_no] < this->routes.size(); 
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
	this->network.add_edge(	train_no, 
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

// pushes list of stations in tabular format, columns returned contain
// name of a station, its code, number of departures, number of arrivals
void Railway::show_stations(stringstream& ss) {
	using std::right;
	using std::left;
	if (this->stations.size() == 0) {
		ss<<"No stations found.\n";
		return;
	}
	ss<<setw(16)<<left<<"Name";
	ss<<setw(16)<<left<<"Code";
	ss<<setw(16)<<left<<"#Departures";
	ss<<setw(16)<<left<<"#Arrivals";	
	ss<<"\n";
	for (int i=0; i<this->stations.size(); ++i) {
		Station& curr = this->stations[i];
		ss<<setw(16)<<left<<curr.name;
		ss<<setw(16)<<left<<curr.code;
		ss<<setw(16)<<left<<this->network.get_in_edges(i).size();
		ss<<setw(16)<<left<<this->network.get_out_edges(i).size()<<"\n";	
	}
}

void Railway::show_connections(stringstream& ss) {
	using std::left;
	using std::right;
	ss<<setw(16)<<left<<"From"<<setw(16)<<left<<"To"<<setw(16)<<left<<"Train No."<<"\n";
	for (Connection& c: this->connections) {
		ss<<setw(16)<<left<<this->stations[c.origin].name;
		ss<<setw(16)<<left<<this->stations[c.dest].name;
		ss<<setw(16)<<left<<c.train_no<<"\n";
	}
}

void Railway::show_routes(stringstream& ss) {
	using std::left;
	using std::right;
	ss<<setw(16)<<left<<"Train No."<<setw(16)<<left<<"From"<<setw(16)<<left<<"To"<<"\n";	
	for(Route& r: this->routes) {
		ss<<setw(16)<<left<<r.train_no;
		ss<<setw(16)<<left<<this->stations[r.origin].name;
		ss<<setw(16)<<left<<this->stations[r.dest].name;
		ss<<"\n";	
	}		
}


void Railway::show_schedule(stringstream& ss, string station_name) {
	using std::left;
	using std::right;
	if (not this->station_exists(station_name)) {
		ss<<"Station "<<station_name<<" does not exist\n";
		return;
	}	
	ss<<setw(16)<<left<<"Train No."<<setw(16)<<left<<"From"<<setw(16)<<left<<"To"<<setw(16)<<left<<"Time"<<"\n";
	const set<int>& out = this->network.get_out_edges( this->station_IDs[station_name] );
	for (auto& id: out) {
		auto& route = this->routes[ this->route_IDs[(this->connections[id].train_no)] ];
		ss<<setw(16)<<left<<route.train_no;
		ss<<setw(16)<<left<<this->stations[route.origin	].name;
		ss<<setw(16)<<left<<this->stations[route.dest	].name;
		ss<<setw(16)<<left<<this->connections[id].departure_time.get_time()<<"\n";
	}
}

void Railway::show_route_details(stringstream& ss, int train_no) {
	using std::left;
	using std::right;

	if (not this->route_exists(train_no)) {
		cerr<<"Route with Train No. "<<train_no<<" does not exits.\n";
		exit(1);
	}
	int route_id = this->route_IDs[train_no];
	vector<int> edges;
	this->network.walk_route_from(this->routes[route_id].origin, train_no, edges);	
	if (edges.size() == 0) {
		ss<<"No route details, it has no stations.\n";
		return;
	}	
	ss<<setw(16)<<left<<"Time"<<setw(16)<<"Station name"<<"\n";
	for (int i=0; i<edges.size(); ++i) {
		Connection& curr = this->connections[edges[i]];
		ss<<setw(16)<<curr.departure_time.get_time();
		ss<<setw(16)<<this->stations[curr.origin].name<<"\n";
	}
	Connection& last = this->connections[edges[edges.size() - 1]];
	ss<<setw(16)<<last.arrival_time.get_time();
	ss<<setw(16)<<this->stations[last.dest].name<<"\n";
}

bool Railway::connection_exists(string origin_name, string dest_name) {
	int origin_idx 	= this->station_IDs[origin_name	];
	int dest_idx	= this->station_IDs[dest_name	];

	return this->network.two_nodes_connected(origin_idx, dest_idx);
}

bool Railway::make_reservation(int train_no, string origin, string dest) {
	int route_id = this->route_IDs[ train_no ];
	
	Route& route = this->routes[route_id];
	if (this->stations[route.origin].name != origin or this->stations[route.dest].name != dest) {
		cerr<<origin<<" -> "<<dest<<"route is not present in the system.\n";
		return false;
	}	

	Ticket ticket;
	ticket.from = origin;
	ticket.to = dest;
	ticket.train_no = train_no;	
	ticket.id = this->ticket_log.size() - 1;
	this->not_cancelled_tickets++;
	this->ticket_log.push_back(ticket);
	return true;
}

bool Railway::cancel_reservation(int ticket_id) {
	for (auto& ticket: this->ticket_log)
		if (ticket_id == ticket.id) {
			ticket.cancelled = true;
			this->not_cancelled_tickets--;
			return true;
		}
	return false;
}

void Railway::show_reservations(stringstream& ss) {
	using std::left;
	using std::right;	

	if (this->not_cancelled_tickets == 0) {
		ss<<"No reservations\n";
		return;
	}	
	ss<<setw(16)<<left<<"Ticket No."<<setw(16)<<left<<"Train No."<<setw(16)<<left<<"From"<<setw(16)<<left<<"To"<<setw(16)<<left<<"Date"<<"\n";
	for (auto& ticket: this->ticket_log) {
		if (ticket.cancelled == false) {
			ss<<setw(16)<<left<<ticket.id;
			ss<<setw(16)<<left<<ticket.train_no;
			ss<<setw(16)<<left<<ticket.from;
			ss<<setw(16)<<left<<ticket.to;
			ss<<setw(16)<<left<<ticket.date.get_day()<<"\n";
		}
	}
}

void Railway::cancel_all_reservations() {
	for (auto& ticket: this->ticket_log)
		ticket.cancelled = true;
		this->not_cancelled_tickets--;
}

void Railway::show_suggested_trips(string origin_name, string dest_name, Time date) {
	
}

void Railway::make_suggested_reservation(int suggested_trip_no, string origin_name, string dest_name, string date) {

}

void Railway::show_diagnostics(stringstream& ss) {
	ss<<"Memory usage\n";
	long long totalVirtualMem = 0;
	long long totalResidentMem = 0;
#if __APPLE__	
	/*vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics64_data_t vm_stats;

	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
	    KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
					    (host_info64_t)&vm_stats, &count))
	{
	    long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

	    long long used_memory = ((int64_t)vm_stats.active_count +
				     (int64_t)vm_stats.inactive_count +
				     (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
	    ss<<"free memory: "<<free_memory/8e6<<"MB\n"<<"used memory: "<<used_memory/8e6<<"MB\n";
	}*/
	struct task_basic_info t_info;
	mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

	if (KERN_SUCCESS != task_info(mach_task_self(),
				      TASK_BASIC_INFO, (task_info_t)&t_info,
				      &t_info_count))
	{
	    exit(-1);
	}
	
	ss<<"Resident size: "<<t_info.resident_size/8e6<<"MB\n";
	ss<<"Virtual size: "<<t_info.virtual_size/8e6<<"MB\n";
#elif __linux__
	struct sysinfo memInfo;

	sysinfo (&memInfo);
	long long totalVirtualMem = memInfo.totalram;
	//Add other values in next statement to avoid int overflow on right hand side...
	totalVirtualMem += memInfo.totalswap;
	totalVirtualMem *= memInfo.mem_unit;

	long long totalPhysMem = memInfo.totalram;
	//Multiply in next statement to avoid int overflow on right hand side...
	totalPhysMem *= memInfo.mem_unit;

	long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
	//Add other values in next statement to avoid int overflow on right hand side...
	virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
	virtualMemUsed *= memInfo.mem_unit;	
	// TODO: Check if the unit are right, supposedly it is in KB
	virtualMemUsed /= 1024;
	totalVirtualMem /= 1024;
	
	ss<<"Virtual memory used:  "<<virtualMemUsed<<"MB\n";
	ss<<"Total virtual memory: "<<totalVirtualMem<<"MB\n";
#elif _WIN64 || _WIN32
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	
	ss<<"Total virtual memory: "<<totalVirtualMem<<"MB\n";
	ss<<"Virtual memory used: "<<virtualMemUsed<<"MB\n";
#endif 
// resident size is in t_info.resident_size;
// virtual size is in t_info.virtual_size;
	ss<<"-----------------------\n";
	ss<<"Graph statistics\n";
	this->network.show_statistics(ss);
}

/*
	IMPLEMENTATION OF TIME CLASS
*/
Railway::Time::Time(string time, string day) {
	this->time = time;
	this->day = day;
	if (not this->correct_format()) {
		cerr<<"Incorrect time format ("<<this->time<<").\n";
		exit(1);
	}
}
// checks if time is in format hh:mm:ss
bool Railway::Time::correct_format() {
	string time = this->time; 
	if(time.size() != 8) return false; 
	if (time[2] != ':' or time[5] != ':') return false; 
	for (int i: {0, 1, 3, 4, 6, 7}) { 
		if (time[i] < '0' or time[i] > '9')
	 		return false; 
	}
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

const string& Railway::Time::get_time() const { 
	return this->time;
}
const string& Railway::Time::get_day() const { 
	return this->day;
}

/*
	IMPLEMENTATION OF GRAPH CLASS	
*/
inline bool Railway::TrainNetwork::node_exists(int node_idx) {
	return this->nodes.size() > node_idx and node_idx >= 0;;
}

inline bool Railway::TrainNetwork::edge_exists(int train_no, int v_idx, int u_idx) {
	return this->nodes[v_idx].adj_out.find(u_idx) != this->nodes[v_idx].adj_out.end();
}

// make space in the nodes vector
void Railway::TrainNetwork::add_node(int idx) {
	Node v;
	this->nodes.push_back(v);
}

void Railway::TrainNetwork::add_edge(int train_no, int v_idx, int u_idx, int arrival_time) {
	if (this->edge_exists(train_no, v_idx, u_idx)) {
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
	e.train_no = train_no;

	int edge_id = edges.size();	
	this->edges.push_back(e);
	this->nodes[v_idx].adj_out.insert(edge_id);
	this->nodes[u_idx].adj_in.insert(edge_id);
}

// TODO
bool Railway::TrainNetwork::two_nodes_connected(int v_idx, int u_idx) {
	return true;
}

void Railway::TrainNetwork::walk_route_from(int v_idx, int train_no, vector<int>& walk) {
	vector<int> v;
	if (not this->node_exists(v_idx)) {
		cerr<<"Node does not exits in the graph.\n";
		exit(1);
	}
	for (int e_idx: this->nodes[v_idx].adj_out) {
		if (e_idx >= this->edges.size()) {
			cerr<<"Accessing edge that does not exist\n";
			exit(1);
		}
		if (this->edges[e_idx].train_no == train_no) {
			v.push_back(e_idx);	
		}
	}
	if (v.size() > 1) {
		cerr<<"Same route_id going out of one node\n";
		exit(1);
	}
	if (v.size() == 1) {
		walk.push_back(v[0]);
		walk_route_from(this->edges[v[0]].u_idx, train_no, walk);
	}
}

// TODO
vector<int> Railway::TrainNetwork::shortest_path(int v_idx, int u_idx) { 
	return {};
}

const set<int>& Railway::TrainNetwork::get_in_edges(int idx) const {
	return this->nodes[idx].adj_in;
}
const set<int>& Railway::TrainNetwork::get_out_edges(int idx) const {
	return this->nodes[idx].adj_out;
}

void Railway::TrainNetwork::show_statistics(stringstream& ss) {
	ss<<"Number of nodes: "<<this->nodes.size()<<"\n";
	ss<<"Number of edges: "<<this->edges.size()<<"\n";
	ss<<"------------------------------\n";
	ss<<"Number of outgoing and incoming edges:\n";
	for (int i=0; i<this->nodes.size(); ++i) {
		ss<<i<<": (out: "<<this->nodes[i].adj_out.size()<<"), ";
		ss<<"(in: "<<this->nodes[i].adj_in.size()<<")\n";
	}
	ss<<"------------------------------\n";
	ss<<"Adjacency list (outgoing):\n";
	for (int i=0; i<this->nodes.size(); ++i) {
		ss<<i<<": [";
		for (int e_idx: this->nodes[i].adj_out) {
			ss<<this->edges[e_idx].u_idx<<"("<<this->edges[e_idx].train_no<<") ";
		}
		ss<<"]\n";
	}
	ss<<"------------------------------\n";
	ss<<"Adjacency list (incoming):\n";
	for (int i=0; i<this->nodes.size(); ++i) {
		ss<<i<<": [";
		for (int e_idx: this->nodes[i].adj_in) {
			ss<<this->edges[e_idx].v_idx<<"("<<this->edges[e_idx].train_no<<") ";
		}
		ss<<"]\n";
	}
	ss<<"------------------------------\n";
}
