#include "stdafx.h"
#include <Mswsock.h>
#include "iocp.h"
#include "tcp_socket.h"

TcpSocket::TcpSocket()
	: socket_(INVALID_SOCKET)
	, port_(0)
{
	memset(address_buffer_, 0x00, kAddressBufferSize);
	accept_overlapped_.Initialize();
}

TcpSocket::~TcpSocket()
{
	Close();
}

bool TcpSocket::RequestAccept(SOCKET listen_socket)
{
	if (socket_ != INVALID_SOCKET)
		return false;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket_ == INVALID_SOCKET)
		return false;

	accept_overlapped_.Initialize();
	accept_overlapped_.io_type = kIOAccept;
	accept_overlapped_.packet = this;

	memset(address_buffer_, 0x00, kAddressBufferSize);
	DWORD	bytes = 0;
	bool result = AcceptEx(listen_socket, socket_, address_buffer_, kAddressBufferSize - ((sizeof(sockaddr_in) + 16) * 2),
								sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,	&bytes, &accept_overlapped_);
	if (result == false)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			//int err = GetLastError();
			closesocket(socket_);
			socket_ = INVALID_SOCKET;
			return false;
		}
	}
	
	return true;
}

void TcpSocket::Close()
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
