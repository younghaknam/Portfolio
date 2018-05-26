#pragma once

class Iocp;

class WorkerThread
{
public:
	WorkerThread();
	virtual ~WorkerThread();

	const shared_ptr<Iocp>& get_iocp() { return iocp_; }

protected:
	bool Start(byte thread_count = 0);
	void Stop();

	void RunWorker();
	virtual void OnAccepted(const void* packet) = 0;
	virtual void OnDisconnected(const void* packet) = 0;
	virtual void OnReceived(const void* packet, const DWORD bytes) = 0;
	virtual void OnSend(const void* packet, const DWORD bytes) = 0;

private:
	shared_ptr<Iocp> iocp_;
	vector<shared_ptr<thread>> threads_;
	bool stop_;
};
