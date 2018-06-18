#pragma once

class iCompletedIO
{
public:
	iCompletedIO() {}
	virtual ~iCompletedIO() {}

	virtual void Accepted(const void* packet) = 0;
	virtual void Disconnected(const void* packet) = 0;
	virtual void Received(const void* packet, const DWORD bytes) = 0;
	virtual void Sent(const void* packet, const DWORD bytes) = 0;
};
