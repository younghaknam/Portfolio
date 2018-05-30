#pragma once

class Packet;

class iContent
{
public:
	iContent() {}
	virtual ~iContent() {}

	virtual bool AddPacket(const Packet* packet) = 0;
};
