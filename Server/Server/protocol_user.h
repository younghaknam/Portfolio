#pragma once

class User;
class Packet;

class ProtocolUser
{
public:
	ProtocolUser();
	~ProtocolUser();

	static void C2S_LoginReq(shared_ptr<User>& user, const Packet* packet);
	static void C2S_Logout(shared_ptr<User>& user, const Packet* packet);
};

