#include "stdafx.h"
#include "BObjectManager.h"
#include "NCMDoc.h"
#include "NCUnitState.h"
#include "NCollisionScene.h"
#include "NCollManagerOrd.h"
#include "BProgram.h"
#include "NToolCombined.h"
#include "MTPart.h"
#include "NProgWalker.h"
#include "NMachUnitPair.h"
#include "NCUnit.h"
#include "NCollRange.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCollRange::NCollRange(CNCMDoc* pD) : pGDoc(pD)
{
	StartHIDAbs.Prog = -1;
	EndHIDAbs.Prog = -1;
	pMUPair = nullptr;
	EndProcessing = false;
	IgnoreWrongAxis = false;
}

NCollRange::~NCollRange(void)
{
	if (pMUPair != nullptr)
		pMUPair->GetMach().DelFullCopyWOVis();
	delete pMUPair;
}
CWinThread *NCollRange::StartChecking(NMachUnitPair* pMU)
{
	MTMachineTool* pAuxMach = new MTMachineTool;
	pAuxMach->FullCopyWOVis(pMU->GetMach());
	pMUPair = new NMachUnitPair(*pAuxMach, pMU->GetUnit());
	
	CWinThread *pThread = AfxBeginThread(CheckingThreadProc, this, THREAD_PRIORITY_BELOW_NORMAL, 2000000, CREATE_SUSPENDED);
	if(pThread)
	{
		pThread->m_bAutoDelete = FALSE;
		pThread->ResumeThread();
	}
	return pThread;
}
UINT NCollRange::CheckingThreadProc( LPVOID pParam )
{
	if(!pParam)
		return 0;
	NCollRange &CollRange = *static_cast<NCollRange *>(pParam);
	cadrID Res = CheckingProc(CollRange, false);
	AfxGetMainWnd()->PostMessage(WM_APP_COLLCHECK_STOPPED, WPARAM(CollRange.EndProcessing));
	CollRange.EndProcessing = false;
	return Res.ID;
}
cadrID NCollRange::CheckingProc( NCollRange &CollRange, bool StopIfStock)
{
//	NCollisionScene *pCollisionScene = pGDoc->GetCollManager().GetScene();
//	BBox StockGabar;
//	if(pGDoc->IsThereAnyStock())
//	{
//		StockGabar = pGDoc->GetStocksGabar();
//		BBox ExObjGabar = pGDoc->GetExObjGabar();
//		if(ExObjGabar.IsDefined())
//		{
//			StockGabar.Zmax = max(ExObjGabar.Zmax, StockGabar.Zmax);
//			StockGabar.Zmin = min(ExObjGabar.Zmin, StockGabar.Zmin);
//		}
//	}
//	SMachState StartB(&CollRange.pMUPair->GetUnit().GetAddCoordsDef());// Create empty arrays these arrays will be filled in FillAddCoords
//	SMachState EndB(&CollRange.pMUPair->GetUnit().GetAddCoordsDef());
//	NProgWalker ProgWalker(CollRange.pMUPair, StockGabar, pGDoc->GetCollManager().GetCheckStep(), &StartB, &EndB, false);
//	int HCadrAbs = CollRange.StartHIDAbs.Cadr;
//	bool EndProcessing = false;
//	bool FirstProg = true;
//	bool FirstCadr = true;
//	int NProg = CollRange.StartHIDAbs.Prog;
//	for (; NProg <= CollRange.EndHIDAbs.Prog && !EndProcessing; ++NProg, FirstProg = false)
//	{// For all programs
//		bool LastProg = (NProg == CollRange.EndHIDAbs.Prog);
//		CString key;
//		BProgram *pProg;
//		pGDoc->NGetProgramList()->GetAt(NProg, key, (CObject*&)pProg);
//		if(FirstProg) 
//		{// First cadr may be a cycle cadr
//			if(HCadrAbs > 0)
//			{
//				for(; pProg->GetCompGeomArray()[HCadrAbs - 1]->IsCycle() && HCadrAbs > 0; --HCadrAbs);
//				CollRange.StartHIDAbs.Cadr = HCadrAbs;
//			}
//		}
//		else
//			HCadrAbs = 0;
//		if(LastProg) 
//		{// Last cadr may be a cycle cadr
//			if(CollRange.EndHIDAbs.Cadr > 0)
//				for(; pProg->GetCompGeomArray()[CollRange.EndHIDAbs.Cadr - 1]->IsCycle() && CollRange.EndHIDAbs.Cadr > 0; --CollRange.EndHIDAbs.Cadr);
//		}
//		int NextToolChangeHistPos = -1;
//		NToolID ToolID;
//		for(int TCInd = pProg->GetTCIndByHist(HCadrAbs); TCInd < pProg->GetTCHistSize() && !EndProcessing; ++TCInd)
//		{ // For all tool changes in the program
//
//			int NextToolChangeHistPos = 0;
//			if(TCInd + 1 < pProg->GetTCHistSize())
//			{
//				NToolID buf;
//				pProg->GetTCHist(TCInd + 1, NextToolChangeHistPos, buf);
//			}
//			else
//			{
//				NextToolChangeHistPos = int(pProg->GetGeomArraySize());
//			}
//			int b;
//			pProg->GetTCHist(TCInd, b, ToolID);
//			NToolCombined *pTool = pGDoc->GetTool(ToolID);
////			NCollManager::ChangeTool(pCollisionScene, *CollRange.pMT, pTool);
//			ProgWalker.SetTool(pTool);
//			bool EndHistIDReached = false;
//			if(LastProg)
//				if(NextToolChangeHistPos >= int(CollRange.EndHIDAbs.Cadr))
//				{
//					EndHistIDReached = true;
//					NextToolChangeHistPos = CollRange.EndHIDAbs.Cadr;
//				}
//			cadrID CadrAbs; 
//			CadrAbs.Prog = -1;
//			CadrAbs.Cadr = 0;
//			bool StockFound = false;
//			for(; HCadrAbs < NextToolChangeHistPos; ++HCadrAbs)
//			{// For all cadrs with the given tool
//				EndProcessing |= CollRange.EndProcessing;
//				if(EndProcessing)
//					break;
//				int CycleStart = HCadrAbs, CycleEnd = HCadrAbs;
//				NCadrGeom *pGeom = pProg->GetCompGeomArray().GetAt(HCadrAbs);
//				if(pGeom->IsCycle())
//				{
//					CycleStart = HCadrAbs;
//					for(++HCadrAbs; pProg->GetCompGeomArray().GetAt(HCadrAbs)->IsCycle(); ++HCadrAbs);
//					CycleEnd = HCadrAbs;
//					pGeom = pProg->GetCompGeomArray().GetAt(HCadrAbs);
//				}
//				for(int i = CycleStart; i <= CycleEnd && !StockFound; ++i)
//				{// Ordinary cadr first and than all cycle cadrs if any
//					ProgWalker.FillAddCoords(pGeom, pProg);
//					if(ProgWalker.SetGeom(pGeom))
//					{
//						CadrAbs.Prog = NProg;
//						CadrAbs.Cadr = HCadrAbs;
//						bool IsFast5X = pGeom->IsFast() && pGeom->Is5x();
//						if(FirstCadr)
//						{
//							FirstCadr = false;
//							ProgWalker.SetStart();
//							NCollManagerOrd::CheckCurPosition(pCollisionScene, &CollRange.pMUPair->GetMach(), pTool, ProgTPoint(CadrAbs, ProgWalker.GetCurTime()), CollRange.IgnoreWrongAxis, false, IsFast5X);
//						}
//						while(ProgWalker.NextStep())// step is inside one cadr only
//						{
//							if(NCollManagerOrd::CheckCurPosition(pCollisionScene, &CollRange.pMUPair->GetMach(), pTool, ProgTPoint(CadrAbs, ProgWalker.GetCurTime()), CollRange.IgnoreWrongAxis, false, IsFast5X))
//								break;
//						}
//						if (StopIfStock)
//						{
//							StockFound = pGDoc->GetCollManager().HaveStockCollisions() & 2;
//							if(StockFound)
//								CollRange.Abort();
//						}
//					}
//					HCadrAbs = i;
//					pGeom = pProg->GetCompGeomArray().GetAt(i);
//				}
//			}
//			if (StockFound)
//				--HCadrAbs;
//			if(EndHistIDReached)
//			{
//				ProgWalker.SetEnd();
//				bool IsFast5X = false;
//				const NCadrGeom* pGeom = ProgWalker.GetGeom();
//				if(pGeom != nullptr)
//					IsFast5X = pGeom->IsFast() && pGeom->Is5x();
//				NCollManagerOrd::CheckCurPosition(pCollisionScene, &CollRange.pMUPair->GetMach(), pTool, ProgTPoint(CadrAbs, ProgWalker.GetCurTime()), CollRange.IgnoreWrongAxis, false, IsFast5X);
//			}
//			EndProcessing |= EndHistIDReached;
//		}
//	}
	cadrID ReachedCadr; // The ID of the first cadr to be processed. It is equal to CollRange.EndHIDAbs if there was no interruption
//	ReachedCadr.Prog = CollRange.EndHIDAbs.Prog;
//	ReachedCadr.Cadr = HCadrAbs;
	return ReachedCadr;
}
