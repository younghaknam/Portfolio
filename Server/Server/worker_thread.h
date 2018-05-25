#pragma once

class Iocp;

class WorkerThread
{
public:
	WorkerThread();
	virtual ~WorkerThread();

protected:
	bool Start(byte thread_count = 0);
	void Stop();

	void RunWorker();
	virtual void OnAccepted(const void* object) = 0;
	virtual void OnDisconnected(const void* object) = 0;
	virtual void OnReceived(const void* object, const DWORD bytes) = 0;
	virtual void OnSend(const void* object, const DWORD bytes) = 0;

	const shared_ptr<Iocp>& get_iocp() { return iocp_;  }

private:
	shared_ptr<Iocp> iocp_;
	vector<shared_ptr<thread>> threads_;
	bool stop_;
};
