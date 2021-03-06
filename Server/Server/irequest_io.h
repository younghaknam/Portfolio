#pragma once

class iRequestIO
{
public:
	iRequestIO() {}
	virtual ~iRequestIO() {}

	virtual bool RequestAccept(WORD client_serial) = 0;
	virtual bool RequestDisconnect(WORD client_serial) = 0;
	virtual bool RequestSend(const void* packet) = 0;
};
