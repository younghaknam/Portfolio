#pragma once

class iRequestIO;

class User
{
public:
	explicit User(WORD client_serial, const shared_ptr<iRequestIO>& request_io);
	~User();

	bool Reset();
	bool Disconnect();
	void Disconnected();

	bool get_is_connected() { return is_connected_; }

private:
	WORD client_serial_;
	shared_ptr<iRequestIO> request_io_;
	bool is_connected_;
};
