// Server.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "irequest_io.h"
#include "io_engine.h"

int main()
{
	auto io_engine = shared_ptr<IOEngine>(new IOEngine);
	if (io_engine->Start() == false)
		return 0;

	while (true)
	{ 
		Sleep(5000);
	}

    return 0;
}

