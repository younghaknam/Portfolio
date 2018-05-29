#include "stdafx.h"
#include "icompleted_io.h"
#include "iocp.h"
#include "worker_thread.h"
#include "client_io_worker.h"

ClientIOWorker::ClientIOWorker()
{
}

ClientIOWorker::~ClientIOWorker()
{
}

bool ClientIOWorker::Start(const shared_ptr<iCompletedIO>& completed_io)
{
	completed_io_ = completed_io;

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
	completed_io_->OnDisconnected(packet);
}

void ClientIOWorker::OnReceived(const void* packet, const DWORD bytes)
{
	completed_io_->OnReceived(packet, bytes);
}

void ClientIOWorker::OnSent(const void* packet, const DWORD bytes)
{
	completed_io_->OnSent(packet, bytes);
}
