#pragma once

class WorkerThread;
class iNetworkEvent;

class ClientIOWorker : public WorkerThread
{
public:
	ClientIOWorker();
	~ClientIOWorker();

	bool Start(const shared_ptr<iNetworkEvent>& network_event);
	void Stop();
	bool Bind(SOCKET socket);

protected:
	virtual void OnAccepted(const void* packet) { if (packet) return; };
	virtual void OnDisconnected(const void* packet);
	virtual void OnReceived(const void* packet, const DWORD bytes);
	virtual void OnSend(const void* packet, const DWORD bytes);

private:
	shared_ptr<iNetworkEvent> network_event_;
};

