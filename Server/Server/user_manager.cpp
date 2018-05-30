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

	for (WORD user_serial = 0; user_serial < user_count_; user_serial++)
	{
		auto user = shared_ptr<User>(new User());
		users_.push_back(user);
	}

	return true;
}

void UserManager::Stop()
{

}

bool UserManager::IsValidSerial(WORD user_serial)
{
	if (user_count_ <= user_serial)
		return false;

	return true;
}

shared_ptr<User>& UserManager::GetUser(WORD user_serial)
{
	return users_[user_serial];
}
