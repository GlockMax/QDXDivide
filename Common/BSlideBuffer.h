#pragma once
#include "stdafx.h"
	enum BSBResult
	{
		BSB_OK,
		BSB_FINISHED,
		BSB_PAUSED,
		BSB_NOT_YET
	};

template <class ElemType, unsigned int BufSize, unsigned int ClientsNum> class BSlideBuffer
{
public:
	BSlideBuffer::BSlideBuffer(void)
	{
		for (int i = 0; i < ClientsNum; ++i)
			Paused[i] = ::CreateEvent(
				NULL,   // default security attributes
				TRUE,  // manual-reset event object
				TRUE,  // initial state is nonsignaled
				NULL);  // unnamed object
		Resumed = ::CreateEvent(
			NULL,   // default security attributes
			TRUE,  // manual-reset event object
			TRUE,  // initial state is nonsignaled
			NULL);  // unnamed object
		Clear();
	}

	BSlideBuffer::~BSlideBuffer(void)
	{
		for (int i = 0; i < ClientsNum; ++i)
			::CloseHandle(Paused[i]);
		::CloseHandle(Resumed);
	}

	void StoreRef(void)
	{// Last element of Buf is filled and can be used
		CritSection.Lock();
		LastDXType = (Buf + GlobalEnd % BufSize)->GetDXType();
		::SetEvent(Resumed);
		++GlobalEnd;
		CritSection.Unlock();
	}

	BSBResult GetRef(ElemType *&pElem)
	{// Get reference to the free (GlobalEnd) element of Buf to fill it
		BSBResult Res = BSB_OK;
		CritSection.Lock();
		if(GlobalEnd + 1 - GlobalStart < BufSize) // Ensure that GlobalEnd element is always empty
		{
			pElem = Buf + GlobalEnd % BufSize;
		}
		else
		{
			// Buffer is full
			// Try to shift GlobalStart up
			int MinUsing = Using[0];
			for(int i = 1; i < ClientsNum; ++i)
				MinUsing = min(Using[i], MinUsing);
			if(MinUsing <= GlobalStart)
			{
				Res = BSB_NOT_YET;
			}
			else
			{
				GlobalStart = MinUsing;
				pElem = Buf + GlobalEnd % BufSize;
			}
		}
		CritSection.Unlock();
		return Res;
	}

	BSBResult GetNext(int Client, ElemType *&pElem)
	{
		BSBResult Res = BSB_OK;
		CritSection.Lock();
		int b = Using[Client];
		++b;
		if (b < GlobalEnd)
		{
			pElem = Buf + b % BufSize;
			Using[Client] = b;
		}
		else if (Finished)
			Res = BSB_FINISHED;
		else
			Res = BSB_NOT_YET;
		CritSection.Unlock();
		return Res;
	}

	void SetEmptyEvent(int Client)
	{
		CritSection.Lock();
		::SetEvent(Paused[Client]);
		CritSection.Unlock();
	}

	void SetResumedEvent()
	{
		CritSection.Lock();
		::SetEvent(Resumed);
		CritSection.Unlock();
	}

	bool Wait4Empty()
	{
		// GlobalEnd can't change after call to this method 
		CritSection.Lock();
		if (Finished)
		{
			CritSection.Unlock();
			return true;
		}
		CritSection.Unlock();
		// Sinchronize cutting threads
		if (WaitForMultipleObjects(ClientsNum, Paused, TRUE, 10000) != WAIT_OBJECT_0)
		{
			Beep(2000, 1000);
			return false;
		}
		::ResetEvent(Resumed);
		return true;
	}

	void WaitForResume()
	{
		WaitForSingleObject(Resumed, 10000);
	}

	bool IsEmpty()
	{// For debug
		CritSection.Lock();
//		bool res = test[0] && test[1] && test[2];
		CritSection.Unlock();
		return res;
	}
	void PrepareEvents()
	{
		CritSection.Lock();
		// Prepare events
		for (int i = 0; i < ClientsNum; ++i)
			::ResetEvent(Paused[i]);
		CritSection.Unlock();
	}

	void SetFinished(void)
	{
		CritSection.Lock();
		Finished = true;
		CritSection.Unlock();
	}

	void Clear(void)
	{
		CritSection.Lock();
		for (int i = 0; i < ClientsNum; ++i)
		{
			Using[i] = -1;
			::SetEvent(Paused[i]);
		}
		::SetEvent(Resumed);
		Finished = false;
		GlobalStart = -1;
		GlobalEnd = 0;
		LastDXType = TT_NO;
		CritSection.Unlock();
	}

	auto GetLastDXType() const 
	{
		return LastDXType;
	}
protected:
	ElemType Buf[BufSize];
	HANDLE Paused[ClientsNum];
	HANDLE Resumed;
	int Using[ClientsNum]; // Indexes of the elements used now by clients
	int GlobalStart; // Global start index in the external data set
	int GlobalEnd; // Global end index in the external data set (this place in the Buf is not occupied yet)
	bool Finished;
	DX_TOOLTYPE LastDXType;
	CCriticalSection CritSection;
};

