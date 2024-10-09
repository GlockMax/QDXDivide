#pragma once
#include "NAnimState.h"
#include "NQueue.h"
#include "NQueueLim.h"

class AFX_EXT_CLASS NRendersList
{
public:
	NRendersList(void);
	~NRendersList(void);
	void Create(NQueue &StartQueue, NQueueLim &EndQueue);
	NAnimState *GetNext(bool forward);
	int GetCurPosDiff(void);
protected:
	int CurPos;
	int StartCurPos;
	int MaxLength;
	CTypedPtrArray <CPtrArray, NAnimState *> RArray;
	NAnimState *pBufState;
};
