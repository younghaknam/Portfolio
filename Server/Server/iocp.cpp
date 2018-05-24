#include "stdafx.h"
#include "iocp.h"

Iocp::Iocp()
	: iocp_handle_(INVALID_HANDLE_VALUE)
{
}

Iocp::~Iocp()
{
	Close();
}

bool Iocp::Create()
{
	iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (iocp_handle_ == nullptr)
	{
		iocp_handle_ = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}

void Iocp::Close()
{
	if (iocp_handle_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(iocp_handle_);
		iocp_handle_ = INVALID_HANDLE_VALUE;
	}
}

bool Iocp::Bind(HANDLE handle) const
{
	ULONG_PTR completion_key = 0;
	HANDLE	ret;
	ret = CreateIoCompletionPort(handle, iocp_handle_, completion_key, 0);
	if (ret == nullptr)
		return false;

	return true;
}

bool Iocp::GetCompletionStatus(DWORD& number_of_bytes, OverlappedEx** ovelapped)
{
	PULONG_PTR completion_key = nullptr;
	return GetQueuedCompletionStatus(iocp_handle_, &number_of_bytes, completion_key, reinterpret_cast<OVERLAPPED**>(ovelapped), INFINITE);
}

bool Iocp::PostCompletionStatus(DWORD number_of_bytes, const OverlappedEx& ovelapped)
{
	ULONG_PTR completion_key = 0;
	return PostQueuedCompletionStatus(iocp_handle_, number_of_bytes, completion_key, const_cast<OverlappedEx*>(&ovelapped));
}
