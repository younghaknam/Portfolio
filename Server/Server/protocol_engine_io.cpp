#include "stdafx.h"
#include "user.h"
#include "protocol_engine_io.h"

ProtocolEngineIO::ProtocolEngineIO()
{
}

ProtocolEngineIO::~ProtocolEngineIO()
{
}

void ProtocolEngineIO::IO2Content_Connected(shared_ptr<User>& user, const Packet* packet)
{
	if (packet == nullptr)
		return;

	user->Connected();
}

void ProtocolEngineIO::IO2Content_Disconnected(shared_ptr<User>& user, const Packet* packet)
{
	if (packet == nullptr)
		return;

	user->Disconnected();
}
