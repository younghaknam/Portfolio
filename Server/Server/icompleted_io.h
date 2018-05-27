#pragma once

class iCompletedIO
{
public:
	iCompletedIO() {}
	virtual ~iCompletedIO() {}

	virtual void OnAccepted(const void* packet) = 0;
	virtual void OnDisconnected(const void* packet) = 0;
	virtual void OnReceived(const void* packet, const DWORD bytes) = 0;
	virtual void OnSent(const void* packet, const DWORD bytes) = 0;
};
