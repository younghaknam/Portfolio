#include "stdafx.h"
#include "singleton.h"
#include "irequest_io.h"
#include "icontent.h"
#include "iprotocol.h"
#include "iocp.h"
#include "worker_thread.h"
#include "content_worker.h"
#include "protocol_handling.h"
#include "user_manager.h"
#include "server_content.h"

ServerContent::ServerContent()
{
	content_worker_ = shared_ptr<ContentWorker>(new ContentWorker);
	protocol_handling_ = shared_ptr<ProtocolHandling>(new ProtocolHandling);
}

ServerContent::~ServerContent()
{
}

bool ServerContent::Start(WORD max_user, const shared_ptr<iRequestIO>& request_io)
{
	request_io_ = request_io;

	if (protocol_handling_->Start() == false)
		return false;

	if (content_worker_->Start(dynamic_pointer_cast<iProtocol>(protocol_handling_)) == false)
		return false;

	if (UserManager::GetSingleton()->Start(max_user, request_io_) == false)
		return false;

	return true;
}

void ServerContent::Stop()
{
	if (content_worker_)
		content_worker_->Stop();

	if (protocol_handling_)
		protocol_handling_->Stop();
}

shared_ptr<iContent>ServerContent::GetiContent()
{
	return dynamic_pointer_cast<iContent>(content_worker_);
}
