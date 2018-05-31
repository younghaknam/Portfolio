#include "stdafx.h"
#include "irequest_io.h"
#include "singleton.h"
#include "user.h"
#include "user_manager.h"

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}

bool UserManager::Start(WORD user_count, const shared_ptr<iRequestIO>& request_io)
{
	if (user_count == 0 || request_io == nullptr)
		return false;

	user_count_ = user_count;
	request_io_ = request_io;

	for (WORD client_serial = 0; client_serial < user_count_; client_serial++)
	{
		auto user = shared_ptr<User>(new User(client_serial, request_io_));
		user->Reset();

		users_.push_back(user);
	}

	return true;
}

void UserManager::Stop()
{
	for (auto user : users_)
	{
		user->Disconnect();
	}
}

bool UserManager::IsValidSerial(WORD client_serial)
{
	if (user_count_ <= client_serial)
		return false;

	return true;
}

shared_ptr<User>& UserManager::GetUser(WORD client_serial)
{
	return users_[client_serial];
}
