#include "stdafx.h"
#include "irequest_io.h"
#include "icompleted_io.h"
#include "singleton.h"
#include "iocp.h"
#include "worker_thread.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"
#include "packet_storage.h"
#include "client_acceptor.h"
#include "client_io_worker.h"
#include "client_manager.h"
#include "io_engine.h"

IOEngine::IOEngine()
{
}

IOEngine::~IOEngine()
{
}

bool IOEngine::Start()
{
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		return false;

	PacketStorage::GetSingleton()->Create(10000);

	client_manager_ = shared_ptr<ClientManager>(new ClientManager);
	client_acceptor_ = shared_ptr<ClientAcceptor>(new ClientAcceptor);
	client_io_worker_ = shared_ptr<ClientIOWorker>(new ClientIOWorker);

	if (client_acceptor_->Start(L"0.0.0.0", 15110, dynamic_pointer_cast<iCompletedIO>(client_manager_)) == false)
		return false;

	if (client_io_worker_->Start(dynamic_pointer_cast<iCompletedIO>(client_manager_)) == false)
		return false;

	client_manager_->Initialize(client_acceptor_->get_socket(), client_io_worker_->get_iocp());
	if (client_manager_->Start(1000) == false)
		return false;

	return true;
}

void IOEngine::Stop()
{
	if (client_manager_)
		client_manager_->Stop();

	Sleep(1000);

	if (client_acceptor_)
		client_acceptor_->Stop();

	Sleep(1000);

	if (client_io_worker_)
		client_io_worker_->Stop();

	WSACleanup();
}

shared_ptr<iRequestIO> IOEngine::GetiRequestIO()
{ 
	return dynamic_pointer_cast<iRequestIO>(client_manager_);
}
