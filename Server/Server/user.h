#pragma once

class iRequestIO;
class Packet;

class User
{
public:
	explicit User(WORD client_serial, const shared_ptr<iRequestIO>& request_io);
	~User();

	bool Reset();
	void Connected();
	bool Disconnect();
	void Disconnected();
	bool Send(Packet* packet);
	bool get_is_connected() { return is_connected_; }

	void Login(const Packet* packet);
	void Logout();

private:
	WORD client_serial_;
	shared_ptr<iRequestIO> request_io_;
	bool is_connected_;
	DWORD id_;
	wstring name_;

};
