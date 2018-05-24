#include "stdafx.h"
#include "tcp_socket.h"

TcpSocket::TcpSocket()
	: socket_(INVALID_SOCKET)
	, port_(0)
{
}

TcpSocket::~TcpSocket()
{
	Close();
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
