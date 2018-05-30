#pragma once

class WorkerThread;
class Packet;
class iContent;

class ContentWorker : public WorkerThread, public iContent
{
public:
	ContentWorker();
	~ContentWorker();

	bool Start();
	void Stop();

	virtual bool AddPacket(const Packet* packet);

protected:
	virtual void OnAccepted(const void* packet) { if (packet) return; };
	virtual void OnDisconnected(const void* packet) { if (packet) return; };
	virtual void OnReceived(const void* packet, const DWORD bytes);
	virtual void OnSent(const void* packet, const DWORD bytes) { if (packet || bytes) return; };
};
