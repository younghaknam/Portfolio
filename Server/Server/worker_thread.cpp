#include "stdafx.h"
#include "worker_thread.h"

WorkerThread::WorkerThread()
	: stop_(false)
{
}

WorkerThread::~WorkerThread()
{
}

bool WorkerThread::Start(byte thread_count)
{
	if (Iocp::Create() == false)
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

	Iocp::Close();

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
		result = Iocp::GetCompletionStatus(bytes, &ovelapped);

		if (stop_ == true)
			break;

		if (ovelapped == nullptr || ovelapped->object == nullptr)
			continue;

		if (result == false)
		{
			OnDisconnected(ovelapped->object);
			continue;
		}

		if (bytes == 0)
		{
			if (ovelapped->io_type == kIOAccept)
				OnAccept(ovelapped->object);
			else
				OnDisconnected(ovelapped->object);

			continue;
		}

		if (ovelapped->io_type == kIORecv)
			OnRecv(ovelapped->object, bytes);
		else if (ovelapped->io_type == kIOSend)
			OnSend(ovelapped->object, bytes);
		else
			OnDisconnected(ovelapped->object);
	}
}
