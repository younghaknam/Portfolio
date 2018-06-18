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

	bool AddPacket(const Packet* packet);
private:
	void Dispatch(const Packet* packet);

protected:
	void OnAccepted(const void* packet);
	void OnDisconnected(const void* packet);
	void OnReceived(const void* packet, const DWORD bytes);
	void OnSent(const void* packet, const DWORD bytes);

private:
	shared_ptr<iProtocol> protocol_;

};
