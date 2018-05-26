#include "stdafx.h"
#include "singleton.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"
#include "packet_storage.h"
#include "tcp_socket.h"
#include "client.h"

Client::Client(WORD serial)
	: serial_(serial)
{
}

Client::~Client()
{
}

bool Client::RequestAccept(SOCKET listen_socket)
{
	auto packet = PacketStorage::GetSingleton()->GetPacket();
	packet->Initialize();
	return tcp_socket_.RequestAccept(listen_socket, packet);
}