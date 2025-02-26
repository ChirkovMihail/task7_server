#pragma once

enum REQ_TYPE
{
	READ_START = 0,
	READ_END = 1,
	WRITE_START = 2,
	WRITE_END = 3
};

class Request 
{
public:
	Request(int _id, int _time_stamp,
		REQ_TYPE _type, int _address, int _size);

	int get_time_stamp();
	REQ_TYPE get_type();
	int get_address();
	int get_size();

private:
	int id;
	int time_stamp;
	REQ_TYPE type;
	int address;
	int size;
};

Request::Request(int _id, int _time_stamp,
	REQ_TYPE _type, int _address, int _size) {
	id = _id;
	time_stamp = _time_stamp;
	type = _type;
	address = _address;
	size = _size;
}

int Request::get_time_stamp() { return time_stamp; }

REQ_TYPE Request::get_type() { return type; }

int Request::get_address() { return address; }

int Request::get_size() { return size; }