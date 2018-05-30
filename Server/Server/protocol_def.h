#pragma once

namespace protocol
{
	enum PacketID : WORD
	{
		kPacketIDNone = 0,
		kPacketIDMax,
	};

	enum  Category : WORD
	{
		kNetworkIO,
		kMax,
	};

	namespace network_io
	{
		enum ID : WORD
		{
			kIO2Con_Connect,
			kIO2Con_Disconnect,
			kMax,
		};
	};

};

// Protocol struct
#pragma pack(push, 1)




#pragma pack(pop)
