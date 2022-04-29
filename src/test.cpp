/*
	The data set the Railway.hpp is tested on is taken from Kaggle (https://www.kaggle.com/datasets/arihantjain09/indian-railways-latest).
	Kaggle provides a tool to exlopre that dataset.

	IMPORTANT!
	At this stage ONLY train_info.csv is used.
*/
#include<iostream>
#include<ctime>
#include<Railway.hpp>

using namespace std;
using namespace Railway;

void test_out(string test_name, bool is_failed, string fail_comment) { 
	cout<<test_name<<": ";
	if (is_failed) {
		cout<<"FAILED\n"<<<fail_comment<<"\n";
		//exit(1);
	}
	else
		cout<<"PASSED\n";
}

int main() {
	// create a railway based on data in specified path
	Railway IndiaExpress("./data");

	time_t date = now();

	stringstream ss;
	bool is_failed;

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
		IndiaExpress.connection_exists("ABHANPUR JN.", "RAJIM"),
		"Connection not recognized by the system");
	/*
		TEST 3
		Check if the schedule of a station is presented corretly
		TODO: More schedules of more stations could be tested
	*/
	ss.str(string()); // clean stream	
	IndiaExpress.show_schedule(ss, "VRIDHA CHALAM JN.");
	is_failed = (ss != 	"76845\tVRI-TPJ PASS\tVRIDHA CHALAM JN.\tTIRUCHIRAPPALLI JN.\tWednesday\n"
				"76849\tVRI-SA PASSR\tVRIDHA CHALAM JN.\tSALEM JN.\tWednesday\n"
				"76847\tVRI-SA PASSR\tVRIDHA CHALAM JN.\tSALEM JN.\tFriday\n");

	test_out("TEST 3",
		is_failed,
		"The schedule is not presented corretly");
	/*
		TEST 4
		Check if reservation is done correctly
	*/
	IndiaExpress.make_reservation(76845);
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss != "1\t76845\tVRI-TPJ PASS\tVRIDHA CHALAM JN.\tTIRUCHIRAPPALLI JN.\tWednesday\n");
	test_out("TEST 4",
		is_failed,
		"Reservation is not made or/and showed correctly");

	/*
		TEST 5
		Check if cancelation is done correctly.
		If the previous test passed there is one reservation that has been made - ticket number is 1
	*/
	IndiaExpress.cancel_reservation(1);
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss !=	"");
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
	is_failed = (ss !=	"");
	test_out("TEST 6",
		is_failed,
		"Reservation is not cancelled correctly if no reservations have been made");
		
	/*
		TEST 7
		Make many reservations of the same train.
		TODO: Add more combinations of adding. Add making and cancellation of many resevations.
	*/
	IndiaExpress.make_reservation(76845);
	IndiaExpress.make_reservation(76845);
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = (ss != 	"1\t76845\tVRI-TPJ PASS\tVRIDHA CHALAM JN.\tTIRUCHIRAPPALLI JN.\tWednesday\n");
				"2\t76845\tVRI-TPJ PASS\tVRIDHA CHALAM JN.\tTIRUCHIRAPPALLI JN.\tWednesday\n");
	test_out("TEST 7",
		is_failed,
		"Two reservations are not made correctly");

	/*
		TEST 8
		Cancel all reservations.
		At this stage two reservations should be in the system.
	*/
	IndiaExpress.cancel_all_reservations();
	ss.str(string()); // clean stream	
	
	IndiaExpress.show_reservations(ss);
	is_failed = ( ss != "" );
	test_out("TEST 8",
		is_failed,
		"Cancellation of all reservations is not made correctly");
	/* 
		TODO: Features to be added
		IndiaExpress.show_suggested_trips(origin);
	*/

	return 0;
}

