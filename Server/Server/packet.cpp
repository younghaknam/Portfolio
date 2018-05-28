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
	client_serial_ = 0;

	memset(memory_, 0x00, kPacketMemorySize);
	io_bytes_ = 0;

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
	overlapped_.wsa_buf.len = GetSize();

	return true;
}

bool Packet::GetData(void* data, WORD size)
{
	if (size > kPacketDataMaxSize)
		return false;

	memcpy(data, data_, size);
	return true;
}

WORD Packet::GetSize()
{
	return kPacketHeaderSize + header_->data_size;
}

bool Packet::IsCompleted()
{
	if (io_bytes_ <= kPacketHeaderSize)
		return false;

	if (io_bytes_ < GetSize())
		return false;

	return true;
}

int Packet::GetCompletedCount()
{
	int packet_count = 0;
	WORD io_bytes = io_bytes_;
	UINT8* memory = memory_;
	PacketHeader* header = nullptr;

	while (true)
	{
		if (io_bytes <= kPacketHeaderSize)
			break;

		header = reinterpret_cast<PacketHeader*>(memory);
		memory += kPacketHeaderSize;
		io_bytes -= kPacketHeaderSize;

		if (io_bytes < header->data_size)
			break;

		memory += header->data_size;
		io_bytes -= header->data_size;

		packet_count++;
	}

	return packet_count;
}

bool Packet::Split(Packet* packet)
{
	if (IsCompleted() == false)
		return false;

	WORD split_size = GetSize();

	packet->Initialize();
	packet->set_client_serial(client_serial_);
	packet->set_io_bytes(split_size);
	memcpy(packet->get_memory(), memory_, split_size);

	io_bytes_ -= split_size;
	memcpy(memory_, (memory_ + split_size), io_bytes_);

	return true;
}

void Packet::SetReceivedBytes()
{
	overlapped_.wsa_buf.buf = reinterpret_cast<char*>(memory_ + io_bytes_);
	overlapped_.wsa_buf.len = kPacketMemorySize - io_bytes_;
}

void Packet::SetSentBytes()
{
	overlapped_.wsa_buf.buf = reinterpret_cast<char*>(memory_ + io_bytes_);
	overlapped_.wsa_buf.len = GetSize() - io_bytes_;
}
