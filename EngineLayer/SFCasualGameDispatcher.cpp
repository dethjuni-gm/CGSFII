#include "stdafx.h"
#include "SFCasualGameDispatcher.h"
#include "SFEngine.h"
#include "SFPacket.h"
#include "SFPacketPool.h"

bool SFCasualGameDispatcher::m_bLogicEnd = false;


SFCasualGameDispatcher::SFCasualGameDispatcher(void)
{
//ĳ��� ���� ������ ��ũ�� ���� ������ ���� �ϳ���
	m_nLogicThreadCnt = 1;	
}

SFCasualGameDispatcher::~SFCasualGameDispatcher(void)
{
}

void SFCasualGameDispatcher::Dispatch(BasePacket* pPacket)
{			
	SFLogicGateway::GetInstance()->PushPacket(pPacket);
}

void SFCasualGameDispatcher::LogicThreadProc(void* Args)
{
	UNREFERENCED_PARAMETER(Args);
	LogicEntry* pEntry = LogicEntry::GetInstance();

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

bool SFCasualGameDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{		
	for (int index = 0; index < m_nLogicThreadCnt; index++)
	{
		tthread::thread* pThread = new tthread::thread(LogicThreadProc, this);
		m_mapThread.insert(std::make_pair(index, pThread));
	}

	LogicEntry::GetInstance()->SetLogic(pLogicEntry);

	return true;
}

bool SFCasualGameDispatcher::ShutDownLogicSystem()
{
	m_bLogicEnd = true;
	
	for (int i = 0; i < m_nLogicThreadCnt; i++)
	{
		BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
		pCommand->SetSerial(-1);
		pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
		SFLogicGateway::GetInstance()->PushPacket(pCommand);
	}

	LogicEntry::GetInstance()->DestroyLogic();

	return true;
}

bool SFCasualGameDispatcher::AddRPCService(IRPCService* pService)
{
	m_pRPCService = pService;
	return true;
}

