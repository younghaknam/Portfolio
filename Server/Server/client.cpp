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

Client::Client(WORD serial, SOCKET listen_socket)
	: serial_(serial)
	, sending_(false)
	, listen_socket_(listen_socket)
{
	send_pool_ = shared_ptr<PacketPool>(new PacketPool);
}

Client::~Client()
{
}

bool Client::RequestAccept()
{
	if (listen_socket_ == INVALID_SOCKET)
		return false;

	auto packet = GetPacket();
	if (tcp_socket_.RequestAccept(listen_socket_, packet) == false)
	{
		PacketStorage::GetSingleton()->AddPacket(packet);
		return false;
	}

	return true;
}

bool Client::RequestDisconnect()
{
	lock_guard<mutex> guard(disconnect_lock_);

	tcp_socket_.Disconnect();
	ReturnSendPackets();

	if (tcp_socket_.get_io_count() == 0)
		RequestAccept();

	return true;
}

bool Client::RequestReceiv()
{
	auto packet = GetPacket();
	if (tcp_socket_.RequestReceiv(packet) == false)
	{
		PacketStorage::GetSingleton()->AddPacket(packet);
		RequestDisconnect();
		return false;
	}

	return true;
}

bool Client::RequestSend(const Packet* packet)
{
	lock_guard<mutex> guard(send_lock_);

	if (sending_ == true)
	{
		send_pool_->AddPacket(packet);
		return true;
	}

	sending_ = true;
	if (tcp_socket_.RequestSend(const_cast<Packet*>(packet)) == false)
	{
		PacketStorage::GetSingleton()->AddPacket(packet);
		RequestDisconnect();
		return false;
	}

	return true;
}

void Client::Accepted(Packet* packet)
{
	tcp_socket_.decrement_io_count();

	packet->Initialize();
	packet->set_client_serial(serial_);
	if (tcp_socket_.RequestReceiv(packet) == false)
	{
		PacketStorage::GetSingleton()->AddPacket(packet);
		RequestDisconnect();
	}
}

void Client::Disconnected(Packet* packet)
{
	lock_guard<mutex> guard(disconnect_lock_);

	tcp_socket_.decrement_io_count();
	tcp_socket_.Disconnect();
	ReturnSendPackets();

	if (tcp_socket_.get_io_count() == 0)
		RequestAccept();
}

void Client::Received(Packet* packet, DWORD bytes)
{
	tcp_socket_.decrement_io_count();
	packet->set_io_bytes(static_cast<WORD>(bytes));

	int packet_count = packet->GetCompletedCount();
	for (int cnt = 0; cnt < packet_count; cnt++)
	{
		auto split_packet = GetPacket();
		if (packet->Split(split_packet) == false)
		{
			PacketStorage::GetSingleton()->AddPacket(split_packet);
			PacketStorage::GetSingleton()->AddPacket(packet);
			RequestDisconnect();
			return;
		}

		// ->AddPacket(split_packet) ÄÁÅÙÃ÷ Å¥¿¡ µî·Ï
	}

	if (packet->IsCompleted() == false)
	{
		packet->SetReceivedBytes();
		tcp_socket_.RequestReceiv(packet);
		return;
	}

	// ->AddPacket(packet) ÄÁÅÙÃ÷ Å¥¿¡ µî·Ï

	RequestReceiv();
}

void Client::Sent(Packet* packet, DWORD bytes)
{
	tcp_socket_.decrement_io_count();
	packet->set_io_bytes(static_cast<WORD>(bytes));

	lock_guard<mutex> guard(send_lock_);
	if (packet->IsCompleted() == false)
	{
		packet->SetSentBytes();
		if (tcp_socket_.RequestSend(packet) == false)
		{
			PacketStorage::GetSingleton()->AddPacket(packet);
			RequestDisconnect();
		}
		return;
	}

	if (send_pool_->Empty() == false)
	{
		auto send_packet = send_pool_->GetPacket();
		if (tcp_socket_.RequestSend(send_packet) == false)
		{
			PacketStorage::GetSingleton()->AddPacket(send_packet);
			RequestDisconnect();
		}
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

void Client::ReturnSendPackets()
{
	lock_guard<mutex> guard(send_lock_);

	while (send_pool_->Empty() == false)
	{
		PacketStorage::GetSingleton()->AddPacket(send_pool_->GetPacket());
	}
}
