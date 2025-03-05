#pragma once

enum REQ_TYPE
{
	READ = 0,
	WRITE = 1,
	NONE = -1
};

enum REQ_TIME_TYPE
{
	START = 0,
	END = 1
};

class Request 
{
public:
	Request(int _id = -1, int _time_stamp = (int)1e9,
		REQ_TYPE _type = NONE, int _address = -1, int _size = -1, REQ_TIME_TYPE _time_type = START);

	int get_time_stamp();
	REQ_TYPE get_type();
	int get_address();
	int get_size();
	int get_start_time();
	int get_lat();
	REQ_TIME_TYPE get_time_type();

	void set_start_time(int t);
	void set_time_type(REQ_TIME_TYPE t);

	bool operator < ( Request req);
	bool operator > (Request req);
	bool operator == (Request req);

private:
	int id;
	int time_stamp;
	REQ_TYPE type;
	int address;
	int size;
	int start_time;
	REQ_TIME_TYPE time_type;
};

Request::Request(int _id, int _time_stamp,
	REQ_TYPE _type, int _address, int _size, REQ_TIME_TYPE _time_type) {
	id = _id;
	time_stamp = _time_stamp;
	type = _type;
	address = _address;
	size = _size;
	start_time = _time_stamp;
	time_type = _time_type;
}

void Request::set_start_time(int t) { this->start_time = t; }

void Request::set_time_type(REQ_TIME_TYPE t) { this->time_type = t; }

int Request::get_lat()
{
	if (type == WRITE)
		return 1;
	if (type == READ)
		return 2;
}

int Request::get_time_stamp() { return time_stamp; }

REQ_TYPE Request::get_type() { return type; }

int Request::get_address() { return address; }

int Request::get_size() { return size; }

int Request::get_start_time() { return start_time; }

REQ_TIME_TYPE Request::get_time_type() { return time_type; }

bool Request::operator < ( Request req)
{
	return this->start_time < req.start_time ||
		this->start_time == req.start_time && this->time_type == END;
}

bool Request::operator > (Request req)
{
	return this->start_time > req.start_time ||
		this->start_time == req.start_time && req.time_type == END;
}

bool Request::operator == (Request req)
{
	return this->id == req.id;
}