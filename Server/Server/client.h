#pragma once

class TcpSocket;

class Client
{
public:
	explicit Client(WORD serial);
	~Client();

	bool RequestAccept(SOCKET listen_socket);
	bool RequestDisconnect();
	bool RequestRecv();
	bool RequestSend();

	void Accepted(Packet* packet);
	void Disconnected(Packet* packet);
	void Received(Packet* packet, DWORD bytes);
	void Sent(Packet* packet, DWORD bytes);

	TcpSocket& get_tcp_socket() { return tcp_socket_; }

private:
	Packet* GetPacket();

private:
	WORD serial_;
	TcpSocket tcp_socket_;
};
