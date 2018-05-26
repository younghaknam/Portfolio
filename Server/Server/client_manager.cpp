#include "stdafx.h"
#include "inetwork_event.h"
#include "tcp_socket.h"
#include "client.h"
#include "client_manager.h"

ClientManager::ClientManager()
	: client_count_(0)
	, listen_socket_(INVALID_SOCKET)
{
}

ClientManager::~ClientManager()
{
}

bool ClientManager::Start(WORD client_count, SOCKET listen_socket)
{
	if (client_count == 0 || listen_socket == INVALID_SOCKET)
		return false;

	client_count_ = client_count;
	listen_socket_ = listen_socket;

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

void ClientManager::OnAccepted(const void* packet)
{

}

void ClientManager::OnDisconnected(const void* packet)
{

}

void ClientManager::OnReceived(const void* packet, const DWORD bytes)
{

}

void ClientManager::OnSend(const void* packet, const DWORD bytes)
{

}
