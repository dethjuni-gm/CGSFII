// TankServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "TankLogicEntry.h"
#include "TankProtocol.h"
#include "SFPacketProtocol.h"
#include "SFMultiLogicDispatcher.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	TankLogicEntry* pLogicEntry = new TankLogicEntry();
	
	auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry, new SFMultiLogicDispatcher(10));
	if (errorCode != NET_ERROR_CODE::SUCCESS)
		return 0;

	SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<TankProtocol>);

	bool result = SFEngine::GetInstance()->Start();

	if (false == result)
		return 0;

	google::FlushLogFiles(google::GLOG_INFO);

	getchar();

	SFEngine::GetInstance()->ShutDown();	

	return 0;
}