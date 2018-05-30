#pragma once

namespace protocol
{
	enum  Category : WORD
	{
		kCategoryNone = 0,
		kCategoryMax = 65535,
	};

	enum PacketID : WORD
	{
		kPacketIDNone = 0,
		kPacketIDMax = 65535,
	};
};

// Protocol struct
#pragma pack(push, 1)




#pragma pack(pop)
