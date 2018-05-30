#include "stdafx.h"
#include "icontent.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "worker_thread.h"
#include "content_worker.h"

ContentWorker::ContentWorker()
{
}

ContentWorker::~ContentWorker()
{
}

bool ContentWorker::Start()
{
	byte thread_count = 1;
	if (WorkerThread::Start(thread_count) == false)
		return false;

	return true;
}

void ContentWorker::Stop()
{
	WorkerThread::Stop();
}

bool ContentWorker::AddPacket(const Packet* packet)
{
	Packet* packet_ptr = const_cast<Packet*>(packet);
	return get_iocp()->PostCompletionStatus(packet_ptr->get_io_bytes(), packet_ptr->get_overlapped());
}

void ContentWorker::OnReceived(const void* packet, const DWORD bytes)
{

}