#pragma once

enum IoType : byte
{
	kIOAccept = 1,
	kIOConnect = 2,
	kIOSend = 3,
	kIORecv = 4,
};

typedef struct _OverlappedEx : WSAOVERLAPPED
{
	WSABUF wsa_buf;
	IoType io_type;
	void* object;

	void Initialize() { memset(this, 0x00, sizeof(OverlappedEx)); }
} OverlappedEx;

class Iocp
{
public:
	Iocp();
	~Iocp();

	bool Register(HANDLE handle) const;
	bool PostCompletionStatus(DWORD number_of_bytes, const OverlappedEx& ovelapped);

protected:
	bool Create();
	void Close();
	bool GetCompletionStatus(DWORD& number_of_bytes, OverlappedEx& ovelapped);

private:
	HANDLE iocp_handle_;
};
