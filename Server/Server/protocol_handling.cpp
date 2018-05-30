#include "stdafx.h"
#include "iprotocol.h"
#include "singleton.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "iocp.h"
#include "packet.h"
#include "user.h"
#include "user_manager.h"
#include "protocol_network_io.h"
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
	BindNetworkIO();
}

void ProtocolHandling::BindNetworkIO()
{
	FunctionVector functions;
	functions[protocol::network_io::ID::kIO2Con_Connect] = ProtocolNetworkIO::IO2Con_Connect;
	functions[protocol::network_io::ID::kIO2Con_Disconnect] = ProtocolNetworkIO::IO2Con_Disconnect;

	category_[protocol::Category::kNetworkIO] = functions;
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
}
