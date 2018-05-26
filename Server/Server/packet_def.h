#pragma once
#pragma pack(push, 1)

struct PacketHeader
{
	WORD data_size;
	WORD category;
	WORD packet_id;
};

const static WORD kPacketMemorySize = 65535;
const static WORD kPacketHeaderSize = sizeof(PacketHeader);
const static WORD kPacketDataMaxSize = kPacketMemorySize - kPacketHeaderSize;

#pragma pack(pop)
