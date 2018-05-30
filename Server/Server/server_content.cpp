#include "stdafx.h"
#include "irequest_io.h"
#include "icontent.h"
#include "iocp.h"
#include "worker_thread.h"
#include "content_worker.h"
#include "server_content.h"

ServerContent::ServerContent()
{
	content_worker_ = shared_ptr<ContentWorker>(new ContentWorker);
}

ServerContent::~ServerContent()
{
}

bool ServerContent::Start(const shared_ptr<iRequestIO>& request_io)
{
	request_io_ = request_io;

	if (content_worker_->Start() == false)
		return false;

	return true;
}

void ServerContent::Stop()
{
	if (content_worker_)
		content_worker_->Stop();
}

shared_ptr<iContent>ServerContent::GetiContent()
{
	return dynamic_pointer_cast<iContent>(content_worker_);
}
