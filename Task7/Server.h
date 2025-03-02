#pragma once

class Server
{
public:
	Server() = default;
	Server(int _cap) {
		cap = _cap;
		free_req_index = 0;
		active_req = new Request[cap];
		heap = new Heap(Request());
	}

	void handle_request(Request req) {
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
		if (req.get_time_type() == END)
		{
			for (int i = 0; i < free_req_index; ++i) {
				if (req == active_req[i]) {
					get_req_stats(active_req[i]);
					delete &active_req[i];
					active_req[i] = NULL;
				}
			}
		}
		delete& req;
	}

	bool handle_sec(int sec) 
	{
		while (heap->get_min().get_start_time() == sec) 
		{
			handle_request(heap->pop_min());
		}
	}

private:
	int cap;
	int free_req_index;
	Request* active_req;
	Heap* heap;

	bool intersect(Request req1, Request req2)
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

	void get_req_stats(Request req) {
		return;
	}
};