#pragma once

class iNetworkEvent;
class Client;

class ClientManager : public iNetworkEvent
{
public:
	ClientManager();
	~ClientManager();

	bool Start(WORD client_count, SOCKET listen_socket);
	void Stop();

	virtual void OnAccepted(const void* packet);
	virtual void OnDisconnected(const void* packet);
	virtual void OnReceived(const void* packet, const DWORD bytes);
	virtual void OnSend(const void* packet, const DWORD bytes);

private:
	WORD client_count_;
	SOCKET listen_socket_;
	vector<shared_ptr<Client>> clients_;
};

