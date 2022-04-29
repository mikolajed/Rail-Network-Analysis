#include<vector>
#include<sstream>
#include<unordered_map>
#include<string>
#include<ctime>
//#include<graph.hpp>

class Railway {
	using namespace std;
	typedef time_t Time;
	
	class Ticket {
	public:
		int id;
		double price;
		int seat_number;
		Time purchase_time;
	};
	vector<Ticket> ticket_log;
	
	class Station {
	public:
		string code;	// aberviation of station name used by railway operator
		string name;	// full name of the station
	};
	vector<Station> stations;

	class Connection {
	public:
		Station* origin;
		Station* dest; 	
		Time arrvial_time;
	};
	vector<Connection> connections;
	
	class Route {
	public:
		int train_no; 					// an identifier of a route
		vector<Connection*> parts;
		string days;
	};
	vector<Route> routes;
	
	// TODO: Graph implementation will be used for searching the best route between two stations
	// TrainNetwork to a large extend implements a directed graph, where 
	// nodes are represented as stations and edges as connections 
	// there is a connection between two nodes if there exist at least one route between them
	class TrainNetwork {
		class Node {
			set<int> adj_in;	// stores list of incoming connections 
			set<int> adj_out;	// stores list of outgoing connections
			friend class TrainNetwork;
		};
		class Edge {
			int route_id;		// there might be many connections between two stations
			int v_idx, u_idx; 	// origin and destination station
			friend class TrainNetwork;
			Time arrvial_time;
		};

		vector<Node> V;
		vector<Edge> E;
		unordered_map<string, int> node_IDs;	// node label is mapped to a number - the index in the vector of nodes
		
	public:	
		// a station has to be added prior to adding a connection between that station and another
		void add_node(string code);
		// a connection can be added only between two added stations
		void add_edge(int route_id, string origin_code, string dest_code, Time arrival_time);
		vector<string> shortest_path(string origin_code, string dest_code);
	};

	// if date is not specified current date is used
	Railway(string data_path);
	Railway(string data_path, Time date);

public:
	void show_schedule(stringstream& ss, string station_name);
	bool connection_exists(string origin_name, string dest_name)
	void make_reservation(int train_no, string origin_code, string dest_code);
	bool cancel_reservation(int ticket_id);
	void show_reseverations(stringstream& ss);
	void cancel_all_reservations();
	
	TODO:
	void show_suggested_trips(string origin_name, string dest_name, Time date);
	void make_suggested_reservation(int suggested_trip_no, string origin_name, string dest_name, Time date);
};
