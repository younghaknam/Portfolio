#pragma once

class TcpSocket;
class PacketPool;
class iContent;

class Client
{
public:
	explicit Client(WORD serial, SOCKET listen_socket, const shared_ptr<iContent>& content);
	~Client();

	bool RequestAccept();
	bool RequestDisconnect();
	bool RequestReceiv();
	bool RequestSend(const Packet* packet);

	void Accepted(Packet* packet);
	void Disconnected(Packet* packet);
	void Received(Packet* packet, DWORD bytes);
	void Sent(Packet* packet, DWORD bytes);

	TcpSocket& get_tcp_socket() { return tcp_socket_; }

private:
	Packet* GetPacket();
	void ReturnSendPackets();

private:
	WORD serial_;
	SOCKET listen_socket_;
	TcpSocket tcp_socket_;
	bool sending_;
	shared_ptr<PacketPool> send_pool_;
	mutex send_lock_;
	mutex disconnect_lock_;
	shared_ptr<iContent> content_;
};
