#include "stdafx.h"
#include "user.h"
#include "protocol_user.h"


ProtocolUser::ProtocolUser()
{
}


ProtocolUser::~ProtocolUser()
{
}

void ProtocolUser::C2S_LoginReq(shared_ptr<User>& user, const Packet* packet)
{
	if (packet == nullptr)
		return;

	user->Login(packet);
}

void ProtocolUser::C2S_Logout(shared_ptr<User>& user, const Packet* packet)
{
	if (packet == nullptr)
		return;

	user->Logout();
}
