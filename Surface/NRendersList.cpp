#include "stdafx.h"
#include "nrenderslist.h"
#include "NQueueElem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NRendersList::NRendersList(void)
{
	CurPos = -1;
	StartCurPos = -1;
	pBufState = NULL;
}

NRendersList::~NRendersList(void)
{
	delete pBufState;
}

void NRendersList::Create(NQueue &StartQueue, NQueueLim &EndQueue)
{
	auto SLength = StartQueue.Length();
	auto ELength = EndQueue.Length();
	RArray.SetSize(SLength + ELength);
	NQueueElem *pElem = StartQueue.GetStartElem();
	for(int i = 0; i < SLength; ++i)
	{
		RArray[i] = (NAnimState *)pElem->GetData();
		pElem = pElem->GetNext();
	}
	CurPos = SLength-1;
	StartCurPos = CurPos;
	for(int i = 0; i < ELength; ++i)
	{
		RArray[SLength + i] = static_cast<NAnimState *>(EndQueue.Get(i));
	}
}
NAnimState *NRendersList::GetNext(bool forward)
{
	if(forward)
	{
		if(CurPos >= RArray.GetSize() - 1)
			return NULL;
		CurPos++;
		return RArray[CurPos]; 
	}
	else
	{
		if(CurPos < 1)
			return NULL;
		delete pBufState;
		pBufState = new NAnimState(*RArray[CurPos]);
		pBufState->CadrGeom.Reverse();
		pBufState->tPart = -1.;
		CurPos--;
		return pBufState;
	}
}
int NRendersList::GetCurPosDiff(void)
{
	return CurPos - StartCurPos;
}

