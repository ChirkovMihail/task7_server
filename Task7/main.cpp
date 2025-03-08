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

#include"tests.h"

int main()
{
	do_tests();

	int curr_sec = 0, i, curr_req;
	int req_id, req_timestamp, req_address, req_size;
	string req_type;
	REQ_TYPE tp;
	vector<Request> trace(50);

	ifstream fin("input.txt");
	for (i = 0; i < trace.size(); ++i) {
		fin >> req_id >> req_timestamp >> req_type >> req_address >> req_size;
		if (req_type == "READ")
			tp = READ;
		else
			tp = WRITE;
		trace[i] = Request(req_id, req_timestamp, tp, req_address, req_size, START);
	}

	int capacity = 1;
	Server server(capacity);

	bool there_are_changes = true;
	curr_sec = trace[0].get_time_stamp();
	curr_req = 0;
	while (there_are_changes)
	{
		there_are_changes = false;

		if (server.handle_sec(curr_sec))
			there_are_changes = true;

		if (curr_req < trace.size() && trace[curr_req].get_time_stamp() == curr_sec) {
			server.handle_request(trace[curr_req]);
			curr_req++;

			there_are_changes = true;
		}

		curr_sec++;
	}

	double a, b;
	tie(a, b) = server.get_average();
	cout << a << ' ' << b << '\n';

	int x, y;
	tie(x, y) = server.get_median();
	cout << x << ' ' << y << '\n';

	fin.close();

	return 0;
}