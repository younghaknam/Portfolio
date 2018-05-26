#pragma once

class TcpSocket;

class Client
{
public:
	explicit Client(WORD serial);
	~Client();

	bool RequestAccept(SOCKET listen_socket);
	TcpSocket& get_tcp_socket() { return tcp_socket_; }

private:
	WORD serial_;
	TcpSocket tcp_socket_;
};
