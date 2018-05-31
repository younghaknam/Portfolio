#pragma once

class ClientManager;
class ClientAcceptor;
class ClientIOWorker;
class iRequestIO;
class iContent;

class IOEngine
{
public:
	IOEngine();
	~IOEngine();

	bool Start(WORD max_client, const shared_ptr<iContent>& content);
	void Stop();
	shared_ptr<iRequestIO> GetiRequestIO();

private:
	shared_ptr<ClientManager> client_manager_;
	shared_ptr<ClientAcceptor> client_acceptor_;
	shared_ptr<ClientIOWorker> client_io_worker_;
	shared_ptr<iContent> content_;
};
