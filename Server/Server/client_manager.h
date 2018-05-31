#pragma once

class iRequestIO;
class iCompletedIO;
class iContent;
class Client;
class Iocp;

class ClientManager : public iRequestIO, public iCompletedIO
{
public:
	ClientManager();
	~ClientManager();

	void Initialize(SOCKET listen_socket, const shared_ptr<Iocp>& io_iocp, const shared_ptr<iContent>& content);
	bool Start(WORD client_count);
	void Stop();

	virtual bool RequestAccept(WORD client_serial);
	virtual bool RequestDisconnect(WORD client_serial);
	virtual bool RequestSend(const void* packet);

	virtual void OnAccepted(const void* packet);
	virtual void OnDisconnected(const void* packet);
	virtual void OnReceived(const void* packet, const DWORD bytes);
	virtual void OnSent(const void* packet, const DWORD bytes);

private:
	bool GetClient(WORD serial, shared_ptr<Client>& client);

private:
	WORD client_count_;
	SOCKET listen_socket_;
	shared_ptr<Iocp> io_iocp_;
	vector<shared_ptr<Client>> clients_;
	shared_ptr<iContent> content_;
};
