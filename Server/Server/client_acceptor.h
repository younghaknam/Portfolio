#pragma once

class WorkerThread;
class TcpSocket;

class ClientAcceptor : public WorkerThread
{
public:
	ClientAcceptor();
	virtual ~ClientAcceptor();

	bool Start(const wstring& ip, WORD port, int acceptor_count = 5);
	void Stop();

private:
	bool Listen();

protected:
	virtual void OnAccepted(const void* object);
	virtual void OnDisconnected(const void* object) { if (object) return; }
	virtual void OnReceived(const void* object, const DWORD bytes) { if (object || bytes) return; }
	virtual void OnSend(const void* object, const DWORD bytes) { if (object || bytes) return; }

private:
	SOCKET socket_;
	wstring ip_;
	WORD port_;
	int acceptor_count_;
	vector<shared_ptr<TcpSocket>> sockets_;
};
