#pragma once

class User;
class Packet;

class ProtocolEngineIO
{
public:
	ProtocolEngineIO();
	~ProtocolEngineIO();

	static void IO2Content_Connected(shared_ptr<User>& user, const Packet* packet);
	static void IO2Content_Disconnected(shared_ptr<User>& user, const Packet* packet);

};
