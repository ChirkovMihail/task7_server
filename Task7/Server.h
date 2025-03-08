#pragma once

class Server
{
public:
	Server() = default;
	Server(int _cap);
	~Server();

	tuple<int, int> get_median();
	tuple<double, double> get_average();
	tuple<int, int> get_max();
	tuple<int, int> get_min();
	void handle_request(Request req);	//processing request on server simulation
	bool handle_sec(int sec);			//passing user's sec simulation
	void show_all_statistics();

private:
	int cap;				//maximum number of simultaneously processing requests
	int free_req_index;		//index of the first free position for request
	Request* active_req;	//list of requests processing in the moment
	Heap* heap;				//priority queue for delayed requests

	vector<int> read_lat;	//lists of latencies for single requests
	vector<int> write_lat;

	//returns true if two requests intersect
	bool intersect(Request req1, Request req2);
	//updates latency statistics after end of processing request
	void update_stats(Request req);
};

Server::Server(int _cap) {
	cap = _cap;
	free_req_index = 0;
	active_req = new Request[cap];
	heap = new Heap(Request());
}

Server::~Server() {
	delete heap;
	delete active_req;
}

tuple<int, int> Server::get_median() {
	int read_median, write_median;
	if (read_lat.size() > 0) {
		sort(read_lat.begin(), read_lat.end());
		read_median = read_lat[read_lat.size() / 2];
	}
	else
		read_median = -1;

	if (write_lat.size() > 0) {
		sort(write_lat.begin(), write_lat.end());
		write_median = write_lat[write_lat.size() / 2];
	}
	else
		write_median = -1;
	return make_tuple(read_median, write_median);
}

tuple<double, double> Server::get_average() {
	double read_average = 0.0;
	double write_average = 0.0;
	if (read_lat.size() > 0) {
		for (int i = 0; i < read_lat.size(); ++i)
			read_average += (double)read_lat[i];
		read_average /= (double)read_lat.size();
	}
	else
		read_average = -1;
	if (write_lat.size() > 0) {
		for (int i = 0; i < write_lat.size(); ++i)
			write_average += (double)write_lat[i];
		write_average /= (double)write_lat.size();
	}
	else
		write_average = -1;
	return make_tuple(read_average, write_average);
}

tuple<int, int> Server::get_max() {
	int read_max, write_max;
	if (read_lat.size() > 0) {
		read_max = *max_element(read_lat.begin(), read_lat.end());
	}
	else
		read_max = -1;
	if (write_lat.size() > 0) {
		write_max = *max_element(write_lat.begin(), write_lat.end());
	}
	else
		write_max = -1;
	return make_tuple(read_max, write_max);
}

tuple<int, int> Server::get_min() {
	int read_min, write_min;
	if (read_lat.size() > 0) {
		read_min = *min_element(read_lat.begin(), read_lat.end());
	}
	else
		read_min = -1;
	if (write_lat.size() > 0) {
		write_min = *min_element(write_lat.begin(), write_lat.end());
	}
	else
		write_min = -1;
	return make_tuple(read_min, write_min);
}

void Server::handle_request(Request req) {
	if (req.get_time_type() == START) {
		int max_end_time = -1;
		//check if current requests intersects with any WRITE-requests
		for (int i = 0; i < free_req_index; ++i) {
			if (active_req[i].get_type() == WRITE && intersect(active_req[i], req)) {
				max_end_time = max(max_end_time, active_req[i].get_start_time() +
					active_req[i].get_size() * active_req[i].get_lat());
			}
		}

		//if so, delay current request
		if (max_end_time != -1) {
			req.set_start_time(max_end_time);
			heap->insert(req);
			return;
		}

		//otherwise check if there is space for another one 
		//active request
		if (free_req_index == cap) {
			int max_end_time = -1;
			for (int i = 0; i < free_req_index; ++i) {
				max_end_time = max(max_end_time, active_req[i].get_start_time() + 
					active_req[i].get_size() * active_req[i].get_lat());
			}

			//if no, delay current request
			req.set_start_time(max_end_time);
			heap->insert(req);
			return;
		}

		//otherwise add current request to active
		active_req[free_req_index] = req;
		free_req_index++;

		//and insert in priority queue request with END-mark
		req.set_start_time(req.get_start_time() + req.get_size() * req.get_lat());
		req.set_time_type(END);
		heap->insert(req);

		return;
	}

	if (req.get_time_type() == END)
	{
		//find the same request with START-mark
		for (int i = 0; i < free_req_index; ++i) {
			if (req == active_req[i]) {
				update_stats(req);
				//shift active requests 
				for (int j = i; j + 1 < free_req_index; ++j)
					active_req[j] = active_req[j + 1];

				//free space for the next request
				free_req_index--;
				active_req[free_req_index] = NULL;
			}
		}
		return;
	}
}

bool Server::handle_sec(int sec)
{
	//check if there is erquest in queue which should be processed
	//at this second
	while (heap->get_min().get_start_time() == sec)
	{
		//if so, handle it and pop
		handle_request(heap->pop_min());
	}
	//return true if there is at least one request in queue
	return !heap->empty();
}

bool Server::intersect(Request req1, Request req2)
{
	int x1, x2, y1, y2;
	x1 = req1.get_start_time();
	x2 = x1 + req1.get_lat() * req1.get_size();

	y1 = req2.get_start_time();
	y2 = y1 + req2.get_lat() * req2.get_size();

	if (x1 <= y1 && y1 <= x2 || x1 <= y2 && y2 <= x2)
		return true;
	return false;
}

void Server::update_stats(Request req) {
	if (req.get_type() == READ)
		read_lat.push_back(req.get_start_time() - req.get_time_stamp());
	if (req.get_type() == WRITE)
		write_lat.push_back(req.get_start_time() - req.get_time_stamp());
	return;
}

void Server::show_all_statistics()
{
	int r, w;
	double rd, wd;

	tie(r, w) = get_median();
	cout << "READ median : " << r << '\n';
	cout << "WRITE median : " << w << '\n';

	tie(rd, wd) = get_average();
	cout << "READ average : " << rd << '\n';
	cout << "WRITE average : " << wd << '\n';

	tie(r, w) = get_max();
	cout << "READ max : " << r << '\n';
	cout << "WRITE max : " << w << '\n';

	tie(r, w) = get_min();
	cout << "READ min : " << r << '\n';
	cout << "WRITE min : " << w << '\n';
}