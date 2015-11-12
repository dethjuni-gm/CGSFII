#include "stdafx.h"
#include "SFClassicDispatcher.h"
#include "SFEngine.h"

SFClassicDispatcher::SFClassicDispatcher()
{
}

SFClassicDispatcher::~SFClassicDispatcher()
{
}

void SFClassicDispatcher::Dispatch(BasePacket* pPacket)
{
	LogicEntry::GetInstance()->ProcessPacket(pPacket);

	if (pPacket->GetPacketType() != SFPACKET_DB)
	{
		ReleasePacket(pPacket);
	}	
}

bool SFClassicDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{
	LogicEntry::GetInstance()->SetLogic(pLogicEntry);
	return true;
}

bool SFClassicDispatcher::ShutDownLogicSystem()
{
	return true;
}