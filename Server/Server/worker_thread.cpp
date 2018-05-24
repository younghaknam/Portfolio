#include "stdafx.h"
#include "worker_thread.h"

WorkerThread::WorkerThread()
{
}

WorkerThread::~WorkerThread()
{
}

bool WorkerThread::Start(byte thread_count)
{
	if (Iocp::Create() == false)
		return false;

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
	bool result = false;
	DWORD bytes = 0;
	OverlappedEx ovelapped;

	while (true)
	{
		bytes = 0;
		ovelapped.Initialize();

		result = Iocp::GetCompletionStatus(bytes, ovelapped);


		
	}
}
