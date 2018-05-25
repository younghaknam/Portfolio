#pragma once

class WorkerThread;

class ClientAcceptor : public WorkerThread
{
public:
	ClientAcceptor();
	virtual ~ClientAcceptor();

	bool Start(const wstring& ip, WORD port);
	void Stop();

	const SOCKET get_socket() { return socket_; }

protected:
	virtual void OnAccepted(const void* object);
	virtual void OnDisconnected(const void* object) { if (object) return; }
	virtual void OnReceived(const void* object, const DWORD bytes) { if (object || bytes) return; }
	virtual void OnSend(const void* object, const DWORD bytes) { if (object || bytes) return; }

private:
	bool Listen();

private:
	SOCKET socket_;
	wstring ip_;
	WORD port_;
};
