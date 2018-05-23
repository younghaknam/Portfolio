#pragma once

#include "iocp.h"

class WorkerThread : public Iocp
{
public:
	WorkerThread();
	~WorkerThread();

	bool Start(byte thread_count = 0);
	void Stop();

	void RunWorker();
	virtual void OnAccept(const void* object) = 0;
	virtual void OnConnect(const void* object) = 0;
	virtual void OnRecv(const void* object, const DWORD bytes) = 0;
	virtual void OnSend(const void* object, const DWORD bytes) = 0;

private:
	vector<shared_ptr<thread>> threads_;
};
