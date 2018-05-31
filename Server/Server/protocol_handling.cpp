#include "stdafx.h"
#include "iprotocol.h"
#include "singleton.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "iocp.h"
#include "packet.h"
#include "packet_storage.h"
#include "user.h"
#include "user_manager.h"
#include "protocol_engine_io.h"
#include "protocol_user.h"
#include "protocol_handling.h"

ProtocolHandling::ProtocolHandling()
{
}


ProtocolHandling::~ProtocolHandling()
{
}

bool ProtocolHandling::Start()
{
	Bind();

	return true;
}

void ProtocolHandling::Stop()
{

}

void ProtocolHandling::Bind()
{
	BindEngineIO();
	BindUser();
}

void ProtocolHandling::BindEngineIO()
{
	FunctionVector functions;
	functions[protocol::engine_io::ID::kIO2Content_Connected] = ProtocolEngineIO::IO2Content_Connected;
	functions[protocol::engine_io::ID::kIO2Content_Disconnected] = ProtocolEngineIO::IO2Content_Disconnected;

	category_[protocol::Category::kEngineIO] = functions;
}

void ProtocolHandling::BindUser()
{
	FunctionVector functions;
	functions[protocol::user::ID::kC2S_LoginReq] = ProtocolUser::C2S_LoginReq;
	functions[protocol::user::ID::kC2S_Logout] = ProtocolUser::C2S_Logout;

	category_[protocol::Category::kUser] = functions;
}

void ProtocolHandling::Dispatch(const Packet* packet)
{
	if (packet == nullptr)
		return;

	auto packet_ptr = const_cast<Packet*>(packet);

	auto header = packet_ptr->get_header();
	if (header->category > protocol::Category::kMax || header->packet_id > category_[header->category].size())
		return;

	if (UserManager::GetSingleton()->IsValidSerial(packet_ptr->get_client_serial()))
		return;

	auto user = UserManager::GetSingleton()->GetUser(packet_ptr->get_client_serial());
	category_[header->category][header->packet_id](user, packet);

	PacketStorage::GetSingleton()->AddPacket(packet);
}
