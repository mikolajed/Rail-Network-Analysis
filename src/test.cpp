/*
	The data set the Railway.hpp is tested on is taken from Kaggle (https://www.kaggle.com/datasets/arihantjain09/indian-railways-latest).
	Kaggle provides a tool to exlopre that dataset.

	IMPORTANT!
	At this stage ONLY train_info.csv is used.
*/
#include<iostream>
#include<fstream>
#include<string>
#include"declarations.hpp"

using namespace std;

void test_out(string test_name, bool is_failed, string fail_comment) { 
	cout<<test_name<<": ";
	if (is_failed) {
		cout<<"FAILED\n"<<fail_comment<<"\n";
		exit(1);
	}
	else
		cout<<"PASSED\n";
}

string file_to_string(string file_path) {
	stringstream ss;
	ifstream file(file_path);
	if (not file.good()) {
		cerr<<"Could not open the file "<<file_path<<"\n";
		exit(1);
	}
	string line = "";
	while (getline(file, line)) {
		ss<<line<<"\n";
		line = "";
	}
	return ss.str();
}

int main() {
	// create a railway based on data in specified path
	Railway IndiaExpress("../data/");

	stringstream ss;
	bool is_failed;
#if 0
	/*
		TEST 1
		The connection does not exist in the database and should not be present in the Railway
	*/
	test_out("TEST 1",
		not IndiaExpress.connection_exists("A", "B"),
		"Connection between station should not exist");
	/*
		TEST 2
		Connection exists but is not recognized by the system
	*/
	test_out("TEST 2",
		IndiaExpress.connection_exists("JAMMU TAWI", "SHMATA VD KATRA"),
		"Connection not recognized by the system");
#endif
	/*
		TEST 3
		Check if the schedule of a station is presented corretly
		TODO: More schedules of more stations could be tested
	*/
	ss.str(string()); // clean stream	
	IndiaExpress.show_schedule(ss, "RAJAPUR ROAD");
	is_failed = (ss.str() != file_to_string("tests/test3.txt"));
	test_out("TEST 3",
		is_failed,
		"The schedule is not presented corretly");
	/*
		TEST 4
		Check if reservation is done correctly
	*/
	IndiaExpress.make_reservation(22439, "NEW DELHI", "SHMATA VD KATRA");
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss.str() != file_to_string("tests/test4.txt"));
	test_out("TEST 4",
		is_failed,
		"Reservation is not made or/and showed correctly");

	/*
		TEST 5
		Check if cancelation is done correctly.
		If the previous test passed there is one reservation that has been made - ticket number is 1
	*/
	IndiaExpress.cancel_reservation(0);
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss.str() != file_to_string("tests/test5.txt"));
	test_out("TEST 5",
		is_failed,
		"Reservation is not cancelled correctly");

	/*
		TEST 6
		Check if cancelation is not performed if the reservation does not exist.
		At this point no resevations should be saved by the system, so cancellation of any reseravation should not have any effect.
	*/
	IndiaExpress.cancel_reservation(42);
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss.str() != file_to_string("tests/test6.txt"));
	test_out("TEST 6",
		is_failed,
		"Reservation is not cancelled correctly if no reservations have been made");
	/*
		TEST 7
		Make many reservations of the same train.
		TODO: Add more combinations of adding. Add making and cancellation of many resevations.
	*/
	IndiaExpress.make_reservation(22439, "NEW DELHI", "SHMATA VD KATRA");
	IndiaExpress.make_reservation(22439, "NEW DELHI", "SHMATA VD KATRA");
	IndiaExpress.make_reservation(107, "SAWANTWADI R", "MADGOAN JN.");
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss.str() != file_to_string("tests/test7.txt"));
	test_out("TEST 7",
		is_failed,
		"Multiple reservations are not made correctly");

	/*
		TEST 8
		Cancel all reservations.
		At this stage two reservations should be in the system.
	*/
	IndiaExpress.cancel_all_reservations();
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss.str() != file_to_string("tests/test8.txt"));
	test_out("TEST 8",
		is_failed,
		"Cancellation of all reservations is not made correctly");
	/* 
		TODO: Features to be added
		IndiaExpress.show_suggested_trips(origin);
	*/

	return 0;
}

