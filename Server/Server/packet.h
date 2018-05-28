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
	bool IsCompleted();
	int GetCompletedCount();
	bool Split(Packet* packet);
	void SetCurrentReceivedBytes();

	UINT8* get_memory() { return memory_; }
	const PacketHeader* get_header() { return header_; }
	void set_io_bytes(WORD bytes) { io_bytes_ += bytes; }
	WORD get_io_bytes() { return io_bytes_; }
	OverlappedEx* get_overlapped() { return &overlapped_; }
	void set_client_serial(WORD serial) { client_serial_ = serial; }
	WORD get_client_serial() { return client_serial_; }

private:
	UINT8 memory_[kPacketMemorySize];
	PacketHeader* header_;
	UINT8* data_;
	WORD io_bytes_;
	OverlappedEx overlapped_;
	WORD client_serial_;
};
