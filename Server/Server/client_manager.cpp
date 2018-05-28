#include "stdafx.h"
#include "irequest_io.h"
#include "icompleted_io.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "tcp_socket.h"
#include "client.h"
#include "client_manager.h"

ClientManager::ClientManager()
	: client_count_(0)
	, listen_socket_(INVALID_SOCKET)
	, io_iocp_(nullptr)
{
}

ClientManager::~ClientManager()
{
}

void ClientManager::Initialize(SOCKET listen_socket, const shared_ptr<Iocp>& io_iocp)
{
	listen_socket_ = listen_socket;
	io_iocp_ = io_iocp;
}

bool ClientManager::Start(WORD client_count)
{
	if (client_count == 0)
		return false;

	client_count_ = client_count;

	for (WORD client_serial = 0; client_serial < client_count_; client_serial++)
	{
		auto client = shared_ptr<Client>(new Client(client_serial));
		if (client->RequestAccept(listen_socket_) == false)
			return false;

		clients_.push_back(client);
	}

	return true;
}

void ClientManager::Stop()
{
	for (auto client : clients_)
	{
		//client->
	}
}

bool ClientManager::RequestAccept(const void* packet)
{
	return true;
}

bool ClientManager::RequestDisconnect(WORD client_serial)
{
	return true;
}

bool ClientManager::RequestReceiv(const void* packet)
{
	return true;
}

bool ClientManager::RequestSend(const void* packet)
{
	return true;
}

void ClientManager::OnAccepted(const void* packet)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	if (packet_ptr->get_client_serial() >= client_count_)
		return;

	auto client = clients_[packet_ptr->get_client_serial()];
	io_iocp_->Bind(reinterpret_cast<HANDLE>(client->get_tcp_socket().get_socket()));
	client->Accepted(packet_ptr);
}

void ClientManager::OnDisconnected(const void* packet)
{

}

void ClientManager::OnReceived(const void* packet, const DWORD bytes)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	if (packet_ptr->get_client_serial() >= client_count_)
		return;

	auto client = clients_[packet_ptr->get_client_serial()];
	client->Received(packet_ptr, bytes);
}

void ClientManager::OnSent(const void* packet, const DWORD bytes)
{

}
