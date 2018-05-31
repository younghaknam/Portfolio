#pragma once

class WorkerThread;
class Packet;
class iContent;
class iProtocol;

class ContentWorker : public WorkerThread, public iContent
{
public:
	ContentWorker();
	~ContentWorker();

	bool Start(const shared_ptr<iProtocol>& protocol);
	void Stop();

	virtual bool AddPacket(const Packet* packet);
private:
	void Dispatch(const Packet* packet);

protected:
	virtual void OnAccepted(const void* packet);
	virtual void OnDisconnected(const void* packet);
	virtual void OnReceived(const void* packet, const DWORD bytes);
	virtual void OnSent(const void* packet, const DWORD bytes);

private:
	shared_ptr<iProtocol> protocol_;

};
