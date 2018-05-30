#pragma once

class Packet;

class iProtocol
{
public:
	iProtocol() {}
	virtual ~iProtocol() {}

	virtual void Dispatch(const Packet* packet) = 0;
};
