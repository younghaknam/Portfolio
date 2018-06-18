#include "stdafx.h"
#include "irequest_io.h"
#include "icompleted_io.h"
#include "icontent.h"
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

void ClientManager::Initialize(SOCKET listen_socket, const shared_ptr<Iocp>& io_iocp, const shared_ptr<iContent>& content)
{
	listen_socket_ = listen_socket;
	io_iocp_ = io_iocp;
	content_ = content;
}

bool ClientManager::Start(WORD client_count)
{
	if (client_count == 0)
		return false;

	client_count_ = client_count;

	for (WORD client_serial = 0; client_serial < client_count_; client_serial++)
	{
		auto client = shared_ptr<Client>(new Client(client_serial, listen_socket_, content_));
		clients_.push_back(client);
	}

	return true;
}

void ClientManager::Stop()
{
	for (auto client : clients_)
	{
		client->RequestDisconnect();
	}
}

bool ClientManager::RequestAccept(WORD client_serial)
{
	shared_ptr<Client> client(nullptr);
	if (GetClient(client_serial, client) == false)
		return false;

	return client->RequestAccept();
}

bool ClientManager::RequestDisconnect(WORD client_serial)
{
	shared_ptr<Client> client(nullptr);
	if (GetClient(client_serial, client) == false)
		return false;

	return client->RequestDisconnect();
}

bool ClientManager::RequestSend(const void* packet)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	shared_ptr<Client> client(nullptr);
	if (GetClient(packet_ptr->get_client_serial(), client) == false)
		return false;

	return client->RequestSend(packet_ptr);
}

void ClientManager::Accepted(const void* packet)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	shared_ptr<Client> client(nullptr);
	if (GetClient(packet_ptr->get_client_serial(), client) == false)
		return;

	io_iocp_->Bind(reinterpret_cast<HANDLE>(client->get_tcp_socket().get_socket()));
	client->Accepted(packet_ptr);
}

void ClientManager::Disconnected(const void* packet)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	shared_ptr<Client> client(nullptr);
	if (GetClient(packet_ptr->get_client_serial(), client) == false)
		return;

	client->Disconnected(packet_ptr);
}

void ClientManager::Received(const void* packet, const DWORD bytes)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	shared_ptr<Client> client(nullptr);
	if (GetClient(packet_ptr->get_client_serial(), client) == false)
		return;

	client->Received(packet_ptr, bytes);
}

void ClientManager::Sent(const void* packet, const DWORD bytes)
{
	auto packet_ptr = reinterpret_cast<Packet*>(const_cast<void*>(packet));
	shared_ptr<Client> client(nullptr);
	if (GetClient(packet_ptr->get_client_serial(), client) == false)
		return;

	client->Sent(packet_ptr, bytes);
}

 bool ClientManager::GetClient(WORD serial, shared_ptr<Client>& client)
{
	 if (serial >= client_count_)
		 return false;

	 client = clients_[serial];
	 return true;
}