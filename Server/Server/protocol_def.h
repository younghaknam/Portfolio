#pragma once

namespace protocol
{
	typedef WORD ID;

	enum  Category : WORD
	{
		kNetworkIO,
		kMax,
	};

	namespace network_io
	{
		enum ID : WORD
		{
			kIO2Content_Connected,
			kIO2Content_Disconnected,
			kMax,
		};
	};

};

// Protocol struct
#pragma pack(push, 1)




#pragma pack(pop)
