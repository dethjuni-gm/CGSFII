#pragma once
#ifdef _WIN32
#include <Windows.h>
#endif

template <typename T>
class SFIOCPQueue
{
public:
	SFIOCPQueue(void)
	{
		Initialize();
	}

	virtual ~SFIOCPQueue(void)
	{
		Finally();
	}

	bool Push(T* pMsg) 
	{
		if(0 == ::PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)pMsg, NULL))
		{
			//DWORD dwError =GetLastError();
			return false;
		}
		return true;
	}

/////////////////////////////////////////////////////////////////////////////////////////////
	T* Pop() 
	{
		ULONG_PTR pCompletionKey = 0;
		DWORD NumberOfBytesTransferred = 0;
		LPOVERLAPPED pOverlapped = NULL;

		::GetQueuedCompletionStatus(m_hIOCP, &NumberOfBytesTransferred, &pCompletionKey, &pOverlapped, INFINITE);

		return (T*)(pCompletionKey);
	}

	T* Pop(int Wait = -1) 
	{
		ULONG_PTR pCompletionKey = 0;
		DWORD numberOfBytesTransferred = 0;
		LPOVERLAPPED pOverlapped = NULL;

		//SFASSERT( FALSE != ::GetQueuedCompletionStatus(m_hIOCP, &NumberOfBytesTransferred, &pCompletionKey, &pOverlapped, Wait));

		if(FALSE != ::GetQueuedCompletionStatus(m_hIOCP, (LPDWORD)&numberOfBytesTransferred, &pCompletionKey, &pOverlapped, Wait))
			return (T*)(pCompletionKey);

		return nullptr;
	}	

protected:
	//virtual
	bool Initialize()
	{
		m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

		return m_hIOCP != NULL;
	}

	bool Finally()
	{
		if (m_hIOCP)
			::CloseHandle(m_hIOCP);

		return true;
	}

private:
	HANDLE m_hIOCP;
};