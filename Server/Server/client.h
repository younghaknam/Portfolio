#pragma once

class TcpSocket;

class Client
{
public:
	explicit Client(WORD serial);
	~Client();

	bool RequestAccept(SOCKET listen_socket);
	TcpSocket& get_socket() { return socket_; }

private:
	WORD serial_;
	TcpSocket socket_;
};
