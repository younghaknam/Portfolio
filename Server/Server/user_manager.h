#pragma once

class iRequestIO;
class User;

class UserManager : public Singleton<UserManager>
{
public:
	UserManager();
	~UserManager();

	bool Start(WORD user_count, const shared_ptr<iRequestIO>& request_io);
	void Stop();
	bool IsValidSerial(WORD user_serial);
	shared_ptr<User>& GetUser(WORD user_serial);

private:
	WORD user_count_;
	vector<shared_ptr<User>> users_;
	shared_ptr<iRequestIO> request_io_;
};
