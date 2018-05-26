#pragma once

class Packet
{
public:
	Packet();
	~Packet();

	void Initialize();
	void SetProtocolID(protocol::Category category, protocol::PacketID packet_id);
	bool SetData(const void* data, WORD size);
	bool GetData(void* data, WORD size);

	UINT8* get_memory() { return memory_; }
	void SetClientSerial(WORD serial) { overlapped_.client_serial = serial; }
	void SetRequestRecvSize() { overlapped_.wsa_buf.len = kPacketMemorySize; }
	const PacketHeader* get_header() { return header_; }
	OverlappedEx* get_overlapped() { return &overlapped_; }

private:
	UINT8 memory_[kPacketMemorySize];
	PacketHeader* header_;
	UINT8* data_;
	OverlappedEx overlapped_;
};
