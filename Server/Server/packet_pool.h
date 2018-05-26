#pragma once

class Packet;

class PacketPool
{
public:
	PacketPool();
	~PacketPool();

	void Initialize(int default_count);
	void AddPacket(const shared_ptr<Packet>& packet);
	shared_ptr<Packet> GetPacket();

private:
	queue<shared_ptr<Packet>> pool_;
	mutex lock_;
};
