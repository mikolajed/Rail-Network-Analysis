#include<vector>
#include<sstream>
#include<unordered_map>
#include<string>
#include<ctime>
//#include<graph.hpp>

class Railway {
	using namespace std;
	typedef time_t Time;
	
	struct Ticket {
		int id;
		double price;
		int seat_number;
		Time purchase_date;
	};
	vector<Ticket> ticket_log;
	
	struct Station {
		string code;	// aberviation of station name used by railway operator
		string name;	// full name of the station
		// other data could be added
	};
	vector<Station> stations;

	class Connection {
		int origin;	// index of origin station in stations vector
		int dest; 	// index of destination station in stations vector	
		Time arrvial_time;
		friend class Railway;
	public:
		Station& get_origin_station();
		Station& get_dest_station();
		string get_time_date();		// returns only time of arrival in format dd:mm:ss
		string get_day_date();		// returns day of arrivial time, e.g. "Monday"
	};
	vector<Connection> connections;
	
	class Route {
		int origin;	// index of origin station in stations vector
		int dest; 	// index of destination station in stations vector	
		friend class Railway;	
	public:	
		int train_no; 	// an identifier of a route
		string days;
		Station& get_origin_station();
		Station& get_dest_station();
	};
	vector<Route> routes;
	
	// TODO: Graph implementation will be used for searching the best route between two stations
	// TrainNetwork to a large extend implements a directed graph, where 
	// nodes are represented as stations and edges as connections 
	// there is a connection between two nodes if there exist at least one route between them
	class TrainNetwork {
		typedef t_time Time;
		class Node {
			set<int> adj_in;	// stores list of incoming connections 
			set<int> adj_out;	// stores list of outgoing connections
			friend class TrainNetwork;
		};
		class Edge {
			int route_id;		// there might be many connections between two stations
			int v_idx, u_idx; 	// indexes of nodes in nodes vector
			friend class TrainNetwork;
			Time arrvial_time;
		};
		vector<Node> nodes;
		vector<Edge> edges;
		unordered_map<string, int> node_IDs;	// node label is mapped to a number - the index in the vector of nodes
	public:	
		void add_node(string label);
		// adding an edge between nodes can be done ONLY IF both nodes exist in the graph
		void add_edge(int route_id, string origin_code, string dest_code, Time arrival_time);
		vector<string> shortest_path(string origin_code, string dest_code);
	};

	TrainNetwork Network;
	// if date is not specified current date is used
	Railway(string data_path);
	Railway(string data_path, Time date);

public:
	void show_schedule(stringstream& ss, string station_name);
	bool connection_exists(string origin_name, string dest_name);
	void make_reservation(int train_no, string origin_code, string dest_code);
	bool cancel_reservation(int ticket_id);
	void show_reseverations(stringstream& ss);
	void cancel_all_reservations();
	
	// TODO:
	void show_suggested_trips(string origin_name, string dest_name, Time date);
	void make_suggested_reservation(int suggested_trip_no, string origin_name, string dest_name, Time date);
};
