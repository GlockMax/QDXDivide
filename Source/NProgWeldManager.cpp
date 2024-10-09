#include "stdafx.h"
#include "NCMDoc.h"
#include "NCUnit.h"
//#include "NOMScaner.h"
#include "NWeldPArt.h"
#include "NProgramWeld.h"
#include "NProgWeldManager.h"

NProgWeldManager::NProgWeldManager(CNCMDoc &iDoc) : Doc(iDoc)
{

}

NProgWeldManager::~NProgWeldManager(void)
{
}

PObjectStatus NProgWeldManager::OnStatusChanged(int ObjectInd, PObjectStatus NewStatus)
{
	MakeAll(ObjectInd);
	if(NewStatus == DELETED)
		return ST_EMPTY;
	return USED;
}
int NProgWeldManager::MakeAll(int ChangedObInd)
{
	// if ChangedObInd == -2 - update sections only
	// if ChangedObInd == -3 - update trajectories only
	// otherwise - update all
	if(!Doc.IsThereAnyProgr())
		return -1;

	// Find all parts
	CMapStringToNStock PartsList;
	int iName = 0;
	//for(NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
	//{
	//	if(Sc.GetStatus() == DELETED)
	//		continue;
	//	NPassiveObject *pPObj = Sc.GetPassiveObject();
	//	if(!pPObj)
	//		continue;
	//	if(!pPObj->IsKindOf(RUNTIME_CLASS(NWeldPart)))
	//		continue;
	//	NWeldPart *pWPart = (NWeldPart *)Sc.GetPassiveObject();
	//	CString Name;
	//	Name.Format("%d", iName++);
	//	PartsList[Name] = pWPart;
	//}
	//CMapStringToNProgram *pPList = Doc.NGetProgramList();
	//// Recalc sections
	//if(ChangedObInd != -3)
	//{
	//	for(POSITION pos = pPList->GetStartPosition(); pos != NULL;)
	//	{
	//		CString str;
	//		NProgram* pProg;
	//		pPList->GetNextAssoc(pos, str, pProg);
	//		ASSERT(pProg->IsKindOf(RUNTIME_CLASS(NProgramWeld)));
	//		((NProgramWeld *)pProg)->PrepareSections(Doc.GetMachTool(), &PartsList);
	//	}
	//}
	//// Recalc trajectories
	//if(ChangedObInd != -2)
	//{
	//	for(POSITION pos = pPList->GetStartPosition(); pos != NULL;)
	//	{
	//		CString str;
	//		NProgram* pProg;
	//		pPList->GetNextAssoc(pos, str, pProg);
	//		ASSERT(pProg->IsKindOf(RUNTIME_CLASS(NProgramWeld)));
	//		((NProgramWeld *)pProg)->PreparePaths(Doc.GetMachTool(), &PartsList);
	//	}
	//}
	return 0;
}
