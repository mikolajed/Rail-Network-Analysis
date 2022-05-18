#include<iostream>
///#include<graph.hpp>
//#include <nlohmann/json.hpp>
#include <string> 
#include <fstream> 
#include"declarations.hpp"
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

//using json = nlohmann::json;


/*	Graph G;
	G.add_node("a", "a");
	G.add_node("b", "b");
	G.add_edge("a", "b", 1);
	G.print();

	
	ifstream i("data/stations.json");

	json j;
	i >> j;

	cout<<j;
	*/
	
int main(int argc, const char * argv[]) {
	vm_size_t page_size;
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
		auto start = high_resolution_clock::now();
		Railway("../data/train_schedule.csv");
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout<<"free memory: "<<free_memory/8e6<<"MB\n"<<"used memory: "<<used_memory/8e6<<"MB\nload time: "<<duration.count()/1e6<<"sec\n";
	}
	else {
		cout<<"Could not check if memeory stats. Abort.";
	}
	    
	return 0;
}
