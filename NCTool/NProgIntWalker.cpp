#include "NProgIntWalker.h"
#include "stdafx.h"
#include "BProgram.h"
#include "BGeomArray.h"
#include "NCadrGeom.h"
#include "NProgIntWalker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NProgIntWalker::NProgIntWalker()
{
	Started = false;
}
void NProgIntWalker::Set(const NProgWalker &ProgW)
{
	ProgWalker = ProgW;
	pGeomArray = NULL;
	StartAbsInd = -1;
	EndAbsInd = -1;
	pProg = NULL;
	HEnd = -1;
	HStart = -1;
	CurProg.Empty();
	BreakWasDetected = false;
	Started = true;
}

NProgIntWalker::~NProgIntWalker(void)
{
}

void NProgIntWalker::SetProgram(const class BProgram &Prog)
{
	pProg = &Prog;
	pGeomArray = &Prog.GetCompGeomArray();
	StartAbsInd = -1;
	EndAbsInd = -1;
	CycleApproachInd = -1;
}

bool NProgIntWalker::SetInterval(int StartAbsI, int EndAbsI, int StartHI, int EndHI)
{
	if(!pGeomArray)
		return false;
	if(StartAbsI >= pGeomArray->GetSize())
		return false;

	StartAbsInd = StartAbsI;
	HStart = StartHI;
	//Откатиться к первому кадру цикла
	while(StartAbsInd > 0 && pGeomArray->GetAt(StartAbsInd - 1)->IsCycle())
		--StartAbsInd;
	EndAbsInd = min(EndAbsI, int(pGeomArray->GetSize()));
	HEnd = EndHI;
	//Откатиться к первому кадру цикла
	while(EndAbsInd > 0 && pGeomArray->GetAt(EndAbsInd - 1)->IsCycle())
		--EndAbsInd;
	return true;
}

bool NProgIntWalker::NextStep()
{
	ProgWalker.SetAnimLengthStep();
	while(!ProgWalker.NextStep())
	{
		NCadrGeom *pGeom = FindNextGeom();
		if(!pGeom)
			return false;
		if(!pGeom->IsCycle())
			++HStart;
		
		ProgWalker.SetGeom(pGeom);// *pAddCPrev и *AddCNext should be actual
		ProgWalker.SetAnimLengthStep();
	}
	return true;
}

TimeSlot NProgIntWalker::GetCurTimeSlot() const
{
	auto Slot = ProgWalker.GetCurTimeSlot();
	if (!Started)
		Slot.second = TimeSlotMax;
	return Slot;
}

NCadrGeom *NProgIntWalker::FindNextGeom()
{
	if(!pGeomArray)
		return NULL;

	if(StartAbsInd == CycleApproachInd)
	{
		++StartAbsInd;
		CycleApproachInd = -1;
	}

	if(StartAbsInd >= EndAbsInd)
		return NULL;


	if(pGeomArray->GetAt(StartAbsInd)->IsCycle() && CycleApproachInd < 0)
	{// First cadr of cycle body
		for(CycleApproachInd = StartAbsInd + 1; pGeomArray->GetAt(CycleApproachInd)->IsCycle(); ++CycleApproachInd);
		return pGeomArray->GetAt(CycleApproachInd);
	}

	return pGeomArray->GetAt(StartAbsInd++);

}

NProgWalker &NProgIntWalker::GetProgWalker()
{
	return ProgWalker;
}

const NProgWalker& NProgIntWalker::GetProgWalker() const
{
	return ProgWalker;
}
