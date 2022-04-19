#include<vector>
#include<unordered_map>
#include<string>

class Ticket {
public:
	int number;
	double price;
	string originName, destName;
	string routeCode;
	int seatNumber;
};

enum class SeatCatgory { window, aisle, middle, special };

class Train {
public:
	int id;
	string name;
	int capacity;
	vector<int, SeatCategory> seats;
};

class System {
	class Graph {
		class Node {
			int id;
			string code;
			string val;

			Node();
			~Node();
		};
		class Edge {
			Node v, u; 	// source and destination node
			int d;		// distance

			Edge(Node v, Node u);
			~Edge();
		};
	
		unordered_map<string, int> nodeIDs;
		vector<Node> V;
		vector<Edge> E;

		void addNode();
		void addEdge();
	};

	class Route {
		int id;
		vector<int> stations;
		string name;

		Route(int id, string name);
		~Route();

	public:
		appendStation(int stationID);
	}

	void addRoute():
	bool makeReservation(int routeID, string originCode, string destCode);
	bool recommendConnection(string originCode, string destCode);
	
	unordered_map<string, int> stationIDs;
	unordered_map<string, int> routeIDs;

	vector<Route> routes;
	vector<Train> trains;
};
