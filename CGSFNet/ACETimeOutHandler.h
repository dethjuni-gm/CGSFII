#pragma once
#include "ACEHeader.h"
#include "INetworkEngine.h"

class ACETimeOutHandler : public ACE_Handler
{
public:
	ACETimeOutHandler(INetworkEngine* pOwner){m_pOwner = pOwner;}
	virtual ~ACETimeOutHandler(void){}

	 virtual void handle_time_out (const ACE_Time_Value &tv,
                                const void *arg)
    {
		IEngine* pEngine = m_pOwner->GetEngine();
		pEngine->OnTimer(arg);
    }

protected:

private:
	INetworkEngine* m_pOwner;
};

