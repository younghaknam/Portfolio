#include "stdafx.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"

Packet::Packet()
{
	header_ = reinterpret_cast<PacketHeader*>(memory_);
	data_ = memory_ + kPacketHeaderSize;

	Initialize();
}

Packet::~Packet()
{
}

void Packet::Initialize()
{
	memset(memory_, 0x00, kPacketMemorySize);

	overlapped_.Initialize();
	overlapped_.wsa_buf.buf = reinterpret_cast<char*>(memory_);
	overlapped_.wsa_buf.len = kPacketMemorySize;
	overlapped_.packet = this;
}

void Packet::SetProtocolID(protocol::Category category, protocol::PacketID packet_id)
{
	header_->category = category;
	header_->packet_id = packet_id;
}

bool Packet::SetData(const void* data, WORD size)
{
	if (size > kPacketDataMaxSize)
		return false;

	memcpy(data_, data, size);
	header_->data_size = size;

	overlapped_.wsa_buf.len = kPacketHeaderSize + size;

	return true;
}

bool Packet::GetData(void* data, WORD size)
{
	if (size > kPacketDataMaxSize)
		return false;

	memcpy(data, data_, size);
	return true;
}
