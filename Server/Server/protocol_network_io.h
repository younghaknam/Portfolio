#pragma once

class User;
class Packet;

class ProtocolNetworkIO
{
public:
	ProtocolNetworkIO();
	~ProtocolNetworkIO();

	static void IO2Con_Connect(shared_ptr<User>& user, const Packet* packet);
	static void IO2Con_Disconnect(shared_ptr<User>& user, const Packet* packet);

};
