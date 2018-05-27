#pragma once

class Packet;

class PacketPool
{
public:
	PacketPool();
	~PacketPool();

	void Initialize(int default_count);
	void AddPacket(const Packet* packet);
	Packet* GetPacket();

private:
	queue<Packet*> pool_;
	mutex lock_;
};
