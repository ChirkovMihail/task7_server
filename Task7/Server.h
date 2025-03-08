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
	void handle_request(Request req);
	bool handle_sec(int sec);

private:
	int cap;
	int free_req_index;
	Request* active_req;
	Heap* heap;

	vector<int> read_lat, write_lat;

	bool intersect(Request req1, Request req2);
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
		for (int i = 0; i < free_req_index; ++i) {
			if (active_req[i].get_type() == WRITE && intersect(active_req[i], req)) {
				max_end_time = max(max_end_time, active_req[i].get_start_time() + active_req[i].get_size() * active_req[i].get_lat());
			}
		}

		if (max_end_time != -1) {
			req.set_start_time(max_end_time);
			heap->insert(req);
			return;
		}

		if (free_req_index == cap) {
			int max_end_time = -1;
			for (int i = 0; i < free_req_index; ++i) {
				max_end_time = max(max_end_time, active_req[i].get_start_time() + active_req[i].get_size() * active_req[i].get_lat());
			}

			req.set_start_time(max_end_time);
			heap->insert(req);
			return;
		}

		active_req[free_req_index] = req;
		free_req_index++;

		req.set_start_time(req.get_start_time() + req.get_size() * req.get_lat());
		req.set_time_type(END);

		heap->insert(req);

	}
	else {
		if (req.get_time_type() == END)
		{
			for (int i = 0; i < free_req_index; ++i) {
				if (req == active_req[i]) {
					update_stats(req);
					for (int j = i; j + 1 < free_req_index; ++j)
						active_req[j] = active_req[j + 1];
					free_req_index--;
					active_req[free_req_index] = NULL;
				}
			}
		}
	}
}

bool Server::handle_sec(int sec)
{
	while (heap->get_min().get_start_time() == sec)
	{
		handle_request(heap->pop_min());
	}
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