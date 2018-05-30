#pragma once

class iRequestIO;
class iContent;
class ContentWorker;

class ServerContent
{
public:
	ServerContent();
	~ServerContent();

	bool Start(const shared_ptr<iRequestIO>& request_io);
	void Stop();
	shared_ptr<iContent> GetiContent();

private:
	shared_ptr<ContentWorker> content_worker_;
	shared_ptr<iRequestIO> request_io_;
};

