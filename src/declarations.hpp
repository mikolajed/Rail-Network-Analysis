#include<vector>
#include<sstream>
#include<unordered_map>
#include<string>
#include<ctime>
#include<set>

//#include<graph.hpp>

using namespace std;

class Railway {
	class Time {
		string time;	// format hh:mm:ss
		string day;
		
		bool correct_format(string time);
	public:
		Time(string time, string day = "");
		int to_int();
		//string get_time();
		//string get_day();
	};
		
	class Ticket {
		int id;
		string from, to;
		int train_no;
		double price;
		int seat_number;
		bool cancelled;
		Time date;
	};
	vector<Ticket> ticket_log;
	int not_cancelled_tickets = 0;	
	
	struct Station {
		string code;	// aberviation of station name used by railway operator
		string name;	// full name of the station
		// other data could be added
	};
	// vector of stations is not going to change once all data is loaded during the Railway object construction, index in the vector is station's unique identifier - it is used in the graph
	vector<Station> stations;
	unordered_map<string, int> station_IDs; // maps station full name to the station's index in the vector

	struct Connection {
		int train_no;	// indentifies a route
		int route_id;
		int origin;	// index of origin station in stations vector
		int dest; 	// index of destination station in stations vector	
		int distance;
		Time arrival_time;
		Time departure_time;
	};
	vector<Connection> connections;
	
	struct Route {
		int origin;	// index of origin station in stations vector
		int dest; 	// index of destination station in stations vector	
		int train_no; 	// an identifier of a route
		//string days;
	};
	vector<Route> routes;
	unordered_map<int, int> route_IDs; // maps train_no to route_id	
	// TODO: Graph implementation will be used for searching the best route between two stations
	// TrainNetwork to a large extend implements a directed graph, where 
	// nodes are represented as stations and edges as connections 
	// there is a connection between two nodes if there exist at least one route between them
	class TrainNetwork {
		struct Node {
			set<int> adj_in;	// stores list of incoming connections 
			set<int> adj_out;	// stores list of outgoing connections
		};
		struct Edge {
			int route_id;		// there might be many connections between two stations
			int v_idx, u_idx; 	// indexes of nodes in nodes vector
			int arrvial_time;
		};
		// IMPORTANT:
		// Date should only be push into vector of nedes and edges
		// By design,the data should only be pushed with add_node and add_edge methonds which are intended to be used by funciotn that reads data from external file and copies is to the RAM
		// The data in the nodes and edges SHOULD NOT be removed
		// Indexes of nodes and edges in their correponding vectors is used as their identified by graph methonds
		vector<Node> nodes;
		vector<Edge> edges;
	
	public:
		// checks if the node with specified code exists is present in the graph
		bool node_exists(int idx);
		bool edge_exists(int route_id, int v_idx, int u_idx);	
		void add_node(int idx);
		// adding an edge between nodes can be done ONLY IF both nodes exist in the graph
		void add_edge(int route_id, int v_idx, int u_idx, int arrival_time);
		bool two_nodes_connected(int v_idx, int u_idx);
		vector<int> shortest_path(int v_idx, int u_idx);
		//const set<int>& get_in_edges(int idx) const;
		//const set<int>& get_out_edges(int idx) const;
	};
	TrainNetwork network;
	
	bool station_exists(string name); 
	void add_station(string name1, string name2);
	void add_connection(string name1, string name2, int train_no, Time arrival_time, Time departure_time, int distance);
	void add_route(string, string, int);
public:
	Railway(string data_path);
	void show_routes(stringstream& ss);
	void show_stations(stringstream& ss);
	void show_schedule(stringstream& ss, string station_name);
	void show_route_details(stringstream& ss, int route_id);
	bool connection_exists(string origin_name, string dest_name);
	bool make_reservation(int train_no, string origin_code, string dest_code);
	bool cancel_reservation(int ticket_id);
	void show_reservations(stringstream& ss);
	void cancel_all_reservations();

		
	// TODO:
	void show_suggested_trips(string origin_name, string dest_name, Time date);
	void make_suggested_reservation(int suggested_trip_no, string origin_name, string dest_name, string date);
};
