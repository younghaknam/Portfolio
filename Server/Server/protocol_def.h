#pragma once

namespace protocol
{
	typedef WORD ID;

	enum  Category : WORD
	{
		kEngineIO,
		kUser,
		kMax,
	};

	namespace engine_io
	{
		enum ID : WORD
		{
			kIO2Content_Connected,
			kIO2Content_Disconnected,
			kMax,
		};
	};

	namespace user
	{
		enum ID : WORD
		{
			kC2S_LoginReq,
			kS2C_LoginAns,
			kC2S_Logout,
			kMax,
		};
	};

};

const static int kUserNameMaxSize = 256;

// Protocol struct
#pragma pack(push, 1)

struct C2SLoginReq
{
	DWORD user_id;
	WCHAR user_name[kUserNameMaxSize + 1];

	void Initialize() { memset(this, 0x00, sizeof(C2SLoginReq)); }
};

struct S2CLoginAns
{
	byte result;
};

#pragma pack(pop)
