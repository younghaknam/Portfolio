#pragma once

const static int kAddressBufferSize = 1024;

class Packet;

class TcpSocket
{
public:
	TcpSocket();
	~TcpSocket();

	bool RequestAccept(SOCKET listen_socket, Packet* packet);
	bool RequestReceiv(Packet* packet);
	bool RequestSend(Packet* packet);
	void Disconnect();

	void set_socket(SOCKET socket) { socket_ = socket; }
	SOCKET get_socket() { return socket_;  }
	void set_ip(const wstring& ip) { ip_ = ip;  }
	const wstring& get_ip() { return ip_;  }
	void set_port(WORD port) { port_ = port;  }
	WORD get_port() { return port_; }
	WORD get_io_count() { return io_count_; }
	void decrement_io_count() { io_count_--; }

private:
	SOCKET socket_;
	wstring ip_;
	WORD port_;
	atomic<WORD> io_count_;
};
