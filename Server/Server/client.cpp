#include "stdafx.h"
#include "singleton.h"
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
	, sending_(false)
{
	send_pool_ = shared_ptr<PacketPool>(new PacketPool);
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

bool Client::RequestSend(const Packet* packet)
{
	lock_guard<mutex> guard(lock_);

	if (sending_ == true)
	{
		send_pool_->AddPacket(packet);
		return true;
	}

	sending_ = true;
	return tcp_socket_.RequestSend(const_cast<Packet*>(packet));
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
	packet->set_io_bytes(bytes);

	int packet_count = packet->GetCompletedCount();
	for (int cnt = 0; cnt < packet_count; cnt++)
	{
		auto split_packet = GetPacket();
		if (packet->Split(split_packet) == false)
		{
			PacketStorage::GetSingleton()->AddPacket(split_packet);
			Disconnected(packet);
			return;
		}

		// ->AddPacket(split_packet) ������ ť�� ���
	}

	if (packet->IsCompleted() == false)
	{
		packet->SetReceivedBytes();
		tcp_socket_.RequestRecv(packet);
		return;
	}

	// ->AddPacket(packet) ������ ť�� ���

	RequestRecv();
}

void Client::Sent(Packet* packet, DWORD bytes)
{
	lock_guard<mutex> guard(lock_);
	packet->set_io_bytes(bytes);

	if (packet->IsCompleted() == false)
	{
		packet->SetSentBytes();
		tcp_socket_.RequestSend(packet);
		return;
	}

	if (send_pool_->Empty() == false)
	{
		tcp_socket_.RequestSend(send_pool_->GetPacket());
		return;
	}

	sending_ = false;
}

Packet* Client::GetPacket()
{
	auto packet = PacketStorage::GetSingleton()->GetPacket();
	packet->Initialize();
	packet->set_client_serial(serial_);
	return packet;
}
