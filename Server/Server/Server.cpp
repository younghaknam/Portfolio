// Server.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "irequest_io.h"
#include "io_engine.h"
#include "icontent.h"
#include "server_content.h"

int main()
{
	auto io_engine = shared_ptr<IOEngine>(new IOEngine);
	auto server_content = shared_ptr<ServerContent>(new ServerContent);
	WORD max_user = 1000;

	if (io_engine->Start(max_user, server_content->GetiContent()) == false)
		return 0;

	if (server_content->Start(max_user, io_engine->GetiRequestIO()) == false)
		return 0;

	while (true)
	{ 
		Sleep(5000);
	}

    return 0;
}
