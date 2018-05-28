#include "stdafx.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"

PacketPool::PacketPool()
{
}


PacketPool::~PacketPool()
{
}

void PacketPool::Initialize(int default_count)
{
	if (default_count == 0)
		return;

	for (int cnt = 0; cnt < default_count; cnt++)
	{
		auto packet = new Packet;
		pool_.emplace(packet);
	}
}

void PacketPool::AddPacket(const Packet* packet)
{
	lock_guard<mutex> guard(lock_);
	pool_.emplace(const_cast<Packet*>(packet));
}

Packet* PacketPool::GetPacket()
{
	Packet* packet;
	lock_guard<mutex> guard(lock_);

	if (pool_.empty() == true)
	{
		packet = new Packet;
		return packet;
	}

	packet = pool_.front();
	pool_.pop();

	return packet;
}

bool PacketPool::Empty()
{
	lock_guard<mutex> guard(lock_);
	return pool_.empty();
}