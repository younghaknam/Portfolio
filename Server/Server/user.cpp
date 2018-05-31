#include "stdafx.h"
#include "irequest_io.h"
#include "user.h"

User::User(WORD client_serial, const shared_ptr<iRequestIO>& request_io)
	: client_serial_(client_serial)
	, request_io_(request_io)
	, is_connected_(false)
{
}

User::~User()
{
}

bool User::Reset()
{
	is_connected_ = false;
	return request_io_->RequestAccept(client_serial_);
}

bool User::Disconnect()
{
	return request_io_->RequestDisconnect(client_serial_);
}

void User::Disconnected()
{
	is_connected_ = true;

	
}
