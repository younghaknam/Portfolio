#pragma once

class PacketPool;

class PacketStorage : public Singleton<PacketStorage>
{
public:
	PacketStorage();
	~PacketStorage();

	void Create(int create_count);
	void Release();
	void AddPacket(const shared_ptr<Packet>& packet);
	shared_ptr<Packet> GetPacket();


private:
	shared_ptr<PacketPool> packet_pool_;
};

