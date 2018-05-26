#include "stdafx.h"
#include "inetwork_event.h"
#include "iocp.h"
#include "worker_thread.h"
#include "client_io_worker.h"

ClientIOWorker::ClientIOWorker()
{
}

ClientIOWorker::~ClientIOWorker()
{
}

bool ClientIOWorker::Start(const shared_ptr<iNetworkEvent>& network_event)
{
	network_event_ = network_event;

	byte thread_count = 0;
	if (WorkerThread::Start(thread_count) == false)
		return false;

	return true;
}

void ClientIOWorker::Stop()
{
	WorkerThread::Stop();
}

bool ClientIOWorker::Bind(SOCKET socket)
{
	return get_iocp()->Bind(reinterpret_cast<HANDLE>(socket));
}

void ClientIOWorker::OnDisconnected(const void* packet)
{

}

void ClientIOWorker::OnReceived(const void* packet, const DWORD bytes)
{

}

void ClientIOWorker::OnSend(const void* packet, const DWORD bytes)
{

}
