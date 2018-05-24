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

bool TcpSocket::RequestAccept(SOCKET listen_socket)
{
	if (socket_ != INVALID_SOCKET)
		return false;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket_ == INVALID_SOCKET)
		return false;


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
