#include "stdafx.h"
#include "singleton.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"
#include "packet_storage.h"

PacketStorage::PacketStorage()
{
}

PacketStorage::~PacketStorage()
{
}

void PacketStorage::Create(int create_count)
{
	packet_pool_ = shared_ptr<PacketPool>(new PacketPool);
	packet_pool_->Initialize(create_count);
}

void PacketStorage::Release()
{
	packet_pool_.reset();
}

void PacketStorage::AddPacket(const Packet* packet)
{
	packet_pool_->AddPacket(packet);
}

Packet* PacketStorage::GetPacket()
{
	return packet_pool_->GetPacket();
}
