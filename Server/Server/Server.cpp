// Server.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "inetwork_event.h"
#include "singleton.h"
#include "iocp.h"
#include "worker_thread.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"
#include "packet_storage.h"
#include "client_acceptor.h"
#include "client_manager.h"

int main()
{
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		return 0;

	PacketStorage::GetSingleton()->Create(10000);

	auto client_manager = shared_ptr<ClientManager>(new ClientManager);
	auto client_acceptor = shared_ptr<ClientAcceptor>(new ClientAcceptor);

	if (client_acceptor->Start(L"192.168.0.12", 15110, client_manager) == true)
	{
		if (client_manager->Start(1000, client_acceptor->get_socket()) == false)
		{
			WSACleanup();
			return 0;
		}
	}

	while (true)
	{ 
		Sleep(1000);
	}

	WSACleanup();	
    return 0;
}

