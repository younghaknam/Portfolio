#pragma once

class ClientManager;
class ClientAcceptor;
class ClientIOWorker;
class iRequestIO;

class IOEngine
{
public:
	IOEngine();
	~IOEngine();

	bool Start();
	void Stop();
	shared_ptr<iRequestIO> GetiRequestIO();

private:
	shared_ptr<ClientManager> client_manager_;
	shared_ptr<ClientAcceptor> client_acceptor_;
	shared_ptr<ClientIOWorker> client_io_worker_;
};

