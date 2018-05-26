#include "stdafx.h"
#include "iocp.h"
#include "worker_thread.h"

WorkerThread::WorkerThread()
	: stop_(false)
{
	iocp_ = shared_ptr<Iocp>(new Iocp);
}

WorkerThread::~WorkerThread()
{
}

bool WorkerThread::Start(byte thread_count)
{
	if (iocp_->Create() == false)
		return false;

	stop_ = false;

	if (thread_count == 0)
	{
		unsigned int cpu_core = thread::hardware_concurrency();
		thread_count = static_cast<byte>(cpu_core * 2 + 1);
	}

	for (byte cnt = 0; cnt < thread_count; cnt++)
	{
		auto thread_ptr = shared_ptr<thread>(new thread( [&](){ RunWorker(); } ));
		threads_.emplace_back(thread_ptr);
	}

	return true;
}

void WorkerThread::Stop()
{
	if (threads_.size() == 0)
		return;

	stop_ = true;

	iocp_->Close();

	for (auto thread_ptr : threads_)
	{
		if (thread_ptr->joinable())
			thread_ptr->join();
	}

	threads_.clear();
}

void WorkerThread::RunWorker()
{
	DWORD bytes = 0;
	OverlappedEx* ovelapped = nullptr;
	bool result = false;

	while (true)
	{
		bytes = 0;
		ovelapped = nullptr;
		result = iocp_->GetCompletionStatus(bytes, &ovelapped);

		if (stop_ == true)
			break;

		if (ovelapped == nullptr || ovelapped->packet == nullptr)
			continue;

		if (result == false)
		{
			OnDisconnected(ovelapped->packet);
			continue;
		}

		if (bytes == 0)
		{
			if (ovelapped->io_type == kIOAccept)
				OnAccepted(ovelapped->packet);
			else
				OnDisconnected(ovelapped->packet);

			continue;
		}

		if (ovelapped->io_type == kIORecv)
			OnReceived(ovelapped->packet, bytes);
		else if (ovelapped->io_type == kIOSend)
			OnSend(ovelapped->packet, bytes);
		else
			OnDisconnected(ovelapped->packet);
	}
}
