#pragma once

class WorkerThread;
class iNetworkEvent;

class ClientAcceptor : public WorkerThread
{
public:
	ClientAcceptor();
	virtual ~ClientAcceptor();

	bool Start(const wstring& ip, WORD port, const shared_ptr<iNetworkEvent>& network_event);
	void Stop();

	const SOCKET get_socket() { return socket_; }

protected:
	virtual void OnAccepted(const void* packet);
	virtual void OnDisconnected(const void* packet) { if (packet) return; }
	virtual void OnReceived(const void* packet, const DWORD bytes) { if (packet || bytes) return; }
	virtual void OnSend(const void* packet, const DWORD bytes) { if (packet || bytes) return; }

private:
	bool Listen();

private:
	SOCKET socket_;
	wstring ip_;
	WORD port_;
	shared_ptr<iNetworkEvent> network_event_;
};
