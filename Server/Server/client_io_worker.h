#pragma once

class WorkerThread;
class iCompletedIO;

class ClientIOWorker : public WorkerThread
{
public:
	ClientIOWorker();
	~ClientIOWorker();

	bool Start(const shared_ptr<iCompletedIO>& completed_io);
	void Stop();
	bool Bind(SOCKET socket);

protected:
	virtual void OnAccepted(const void* packet) { if (packet) return; };
	virtual void OnDisconnected(const void* packet);
	virtual void OnReceived(const void* packet, const DWORD bytes);
	virtual void OnSent(const void* packet, const DWORD bytes);

private:
	shared_ptr<iCompletedIO> completed_io_;
};

