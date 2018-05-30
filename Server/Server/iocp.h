#pragma once

enum IoType : byte
{
	kIONone = 0,
	kIOAccept = 1,
	kIOConnect = 2,
	kIOSend = 3,
	kIORecv = 4,
};

struct OverlappedEx : WSAOVERLAPPED
{
	WSABUF wsa_buf;
	IoType io_type;
	void* packet;

	void Initialize() { memset(this, 0x00, sizeof(OverlappedEx)); }
};

class Iocp
{
public:
	Iocp();
	~Iocp();

	bool Create();
	void Close();
	bool Bind(HANDLE handle) const;
	bool GetCompletionStatus(DWORD& number_of_bytes, OverlappedEx** ovelapped);
	bool PostCompletionStatus(DWORD number_of_bytes, const OverlappedEx* ovelapped);

private:
	HANDLE iocp_handle_;
};
