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
	return tcp_socket_.RequestAccept(listen_socket, GetPacket());
}

bool Client::RequestDisconnect()
{
	return true;
}

bool Client::RequestRecv()
{
	auto packet = GetPacket();
	return tcp_socket_.RequestRecv(packet);
}

bool Client::RequestSend()
{
	return true;
}

void Client::Accepted(Packet* packet)
{
	packet->Initialize();
	tcp_socket_.RequestRecv(packet);
}

void Client::Disconnected(Packet* packet)
{

}

void Client::Received(Packet* packet, DWORD bytes)
{
	// 요청 처리
}

void Client::Sent(Packet* packet, DWORD bytes)
{

}

Packet* Client::GetPacket()
{
	auto packet = PacketStorage::GetSingleton()->GetPacket();
	packet->Initialize();
	packet->set_client_serial(serial_);
	return packet;
}
