#include "stdafx.h"
#include <Mswsock.h>
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "tcp_socket.h"

TcpSocket::TcpSocket()
	: socket_(INVALID_SOCKET)
	, port_(0)
{
}

TcpSocket::~TcpSocket()
{
	Disconnect();
}

bool TcpSocket::RequestAccept(SOCKET listen_socket, Packet* packet)
{
	if (socket_ != INVALID_SOCKET)
		return false;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket_ == INVALID_SOCKET)
		return false;

	packet->get_overlapped()->io_type = kIOAccept;

	DWORD	bytes = 0;
	bool result = AcceptEx(listen_socket, socket_, packet->get_memory(), kAddressBufferSize - ((sizeof(sockaddr_in) + 16) * 2),
								sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,	&bytes, packet->get_overlapped());
	if (result == false && WSAGetLastError() != ERROR_IO_PENDING)
	{
		//int err = GetLastError();
		Disconnect();
		return false;
	}
	
	return true;
}

bool TcpSocket::RequestRecv(Packet* packet)
{
	if (socket_ != INVALID_SOCKET)
		return false;

	packet->get_overlapped()->io_type = kIORecv;

	DWORD flag = 0;
	DWORD bytes = 0;
	int result = WSARecv(socket_, &packet->get_overlapped()->wsa_buf,  1, &bytes, &flag, packet->get_overlapped(), NULL);
	if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		//int err = GetLastError();
		Disconnect();
		return false;
	}

	return true;
}

bool TcpSocket::RequestSend(Packet* packet)
{
	if (socket_ != INVALID_SOCKET)
		return false;

	packet->get_overlapped()->io_type = kIOSend;

	DWORD bytes = 0;
	int result = WSASend(socket_, &packet->get_overlapped()->wsa_buf, 1, &bytes, 0, packet->get_overlapped(), NULL);
	if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		//int err = GetLastError();
		Disconnect();
		return false;
	}

	return true;
}

void TcpSocket::Disconnect()
{
	if (socket_ == INVALID_SOCKET)
		return;

	shutdown(socket_, SD_BOTH);
	struct linger	li = { 0, 0 };
	li.l_onoff = 1;
	setsockopt(socket_, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&li), sizeof(li));
	closesocket(socket_);
	socket_ = INVALID_SOCKET;
}
