#include "stdafx.h"
#include "inetwork_event.h"
#include "iocp.h"
#include "worker_thread.h"
#include "client_acceptor.h"

ClientAcceptor::ClientAcceptor()
	: socket_(INVALID_SOCKET)
	, port_(0)
	, network_event_(nullptr)
{
}

ClientAcceptor::~ClientAcceptor()
{
}

bool ClientAcceptor::Start(const wstring& ip, WORD port, const shared_ptr<iNetworkEvent>& network_event)
{
	ip_ = ip;
	port_ = port;

	if (Listen() == false)
		return false;

	network_event_ = network_event;

	byte thread_count = 2;
	if (WorkerThread::Start(thread_count) == false)
		return false;

	if (get_iocp()->Bind(reinterpret_cast<HANDLE>(socket_)) == false)
		return false;

	return true;
}

void ClientAcceptor::Stop()
{
	WorkerThread::Stop();

	if (socket_ != INVALID_SOCKET)
	{
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}
}

void ClientAcceptor::OnAccepted(const void* packet)
{
	if (packet == nullptr)
		return	;

	network_event_->OnAccepted(packet);
}

bool ClientAcceptor::Listen()
{
	if (socket_ != INVALID_SOCKET)
		return false;

	if (ip_.empty() == true || port_ == 0)
		return false;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket_ == INVALID_SOCKET)
		return false;

	SOCKADDR_IN	sock_addr;
	memset(&sock_addr, 0x00, sizeof(SOCKADDR_IN));

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(static_cast<short>(port_));
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	if (ip_.compare(L"0.0.0.0") != 0)
	{
		int sock_addr_size = sizeof(SOCKADDR_IN);
		if (WSAStringToAddressW(const_cast<LPWSTR>(ip_.c_str()), AF_INET, nullptr, reinterpret_cast<LPSOCKADDR>(&sock_addr), &sock_addr_size) != 0)
			return false;
	}

	if (::bind(socket_, reinterpret_cast<LPSOCKADDR>(&sock_addr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		return false;

	if (::listen(socket_, SOMAXCONN) == SOCKET_ERROR)
		return false;

	int so_reuse_addr = 1;
	setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&so_reuse_addr), sizeof(so_reuse_addr));

	return true;
}
