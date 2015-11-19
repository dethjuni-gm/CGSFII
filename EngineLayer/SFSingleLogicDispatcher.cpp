#include "stdafx.h"
#include "SFSingleLogicDispatcher.h"
#include "SFEngine.h"
#include "SFPacket.h"
#include "SFPacketPool.h"

bool SFSingleLogicDispatcher::m_bLogicEnd = false;


SFSingleLogicDispatcher::SFSingleLogicDispatcher(void)
{

}

SFSingleLogicDispatcher::~SFSingleLogicDispatcher(void)
{

}

void SFSingleLogicDispatcher::Dispatch(BasePacket* pPacket)
{			
	SFLogicGateway::GetInstance()->PushPacket(pPacket);
}

void SFSingleLogicDispatcher::LogicThreadProc(void* Args)
{
	UNREFERENCED_PARAMETER(Args);
	LogicEntry* pEntry = LogicEntry::GetInstance();
	pEntry->Initialize();

	while (m_bLogicEnd == false)
	{
//��������Ʈ���� ť���� ��Ŷ�� ������.
//������Ʈ�� ��ü�� ProcessPacket �޼ҵ带 ȣ���ؼ� ��Ŷ ó���� �����Ѵ�.
		BasePacket* pPacket = SFLogicGateway::GetInstance()->PopPacket();
		pEntry->ProcessPacket(pPacket);

//20150113
//DB ó���� ������Ʈ�� ��ü�� �����ؼ� �������̾ �����ͺ��̽� ���̾ ���ӵ��� �ʵ��� �Ѵ�.
//DB ��Ŷ�� ���Ŵ� �������̾�� ó���ϵ��� �Ѵ�.
//���� ������ ������ ���̴�.
		if (pPacket->GetPacketType() != SFPACKET_DB)
		{
			ReleasePacket(pPacket);			
		}
	}
}

bool SFSingleLogicDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{
	m_pLogicThread = new tthread::thread(LogicThreadProc, this);
	LogicEntry::GetInstance()->SetLogic(pLogicEntry);

	return true;
}

bool SFSingleLogicDispatcher::ShutDownLogicSystem()
{
	m_bLogicEnd = true;

	BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
	pCommand->SetSerial(-1);
	pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
	SFLogicGateway::GetInstance()->PushPacket(pCommand);

	if (m_pLogicThread->joinable())
	{
		m_pLogicThread->join();
	}

	delete m_pLogicThread;

	LogicEntry::GetInstance()->Destroy();

	return true;
}

bool SFSingleLogicDispatcher::AddRPCService(IRPCService* pService)
{
	m_pRPCService = pService;
	return true;
}