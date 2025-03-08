#include<iostream>
#include<fstream>
#include<algorithm>
#include<random>
#include<functional>
#include<set>
#include <string>

using namespace std;

#include"Request.h"
#include"Treap.h"
#include"Heap.h"
#include"Server.h"

const int SERVER_CAP = 10;
const int TOTAL_TRACE = 50;

void init_trace(vector<Request>&trace)
{
	int req_id, req_timestamp, req_address, req_size, i;
	string req_type;
	REQ_TYPE tp;

	ifstream fin("input.txt");
	for (i = 0; i < trace.size(); ++i) {
		fin >> req_id >> req_timestamp >> req_type >> req_address >> req_size;
		if (req_type == "READ")
			tp = READ;
		else
			tp = WRITE;
		trace[i] = Request(req_id, req_timestamp, tp, req_address, req_size, START);
	}

	fin.close();
}

int main()
{
	//input requests
	vector<Request> trace(TOTAL_TRACE);
	init_trace(trace);

	//create server
	int capacity = SERVER_CAP;
	Server server(capacity);


	bool there_are_changes = true;
	int curr_sec = trace[0].get_time_stamp();
	int curr_req = 0;

	//while there are request to processed
	//increase current second
	while (there_are_changes)
	{
		there_are_changes = false;

		//simulate another second on server
		if (server.handle_sec(curr_sec))
			there_are_changes = true;

		//if there is request to be processed at this moment
		if (curr_req < trace.size() && trace[curr_req].get_time_stamp() == curr_sec) {
			server.handle_request(trace[curr_req]);
			curr_req++;

			there_are_changes = true;
		}

		curr_sec++;
	}

	server.show_all_statistics();

	return 0;
}