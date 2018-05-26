#pragma once

class iNetworkEvent
{
public:
	iNetworkEvent() {}
	virtual ~iNetworkEvent() {}

	virtual void OnAccepted(const void* packet) = 0;
	virtual void OnDisconnected(const void* packet) = 0;
	virtual void OnReceived(const void* packet, const DWORD bytes) = 0;
	virtual void OnSend(const void* packet, const DWORD bytes) = 0;
};
