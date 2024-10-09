// NCMDoc.cpp : implementation of the CNCMDoc class
//

#include "stdafx.h"
#include "zlib.h"
#include "ConstDef.h"
#include "NCMProject.h"
#include "NCMVersion.h"
#include "BSpacePos.h"
#include "LeftView.h"
#include "NPositionDlg.h"
#include "NCM.h"
#include "NToolStd.h"
#include "BObjectManager.h"
#include "NStockSection.h"
#include "NCUnit.h"
#include "StContOut.h"
#include "NTextView.h"
#include "MainFrm.h"
#include "NProgram.h"
#include "NProgramWeld.h"
#include "NToolCombinedMill.h"
#include "NToolCombinedTurn.h"
#include "NToolCombinedWeld.h"
#include "NToolTurnBladeISO.h"
#include "NToolTurnBladeOut.h"
#include "NToolTurnBladeCirc.h"
#include "NToolTurnBladeCut.h"
#include "NToolTurnBladeMill.h"
#include "NToolTurnBladeThr.h"
#include "NToolTurnHolderBar.h"
#include "NToolTurnHolderCyl.h"
#include "NToolTurnHolderSTL.h"
#include "GQuadRenderRot.h"
#include "InfoTool.h"
#include "MTMashineTool.h"
#include "MBSPForest.h"
#include "NElemIDProc.h"
#include "NMachNameDlg.h"
#include "BDebugState.h"
#include "MQuadTree.h"
#include "NStockCrBoxDial.h"
#include "NStockCrCylDial.h"
#include "NStockCrStlDial.h"
#include "NStockCrPipDial.h"
#include "NStockCrContDial.h"
#include "NStockCrContMDial.h"
#include "NStockBar.h"
#include "NStockCont.h"
#include "NStockSweep.h"
#include "NStockBSP.h"
#include "NCollisionScene.h"
#include "MTPart.h"
#include "GDynamicCadr.h"
#include "FCutting.h"
#include "FProg2BSP.h"
#include "GReRenderCadr.h"
#include "JobKeyWords.h"
#include "PropList.h"
#include "NDocClipboard.h"
#include "DXDirection5X.h"
#include "DXSubModel.h"
#include "NCadr.h"
#include "FAnimateDisp.h"
#include "BFileDialogWPath.h"
#include "Snaps.h"
#include "GrazingCurveElemContour.h"
#include "NAnimState.h"
#include "RDynViewChange.h"
#include "NMachUnitPair.h"
#include "GAnimateCadr.h"
#include "FRange.h"
#include "NSyncGenerator.h"
#include "UCAddDelStock.h"
#include "UCAddDelPart.h"
#include "UCAddDelFixture.h"
#include "UCTransMovObj.h"
#include "GAnimateCadrMill.h"
#include "MTConfig.h"
#include "NCMDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNCMApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CNCMDoc

IMPLEMENT_DYNCREATE(CNCMDoc, CDocument)

/////////////////////////////////////////////////////////////////////////////
// CNCMDoc construction/destruction

CNCMDoc::CNCMDoc()
		: ProgWeldManager(*this), ProgErrHandler(int(NCM_PROJECT.Defaults.GetDouble("Defaults/Editor/Errors@Reaction", 2.0))),
	Measure(*this), Comparison(*this), SnapsGlob(this), Dimensions(SnapsGlob), CollManager(this),
#ifdef NC_DEMO
	MachToolEditor(true)
#else
	MachToolEditor()
#endif
{
	DynamicLavel = DynamicLavelDef;
	pExternData = NULL;
	StatusX = 0;
	StatusY = 0;
	StatusZ = 0;
	CalcVolume = FALSE;
	AutoUpdateProg = false;
	CalcTime = TRUE;
	CalcGeom = TRUE;
	StatusCadr.Prog = -1;
	StatusCadr.Cadr = 0;
	StatusCadrAbs.Prog = -1;
	StatusCadrAbs.Cadr = 0;
	StatusCadrAbsTime = 0;
	pRepArray = NULL;
	pStockSection = NULL;
	BSpacePos::ClearRotMatrs();

	MFOREST.SetIDProc(new NElemIDProc(this));

	pCreateBoxDlg = new NStockCrBoxDial;
	pCreateCylDlg = new NStockCrCylDial;
	pCreateStlDlg = new NStockCrStlDial;
	pCreatePipDlg = new NStockCrPipDial;
	pCreateContDlg = new NStockCrContDial;
	pCreateContMDlg = new NStockCrContMDial;

	BlockUpdateLevel = 0;
	pSliderRRCadr = NULL;
	pClipboard = NULL;
	SilentOpen = false;
	FastCut5xEnabled = false;
	CutEnabled = false;
	SaveEnvironment = false;

	LoadLayoutProject = false;

	pMUPair = nullptr;
	pCurChannel = nullptr;
	DifferentBodyRotFlag = true;
	Interrupt = false;
	pDivide = nullptr;
}

CNCMDoc::~CNCMDoc()
{
	delete pStockSection;
	if(pExternData)
		delete pExternData;
	delete pRepArray;

	delete pCreateBoxDlg;
	delete pCreateCylDlg;
	delete pCreateStlDlg;
	delete pCreatePipDlg;
	delete pCreateContDlg;
	delete pCreateContMDlg;
	delete pClipboard;

	delete pMUPair;
	for each (auto pEl in Channels)
		delete pEl;
}

/////////////////////////////////////////////////////////////////////////////
// CNCMDoc serialization

void CNCMDoc::Serialize(CArchive& ar)
{
	// Calling the base class CDocument enables serialization
	// of the container document's COleClientItem objects.
	// set CDocument::m_bRTF = FALSE if you are serializing as text
	BlockUpdateLevel = 1;
	CString Title("NCM File.");
	CString v(" Ver.");
	CString vd(" Ver:");// for demo project
	CString str = Title;
	SerializeElements(ar,&str,1);
	bool DemoProject = false;
	if(ar.IsStoring())
	{
		str = CM_MakeDemo & theApp.GetConfig() ? vd : v;
		SerializeElements(ar,&str,1);
		CString Buf(NCMVersion);
		SerializeElements(ar, &Buf, 1);
		if (SaveEnvironment)
			FillEnvironment();
		else
			NCM_PROJECT.DFNFile = DFNFileName;
	}
	else
	{
		if(str != Title)
		{
			ar.Flush();
			CFile *f = ar.GetFile();
			f->Seek(0, CFile::begin);
#ifndef NC_DEMO
			ReadJobFile(ar);
			UpdateAllViews(NULL, N_STATE | N_SETTINGS | N_UNIT_CHANGED | N_TOOLCHANGED | N_LEFT | N_PROGRAMCHANGED);
#endif
			for each(const auto& el in NStocksList)
			{
				class NStock *pS = static_cast<class NStock*>(el.second);
				pS->SetGroup(OG_STOCKS);
			}
			BlockUpdateLevel = 0;
			LoadSettings();
			PrepareNewData();//shold be before theApp.CWinAppEx::LoadState(frame) to ensure proper value of NStateUnitPane::pUnitState
			return;
		}
		SerializeElements(ar,&str,1);
		DemoProject = (str == vd);
		if(str != v && !DemoProject)
		{
			AfxMessageBox(IDS_MES_ERR_INDOC);
			BlockUpdateLevel = 0;
			return;
		}
		else
		{
			SerializeElements(ar,&str,1);
			NCM_PROJECT.SetInFileVersion(atof(str));
		}
		if(NCM_PROJECT.GetInFileVersion() > atof(NCMVersion))
		{
			AfxMessageBox(IDS_MES_NEWV);
			BlockUpdateLevel = 0;
			return;
		}
		if(NCM_PROJECT.GetInFileVersion() < atof(NCMVersion))
		{
//			AfxMessageBox(IDS_MES_OLDV);
		}
	}
	// for icon handler only
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.3")))
	{
		NCMApplicationType Type = NCMComData::GetType();
		SerializeElements(ar, &Type, 1);
	}
	// END: for icon handler only
	if(ar.IsStoring())
	{
		if(pExternData)
		{
			pExternData->Serialize(ar);
			delete pExternData;
			pExternData = NULL;
		}
	}
	else
	{//Read and forget ExternData
		if(pExternData)
			delete pExternData;
		pExternData = new NCMData;
		pExternData->Serialize(ar);
		delete pExternData;
		pExternData = NULL;
	}
	CDocument::Serialize(ar);

	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.3")))
	{
		ImageRGB.Serialize(ar);
		if (ar.IsLoading())
			ImageRGB.Clear();
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.4")))
	{
		CString DocPath;
		if (ar.IsLoading())
		{
			DocPath = ar.m_strFileName;
			int nIndex = DocPath.ReverseFind(_T('\\'));
			if (nIndex > 0)
				DocPath = DocPath.Left(nIndex + 1);
		}
		if(NCMProject::GetProject().DFNFile.IsEmpty())
			NCMProject::GetProject().DFNFile = GetDFNFileName(); // change in 2025
		NCMProject::GetProject().Serialize(ar);
		NCMProject::GetProject().PostSerialize(ar, DocPath);
		SetDFNFileName(NCMProject::GetProject().DFNFile); // change in 2025
	}

	if(ar.IsLoading())
	{
		if(!(NCM_PROJECT.GetInFileVersion() < atof("3.3")))
		{
			NCMApplicationType buf;
			SerializeElements(ar, &buf, 1);
			theApp.ChangeType(buf);
		}
		else
			theApp.ChangeType(AT_MILL);
	}
	else
	{
		NCMApplicationType buf = NCMComData::GetType();
		SerializeElements(ar, &buf, 1);
	}

	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.0")))
	{
		SerializeElements(ar, &FastCut5xEnabled, 1);
		if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() >= atof("4.0") && NCM_PROJECT.GetInFileVersion() < atof("4.5"))
		{
			bool b;
			SerializeElements(ar, &b, 1);
		}
		if (ar.IsStoring())
		{
			if (IsThereAnyStockImpExpDX())
				CutEnabled = false;
		}
		SerializeElements(ar, &CutEnabled, 1);
	}
	else
	{
		FastCut5xEnabled = true;
		CutEnabled = true;
	}
	if (ar.IsLoading())
	{
		auto Flag = theApp.GetVerifFlag();
		if (!(theApp.GetConfig() & Flag))
			CutEnabled = false;
		if (!(theApp.GetConfig() & CM_VerifierM5X))
			FastCut5xEnabled = false;
		switch (theApp.GetGlobalState().GetModelMode())
		{
		case NGlobalState::MM_EMPTY:
			break;
		case NGlobalState::BSP:
			if (!CutEnabled)
				theApp.GetGlobalState().SetModelMode(FastCut5xEnabled ? NGlobalState::DX : NGlobalState::MM_EMPTY);
			break;
		case NGlobalState::DX:
			if (!FastCut5xEnabled)
				theApp.GetGlobalState().SetModelMode(CutEnabled ? NGlobalState::BSP : NGlobalState::MM_EMPTY);
			break;
		}
	}

	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.2")))
	{
		NExObjList.Serialize(ar);
		SerializeStocks(ar);
		SerializeTraj(ar);
	}

	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.5")) // Do not write for the versions after 4.4
	{
		bool b0;
		BOOL b1;
		int b2;
		SerializeElements(ar,&b0,1);
		SerializeElements(ar,&b0,1);
		SerializeElements(ar,&b0,1);
		SerializeElements(ar,&b1,1);
		SerializeElements(ar,&b1,1);
		SerializeElements(ar,&b1,1);
		SerializeElements(ar,&b1,1);
		SerializeElements(ar,&b1,1);
		SerializeElements(ar,&b2,1);
		SerializeElements(ar,&b2,1);
	}

	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")) // Do not write for the versions after 5.0
	{
		CString ProgrToDispText;
		SerializeElements(ar, &ProgrToDispText, 1);
		int ProgrToDispTextNum = 0;
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.91")))
		{
			SerializeElements(ar, &ProgrToDispTextNum, 1);
		}
		pCurChannel->NGetProgramListCn()->Serialize(ar);
		pCurChannel->SetProgrTDTCn(ProgrToDispText);
	}
	else
	{// 5.1 and upper
		size_t Size;
		size_t CurInd = 0;
		if (ar.IsStoring())
		{
			Size = Channels.size();
			CurInd = pCurChannel->GetChannelInd();
		}
		SerializeElements(ar, &Size, 1);
		SerializeElements(ar, &CurInd, 1);
		if (ar.IsLoading())
		{
			for each (auto pEl in Channels)
				delete pEl;
			Channels.resize(Size);
			NCM_PROJECT.SetUseQuadTreeParallel(!IsMultiChannel());
			for(int i = 0; i < Channels.size(); ++i)
			{
				auto pEl = new NChannel;
				Channels[i] = pEl;
				pEl->MakeNewChannel(this, i);
			}
		}
		bool LoadFromSerializeAll = ar.IsLoading() && !NCM_PROJECT.MachineFile.IsEmpty();
		pCurChannel = Channels.at(CurInd);
		if (ar.IsLoading())
			BSpacePos::ClearRotMatrs();
		for each (auto pEl in Channels)
		{
			pEl->Serialize(ar);
			if(!LoadFromSerializeAll)
				pEl->GetUnitCn()->LoadSubprograms();// reload sub programs to overwrite it's loading in NProgramList.Serialize(ar);
		}
		NCM_PROJECT.UnitConfs.RemoveAll();
		static_cast<CMainFrame*>(AfxGetMainWnd())->SetNCUnitName(GetUnit()->GetConfFileName());
	}
	FillMUPair();
#ifdef NC_DEMO
	//// Special DEMO comments
	//CString Key;
	//NProgram *pProg;
	//if(NProgramList.GetAt(0, Key, (CObject *&)pProg))
	//{
	//	if(pProg->GetComment().GetLength() != Key.GetLength())
	//	{
	//		AfxMessageBox(IDS_MES_DEMO);
	//		DeleteContents();
	//		BlockUpdateLevel = 0;
	//		return;
	//	}
	//}
	if (!DemoProject)
	{
		AfxMessageBox(IDS_MES_DEMO);
		DeleteContents();
		BlockUpdateLevel = 0;
		return;
	}
#endif
	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.2"))
	{
		NExObjList.Serialize(ar);
	}
	NToolList.Serialize(ar);

	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.4")))
		Dimensions.Serialize(ar);

	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.3"))
	{
		// convert NToolCombined to NToolCombinedMill
		for(int i = 0; i < NToolList.GetSize(); ++i)
		{
			CString Key;
			NToolCombined *pTool;
			NToolList.GetAt(i, Key, (CObject *&)pTool);
			NToolCombinedMill *pToolMill = ((NToolCombinedMill *)pTool)->NToolCombinedMill::CreateCopyTool();
			pToolMill->Turret = 0;
			pToolMill->Position = atoi(Key);
			delete pTool;
			NToolList.SetAt(Key, pToolMill);
		}
	}
	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")) // Do not write for the versions after 5.0
	{
		pCurChannel->GetUnitCn()->Serialize(ar);
		static_cast<CMainFrame *>(AfxGetMainWnd())->SetNCUnitName(GetUnit()->GetConfFileName());
	}

	if (!MachTool.GetCurConfig().IsDefined())
		if (!LoadMachTool(NCM_PROJECT.GetMachinesPath(), MachTool.GetFileName()))
		{
			AfxMessageBox(IDS_MT_LOADERR);
		}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.1")))
	{
		MachTool.Serialize(ar);
	}
	if (ar.IsLoading() && !NCM_PROJECT.MachineFile.IsEmpty())
	{
		MachTool.SetFileName(NCM_PROJECT.MachineFile.GetAt(0));
		NCM_PROJECT.MachineFile.RemoveAll();
	}

	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() >= atof("4.3") && NCM_PROJECT.GetInFileVersion() <= atof("4.6"))
	{
		BMatr matr;
		SerializeElements(ar, &matr, 1);
	}

	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.2"))
	{
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.4")))
		{
			SerializeStocks(ar); 
		}
		else
		{
			LoadSingleStock(ar);
		}
	}

	if(ar.IsStoring())
	{
		bool CompOn = NGetProgramList()->HaveCompTraj();
		SerializeElements(ar,&CompOn,1);
		bool ProgOn = NGetProgramList()->HaveProgTraj();
		SerializeElements(ar,&ProgOn,1);
	}
	else
	{
		bool CompOn;
		SerializeElements(ar,&CompOn,1);
		bool ProgOn;
		SerializeElements(ar,&ProgOn,1);
		TrajType TType = TT_BOTH;
		if(!CompOn)
			TType = TT_PROG;
		else if(!ProgOn)
			TType = TT_COMP;
		NGetProgramList()->ProgComp = TType;
	}

	SerializeElements(ar,&NGetProgramList()->PartialDraw,1);

	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.7"))
	{
		BOOL OpaqueTool = FALSE;
		SerializeElements(ar, &OpaqueTool, 1);
	}
	// 	сравнение версий при чтении (read and ignore if FileVer < 2025 )
	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("2025"))
	{
		int buf;
		SerializeElements(ar, &buf, 1);
	}

	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.0"))
	{
		GLdouble Buf[6][4][4];
		SerializeElements(ar,(GLdouble *)Buf,6*4*4);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.1")))
	{
		pCreateBoxDlg->Serialize(ar);
		pCreateCylDlg->Serialize(ar);
		pCreateStlDlg->Serialize(ar);
		if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.3")))
		{
			pCreatePipDlg->Serialize(ar);
			if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.6")))
			{
				pCreateContDlg->Serialize(ar);
				pCreateContMDlg->Serialize(ar);
				NPartsList.Serialize(ar);
			}
		}
		if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.6"))
		{
			NStock *pPart;
			ar >> (CObject *&)pPart;
			if(pPart)
				AddPartWU(pPart);
		}
	}
	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() >= atof("3.2") && NCM_PROJECT.GetInFileVersion() < atof("4.7"))
	{
		double TolArr[6];
		SerializeElements(ar, TolArr, 6);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.8")))
	{
		CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
		frame->SerializeMatrices(ar);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		Comparison.Serialize(ar);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 4.9))
	{
		CollManager.Serialize(ar);
		if (ar.IsLoading())
		{
			CollManager.SetCuttingDisabled(!theApp.IsVerifEnabled());
			if (!theApp.IsVerifEnabled())
				CollManager.RemFromMask(OG_STOCKS);
		}
	}
#ifdef NC_DEMO	
	//if(ar.IsLoading())
	//{
	//	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	//	if(GetMachTool().GetFileName() != "3X_DEFAULT" &&
	//		GetMachTool().GetFileName() != "TURN_DEFAULT" &&
	//		GetMachTool().GetFileName() != "MILLTURN_DEFAULT")
	//		frame->OnWinVert();
	//	else
	//		frame->OnWinStock();
	//}
#endif
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("2025")))
	{
		CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
		frame->BreakpointDlg.Serialize(ar);
		MileStones.FillFromDlg(frame->BreakpointDlg);
	}
	if (ar.IsLoading())
	{
		LoadSettings();
		PrepareNewData();//shold be before theApp.CWinAppEx::LoadState(frame) to ensure proper value of NStateUnitPane::pUnitState
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 5.0))
	{
		CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
		frame->SerializeOGLPanes(ar);
		if (ar.IsStoring())
		{
			theApp.AllowLoadRibbonState = false;
			theApp.CWinAppEx::SaveState(frame);
			theApp.AllowLoadRibbonState = true;
			HKEY hkResult;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Inteks\\NCManager\\Workspace\\")
				, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hkResult) == ERROR_SUCCESS)
							theApp.RegSaveBranch(hkResult, ar);
		}
		else
		{
			if (GetLoadLayoutProject())
			{
				HKEY hkResult;
				if (RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Inteks\\NCManager\\Workspace\\")
					, 0L, NULL, REG_OPTION_NON_VOLATILE,
					KEY_WRITE, NULL, &hkResult, NULL) == ERROR_SUCCESS)
				{
					auto Res = theApp.RegLoadBranch(hkResult, ar);
					if (Res != ERROR_SUCCESS)
					{
						AfxMessageBox(IDS_MES_LAYOUT_PROJECT);
						return;
					}
					theApp.AllowLoadRibbonState = false;
					theApp.CWinAppEx::LoadState(frame);
					theApp.AllowLoadRibbonState = true;
					CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
					if (pMainFrame)
						pMainFrame->LoadTBFromIcons();
				}
			}
		}
		//		pDrawWEdges = NCM_PROJECT.Defaults.GetBool("Defaults/Render/DX_VBO@Enable", true) ?
			//CString Path = GetPathName();
		//if (Path.Right(6) != _T("NCMPrt"))
		//{
			//CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
			//frame->SerializeOGLPanes(ar);
		//}
	}

	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() == atof("3.8"))
	{ // For the compatibility with 3.8 only
		CStringArray UnvisibleMTParts;
		CStringArray WFrameMTParts;
		UnvisibleMTParts.Serialize(ar);
		WFrameMTParts.Serialize(ar);
	}//if
	GetUndoHistory().Clear();
	BlockUpdateLevel = 0;
	static_cast<CMainFrame*>(AfxGetMainWnd())->SetMachName(MachTool.GetFileName());

}
void CNCMDoc::PrepareNewData()
{
	ApplyAppearSettings();
	if (NTool::GetToolStockCollision() != CWS_OFF)
	{
		GetCollManager().SetToolStockMode(NTool::GetToolStockCollision());
//		GetCollManager().SetToolStockActive(true);
	}
	//else
	//{
	//	GetCollManager().SetToolStockActive(false);
	//}
	if(GetUnit())
		if(!GetMachTool().GetFileName().IsEmpty())
		{
			if(!LoadMachTool(NCM_PROJECT.GetMachinesPath(), GetMachTool().GetFileName()))
			{
				CString Mess;
				Mess.Format(IDS_MES_ERR_FILE2, NCM_PROJECT.GetMachinesPath() + GetMachTool().GetFileName());
				AfxMessageBox(Mess);
			}
			GetMachTool().ApplyPartsPropList();
		}

	CString key;
	NPassiveObject *pObj = NULL;
	for(int i = 0; i < NToolList.GetSize(); ++i)
	{
		NToolList.GetAt(i, key, (CObject *&)pObj);
		pObj->SetGroup(OG_TOOLS);
	}

	for each (const auto & el in NStocksList)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->SetGroup(OG_STOCKS);
	}

	for(int i = 0; i < NPartsList.GetSize(); ++i)
	{
		NPartsList.GetAt(i, key, (CObject *&)pObj);
		if(((NStock *)pObj)->IsCreated)
		{
			pObj->SetGroup(OG_PARTS);
			pObj->SetEdgesAttrs(NPartsList.GetEdgesAttrs());
		}
		else
		{
			delete pObj;
			NPartsList.RemoveKey(key);
			--i;
		}
	}

	for(int i = 0; i < NExObjList.GetSize(); ++i)
	{
		NExObjList.GetAt(i, key, (CObject *&)pObj);
		if(((NStockStl *)pObj)->IsCreated)
		{
			pObj->SetGroup(OG_FIXTURES);
			pObj->SetEdgesAttrs(NExObjList.GetEdgesAttrs());
		}
		else
		{
			delete pObj;
			NExObjList.RemoveKey(key);
			--i;
		}
	}

	if(NCMComData::GetType() == AT_WELD)
	{
		InitToolSection();
		InitProgWeldManager();
	}
	for each (auto pCh in Channels)
		ImplReplAll(pCh);
	UpdateAllViews(NULL, N_OGL | N_UNIT_CHANGED | N_LEFT);
	if (NCM_PROJECT.GetInFileVersion() < 4.7)
		RegenerateStocks();

	RegeneratePrograms();
	for each (auto pCh in Channels)
	{
		if (pCh->GetUnitCn())
			pCh->GetUnitCn()->Reset();
	}
//	pCurChannel->GetUnitCn()->GetProgGroupList().Restart(1);
	UpdateAllViews(NULL, N_OGL | N_PROGRAMCHANGED | N_STATE | N_NEWDOC | N_TOOLCHANGED | N_STOCKSTATECHANGE);// N_SETTINGS added 22.02.21 to ensure proper russian text while reading project
}

/////////////////////////////////////////////////////////////////////////////
// CNCMDoc diagnostics

#ifdef _DEBUG
void CNCMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNCMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNCMDoc commands


void CNCMDoc::NAddMainProgram(const CString &name, NProgram *pProg)
{
	int Ind = NGetProgramList()->GetIndex(name);
	if(Ind >= NGetProgramList()->GetMainProgCount())
	{
		DeleteProgObj(pProg);
		return;
	}
	if(Ind >= 0)
	{// delete old program
		NProgram *pObj = NULL;
		CString Key;
		NGetProgramList()->GetAt(Ind, Key, (CObject *&)pObj);
		DeleteProgObj(pObj);
	}
	NGetProgramList()->AddMainProg(name, pProg);
	SetModifiedFlag();
// Set program name as a document title if title is empty
	CString DocTitle;
	DocTitle.LoadString(IDS_EMPTY_DOC_TITLE);
	if(DocTitle == GetTitle())
	{
		CString ItemText;
		int index = name.ReverseFind( '\\' );
		for( int i = index + 1; i < name.GetLength(); i++ )
		{
			if(name[ i ] == '.')
				break;
			ItemText += name[ i ];
		}
		SetTitle(ItemText);
		SetPathName(ItemText + _T(".ncm"), false);
	}
	ImplRepl(pProg);
	SetModifiedFlag();
}

void CNCMDoc::NAddAuxProgram(const CString &name, NProgram *pProg)
{
	pProg->SetAux();
	ImplRepl(pProg);
	int Ind = NGetProgramList()->GetIndex(name);
	if(Ind < 0)
	{
		(*NGetProgramList())[name] = pProg;
		SetModifiedFlag();
		return;
	}
	if(Ind < NGetProgramList()->GetMainProgCount())
	{
		DeleteProgObj(pProg);
		return;
	}

	NProgram *pObj = NULL;
	CString Key;
	NGetProgramList()->GetAt(Ind, Key, (CObject *&)pObj);
	DeleteProgObj(pObj);
	(*NGetProgramList())[name] = pProg;
	SetModifiedFlag();
}

NMapStringToNTool * CNCMDoc::NGetToolList()
{
	return &NToolList;
}

const NMapStringToNTool * CNCMDoc::NGetToolList() const
{
	return &NToolList;
}

CMapStringToNStock * CNCMDoc::NGetExObjList()
{
	return &NExObjList;
}

const CMapStringToNStock * CNCMDoc::NGetExObjList() const
{
	return &NExObjList;
}

CMapStringToNStock* CNCMDoc::NGetPosChecks()
{
	return &NPosChecks;
}

const CMapStringToNStock* CNCMDoc::NGetPosChecks() const
{
	return &NPosChecks;
}

bool CNCMDoc::IsStockPosChecked()
{
	for each (const auto & el in NPosChecks)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		if (key.Left(CMapStringToNStock::PrefixStock.GetLength()) == CMapStringToNStock::PrefixStock)
			return true;
	}
	return false;
}

CMapStringToNStock * CNCMDoc::NGetPartList()
{
	return &NPartsList;
}

const CMapStringToNStock * CNCMDoc::NGetPartList() const
{
	return &NPartsList;
}

CStocksCollection * CNCMDoc::NGetStockList()
{
	return &NStocksList;
}

const CStocksCollection * CNCMDoc::NGetStockList() const
{
	return &NStocksList;
}

void CNCMDoc::DeleteContents() 
{
	UpdateAllViews(NULL, N_DELETECONT);
	StatusX = 0;
	StatusY = 0;
	StatusZ = 0;
	StatusCadr.Prog = -1;
	StatusCadr.Cadr = 0;
	SelectedPrev = NCadrGeom();
	SelectedNext = NCadrGeom();
	CadrToolAxis.Set(0., 0., 1., 0.);
//Clear Record
//	StopRecord();

	Measure.DeActivate();
	Dimensions.DeActivate();
	Comparison.DeActivate();
	Dimensions.Clear();
	if(pStockSection)
		pStockSection->Deactivate();
	delete pStockSection;
	pStockSection = NULL;

	NCM_PROJECT.GetGlobalState().Reset();
	for each (auto pEl in Channels)
	{
		pEl->DeleteContents();
		delete pEl;
	}
	Channels.clear();
	pCurChannel = nullptr;
	AddNewChannel();
	GetCollManager().ClearLogs();
	//Clear Reports
	delete pRepArray;
	pRepArray = new NRepReportsArray;
	pRepArray->SetUnit(GetUnit());
//Clear ProgramList
	for (int i = 0; i < NGetProgramList()->GetSize(); ++i)
	{
		CObject* pObj = nullptr;
		CString Key;
		NGetProgramList()->GetAt(i, Key, pObj);
		ReleaseSciDoc((NProgram*)pObj);
	}
	SelectedPrev.Clear(); // Should be done before ClearAllCurves
	SelectedNext.Clear();
	CadrToolAxis.Set(0., 0., 1., 0.);

	BCurve::Storage.ClearAll();
//Clear ToolList
	NToolList.FullRemoveAll();


	ResetCutting();

	DeleteList(NExObjList);
	DeleteList(NPartsList);
	DeleteStocks();


	UndoHistory.Clear();// should be before MFOREST.ClearAll();
	MFOREST.ClearAll();
	MFOREST.Init();
	MFOREST.SetIDProc(new NElemIDProc(this));

	ToolSectionDyn.Disable();
	ProgWeldManager.Disable();

	C_OBJECT_MANAGER(ToolSectionDyn)->DeleteActiveObject(&ToolSectionDyn);

	if (StocksSaveThread.joinable())
	{
		StocksSaveThread.join();
		StocksSaveThread = std::thread();
	}

	CDocument::DeleteContents();
	UpdateAllViews(NULL, N_NEWDOC | N_STATE | N_UNIT_CHANGED | N_TOOLCHANGED);	
}
void CNCMDoc::PrepReadJob() 
{
	UpdateAllViews(NULL, N_DELETECONT);
	StatusX = 0;
	StatusY = 0;
	StatusZ = 0;
	StatusCadr.Prog = -1;
	StatusCadr.Cadr = 0;

	Measure.DeActivate();
	Dimensions.DeActivate();
	Comparison.DeActivate();
	if(pStockSection)
		pStockSection->Deactivate();
	delete pStockSection;
	pStockSection = NULL;

	NCM_PROJECT.GetGlobalState().Reset();
//Clear Reports
	delete pRepArray;
	pRepArray = new NRepReportsArray;
	pRepArray->SetUnit(GetUnit());
	
//Clear ToolList
	
	for each (const auto & el in NToolList)
	{
		NToolCombined* pMyObject = static_cast<NToolCombined*>(el.second);;
		ASSERT_VALID(pMyObject);
		delete pMyObject;
	}
	NToolList.RemoveAll();

	pCurChannel->SetProgrTDTCn(-1);

	ToolSectionDyn.Disable();
	ProgWeldManager.Disable();

	C_OBJECT_MANAGER(ToolSectionDyn)->DeleteActiveObject(&ToolSectionDyn);
}

class NCUnit * CNCMDoc::GetUnit() const
{
	if (pCurChannel == nullptr)
		return nullptr;
	return pCurChannel->GetUnitCn();
}

CMapStringToNProgram* CNCMDoc::NGetProgramList()
{
	return pCurChannel->NGetProgramListCn();
}

const CMapStringToNProgram* CNCMDoc::NGetProgramList() const
{
	return pCurChannel->NGetProgramListCn();
}

void CNCMDoc::AddExobjWU(NStock *ppart)
{
	ppart->WFrame = false;
	NStockStl *pStlPart = NULL;
	CString Name;
	if(ppart->IsKindOf(RUNTIME_CLASS(NStockStl)))
	{
		pStlPart = (NStockStl *)ppart;
		Name = pStlPart->GetFileName();
	}
	ExtractFileName(Name);
	int Index = -1;
	GetUndoHistory().AddAndDo(new UCAddDelFixture(this, CMapStringToNStock::PrefixFixt + Name, Index, ppart, true));
}

void CNCMDoc::AddPartWU(NStock *ppart)
{
	ppart->WFrame = false;
	NStockCont *pContPart = NULL;
	CString Name("");
	if(ppart->IsKindOf(RUNTIME_CLASS(NStockStl)))
	{
		NStockStl *pStlPart = (NStockStl *)ppart;
		Name = pStlPart->GetFileName();
	}
	if(ppart->IsKindOf(RUNTIME_CLASS(NStockCont)))
	{
		Name = ((NStockCont *)ppart)->GetFileName();
		if(Name.IsEmpty())
			Name = _T("Contour");
	}
	ExtractFileName(Name);
	int Index = -1;
	GetUndoHistory().AddAndDo(new UCAddDelPart(this, CMapStringToNStock::PrefixPart + Name, Index, ppart, true));
}

void CNCMDoc::AddStockWU(NStock *pstock, const BMatr& matr, INT_PTR Index /*= -1*/)
{
	if(pStockSection)
		pStockSection->Deactivate();
	pstock->WFrame = !theApp.IsInCuttingMode();
	NStockCont *pContPart = NULL;
	CString Name("");
	Name.LoadString(pstock->IsTemporary() ? IDS_STOCK_TEMP : IDS_STOCK);

	if(pstock->IsKindOf(RUNTIME_CLASS(NStockCont)))
	{
		Name = ((NStockCont *)pstock)->GetFileName();
		if(Name.IsEmpty())
			Name = _T("Contour");
	}
	const bool InternalGroup = GetUndoHistory().StartGroup();
	if (!matr.IsE())
	{
		TransMovObjWU(pstock, matr);
		GetUndoHistory().ResetStockChanged();
	}

	GetUndoHistory().AddAndDo(new UCAddDelStock(this, CMapStringToNStock::PrefixStock + Name, int(Index), pstock, true));
	GetUndoHistory().EndGroup(InternalGroup);
}

void CNCMDoc::DeleteStockWU(NStock* pstock)
{
	GetUndoHistory().AddAndDo(new UCAddDelStock(this, pstock));
}

void CNCMDoc::AddStock(NStock *pstock, INT_PTR Index /*= -1*/)
{
	if(pstock->IsTemporary())
		AddStockWU(pstock, BMatr(), Index);
	else if (IsThereAnyStock())
		AddStockWU(pstock, NStocksList.GetLastStock()->GetMatr(), Index);
	else 
		AddStockWU(pstock, BMatr(), Index);
}

void CNCMDoc::ReplaceStock(INT_PTR Index, NStock *pstock)
{
	NStock* pOldStock = GetStocksList().GetStock(int(Index));
	if (pstock == nullptr)
		return;
	BMatr Matr;
	Matr.SetE();
	if (pOldStock != nullptr)
		Matr = pOldStock->GetMatr();
	const bool InternalGroup = GetUndoHistory().StartGroup();
	DeleteStockWU(pOldStock);
	AddStockWU(pstock, Matr, Index);
	GetUndoHistory().EndGroup(InternalGroup);
}

void CNCMDoc::DrawFixture(RenderMode GlobRMode, bool DrawSurf)
{
	if(PosTool.IsActive())
		if(!NStocksList.empty())
		{
			//BMatr Stock2ToolMatr;
			//GetToolMatr(Stock2ToolMatr); // commented 25.10.19
			PosTool.Draw(GlobRMode);
		}
	bool SectionActive = false;
	if (pStockSection)
		if (pStockSection->IsActive() && pStockSection->NeedCutAll())
			SectionActive = true;
	if (SectionActive)
		pStockSection->EnableClipPlane();
	glPushName(OG_FIXTURES);
	NExObjList.Draw(GlobRMode, DrawSurf);
	glPopName();
	if (SectionActive)
		pStockSection->DisableClipPlane();
}

void CNCMDoc::SetClipPlane()
{
	if (GetSectionActive())
		pStockSection->SetClipPlane();
}

bool CNCMDoc::GetSectionActive()
{
	bool SectionActive = false;
	if (pStockSection)
		if (pStockSection->IsActive())
			SectionActive = true;
	return SectionActive;
}

void CNCMDoc::RemoveClipPlane()
{
	if (GetSectionActive())
		pStockSection->RemoveClipPlane();
}

void CNCMDoc::DrawParts(RenderMode GlobRMode, bool DrawSurf)
{
	if (GetSectionActive())
	{
		pStockSection->EnableClipPlane();
		if (pStockSection->NeedFillSection())
		{
			bool BSharpEdges = NPartsList.GetDrawSharpEdges();
			NPartsList.SetDrawSharpEdges(false);
			pStockSection->BeginTest1();
			NPartsList.Draw(GlobRMode, DrawSurf);
			pStockSection->BeginTest2();
			NPartsList.Draw(GlobRMode, DrawSurf);
			pStockSection->EndTest();
			NPartsList.SetDrawSharpEdges(BSharpEdges);
		}
		glPushName(OG_PARTS);
		NPartsList.Draw(GlobRMode, DrawSurf);
		glPopName();
		pStockSection->DrawClipPlane();
		pStockSection->DisableClipPlane();
	}
	else
	{
		glPushName(OG_PARTS);
		NPartsList.Draw(GlobRMode, DrawSurf);
		glPopName();
	}
}

void CNCMDoc::CreateStocksRender()
{
	NStocksList.CreateStocksRender();
}

void CNCMDoc::CreateModifStocksRender()
{
	NStocksList.CreateModifStocksRender();
}

void CNCMDoc::CreateStocksSectGeom()
{
	for each (const auto & el in NStocksList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		if (pS)
		{
			pS->CreateTurnSkeleton();	
		}
	}
}

void CNCMDoc::CreatePartsSectGeom()
{
	for each (const auto & el in NPartsList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		if (pS)
		{
			pS->CreateTurnSkeleton();
		}
	}
}

void CNCMDoc::DeleteStocks()
{
	NStocksList.DeleteStocks();
	UpdateAllViews(NULL, N_STOCKSTATECHANGE, NULL);
}

void CNCMDoc::DeleteProg(const CString &prog)
{
	if(pCurChannel->DeleteProgCn(prog))
		RegeneratePrograms();
}

void CNCMDoc::DeleteProgObj(NProgram* pProg)
{
	ReleaseSciDoc(pProg);
	NProgram::DeleteObj(pProg);
}

NProgram* CNCMDoc::LoadNewProgram(const CString& FileName)
{
	NProgram* pP = NewProgram();
	CString name = pP->NLoad4LoadNewProgram(FileName, *GetUnit());
	if (name.IsEmpty())
	{
		DeleteProgObj(pP);
		pP = nullptr;
	}
	return pP;
}

bool CNCMDoc::ReloadProgram(NProgram* pProg, const CString& FileName)
{
	bool Res = pProg->NReRead(FileName, *GetUnit());
	if (!Res)
		return false;
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	NTextView* pTextView = (NTextView*)pMainFrame->GetTextView();
	if (pTextView != nullptr)
		pTextView->ClearLoadedProg();
	ReleaseSciDoc(pProg);
	return true;
}

void CNCMDoc::ReleaseSciDoc(NProgram* pProg)
{
	if (pProg == nullptr)
		return;
	void* pSciDoc = pProg->GetSciDoc();
	if (pSciDoc == nullptr)
		return;
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrame == nullptr)
		return;
	NTextView* pTextView = (NTextView*)pMainFrame->GetTextView();
	if (pTextView == nullptr)
		return;
	void* pLoadedSciDoc = (void*)pTextView->SendToScentilla(SCI_GETDOCPOINTER);
	if (pSciDoc == pLoadedSciDoc)
		return;
	pTextView->SendToScentilla(SCI_RELEASEDOCUMENT, 0, (LPARAM)pSciDoc);
	pProg->SetSciDoc(nullptr);
}

COLORREF CNCMDoc::GetStockColor(int index)
{
	NStock * pS = GetStock(index);
	if(pS == NULL)
		return 0;
	return pS->GetColor().GetRGB();
}

void CNCMDoc::SetWFrameToAllStocks(BOOL value)
{
	for each (const auto & el in NStocksList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->WFrame = value;
	}
}

void CNCMDoc::SetTransparentToAllStocks(BOOL value)
{
	for each (const auto & el in NStocksList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Transparent = value;
	}
}

void CNCMDoc::TransformAllStocks(const BMatr &matr)
{
	NStocksList.ResetBodiesAndRendersLists();
	for each (const auto & el in NStocksList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		TransMovObjWU(pS, matr);
	}
	NStocksList.CreateBodiesAndRendersLists();
}

PackedColor CNCMDoc::GetStockColor4(int index)
{
	PackedColor col;
	col.ival = 0;
	NStock * pS = GetStock(index);
	if(pS == NULL)
		return col;
	const NColor &Color = pS->GetColor();
	col.fr = Color.r();
	col.fg = Color.g();
	col.fb = Color.b();
	col.fa = Color.a();
	return col;
}

PackedColor CNCMDoc::GetStocksColor()
{
	PackedColor col;
	col.ival = 0;
	
	const NColor &Color = NStocksList.GetColor();
	col.fr = Color.r();
	col.fg = Color.g();
	col.fb = Color.b();
	col.fa = Color.a();
	return col;
}

void CNCMDoc::SetStockColor(COLORREF inColor, int index)
{
	NStock* pS = GetStock(index);
	if(pS == NULL)
		return;
	NColor buf;
	buf.SetRGB(inColor);
	buf.A() = pS->GetColor().GetA();
	pS->SetColor(buf);
}

NStock* CNCMDoc::GetStock(int index)
{
	return NStocksList.GetStock(index);
}

NStock* CNCMDoc::GetStockUnsafe(int index) const
{
	return NStocksList.GetStockUnsafe(index);
}

NStock* CNCMDoc::GetFirstVisStock() const
{
	for (int i = 0; i < GetStocksList().GetSize(); ++i)
	{
		NStock* pStock = GetStockUnsafe(i);
		if (pStock != nullptr)
			if (pStock->IsVisible())
				return pStock;
	}
	return nullptr;
}

bool CNCMDoc::ReloadAllProg()
{
	// Reload all programs from the corresponding files
	// create all child items
	GetUnit()->Reset();
	NMapStringToOb *plist = NGetProgramList();
	CString mes;
	CFile tmp_file;
	for each (const auto & el in *plist)
	{
		const CString& key = el.first;
		NProgram* pP = static_cast<NProgram*>(el.second);
		if (pP->IsManChanged())
		{
			mes.Format(IDS_MES_PROG_CHANGED, key);
			if (AfxMessageBox(mes, MB_YESNO) != IDYES)
				continue; // Do not reload
		}
		if (ReloadProgram(pP, key))
		{
			ImplRepl(pP);
		}
		else // Can't open file
		{
			mes.Format(IDS_MES_DEL_PROG, key);
			if (AfxMessageBox(mes, MB_YESNO) == IDYES)
			{
				DeleteProg(key);
			}
		}
	}
	RegeneratePrograms();
	UpdateAllViews(NULL, N_LEFT | N_NEW_TEXT | N_OGL | N_STATE | N_PROGRAMCHANGED);
	return true;
}

void CNCMDoc::FillMUPair(void)
{
	if (pMUPair == nullptr)
		pMUPair = new NMachUnitPair(GetMachTool(), *GetUnit());
	else
		pMUPair->Set(GetMachTool(), *GetUnit());
}

void CNCMDoc::ReadJobFile(CArchive &ar)
{
	PrepReadJob();

	NCUnitState StartState;
	NTool *pToolHolder = NULL; 
	CString str;
	CString key;
	NCMApplicationType AppType = AT_MILL;
	bool ToolPointType = true; // true - ToolCenter; false - ToolTip
	int ProgNum2Load = 0;
	int FirstChangedProgNum = -1;
	GetUnit()->Reset();
	NCUnitState * pOldStartState = &GetUnit()->GetStartState();
	if (pOldStartState == nullptr)
		return;
	bool UnitChanged = false;
	bool MachineChanged = false;
	bool StartStateChanged = false;
	bool ProgramsDeleted = false;
	int NewStocks = 0;
	while(ar.ReadString(str))
	{
		key = str.SpanExcluding(" \t");
		str.Delete(0, key.GetLength());
		str.TrimLeft();
		str.TrimRight();
		if(key == ViewName)
		{
			NOpenGLWnd::ViewName = str;
		}
		if(key == Operation)
		{
			if(str == Turning)
				AppType = AT_TURN;
			else if(str == MillTurn)
				AppType = AT_MILL_TURN;
			else if(str == Welding)
				AppType = AT_WELD;
			if(NCMComData::GetType() != AppType)
			{
				DeleteContents();
				theApp.ChangeType(AppType);
				pOldStartState = &GetUnit()->GetStartState();
				if (pOldStartState == nullptr)
					return;
			}
			continue;
		}
		if(key == ControllerFile)
		{
		// Try to set and load ConfFile
			if(GetUnit()->GetConfFileName() != str)
			{
				UnitChanged = true;
				int Pos = str.ReverseFind('\\');
				if(Pos > 0)
				{
					CString Path = str.Left(Pos + 1);
					NCM_PROJECT.GetCommonPaths().SetPath(CF_UNITS, Path);
					str.Delete(0, Pos + 1);
				}
				if (GetUnit()->SetDef(str) >= 0)
					static_cast<CMainFrame*>(AfxGetMainWnd())->SetNCUnitName(GetUnit()->GetConfFileName());
			}
			continue;
		}
		if(key == MachineFile)
		{
			if(GetMachTool().GetFileName() != str)
			{
				MachineChanged = true;
				int Pos = str.ReverseFind('\\');
				if(Pos > 0)
				{
					CString Path = str.Left(Pos + 1);
					NCM_PROJECT.GetCommonPaths().SetPath(CF_MACHINES, Path);
					str.Delete(0, Pos + 1);
				}
				GetMachTool().SetFileName(str);
				continue;
			}
		}
		if (key == Gcodefile || key == Gcodefilesub)
		{
			if(FirstChangedProgNum < 0)
			{
				// Try to prevent program reloading and regeneration
				CString key;
				NProgram *pProg;
				if(NGetProgramList()->GetAt(ProgNum2Load, key, (CObject *&)pProg))
				{
					CString buf(str);
					if(key.MakeUpper() == buf.MakeUpper())
					{
						if(!pProg->IsManChanged())
						{
							CStdioFile f;
							if(f.Open(str, CFile::typeText | CFile::modeRead ))
							{
								FILETIME Creation, Access, Write;
								if(GetFileTime(f, &Creation, &Access, &Write))
								{
									CTime NewFileTime(Write);
									if(NewFileTime == pProg->GetFileTime())
									{
										++ProgNum2Load;
										continue;
									}
								}
							}
						}
					}
				}
				FirstChangedProgNum = ProgNum2Load;
				for(int i = int(NGetProgramList()->GetSize() - 1); i >= ProgNum2Load; --i)
				{
					ProgramsDeleted = true;
					if(NGetProgramList()->GetAt(i, key, (CObject *&)pProg))
					{
						DeleteProgObj(pProg);
						NGetProgramList()->RemoveAt(i);
					}
				}
			}

		// Try to load and add program
			NProgram *pP = LoadNewProgram(str);
			if(pP == nullptr)
			{
				CString mes;
				mes.Format(IDS_MES_ERR_PR,str);
				AfxMessageBox(mes);
				DeleteProgObj(pP);
			}
			else
			{
				if (key == Gcodefile)
					NAddMainProgram(str, pP);
				else
					NAddAuxProgram(str, pP);
				++ProgNum2Load;
//				RegeneratePrograms();
			}
			continue;
		}
		if(key == StockBox)
		{
			++NewStocks;
			BBox Gabar;
			double x, y, z;
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			x = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			y = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			z = atof(buf);

			Gabar.Expand(x, y, z);

			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			x = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			y = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			z = atof(buf);

			Gabar.Expand(x, y, z);

			if(AppType != AT_MILL)
			{
				AfxMessageBox(IDS_MES_ERR_STOCK);
				continue;
			}
			NStockBox *pStock = new NStockBox;
			if(!pStock->Create(Gabar))
			{
				AfxMessageBox(IDS_MES_ERR_ST);
			}
			ReplaceStock(NewStocks - 1, pStock);
			continue;
		}
		if(key == StockCylinder)
		{
			++NewStocks;
			double x, y, z, x1, y1, z1, d;
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			x = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			y = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			z = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			x1 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			y1 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			z1 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			d = atof(buf);

			BPoint e = BPoint(x1, y1, z1, 1.) - BPoint(x, y, z, 1.);
			double h = sqrt(e.D2());

			int Axis = 0;// the number of the biggest coord of e
			double b = e.GetX();
			if(e.GetY() > b)
			{
				Axis = 1;
				b = e.GetY();
			}
			if(e.GetZ() > b)
				Axis = 2;

			if(AppType == AT_MILL)
			{
				NStockCyl *pStock = new NStockCyl;
				pStock->Create(x, y, z, d, h, Axis);
				ReplaceStock(NewStocks - 1, pStock);
			}
			else // Turning or MillTurn
			{
				if(Axis != 2)
				{
					AfxMessageBox(IDS_MES_ERR_FILE3);
					continue;
				}
				NStockBar *pStock = new NStockBar;
				pStock->Create(0, d, d, 0., 0., h, 0., BPoint(x, y, z, 1.), 2);
				ReplaceStock(NewStocks - 1, pStock);
			}
			continue;
		}
		if(key == StockGen)// NStockBar class
		{
			++NewStocks;
			double z, Ed0, Ed1, Id0, Id1, L, Ra;
			int Pn;
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			z = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			Ed0 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			Ed1 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			Id0 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			Id1 = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			L = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			Pn = atoi(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			Ra = atof(buf);

			if(AppType == AT_MILL)
			{
				AfxMessageBox(IDS_MES_ERR_FILE3);
			}
			else // Turning or MillTurn
			{
				NStockBar *pStock = new NStockBar;
				pStock->Create(Pn, Ed0, Ed1, Id0, Id1, L, Ra, BPoint(0., 0., z, 1.), 2);
				ReplaceStock(NewStocks - 1, pStock);
			}
			continue;
		}
		if(key == StockStl || key == DesignModel)
		{
		// Try to load stock stl file
			if(AppType != AT_MILL && key == StockStl)
			{
				AfxMessageBox(IDS_MES_ERR_FILE3);
				continue;
			}
			if(str.Find('*'))
			{
				CFileFind search;
				if(!search.FindFile(str))
					continue;
				if(!search.FindNextFile())
					str = search.GetFilePath();
				else // There are more than one file matching str
				{
					search.Close();
					CFileDialog fd(TRUE,NULL,str,NULL,NULL);
					CString tmp;
					tmp.LoadString(IDS_STOCK_M);
					fd.m_ofn.lpstrTitle = tmp;
					if(fd.DoModal() == IDOK)
						str = fd.GetPathName();
					else
						continue;
				}
			}
			if(key == StockStl)
			{
			// Prevent stock reload if file is not changed
				++NewStocks;
				NStock *pOldStock = GetStock(NewStocks - 1);
				if(pOldStock)
				{
					if(pOldStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
					{
						CString OldFileName(((NStockStl *)pOldStock)->GetFileName());
						if(OldFileName.MakeUpper() == str.MakeUpper())
						{
							CStdioFile f;
							if(f.Open(str, CFile::typeText | CFile::modeRead ))
							{
								FILETIME Creation, Access, Write;
								if(GetFileTime(f, &Creation, &Access, &Write))
								{
									CTime NewFileTime(Write);
									if(NewFileTime == ((NStockStl *)pOldStock)->GetFileTime())
										continue;
								}
							}
						}
					}
				}
				NStockStl *pNewStock = new NStockStl;
				pNewStock->CreateEx(str);
				if(pNewStock->IsCreated)
					ReplaceStock(NewStocks - 1, pNewStock);
				else
					delete pNewStock;
			}
			else
			{
				NStockStl *pNewStock = new NStockStl;
				pNewStock->CreateEx(str);
				if(pNewStock->IsCreated)
					AddPartWU(pNewStock);
				else
					delete pNewStock;
			}
			continue;
		}
		if( key == StockExtrude || key == DesignExtrude)
		{
		// Try to load stock contour file
			if(AppType != AT_MILL && key == StockExtrude)
			{
				AfxMessageBox(IDS_MES_ERR_FILE3);
				continue;
			}

			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			CString Format = buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			CString ContStr = buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Zmin = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Zmax = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double RotX = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double RotY = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double RotZ = atof(buf);

			StContOut Import;
			size_t Size = 0;
			Format.MakeUpper();
			if(Format == _T("FIL"))
			{
				int Pos = ContStr.ReverseFind('.');
				if(Pos > 0)
				{
					Format = ContStr.Right(ContStr.GetLength() - Pos - 1);
					Format.MakeUpper();
					Size = Import.ReadFile(ContStr, ContStr);
				}
			}
			if(Size == 0)
			{
				Size = 0;
				for(int i = 0; i >= 0; i = ContStr.Find('G', i + 1))
				{
					++Size;
				}
			}
			NCadrGeom *GeomArr = NULL;
			Size = Import.ReadFromStr(GeomArr, Size, ContStr);

			NStockSweep *pNewStock = new NStockSweep;
			BPoint Vector = BPoint(0., 0., Zmax - Zmin, 0.); 
			pNewStock->Create(ContStr, GeomArr, int(Size), BPoint(0.,0.,Zmin,1.), Vector, BPoint(RotX, RotY, RotZ, 1.));
			delete [] GeomArr;
			if(pNewStock->IsCreated)
			{
				if(key == StockExtrude)
				{
					++NewStocks;
					ReplaceStock(NewStocks - 1, pNewStock);
				}
				else
					AddPartWU(pNewStock);
			}
			else
				delete pNewStock;
			continue;
		}
		if( key == StockContour || key == DesignContour)
		{
		// Try to load stock contour file
			if(AppType == AT_MILL)
			{
				AfxMessageBox(IDS_MES_ERR_FILE3);
				continue;
			}
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			CString Format = buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			CString ContStr = buf;

			StContOut Import;// ADD ROT AXIS
			size_t Size = 0;
			Format.MakeUpper();
			if(Format == _T("FIL"))
			{
				int Pos = ContStr.ReverseFind('.');
				if(Pos > 0)
				{
					Format = ContStr.Right(ContStr.GetLength() - Pos - 1);
					Format.MakeUpper();
					Size = Import.ReadFile(ContStr, ContStr);
				}
			}
			if(Size <= 0)
			{
				Size = 0;
				for(int i = 0; i >= 0; i = ContStr.Find('G', i + 1))
				{
					++Size;
				}
			}
			NCadrGeom *GeomArr = NULL;
			Size = Import.ReadFromStr(GeomArr, Size, ContStr);

			BBox Box;
			if(Box.Xmin < 0.)
			{
				AfxMessageBox(IDS_STOCK_Q);  
				return;
			}

			NStockCont *pNewStock = new NStockCont();
			pNewStock->Create(GeomArr, int(Size), BPoint(0.,0.,0.,1.), CString(_T("")), false);
			delete [] GeomArr;
			if(pNewStock->IsCreated)
			{
				if(key == StockContour)
				{
					++NewStocks;
					ReplaceStock(NewStocks - 1, pNewStock);
				}
				else
					AddPartWU(pNewStock);
			}
			else
				delete pNewStock;
			continue;
		}
		if( key == DesignModel)
		{
		// Try to load part stl file
			continue;
		}
		if( key == ToolPointType )
		{
			if(str.Left(7) == ToolTip)
				ToolPointType = false;
			else if(str.Left(10) == ToolCenter)
				ToolPointType = true;
			continue;
		}
		if( key == Holder )
		{
			if(str.Left(5) == Empty)
			{
				if(pToolHolder)
				{
					delete pToolHolder;
					pToolHolder = NULL;
				}
				continue;
			}
			if(str.Left(4) == Mill)
			{
				str.Delete(0,4);
				str.TrimLeft("\t ");
				CString buf;
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				double DTop = atof(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				double DBot = atof(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				double H = atof(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				double Hc = atof(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				double ShiftZ = atof(buf);
				if(pToolHolder)
					delete pToolHolder;
				if (Hc == 0.) // инструмент без фаски
				{
					double alf = atan((DTop - DBot)/(2. * H));
					alf = alf *180./PI;
					pToolHolder = new NToolStd(CString("Holder")
						, DBot, H, alf, 0., 0., 0., 0., H);
					if (DBot == DTop)
						((NToolMill *)pToolHolder)->SetType(41);
					else
						((NToolMill *)pToolHolder)->SetType(42);
				}
				else // инструмент с фаской
				{
					double alf = atan((DTop - DBot)/(2. * Hc));
					alf = alf *180./PI;
					alf = 90 - alf;
					double faset = (DTop - DBot)/2.;
					pToolHolder = new NToolStd(CString("Holder")
						, DTop, H, 0., faset, alf, 0., 0., H);
					((NToolMill *)pToolHolder)->SetType(43);
				}
				if(pToolHolder->GenInit() != 0)
				{
					AfxMessageBox(IDS_MES_TOOL);
					delete pToolHolder;
					pToolHolder = NULL;
					continue;
				}
				((NToolMill *)pToolHolder)->SetZdisp(ShiftZ);
				continue;
			}
			if(str.Left(4) == Turn)
			{
				str.Delete(0,4);
				str.TrimLeft("\t ");
				if(str.Left(3) == Bar)
				{
					str.Delete(0,3);
					str.TrimLeft("\t ");

					CString buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Dx = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Dy = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Dz = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftY = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					if(pToolHolder)
						delete pToolHolder;
					pToolHolder = new NToolTurnHolderBar("HolderBar"
						, Dx, Dy, Dz, ShiftY, MountAngle, ShiftX, 0., ShiftZ);
					if(pToolHolder->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pToolHolder;
						pToolHolder = NULL;
						continue;
					}
					continue;
				}
				if(str.Left(3) == Cyl)
				{
					str.Delete(0,3);
					str.TrimLeft("\t ");

					CString buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Dx = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double D1 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double D2 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int FH = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftY = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					if(pToolHolder)
						delete pToolHolder;
					pToolHolder = new NToolTurnHolderCyl("HolderCyl"
						, Dx, ShiftY, D1, D2, FH, MountAngle, ShiftX, 0., ShiftZ);
					if(pToolHolder->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pToolHolder;
						pToolHolder = NULL;
						continue;
					}
					continue;
				}
				if (str.Left(3) == Stl)
				{
					str.Delete(0, 3);
					str.TrimLeft("\t ");

					CString buf;
					buf = str.SpanExcluding("\t "); str.Delete(0, buf.GetLength()); str.TrimLeft();
					CString FileName = buf;
					buf = str.SpanExcluding("\t "); str.Delete(0, buf.GetLength()); str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t "); str.Delete(0, buf.GetLength()); str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t "); str.Delete(0, buf.GetLength()); str.TrimLeft();
					double ShiftY = atof(buf);
					buf = str.SpanExcluding("\t "); str.Delete(0, buf.GetLength()); str.TrimLeft();
					double ShiftZ = atof(buf);
					if (pToolHolder)
						delete pToolHolder;
					pToolHolder = new NToolTurnHolderSTL("HolderSTL"
						, FileName, MountAngle, ShiftX, ShiftY, ShiftZ);
					if (pToolHolder->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pToolHolder;
						pToolHolder = NULL;
						continue;
					}
					continue;
				}
				continue;
			}
			continue;
		}
		if( key == ComTool )// Complex tool
		{
			if(str.Left(13) == ComToolColors)
			{
				str.Delete(0,13);
				str.TrimLeft("\t ");
				CString buf;
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				int Position = atoi(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				int Turret = atoi(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				COLORREF CutColor = atoi(buf);
				buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
				COLORREF NonCutColor = atoi(buf);
				NToolID ToolID;
				NToolCombined *pToolCon = NULL;
				ToolID.Set(Position, Turret);
				if(!NGetToolList()->Lookup(ToolID, pToolCon))
				{
					pToolCon = new NToolCombinedMill;
				}
				pToolCon->SetColor(NColor().SetRGB(CutColor));
				pToolCon->SetColorPassive(NColor().SetRGB(NonCutColor));
				pToolCon->SetColorHolder(NColor().SetRGB(NonCutColor));
				NToolList[ToolID] = pToolCon;
				continue;
			}
			if(str.Left(4) == Mill || str.Left(4) == Turn)
			{
				bool IsTrueMill = (str.Left(4) == Mill);
				if(AppType == AT_MILL && !IsTrueMill)
					IsTrueMill = true;
				if(AppType == AT_TURN && IsTrueMill)
					IsTrueMill = false;
				str.Delete(0,4);
				str.TrimLeft("\t ");
				if(str.Left(8) == CuttCont || str.Left(8) == HoldCont || str.Left(8) == VertCont)
				{
					bool CutCont = (str.Left(8) == CuttCont);
					bool VerCont = (str.Left(8) == VertCont);
					str.Delete(0,8);
					str.TrimLeft("\t ");
					CString buf, ContStr;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					CString Format = buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Position = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Turret = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Axis = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					ContStr = buf;
//					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
//					COLORREF Color = atoi(buf);
					StContOut Import;// ADD ROT AXIS
					size_t Size = 0;
					Format.MakeUpper();
					if(Format == _T("FIL"))
					{
						int Pos = ContStr.ReverseFind('.');
						if(Pos > 0)
						{
							Format = ContStr.Right(ContStr.GetLength() - Pos - 1);
							Format.MakeUpper();
							Size = Import.ReadFile(ContStr, ContStr);
						}
					}
					if(Size <= 0)
					{
						Size = 0;
						for(int i = 0; i >= 0; i = ContStr.Find('G', i + 1))
						{
							++Size;
						}
					}
					NCadrGeom *GeomArr = NULL;
					Size = Import.ReadFromStr(GeomArr, Size, ContStr);

					--Size;// Contour should not be closed
					NToolID ToolID;
					NToolCombined *pToolCon = NULL;
					ToolID.Set(Position, Turret);
					if(NGetToolList()->Lookup(ToolID, pToolCon))
					{
						if(!pToolCon->IsKindOf(RUNTIME_CLASS(NToolCombinedMill)))
						{
							NToolCombined *pToolNew = new NToolCombinedMill;
							pToolNew->Color = pToolCon->Color;
							pToolNew->ColorPassive = pToolCon->ColorPassive;
							delete pToolCon;
							pToolCon = pToolNew;
						}
					}
					if(!pToolCon)
						pToolCon = new NToolCombinedMill;

					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->Attach = Axis;
					pToolCon->SetName("ContTool");
					NContour ToolCont;
					ToolCont.MakeFromCadrGeom(GeomArr, int(Size));
					delete[] GeomArr;
					ToolCont.ExchangeYZ();
					if (VerCont)
					{
						if (!((NToolCombinedMill *)pToolCon)->MakeVertTool(ToolCont, true))
						{
							AfxMessageBox(IDS_MES_TOOL);
							delete pToolCon;
							continue;
						}

					}
					else
						((NToolCombinedMill *)pToolCon)->MakeShapedTools(ToolCont, CutCont);
					if(!IsTrueMill)
					{
						NToolCombinedTurn *pToolComTurn = ((NToolCombinedMill *)pToolCon)->Convert2TurnTool();
						delete pToolCon;
						pToolCon = pToolComTurn;
					}
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				continue;
			}
			if(str.Left(6) == Insert)
			{
				str.Delete(0, 6);
				str.TrimLeft("\t ");
				if(AppType != AT_TURN && AppType != AT_MILL_TURN)
					continue;
				if(str.Left(8) == CuttCont || str.Left(8) == HoldCont)
				{
					bool CutCont = (str.Left(8) == CuttCont);
					str.Delete(0,8);
					str.TrimLeft("\t ");
					CString buf, ContStr;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					CString Format = buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Position = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Turret = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Axis = atoi(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					ContStr = buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Thick = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Angle = atof(buf);
					StContOut Import;// ADD ROT AXIS
					size_t Size = 0;
					Format.MakeUpper();
					if(Format == _T("FIL"))
					{
						int Pos = ContStr.ReverseFind('.');
						if(Pos > 0)
						{
							Format = ContStr.Right(ContStr.GetLength() - Pos - 1);
							Format.MakeUpper();
							Size = Import.ReadFile(ContStr, ContStr);
						}
					}
					if(Size <= 0)
					{
						Size = 0;
						for(int i = 0; i >= 0; i = ContStr.Find('G', i + 1))
						{
							++Size;
						}
					}
					NCadrGeom *GeomArr = NULL;
					Size = Import.ReadFromStr(GeomArr, Size, ContStr);

					NToolID ToolID;
					NToolCombined *pToolCon = NULL;
					ToolID.Set(Position, Turret);
					if(NGetToolList()->Lookup(ToolID, pToolCon))
					{
						if(!pToolCon->IsKindOf(RUNTIME_CLASS(NToolCombinedTurn)))
						{
							NToolCombined *pToolNew = new NToolCombinedTurn;
							pToolNew->Color = pToolCon->Color;
							pToolNew->ColorPassive = pToolCon->ColorPassive;
							delete pToolCon;
							pToolCon = pToolNew;
						}
					}
					if(!pToolCon)
						pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if (pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive);
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->Attach = Axis;
					pToolCon->SetName("ContTool");
					TBladeCont ToolCont;
					ToolCont.MakeContour(GeomArr, int(Size));
					delete[] GeomArr;
					((NToolCombinedTurn *)pToolCon)->MakeShapedTools(ToolCont, CutCont, Thick, Angle);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				continue;
			}
		}
		if( key == Tool )
		{
			if(str.Left(4) == Mill)
			{
				str.Delete(0,4);
				str.TrimLeft("\t ");
				if(str.Left(4) == Apt7)
				{
					str.Delete(0,4);
					str.TrimLeft("\t ");
				// Define Apt7 tool:Num;Diameter;CornerRadius;ELength;FLength;
				//                  BottomAngle;SideAngle;Height;[ToolSetLength]
					int Num;
					double Diameter,CornerRadius,ELength,FLength;
					double BottomAngle,SideAngle,Height,ToolSetLength;
					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					Diameter = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					CornerRadius = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					ELength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					FLength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					BottomAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					SideAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					Height = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					ToolSetLength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Attach = GetAttachNum(buf);

					if(ToolSetLength == 0.)
						ToolSetLength = Height;
					NToolStd *pTool = new NToolStd("TOOL"+n,Diameter,
						ToolSetLength,SideAngle,CornerRadius,BottomAngle,0.,0.,Height);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedMill;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->Attach = Attach;
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				continue;
			}
			if(str.Left(4) == Turn)
			{
				str.Delete(0,4);
				str.TrimLeft("\t ");
				if(str.Left(4) == Apt7)
				{
					str.Delete(0,4);
					str.TrimLeft("\t ");
				// Define Apt7 tool:Num;Diameter;CornerRadius;ELength;FLength;
				//                  BottomAngle;SideAngle;Height;[ToolSetLength]
					int Num;
					double Diameter,CornerRadius,ELength,FLength;
					double BottomAngle,SideAngle,Height,ToolSetLength;
					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					Diameter = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					CornerRadius = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					ELength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					FLength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					BottomAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					SideAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					Height = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					ToolSetLength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Attach = GetAttachNum(buf);

					if(ToolSetLength == 0.)
						ToolSetLength = Height;
					NToolStd *pTool = new NToolStd("TOOL"+n,Diameter,
						ToolSetLength,SideAngle,CornerRadius,BottomAngle,0.,0.,Height);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					NTool * pToolTurn = new NToolTurnBladeMill;
					((NToolTurnBladeMill *)pToolTurn)->SetMillTool(pTool);
					pToolTurn->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					if(pToolTurn->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pToolTurn;
						continue;
					}
					pToolCon->AddTool(pToolTurn);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->Attach = Attach;
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(3) == ISO)
				{
					str.Delete(0,3);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					CString Form = buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double L1 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double L2 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double AngleC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Height = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Edge = atoi(buf);
					NToolTurnBladeISO *pTool = new NToolTurnBladeISO("Blade"
						, Form
						, L1, L2, NoseR, AngleC, Height, MountAngle, ShiftX, ShiftZ
						, ToolPointType
						, Edge);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(6) == Square)
				{
					str.Delete(0,6);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double InscC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double TSL = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					NToolTurnBladeISO *pTool = new NToolTurnBladeISO("Blade"
						, "S"
						, InscC, InscC, NoseR, 0, 0, MountAngle, ShiftX, ShiftZ
						, ToolPointType
						, 0);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(8) == Triangle)
				{
					str.Delete(0,8);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double InscC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double TSL = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					double L = sqrt(3.) * InscC / 4.;
					NToolTurnBladeISO *pTool = new NToolTurnBladeISO("Triangle"
						, "T"
						, L, L, NoseR, 0, 0, MountAngle, ShiftX, ShiftZ
						, ToolPointType
						, 0);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(6) == Thread)
				{
					str.Delete(0,6);
					str.TrimLeft("\t ");
					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ThreadAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double CutWhide = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double CutLength = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double HalfT = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double R = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double AngleC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Height = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					NToolTurnBladeThr *pTool = new NToolTurnBladeThr("Thread"
						, ThreadAngle
						, CutWhide, CutLength, HalfT, R, AngleC, Height, MountAngle, ShiftX, ShiftZ
						, ToolPointType);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(7) == Diamond)
				{
					str.Delete(0,7);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double InscC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseA = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double TSL = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					double Arad = NoseA * PI / 180.;
					double L = InscC * (1. / tan(Arad) + tan(Arad));
					CString ISOType;
					ISOType.Format("C=%lf", NoseA);
					NToolTurnBladeISO *pTool = new NToolTurnBladeISO("Diamond"
						, ISOType
						, L, L, NoseR, 0, 0, MountAngle, ShiftX, ShiftZ
						, ToolPointType
						, 0);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(4) == Quad)
				{
					str.Delete(0,4);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double AngleA = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double AngleB = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double L1 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double L2 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double AngleC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Height = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					NToolTurnBladeOut *pTool = new NToolTurnBladeOut("Blade"
						, AngleA, AngleB
						, L1, L2, NoseR, AngleC, Height, MountAngle, ShiftX, ShiftZ
						, ToolPointType);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(6) == Circle)
				{
					str.Delete(0,6);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double D = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double TSL = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					NToolTurnBladeCirc *pTool = new NToolTurnBladeCirc("Circle"
						, D * 0.5, TSL
						, ShiftX, ShiftZ
						, ToolPointType);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				if(str.Left(6) == Groove)
				{
					str.Delete(0,6);
					str.TrimLeft("\t ");

					CString n,buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Num = atoi(buf);
					n=buf;
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Width = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Length = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double MountAngle = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double TSL = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseA = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double NoseR1 = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double AngleC = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double Height = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftX = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					double ShiftZ = atof(buf);
					buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
					int Edge = atoi(buf);
					NToolTurnBladeCut *pTool = new NToolTurnBladeCut("Groove"
						, NoseR, Width, Length
						, MountAngle, TSL, NoseA, NoseR1, AngleC, Height
						, ShiftX, ShiftZ
						, ToolPointType
						, Edge);
					if(pTool->GenInit() != 0)
					{
						AfxMessageBox(IDS_MES_TOOL);
						delete pTool;
						continue;
					}
					NToolID ToolID;
					NToolCombined *pOldTool;
					ToolID.Set(Num, 0);
					if(NGetToolList()->Lookup(ToolID,pOldTool))
						delete pOldTool;
					NToolCombined *pToolCon = new NToolCombinedTurn;
					pToolCon->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
					if(pToolHolder)
					{
						pToolHolder->SetColors(pToolCon->ColorPassive, pToolCon->ColorPassive); 
						pToolCon->AddTool(pToolHolder);
						pToolHolder = pToolHolder->CreateCopyTool();
					}
					pTool->SetColors(pToolCon->Color, pToolCon->ColorPassive); 
					pToolCon->AddTool(pTool);
					pToolCon->Position = ToolID.GetToolNum();
					pToolCon->Turret = ToolID.GetTurretNum();
					pToolCon->SetName("TOOL" + n);
					pToolCon->CalcCompShift();
					NToolList[ToolID] = pToolCon;
					continue;
				}
				continue;
			}
			continue;
		}
		if(key == StartToolNumber)
		{
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			int Num = atoi(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			int Tur = atoi(buf);

			StartState.CurToolN.Set(Num, Tur);
			NCUnitState& OldStartState = *pOldStartState;
			if(OldStartState.CurToolN != StartState.CurToolN)
			{
				OldStartState.CurToolN = StartState.CurToolN;
				StartStateChanged = true;
			}
		}
		if(key == StartToolPosition)
		{
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double X = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Y = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Z = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double A = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double B = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double C = atof(buf);

			StartState.MashcX = StartState.MashX = X;
			StartState.MashcY = StartState.MashY = Y;
			StartState.MashcZ = StartState.MashZ = Z;
			StartState.MashA = A;
			StartState.MashB = B;
			StartState.MashC = C;
			NCUnitState& OldStartState = *pOldStartState;
			if(
				StartState.MashX != OldStartState.MashX ||
				StartState.MashY != OldStartState.MashY ||
				StartState.MashZ != OldStartState.MashZ ||
				StartState.MashA != OldStartState.MashA ||
				StartState.MashB != OldStartState.MashB ||
				StartState.MashC != OldStartState.MashC
				)
			{
				OldStartState.MashX = StartState.MashX;
				OldStartState.MashY = StartState.MashY;
				OldStartState.MashZ = StartState.MashZ;
				OldStartState.MashA = StartState.MashA;
				OldStartState.MashB = StartState.MashB;
				OldStartState.MashC = StartState.MashC;
				StartStateChanged = true;
			}
		}
		if(key == Hcode)
		{
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			int Num = atoi(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Val = atof(buf);
			
			StartState.HComp[Num] = Val;
		}
		if(key == Dcode)
		{
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			int Num = atoi(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Val = atof(buf);
			
			StartState.DComp[Num] = Val;
		}
		if(key == CompensationTurn)
		{
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			int Num = atoi(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double shiftX = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double shiftZ = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			int Direction = atoi(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Value = atof(buf);

			NCUnitState &StartState = GetUnit()->GetStartState();
			if(
				StartState.DComp[Num] != Value ||
				StartState.DComp[1000 + Num] != Direction ||
				StartState.DComp[2000 + Num] != shiftX ||
				StartState.DComp[3000 + Num] != shiftZ
			)
			{
				StartState.DComp[Num] = Value;
				StartState.DComp[1000 + Num] = Direction;
				StartState.DComp[2000 + Num] = shiftX;
				StartState.DComp[3000 + Num] = shiftZ;
				StartStateChanged = true;
			}
		}
		if(key == FixOffset)
		{
			str.TrimLeft("\t ");
			CString buf;
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			CString Name(buf);

			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double X = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Y = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double Z = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double A = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double B = atof(buf);
			buf = str.SpanExcluding("\t ");str.Delete(0,buf.GetLength());str.TrimLeft();
			double C = atof(buf);

			NUCSs &UCSs = GetUnit()->GetStartState().UCSs;
			int Ind = -1;
			if(Name == G54)
				Ind = 0;
			else if(Name == G55)
				Ind = 1;
			else if(Name == G56)
				Ind = 2;
			else if(Name == G57)
				Ind = 3;
			else if(Name == G58)
				Ind = 4;
			else if(Name == G59)
				Ind = 5;
			if(Ind >= 0 && Ind < 6)
			{
				if(
					UCSs[Ind].x0 != X ||
					UCSs[Ind].y0 != Y ||
					UCSs[Ind].z0 != Z ||
					UCSs[Ind].a0 != A ||
					UCSs[Ind].b0 != B ||
					UCSs[Ind].c0 != C
				)
				{
					UCSs[Ind].x0 = X;
					UCSs[Ind].y0 = Y;
					UCSs[Ind].z0 = Z;
					UCSs[Ind].a0 = A;
					UCSs[Ind].b0 = B;
					UCSs[Ind].c0 = C;
					StartStateChanged = true;
					GetUnit()->GetStartState().ResetRegisters(); // To transfer new UCSs values to registers
				}
			}
		}
	}
	NCUnitState& OldStartState = *pOldStartState;
	if(OldStartState.DComp != StartState.DComp)
	{
		OldStartState.DComp = StartState.DComp;
		StartStateChanged = true;
	}
	if(OldStartState.HComp != StartState.HComp)
	{
		OldStartState.HComp = StartState.HComp;
		StartStateChanged = true;
	}

	if (NewStocks == 0)
		DeleteStocks();
	else
	{
		for (auto i = NStocksList.GetSize() - 1; i >= NewStocks; --i)
			NStocksList.DeleteStockByInd(i);
	}
	//Clear ProgramList
	NProgram *pProg;
	for(int i = int(NGetProgramList()->GetSize() - 1); i >= ProgNum2Load; --i)
	{
		ProgramsDeleted = true;
		if(NGetProgramList()->GetAt(i, key, (CObject *&)pProg))
		{
			DeleteProgObj(pProg);
			NGetProgramList()->RemoveAt(i);
		}
	}
	if(NGetProgramList()->GetAt(0, key, (CObject *&)pProg))
		SetProgrTDT(0);
// Update machine
	if(GetUnit() && MachineChanged)
		if(!GetMachTool().GetFileName().IsEmpty())
		{
		if (!LoadMachTool(NCM_PROJECT.GetMachinesPath(), GetMachTool().GetFileName()))
			{
				CString Mess;
				Mess.Format(IDS_MES_ERR_FILE2, NCM_PROJECT.GetMachinesPath() + GetMachTool().GetFileName());
				AfxMessageBox(Mess);
			}
		}
// Prepare tool list
	NPassiveObject *pObj;
	for(int i = 0; i < NToolList.GetSize(); ++i)
	{
		NToolList.GetAt(i, key, (CObject *&)pObj);
		((NToolCombined *)pObj)->SetChildrenColors();
		pObj->SetGroup(OG_TOOLS);
	}

// Prepare parts list
	for(int i = 0; i < NPartsList.GetSize(); ++i)
	{
		NPartsList.GetAt(i, key, (CObject *&)pObj);
		if(((NStock *)pObj)->IsCreated)
		{
			pObj->SetGroup(OG_PARTS);
		}
		else
		{
			NPartsList.RemoveKey(key);
			--i;
		}
	}

// Prepare ex obj list
	for(int i = 0; i < NExObjList.GetSize(); ++i)
	{
		NExObjList.GetAt(i, key, (CObject *&)pObj);
		if(((NStock *)pObj)->IsCreated)
		{
			pObj->SetGroup(OG_FIXTURES);
		}
		else
		{
			NExObjList.RemoveKey(key);
			--i;
		}
	}
	if(NCMComData::GetType() == AT_WELD)
	{
		InitToolSection();
		InitProgWeldManager();
	}
	if(StartStateChanged || MachineChanged || FirstChangedProgNum >= 0 || ProgramsDeleted)
	{
		GetUnit()->SetDef(GetUnit()->GetConfFileName());
		ImplReplAll();
		GetUnit()->Reset();
		RegeneratePrograms();
		UpdateAllViews(NULL, N_STATE | N_SETTINGS | N_UNIT_CHANGED | N_TOOLCHANGED | N_LEFT | N_PROGRAMCHANGED);
	}
	SetModifiedFlag();
}

BBox CNCMDoc::GetProgDim()
{
	BMatr UCSMatr;
	bool UCSZero = GetUnit()->GetUcsToolMatr(0, UCSMatr);
	BBox Res;
	CMapStringToNProgram *pPList = NGetProgramList();
	for(int i = 0; i < pPList->GetMainProgCount(); ++i)
	{
		CString Key;
		NProgram *pProg = NULL;
		pPList->GetAt(i, Key, (CObject *&)pProg);
		if(pProg->IsVisible())
		{
			for (int l = 0; l < pProg->GetGeomArraysCount(); ++l)
			{
				for (int k = 0; k < pProg->GetGeomArraySize(); k++)
				{
					NCadrGeom* pGeom = pProg->GetGeomArray(l)[k];
					if (pGeom->GetMatrNum() == 0 && UCSZero)
						pGeom->ExpandWorkBox(&Res);
					else
					{
						NCadrGeom BGeom = *pGeom;
						BGeom.Tr(BSpacePos::GetRotMatr(pGeom->GetMatrNum()) * UCSMatr).ExpandWorkBox(&Res);
					}
				}
			}
		}
	}
	return Res;
}

BOOL CNCMDoc::IsThereAnyTool() const
{
	return ! NToolList.empty();
}

BOOL CNCMDoc::IsThereAnyProgr() const
{
	return NGetProgramList()->GetMainProgCount() > 0;
}

BOOL CNCMDoc::IsThereAnyErrProgr()
{
	for each (const auto & el in *NGetProgramList())
	{
		NProgram* pP = static_cast<NProgram*>(el.second);
		if (pP != nullptr)
			if(pP->HaveErr())
				return TRUE;
	}
	return FALSE;
}

BOOL CNCMDoc::IsThereAnyExObj() const
{
	return ! NExObjList.empty();
}

BOOL CNCMDoc::IsThereAnyPart() const
{
	return ! NPartsList.empty();
}

BOOL CNCMDoc::IsThereAnyStock() const
{
	return ! NStocksList.empty();
}

BOOL CNCMDoc::IsThereAnyStockImpExpDX() const
{
	if (NCMComData::GetType() == AT_TURN)
		return FALSE;
	for (int i = 0; i < NStocksList.GetSize(); ++i)
	{
		NStockBSP* pS = dynamic_cast<NStockBSP*>(GetStockUnsafe(i));
		if (pS != nullptr)
		{
			if (pS->GetStockType() != AT_TURN)
			{
				const MQuadTree* pQuadTree = dynamic_cast<const MQuadTree*>(pS->GetOrigBody());
				if (pQuadTree == nullptr)
					return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CNCMDoc::IsThereMoreThanOneStock() const
{
	return (NStocksList.GetSize() > 1);
}

BOOL CNCMDoc::IsThereAnyTransluc()
{
	if(GetMachTool().HasTranslucent())
		return TRUE;

	if (GetMachTool().GetDrawAllTools() && NTool::IsTranslucent())
		return TRUE;

	if (NTool::IsShapeVisible())
		if (NTool::IsTranslucent())
			return TRUE;

	for each (const auto & el in NStocksList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		if(pS->GetRMode() == RM_TRANSLUCENT)
			return TRUE;
	}

	if(NExObjList.HasTranslucent())
		return TRUE;

	if(NPartsList.HasTranslucent())
		return TRUE;

	return FALSE;
}

//BOOL CNCMDoc::IsThereAnyVisProgr()
//{
//	for(int i = 0; i < NGetProgramList().GetMainProgCount(); ++i)
//	{
//		CString Key;
//		NProgram *pProg = NULL;
//		NGetProgramList().GetAt(i, Key, (CObject *&)pProg);
//		if(pProg)
//			if(pProg->IsVisible())
//				return TRUE;
//	}
//	//NProgram *pP;
//	//CString key;
//	//for( POSITION pos = NGetProgramList().GetStartPosition(); pos != NULL; )
//	//{
//	//	NGetProgramList().GetNextAssoc( pos, key, pP );
//	//	if(pP->IsVisible())
//	//		return TRUE;
//	//}
//	return FALSE;
//}

BOOL CNCMDoc::IsThereAnyVisTool()
{
	for each (const auto & el in NToolList)
	{
		NToolCombined* pTc = static_cast<NToolCombined*>(el.second);;
		if(pTc->IsVisible())
			return TRUE;
	}
	return FALSE;
}



BBox CNCMDoc::GetProgCyl()
{
	return GetProgDim();
}

void CNCMDoc::GotoCadr(const NChannel* pChannel, cadrID CadrID)
{
	if (pChannel != pCurChannel)
		return;
	if(CadrID.Prog < 0 || CadrID.Prog >= NGetProgramList()->GetSize())
		return;
	if(!SetProgrTDT(CadrID.Prog))
		return;
	SetModifiedFlag();
	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	NTextView * pTextView = (NTextView *) frame->GetTextView();
	pTextView->GotoCadr(CadrID.Cadr);
}

void CNCMDoc::GotoCadrFast(const NChannel* pChannel, cadrID CadrID)
{
	if (pChannel != pCurChannel)
		return;
	if(CadrID.Prog < 0 || CadrID.Prog >= NGetProgramList()->GetSize())
		return;
	if(!SetProgrTDT(CadrID.Prog))
		return;
	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	NTextView * pTextView = (NTextView *) frame->GetTextView();
	pTextView->GotoCadrFast(CadrID.Cadr);
}

void CNCMDoc::GotoAbsCadr(const NChannel* pChannel, cadrID CadrIDabs)
{
	if (pChannel != pCurChannel)
		return;
	if(CadrIDabs.Prog < 0)
		return;
	CMapStringToNProgram *plist = NGetProgramList();
	cadrID CadrID = CadrIDabs;
	BProgram *pProg = plist->GetProg(CadrID);
	if(!pProg)
		return;
	CadrID = pProg->GetCadrByHist(CadrIDabs.Cadr);
	GotoCadr(pCurChannel, CadrID);
}

void CNCMDoc::Jump2AbsCadr(cadrID CadrIDabs, bool NeedMessage)
{// Jump to CadrIDAbs
	ClearAllCollisions();
	int ChanCount = GetChannelsCount();
	int ProgInd = CadrIDabs.Prog;
	int ChanInd = ProgInd % ChanCount;
	if (ChanInd != pCurChannel->GetChannelInd())
		return;
	ProgInd /= ChanCount;
	auto pChannel = GetChannel(ChanInd);
	// Stop BEFORE given cadr execution
	if (NeedMessage)
	{
		if (!((CMainFrame*)AfxGetMainWnd())->CheckCutting())
			return;
	}
	else
	{
		if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
			return;
	}
	int cadr = CadrIDabs.Cadr;
	if(cadr < 0)
		return;
	int nprog = ProgInd;
	if(nprog < 0)
		return;
	CMapStringToNProgram *plist = pChannel->NGetProgramListCn();
	NProgram *pProg;
	CString key;
	if(!plist->GetAt(nprog, key, (CObject *&)pProg))
		return;
	// Recalc "cadr" to previous cadr
	for(cadrID BaseCadrNum = pProg->GetCadrByHist(cadr);
		(pProg->GetCadrByHist(cadr) == BaseCadrNum) && (cadr > 0);
		--cadr);
	// Do jump
	GetUnit()->JumpToHCadrRel(pProg, cadr + 1, true); // issue 174
	UpdateAllViews(NULL, N_CADRCHANGED | N_OGL | N_STATE);
}

void CNCMDoc::GotoStatusCadr()
{
	GotoCadr(pCurChannel, StatusCadr);
}

BOOL CNCMDoc::OnNewDocument() 
{
	CString DocTitle;
	DocTitle.LoadString(IDS_EMPTY_DOC_TITLE);
	SetTitle(DocTitle);
	
	return CDocument::OnNewDocument();
}
int compare_int1( const void *a, const void *b )
{
	if(*(int*)a<*(int*)b)
		return -1;
	return (*(int*)a==*(int*)b) ? 0 : 1;
}

BBox CNCMDoc::GetStocksGabar()
{
	BBox gabar;
	if (!IsThereAnyStock())
	{
		return gabar;
	}
	for each (const auto & el in NStocksList)
	{
		const CString& key = el.first;
		class NStock* pS = static_cast<class NStock*>(el.second);
		if(pS)
			gabar.Expand(pS->GetGabar());
	}
	return gabar;
}

void CNCMDoc::FillToolsStr(CString &Tools)
{
#define FormStr(ID, Var) 		{ if (Var != 0)\
{\
	CString Format;\
	Format.LoadString(ID);\
	CString Str;\
	Str.Format(Format, Var);\
	Lines[index] += Str;\
}}

	//	Fill tool list info
	FillToolsInfo();
	auto ToolsNum = NToolList.GetSize();
	int *ToolPos = new int[NToolList.GetSize()];
	for (int j = 0; j < NToolList.GetSize(); ++j)
	{
		NToolID Key;
		NToolCombined *pTool;
		NToolList.GetAt(j, Key, (CObject *&)pTool);
		ToolPos[j] = Key.GetTurretNum() * 1000 + Key.GetToolNum();
	}
	qsort((void *)ToolPos, NToolList.GetSize(), sizeof(int), compare_int1);

	CString BlockSkip;
	GetUnit()->GetWordValue("BlockSkip", BlockSkip);
	CString *Lines = new CString[NToolList.GetSize()];
	for (int j = 0; j < NToolList.GetSize(); ++j)
	{
		NToolID Key;
		NToolCombined *pTool = NULL;
		NToolList.GetAt(j, Key, (CObject *&)pTool);
		double D, R, EL, FL, BA, SA, H;
		auto res = pTool->GetStdParams(D, R, EL, FL, BA, SA, H);
		int T = Key.GetTurretNum();
		int P = Key.GetToolNum();
		
		// Find proper tool index
		int index = 0;
		for (; index < NToolList.GetSize(); ++index)
		{
			if (ToolPos[index] == T * 1000 + P)
				break;
		}

		Lines[index] += BlockSkip;
		FormStr(IDS_TURRET_NUM, T);
		FormStr(IDS_TOOL_NUM, P);
		int TypeID = IDS_TOOLTYPE;
		if (BA != 0.)
			TypeID = IDS_TOOLTYPE_DRILL;
		else
		{
			if (SA == 0 && R == 0.)
				TypeID = IDS_TOOLTYPE_ENDMILL;
			else
			{
				if (SA == 0 && D == 2. * R)
					TypeID = IDS_TOOLTYPE_BALL;
			}
		}
		CString Str;
		Str.LoadStringA(TypeID);
		Lines[index] += Str;
		FormStr(IDS_TOOL_DIAM, D);
		FormStr(IDS_TOOL_DLINA, H);
		FormStr(IDS_TOOL_RAD, R);

		Lines[index] += _T("\n");
	}
	delete[] ToolPos;
	for (int i = 0; i < NToolList.GetSize(); ++i)
		Tools += Lines[i];
	delete[] Lines;
#undef FormStr
}

void CNCMDoc::FillExternData(NCMData *pExternData)
{
//  Fill unit info
	if(GetUnit()->Defined())
	{
		pExternData->UnitFile = GetUnit()->GetConfFileName();
		GetUnit()->LookupSGeneral("Name",pExternData->UnitName);
		GetUnit()->LookupSGeneral("Type",pExternData->UnitType);
		GetUnit()->LookupSGeneral("Manufacturer",pExternData->UnitManufacturer);
		GetUnit()->LookupSGeneral("Operation",pExternData->UnitOperation);
		GetUnit()->LookupSGeneral("NumAxes",pExternData->UnitAxes);
	}
//  Fill stock info
	if(!NStocksList.empty())
	{
		pExternData->Stock.BasePoint = -1;
		pExternData->Stock.Gabar = GetStocksGabar();
		pExternData->Stock.Type = _T("Empty");
	}
//	Fill program list info
	for(int i=0; i<NGetProgramList()->GetMainProgCount();++i)
	{
		CString Key;
		NProgram *pProg = NULL;
		NGetProgramList()->GetAt(i,Key,(CObject *&)pProg);
		pProg->FillInfo(NULL);
		InfoProgram *pInfo = new InfoProgram;
		(*pInfo) = pProg->GetInfo();
		pExternData->ProgArray.Add(pInfo);
	}
//	Fill tool list info
	FillToolsInfo();
	pExternData->ToolArray.SetSize(NToolList.GetSize());	
	int *ToolPos = new int[NToolList.GetSize()];
	for(int j = 0; j < NToolList.GetSize(); ++j)
	{
		NToolID Key;
		NToolCombined *pTool;
		NToolList.GetAt(j, Key, (CObject *&)pTool);
		ToolPos[j] = Key.GetTurretNum() * 1000 + Key.GetToolNum();
	}
	qsort((void *)ToolPos, NToolList.GetSize(), sizeof(int), compare_int1);
	for(int j = 0; j < NToolList.GetSize(); ++j)
	{
		NToolID Key;
		NToolCombined *pTool = NULL;
		NToolList.GetAt(j, Key, (CObject *&)pTool);
		InfoTool *pInfo = new InfoTool;
		(*pInfo) = pTool->GetInfo();
		pInfo->Pos = Key.GetTurretNum() * 1000 + Key.GetToolNum();
		pInfo->Name = pTool->GetName();
		int k = 0;
		for(; k < NToolList.GetSize(); ++k)
			if(ToolPos[k] == pInfo->Pos)
				break;
		pExternData->ToolArray.SetAt(k, pInfo);
	}
	delete[] ToolPos;
}

BOOL CNCMDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
#ifdef NC_DEMO
	AfxMessageBox(IDS_MES_DEMO);
	return FALSE;
#endif
#ifndef NC_DEMO
	if (!((CMainFrame*)AfxGetMainWnd())->CheckCutting())
		return FALSE;
	delete pExternData;
	pExternData = new NCMData;
	FillExternData(pExternData);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrame)
		pMainFrame->GetMainImage(ImageRGB);

	const CString Path(lpszPathName);
	if (Path.GetLength() > 6)
		if (Path.Right(6).MakeUpper() == _T("NCMALL"))
			SaveEnvironment = true;

	const auto Res = CDocument::OnSaveDocument(lpszPathName);
	SaveEnvironment = false;
	return Res;
#endif
}

void CNCMDoc::FillToolsInfo()
{
	for(int j = 0; j < NToolList.GetSize(); ++j)
	{
		CString Key;
		NToolCombined *pTool = NULL;
		NToolList.GetAt(j, Key, (CObject *&)pTool);
		pTool->Info.Clear();
	}
	for(int i=0; i<NGetProgramList()->GetSize();++i)
	{
		CString Key;
		NProgram *pProg = NULL;
		NGetProgramList()->GetAt(i,Key,(CObject *&)pProg);
		if(!pProg)
			return;
		int pos;
		NToolID ToolID;
		if(!pProg->GetTCHist(0, pos, ToolID))
			return;
		NToolCombined *pTl = NULL;
		NGetToolList()->Lookup(ToolID, pTl);
		const BGeomArray *pGeomArray = &(pProg->GetCompGeomArray(0));
		for(int i = 1; i < pProg->GetTCHistSize() + 1; ++i)
		{
			int j = pos;
			if(i == pProg->GetTCHistSize())
				pos = int(pGeomArray->GetSize());
			else
			{
				if(!pProg->GetTCHist(i, pos, ToolID))
					continue;
			}
			if(pTl)
			{
				
				for(int jh = pProg->GetHistByHCadr(j); jh < pProg->GetHistByHCadr(pos); ++jh)
				{
					NCadrGeom *pGeom = (*pGeomArray)[jh];
					double CadrTime = pGeom->GetWorkTimeS();
					double CadrLength = pGeom->Length();
					if(pGeom->HaveGeom() || pGeom->IsRotate())
					{
						if(pGeom->IsFast())
						{
							pTl->Info.FastLength += CadrLength;
							pTl->Info.FastTime += CadrTime;
						}
						else
						{
							pTl->Info.WorkLength += CadrLength;
							pTl->Info.WorkTime += CadrTime;
						}
					}
					pTl->Info.Time += CadrTime;
				}
			}
			pTl = NULL;
			NGetToolList()->Lookup(ToolID, pTl);
		}
	}
}


bool CNCMDoc::JumpToStatusCadrAbs()
{
	bool IsFresh = GetTickCount64() - StatusCadrAbsTime < 2000;
	if(IsFresh)
		Jump2AbsCadr(StatusCadrAbs, false);
	return IsFresh;
}

void CNCMDoc::MarkErrCadr(const NChannel* pChannel, cadrID Cadr)
{
	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	if (frame != nullptr)
	{
		NTextView* pTextView = (NTextView*)frame->GetTextView();
		if(pTextView != nullptr)
			pTextView->MarkErrCadr(Cadr.Cadr, pChannel->NGetProgramListCn()->GetProgName(Cadr));
	}
}

void CNCMDoc::ResetStocks()
{
	for each(auto pEl in Channels)
		pEl->StopDynamicCadr();

	NStocksList.ResetStocks();
}

void CNCMDoc::OnMtLoad()
{
	if (!MachToolEditor.CheckEditing())
		return;
	CString path = NCM_PROJECT.GetMachinesPath();
	NMachNameDlg mnd;
	if(mnd.DoModal(path, GetMachTool().GetFileName()) != IDOK)
		return;
	LoadMT(mnd.GetName());
}

void CNCMDoc::LoadNCUnit(const CString &UName)
{
	int res = GetUnit()->SetDef(UName);
	if (res != 0)
		return;// User did not select new file or error

	ImplReplAll();
	RegeneratePrograms();
	SetModifiedFlag();
	UpdateAllViews(NULL, N_OGL | N_STATE | N_LEFT | N_UNIT_CHANGED | N_PROGRAMCHANGED);
}

void CNCMDoc::LoadMT(const CString &MTName)
{
	LoadMachTool(NCM_PROJECT.GetMachinesPath(), MTName);
	RegeneratePrograms();

	SetModifiedFlag();
	UpdateAllViews(NULL, N_OGL | N_LEFT  | N_PROGRAMCHANGED);
}

NStockSection * CNCMDoc::GetStockSection(void)
{
	return pStockSection;
}

NStockSection * CNCMDoc::CreateStockSection(CWnd *pParent)
{
	pStockSection = new NStockSection(pParent);
	return pStockSection;
}

void CNCMDoc::DrawStockWSect(RenderMode GlobRMode, bool DrawSurf)
{
	if (!NStocksList.IsVisible())
		return;
	if (GetSectionActive())
	{
		pStockSection->EnableClipPlane();
		if (pStockSection->NeedFillSection())
		{
			pStockSection->BeginTest1();
			DrawAllStocks(GlobRMode, 3, DrawSurf);
			pStockSection->BeginTest2();
			DrawAllStocks(GlobRMode, 3, DrawSurf);
			pStockSection->EndTest();
		}
		DrawAllStocks(GlobRMode, 7, DrawSurf);
		pStockSection->DrawClipPlane();
		pStockSection->DisableClipPlane();
	}
	else
		DrawAllStocks(GlobRMode, 7, DrawSurf);
}

COLORREF CNCMDoc::GetToolColor(int n, int t)
{
	NToolID ToolID;
	NToolCombined *pTool;
	ToolID.Set(n, t);
	if(!NGetToolList()->Lookup(ToolID,pTool))
		return 0;
	return pTool->Color.GetRGB();
}

bool CNCMDoc::IsAnaliseActive(void)
{
	if(Measure.IsActivated())
		return true;
	if(Dimensions.IsActivated())
		return true;
	if(pDivide != nullptr)
		return true;
	if(Comparison.IsActive())
		return true;
	return false;
}


void CNCMDoc::ImplReplAll(NChannel* pCh/* = nullptr*/)
{
	if (pCh == nullptr)
		pCh = GetCurChannel();
	CMapStringToNProgram* pPList = pCh->NGetProgramListCn();
	for (int i = 0; i < pPList->GetSize(); ++i)
	{
		CString Key;
		NProgram* pProg = nullptr;
		pPList->GetAt(i, Key, (CObject * &)pProg);
		if (pProg)
		{
			ImplRepl(pProg);
		}
	}
}

void CNCMDoc::RegenerateTools(void)
{
	for(int i = 0; i < NToolList.GetSize(); ++i)
	{
		CString Key;
		NToolCombined *pTool = NULL;
		NToolList.GetAt(i, Key, (CObject *&)pTool);
		pTool->Regenerate();
	}
}

void CNCMDoc::RegeneratePrograms(void)
{
	bool NeedSecondRegen = RegeneratePrograms1Time();
	if (NeedSecondRegen)
		RegeneratePrograms1Time();

	std::vector<NCadrGeomArray*> Geoms(Channels.size());
	for (int ChInd = 0; ChInd < Channels.size(); ++ChInd)
	{
		NCadrGeomArray* pArray = new NCadrGeomArray;//don't use BGeomArray. Destructor deletes all geoms
		const auto& Channel = *Channels.at(ChInd);
		for (int i = 0; i < Channel.NGetProgramListCn()->GetSize(); ++i)
		{
			BProgram* pProg = nullptr;
			CString Key;
			Channel.NGetProgramListCn()->GetAt(i, Key, (CObject*&)pProg);
			for (int k = 0; k < pProg->GetGeomArraySize(); ++k)
				pArray->Add(pProg->GetCompGeomArray(0).GetAt(k));
		}
		Geoms.at(ChInd) = pArray;
	}
		
	NCM_PROJECT.GetTimeLine().MakeFromGeom(Geoms);

	for (int ChInd = 0; ChInd < Channels.size(); ++ChInd)
	{
		delete Geoms.at(ChInd);
		const auto& Channel = *Channels.at(ChInd);
		for (int i = 0; i < Channel.NGetProgramListCn()->GetSize(); ++i)
		{
			BProgram* pProg = nullptr;
			CString Key;
			Channel.NGetProgramListCn()->GetAt(i, Key, (CObject*&)pProg);
			pProg->PropogateTimeInd();
		}
	}
}

bool CNCMDoc::RegeneratePrograms1Time(void)
{
	CWaitCursor w;
	GetCollManager().AbortBack();
	BSpacePos::ClearRotMatrs();
	SelectedPrev.Clear(); // Should be done before ClearAllCurves
	SelectedNext.Clear();
	CadrToolAxis.Set(0., 0., 1., 0.);
	BCurve::Storage.ClearAll();
	const bool MS = MileStones.IsActive();
	MileStones.DeActivate();
	GetMachTool().ResetConfigs();
	bool NeedSecondRegen = false;
	for each(auto pCh in Channels)
	{
		std::set<int> StPosSet;
		pCh->GetStPosSet(StPosSet);
		pCh->RegenerateProgramsCn(StPosSet);
		pCh->PrepErrorHistory();
		pCh->PrepProgRendersCn(NCM_PROJECT.GetDouble("TolLineApp", 0.01));
		if (!pCh->GetUnitCn()->GetAllowToolString())
		{
			pCh->MakeUsedTools();
			NeedSecondRegen |= pCh->MakeUsedTools();
		}

	}
	
	NSyncGenerator SyncGen(*this);
	SyncGen.SetTimes();

	ProgramChangeEnded();
	BCurve::Storage.Close();
	GetRayCastManager().Initialize(this);
	if (Channels.size() > 1)
	{
		NSpeedPar::Extern.SetTrackFeed(true);
		if (NCM_PROJECT.GetGlobalState().GetAnimateMode())
			theApp.OnSmothnessModeRib();
	}
	if (MS)
		MileStones.Activate();

	return NeedSecondRegen;
}

bool CNCMDoc::SaveAllStocks() const
{
	CString TempStocksFolder;
	TempStocksFolder.LoadString(IDS_TEMPSTOCKS_FOLDER);
	CString Path = NCM_PROJECT.GetPrototypesPath() + TempStocksFolder;
	// Delete all temporary stocks
	NCM_PROJECT.DeleteDirectory(Path, true);
	// Save new
	_mkdir(Path);
	CString NameFormat;
	NameFormat.LoadString(IDS_TEMPSTOCKS_NAMEFORMAT);
	for (int i = 0; i < GetStocksList().GetSize(); ++i)
	{
		CString Name;
		Name.Format(NameFormat, i);
			
		CFile fu;//WRITE PACKED
		bool OpenError = (!fu.Open(Path + Name, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary));
		if(OpenError)
			return false;
		CMemFile f(2 * 1024 * 1024);
		if (theApp.GetGlobalState().GetModelMode() == NGlobalState::BSP)
			GetStockUnsafe(i)->ExportBSP(&f);
		else
			GetStockUnsafe(i)->ExportDX(&f);
		WritePacked(f, fu);
	}
	return true;
}

void CNCMDoc::WritePacked(CMemFile& SourceFile, CFile& DestFile)
{
	unsigned long Length = UINT(SourceFile.GetLength());
	DestFile.Write(&Length, sizeof(Length));
	BYTE* pBuffer = SourceFile.Detach();
	ULONG DestLen = compressBound(Length);
	BYTE* pDest = new BYTE[DestLen];
	compress2(pDest, &DestLen, pBuffer, Length, Z_DEFAULT_COMPRESSION);
	delete[] pBuffer;
	DestFile.Write(pDest, DestLen);
	delete[] pDest;
	DestFile.Close();
}

void CNCMDoc::RegenerateProgramsProbePart(NChannel* pChannel)
{
	CWaitCursor w;
	GetCollManager().AbortBack();
	SelectedPrev.Clear(); // Should be done before ClearAllCurves
	SelectedNext.Clear();
	bool MS = MileStones.IsActive();
	MileStones.DeActivate();

	class NCUnit &Unit = *pChannel->GetUnitCn();
	Unit.GetStates().Open();
	NProgram *pProg = Unit.GetCurRunProg();
	if(pProg == nullptr)
		return; // internal error
	// continue with current program
	int res = 0;
	res = Unit.RunProgramProbePart(pProg, pProg);
	if (res == 0)
	{
		pProg->PrepRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));
		// continue with rest programs
		CMapStringToNProgram *pPList = NGetProgramList();
		int i = pPList->GetIndex(pProg);
		if (i < 0 || i >= pPList->GetMainProgCount())
			return; // internal error
		for (++i; i < pPList->GetMainProgCount(); ++i)
		{
			CString Key;
			pPList->GetAt(i, Key, (CObject *&)pProg);
			if (!pProg)
				continue;
			Unit.SetCurProg(Key);
			Unit.GetState()->SetCurCadrEx(cadrID(i, 0));
			if (Unit.GetCheckProgramText())
				Unit.CheckProgramText(pProg);
			Unit.GetState()->SetCurCadr(0);
			Unit.GetState()->ProgramStarted = false;

			Unit.GetState()->EndProgram = false;

			res = Unit.RunProgramProbePart(pProg, pProg);
			if (res == 1 )
				break;// error
			pProg->PrepRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));
			if (Unit.GetState()->ProbeCadr)
				break;
		}
	}
	Unit.GetUsedUCSs().insert(Unit.GetState()->UCSs.GetUsed().begin(), Unit.GetState()->UCSs.GetUsed().end());
	Unit.GetStates().AddState(*Unit.GetState(), *Unit.GetState(), *Unit.GetLastGeomState());// Very last state
	Unit.GetStates().Close();

	Unit.Reset();// To set the machine tool to a proper position
	pChannel->PrepErrorHistory();
	ProgramChangeEnded();
	BCurve::Storage.Close();
	if (MS)
		MileStones.Activate();
	if (!Unit.GetAllowToolString())
		pChannel->MakeUsedTools();
	GetRayCastManager().Initialize(this);
	UpdateAllViews(nullptr, N_PROGRAMCHANGED);
}

void CNCMDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	CString PathName(lpszPathName);
	// Set Untitled if NCMPrt is loaded
	int index = PathName.ReverseFind('.');
	if(index > 0)
	{
		if(PathName.Right(PathName.GetLength() - index - 1) == "NCMPrt")
		{
			CString Untitled;
			Untitled.LoadString(IDS_EMPTY_DOC_TITLE);
			CDocument::SetTitle(Untitled);
			m_strPathName.Empty();
			return;
		}
	}

	CDocument::SetPathName(lpszPathName, bAddToMRU);
}

NToolCombined * CNCMDoc::GetTool(const NToolID & ToolID)
{
	NToolCombined *pTool = NULL;
	NGetToolList()->Lookup(ToolID, pTool);
	return pTool;
}

int CNCMDoc::GetAttachNum(const CString & buf)
{
	if(buf == "Axial")
		return 1;
	else if(buf == "Radial")
		return 0;
	else
		return atoi(buf);
}

void CNCMDoc::PrepProgRenders(double Eps)
{
	for each (auto pCh in Channels)
		pCh->PrepProgRendersCn(Eps);
}

void CNCMDoc::DeleteList(CMapStringToNStock &List)
{
	for each (const auto & el in List)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		delete pS;
	}
	List.RemoveAll();
}

void CNCMDoc::DeletePartList()
{
	std::vector<class NStock*> Stocks;
	for each (const auto & el in *NGetPartList())
	{
		NStock* pS = static_cast<NStock*>(el.second);
		Stocks.push_back(pS);
	}
	const bool InternalGroup = GetUndoHistory().StartGroup();
	for each (auto pS in Stocks)
		DeletePartWU(pS);
	GetUndoHistory().EndGroup(InternalGroup);
}

void CNCMDoc::DeleteExobjList()
{
	std::vector<class NStock*> Stocks;
	for each (const auto & el in *NGetExObjList())
	{
		NStock* pS = static_cast<NStock*>(el.second);
		Stocks.push_back(pS);
	}
	const bool InternalGroup = GetUndoHistory().StartGroup();
	for each (auto pS in Stocks)
		DeleteFixtWU(pS);
	GetUndoHistory().EndGroup(InternalGroup);
}

bool CNCMDoc::ExtractFileName(CString &Path)
{
	int iSlash = Path.ReverseFind('\\');
	Path.Delete(0, iSlash + 1);
	int iDot = Path.ReverseFind('.');
	if(iDot < 0)
		iDot = Path.GetLength();
	Path.Delete(iDot, Path.GetLength());
	return true;
}

bool CNCMDoc::IsThereAnyVisExObj(void) const
{
	return NExObjList.IsThereAnyVisObj();
}

bool CNCMDoc::IsThereAnyVisPart(void) const
{
	return NPartsList.IsThereAnyVisObj();
}

bool CNCMDoc::IsThereAnyVisStock(void) const
{
	return NStocksList.IsThereAnyVisObj();
}

void CNCMDoc::UpdateAllViews(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	CDocument::UpdateAllViews(NULL, lHint, pHint);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if(!pMainFrame)
		return;
	if (pMainFrame->IsIconic())
		return;
	if (lHint & N_SETTINGS)
		PrepProgRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));

	((NTextView*)pMainFrame->GetTextView())->OnUpdate(NULL, lHint, pHint);
	((NStateUnitPane *)pMainFrame->GetUnitPane())->OnUpdate(NULL, lHint, pHint);
	((NStateAnalysePane *)pMainFrame->GetAnalysePane())->OnUpdate(NULL, lHint, pHint);
	((NStateGeomPane *)pMainFrame->GetGeomPane())->OnUpdate(NULL, lHint, pHint);
	((NStateMesurePane *)pMainFrame->GetMesurePane())->OnUpdate(NULL, lHint, pHint);
	((NStateRegsPane *)pMainFrame->GetRegsPane())->OnUpdate(NULL, lHint, pHint);
	((NReportPane *)pMainFrame->GetReportPane())->OnUpdate(NULL, lHint, pHint);
	pMainFrame->UpdateLeftView(NULL, lHint, pHint);
	pMainFrame->UpdateOGLPanes(NULL, lHint, pHint);
	((NPSlidePane *)pMainFrame->GetPSlidePane())->OnUpdate(NULL, lHint, pHint);
	((PropPane *)pMainFrame->GetPropPane())->OnUpdate(NULL, lHint, pHint);
}

NCollManagerOrd &CNCMDoc::GetCollManager(void)
{
	return CollManager;
}

const NCollManagerOrd& CNCMDoc::GetCollManager(void) const
{
	return CollManager;
}
	
int CNCMDoc::FullCollisionDetection(bool EnableLog)
{
	if(NGetProgramList()->empty())
		return -1;
	cadrID EndHIDAbs;
	EndHIDAbs.Prog = NGetProgramList()->GetMainProgCount() - 1;
	CString key;
	NProgram *pLastProg = 0;
	NGetProgramList()->GetAt(EndHIDAbs.Prog, key, (CObject *&)pLastProg);
	if(!pLastProg)
		return -1;
	EndHIDAbs.Cadr = pLastProg->GetGeomArraySize() - 1;

	cadrID StartHIDAbs;
	StartHIDAbs.Prog = 0;
	StartHIDAbs.Cadr = 0;

	return CollManager.StartBack(StartHIDAbs, EndHIDAbs, GetpMUPair());
}

void CNCMDoc::ProgramChangeEnded()
{
	CollManager.SetLogIsStale();
}

int CNCMDoc::RunNSteps(int N)
{
	bool MS = MileStones.IsActive();
	MileStones.DeActivate();
	ResetInterrupt();
	RunSteps(N);
	if(MS)
		MileStones.Activate();
	SetCurProgrTDT();
	return 0;
}

RunStepsStopReason CNCMDoc::RunSteps(int N)
{
	ClearAllCollisions();
	RunStepsStopReason res = SR_OK;
	for each (auto pChan in Channels)
	{
		SetCuttingEnabled(*pChan, false);
		pChan->RunStepsCn(N);
	}
	SetCuttingEnabled(*Channels[0], true);
	return res;
}

NToolCombined * CNCMDoc::GetCurTool()
{
	return GetCurChannel()->GetCurToolCn();
}

int CNCMDoc::GetPartsPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N, RPickID& PickID)
{
	// in: P - start ray point; V - ray direction
	// out: R - first intersection point, N - external normal
	// Ray must be defined in stock coordinates
	R.Set(0., 0., 0., -1.);
	PickID.Set0();
	if(NGetPartList()->IsVisible())
	{
		for(int i = 0; i < NGetPartList()->size(); ++i)
		{
			CString Key;
			NStock* pS = nullptr;
			NGetPartList()->GetAt(i, Key, (CObject*&)pS);
			if (pS == nullptr)
				continue;
			if(!pS->IsVisible())
				continue;
			if(!pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
				continue;
			NStockStl *pSS = (NStockStl *)pS;
			BPoint Nc;
			BPoint Rc = pSS->RayCastingOr(P, V, 1.e-6, Nc);
			if(Rc.GetH() > 0.)// Find nearest
			{
				if(R.GetH() > 0.)
				{
					if((Rc - P) * V < (R - P) * V)
					{
						R = Rc;
						N = Nc;
						PickID.Set2(OG_PARTS, i);
					}
				}
				else
				{
					R = Rc;
					N = Nc;
					PickID.Set2(OG_PARTS, i);
				}
			}
		}
	}
	if(NExObjList.IsVisible())
	{
		for (int i = 0; i < NGetExObjList()->size(); ++i)
		{
			CString Key;
			NStock* pS = nullptr;
			NGetExObjList()->GetAt(i, Key, (CObject*&)pS);
			if (pS == nullptr)
				continue;
			if(!pS->IsVisible())
				continue;
			if(!pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
				continue;
			NStockStl *pSS = (NStockStl *)pS;
			BPoint Nc;
			BPoint Rc = pSS->RayCastingOr(P, V, 1.e-6, Nc);
			if(Rc.GetH() > 0.)// Find nearest
			{
				if(R.GetH() > 0.)
				{
					if((Rc - P) * V < (R - P) * V)
					{
						R = Rc;
						N = Nc;
						PickID.Set2(OG_FIXTURES, i);
					}
				}
				else
				{
					R = Rc;
					N = Nc;
					PickID.Set2(OG_FIXTURES, i);
				}
			}
		}
	}
	if(NGetStockList()->IsVisible())
	{
		BPoint Nc;
		BPoint Rc(0., 0., 0., -1.);
		if (theApp.IsInCuttingMode() && GetChannel(0)->GetDynamicCadr())
		{
			for (GBodyRender *render = GetChannel(0)->GetDynamicCadr()->GetActiveRender(); render != nullptr; render = render->GetNextRender())
			{
				if (render->GetStockPos() != BStockGrID())
				{
					MTMachineTool& MT = GetMachTool();
					BMatrPair M = MT.GetStockMovedMatr(0, true) * MT.GetStockMovedMatrSGr(render->GetStockPos(), true).invr();
					Rc = render->RayCasting(P * M.Or(), V * M.Or(), 1.e-6, Nc) * M.Inv();
					Nc = Nc * M.Inv();
				}
				else
					Rc = render->RayCasting(P, V, 1.e-6, Nc);
				if (Rc.GetH() > 0.)// Find nearest
				{
					if (R.GetH() > 0.)
					{
						if ((Rc - P) * V < (R - P) * V)
						{
							R = Rc;
							N = Nc;
							PickID.Set1(OG_STOCKS);
						}
					}
					else
					{
						R = Rc;
						N = Nc;
						PickID.Set1(OG_STOCKS);
					}
				}
			}
		}
		else
		{
			GetStaticStockPoint(P, V, Rc, Nc);
			if (Rc.GetH() > 0.)// Find nearest
			{
				if (R.GetH() > 0.)
				{
					if ((Rc - P) * V < (R - P) * V)
					{
						R = Rc;
						N = Nc;
						PickID.Set1(OG_STOCKS);
					}
				}
				else
				{
					R = Rc;
					N = Nc;
					PickID.Set1(OG_STOCKS);
				}
			}
		}
	}
	return R.GetH() > 0. ? 0 : -1;
}

int CNCMDoc::GetStaticStockPoint(const BPoint& P, const BPoint& V, BPoint& R, BPoint& N)
{
	R.Set(0., 0., 0., -1.);
	if (NGetStockList()->IsVisible())
	{
		BPoint Nc;
		BPoint Rc(0., 0., 0., -1.);
		for each (const auto & el in NStocksList)
		{
			const CString& key = el.first;
			class NStock* pS = static_cast<class NStock*>(el.second);
			if (!pS->IsVisible())
				continue;
			if (pS->GetStockGr().Get() != 0)
			{
				MTMachineTool& MT = GetMachTool();
				if (theApp.IsInCuttingMode())
				{
					BMatrPair M(MT.GetStockMovedMatr(0, true) * MT.GetStockMovedMatrSGr(pS->GetStockGr(), true).invr());
					Rc = pS->RayCasting(P * M.Or(), V * M.Or(), 1.e-6, Nc) * M.Inv();
					Nc = Nc * M.Inv();
				}
				else
				{
					BMatrPair M(MT.GetStockMovedMatr(0, true) * MT.GetStockMovedMatrSGr(pS->GetStockGr(), true).invr() * pS->GetMatr().invr());
					Rc = pS->RayCastingOr(P * M.Or(), V * M.Or(), 1.e-6, Nc) * M.Inv();;
					Nc = Nc * M.Inv();
				}
			}
			else
			{
				if (theApp.IsInCuttingMode())
				{
					Rc = pS->RayCasting(P, V, 1.e-6, Nc);
				}
				else
				{
					BMatrPair M(pS->GetMatr());
					Rc = pS->RayCastingOr(P * M.Inv(), V * M.Inv(), 1.e-6, Nc) * M.Or();;
					Nc = Nc * M.Or();
				}
			}
			if (Rc.GetH() > 0.)// Find nearest
			{
				if (R.GetH() > 0.)
				{
					if ((Rc - P) * V < (R - P) * V)
					{
						R = Rc;
						N = Nc;
					}
				}
				else
				{
					R = Rc;
					N = Nc;
				}
			}
		}
	}
	return R.GetH() > 0. ? 0 : -1;
}

int CNCMDoc::GetMachPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N, RPickID& PickID)
{
	// in: P - start ray point; V - ray direction
	// out: R - first intersection point, N - external normal
	// Ray must be defined in stock coordinates
	MTMachineTool &Mach = GetMachTool();
	if(!Mach.GetCurConfig().IsDefined() || !Mach.IsVisible())
		return -1;
	Mach.MakeActual();
	R.Set(0., 0., 0., -1.);
	R = Mach.RayCasting(P, V, 1.e-6, N, PickID);

	return R.GetH() > 0. ? 0 : -1;
}

size_t CNCMDoc::FindSegmRopes(int ObjectType, bool StockView, SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr)
{
    // поиск отрезков каркаса
    // элемент oSegments - два указател€ на два массива float[3]
    // oMatrsInvr - матрицы каждой части станка или единичные дл€ остальных моделей
	// Ёти матрицы должны переводить из CS заготовки в CS объекта
	if(ObjectType & Snaps::Skeleton)
	{
		MTMachineTool &Mach = GetMachTool();
		BMatr StockMatr = Mach.GetStockMatr(0, true);
		BMatr StockMovedMatr = Mach.GetStockMovedMatr(0, true);
		BMatr Rel = StockMovedMatr.invr() * StockMatr;
		if(Mach.GetCurConfig().IsDefined() && Mach.IsVisible() && !StockView)
		{
			Mach.FindSegmentsWithStl(oSegments, oMatrsInvr);
			for (size_t i = 0; i < oMatrsInvr.size(); ++i)
				oMatrsInvr[i] = oMatrsInvr[i] * StockMatr;
		}
		{
			CMapStringToNStock& Map = *NGetPartList();
			if (Map.IsVisible())
			{
				for each (const auto & el in Map)
				{
					NStock* pS = static_cast<NStock*>(el.second);
					if (!pS->IsVisible())
						continue;
					if (!pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
						continue;
					size_t Start = oMatrsInvr.size();
					pS->FindSegmentsWithStl(oSegments, oMatrsInvr);
					for (size_t i = Start; i < oMatrsInvr.size(); ++i)
						oMatrsInvr[i] = oMatrsInvr[i] * Rel;
				}
			}
		}
		{
			CMapStringToNStock& Map = *NGetExObjList();
			if (Map.IsVisible())
			{
				for each (const auto & el in Map)
				{
					NStock* pS = static_cast<NStock*>(el.second);
					if (!pS->IsVisible())
						continue;
					if (!pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
						continue;

					size_t Start = oMatrsInvr.size();
					pS->FindSegmentsWithStl(oSegments, oMatrsInvr);
					for (size_t i = Start; i < oMatrsInvr.size(); ++i)
						oMatrsInvr[i] = oMatrsInvr[i] * Rel;
				}
			}
		}
		for (int i = 0; i < NStocksList.GetSize(); ++i)
		{
			NStock* pS = GetStock(i);
			if (pS)
				if (pS->IsVisible())
				{
					size_t Start = oMatrsInvr.size();
					if (theApp.IsInCuttingMode())
						pS->FindSegmentsWithRender(oSegments, oMatrsInvr);
					else
						pS->FindSegmentsWithStl(oSegments, oMatrsInvr);
					BMatr CurRel = Mach.GetStockMovedMatr(pS->GetStockGr().Get(), true).invr() * StockMatr;

					for (size_t i = Start; i < oMatrsInvr.size(); ++i)
						oMatrsInvr[i] = oMatrsInvr[i] * CurRel;
				}
		}
		if (NTool::IsShapeVisible() || (Mach.IsVisible() && Mach.GetDrawAllTools()))
		{
			for (int ti = 0; ti < NToolList.GetSize(); ++ti)
			{
				CString Key;
				NToolCombined* pTool;
				NToolList.GetAt(ti, Key, (CObject*&)pTool);
				if (pTool != nullptr)
				{
					BMatr M;
					if (!GetFullToolMatr(pTool, StockView, M))
						continue;
					M = M.invr();
					for (int k = 0; k < pTool->GetToolsNum(); ++k)
					{
						if (pTool->GetTool(k)->GetType() != 62/*STL holder*/)
							continue;
						NToolTurnHolderSTL* pTHS = static_cast<NToolTurnHolderSTL*>(pTool->GetTool(k));
						if (pTHS == nullptr)
							continue;
						pTHS->FindSegments(oSegments);
					}
					size_t Start = oMatrsInvr.size();
					for (size_t i = Start; i < oSegments.size(); ++i)
						oMatrsInvr.push_back(M);
				}
			}
		}
	}

    return 0;
}

int CNCMDoc::GetRayPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N)
{
	// returns intersected part number
	// in: P - start ray point; V - ray direction
	// out: R - first intersection point, N - external normal
	// Ray must be defined in stock coordinates
	return -1;
}

NAnalyseState &CNCMDoc::GetAnalyseState()
{
	return AnalyseState;
}

void CNCMDoc::RegenerateStock(int index)
{
#ifdef _DEBUG
	if (index >= NStocksList.GetSize() && index < 0)
		return;
#endif
	GetStock(index)->CreateRender();
}

void CNCMDoc::RegenerateStocks()
{
	for (int index = 0; index < NStocksList.GetSize(); ++index)
		RegenerateStock(index);
}

void CNCMDoc::RegenerateParts()
{
	CString key;
	NStock *pObj;
	for (int i = 0; i < NPartsList.GetSize(); ++i)
	{
		NPartsList.GetAt(i, key, (CObject *&)pObj);
		((NStock *)pObj)->CreateRender();
	}
}

void CNCMDoc::RegenerateFixtures()
{
	CString key;
	NStock *pObj;
	for (int i = 0; i < NExObjList.GetSize(); ++i)
	{
		NExObjList.GetAt(i, key, (CObject *&)pObj);
		((NStock *)pObj)->CreateRender();
	}
}

void CNCMDoc::Regenerate(void)
{
	RegeneratePrograms();
	RegenerateTools();
	if(!Comparison.IsActive())
		RegenerateStocks();
	UpdateAllViews(NULL, N_OGL | N_LEFT | N_STATE | N_PROGRAMCHANGED);
}

bool CNCMDoc::InitToolSection()
{
	ToolSectionDyn.SetPosition(BPoint(0., 0., 0., 1.), BPoint(1., 0., 0., 0.), BPoint(0., 0., -1., 0.));
	ToolSectionDyn.Clear();
	C_OBJECT_MANAGER(ToolSectionDyn)->AddActiveObject(&ToolSectionDyn, 0, OG_PARTS);
	return true;
}

NSectionSetDyn &CNCMDoc::GetToolSection()
{
	return ToolSectionDyn;
}

void CNCMDoc::DrawToolSection() const
{
	ToolSectionDyn.Draw(true, false);
}
// ProgSections

bool CNCMDoc::InitProgWeldManager()
{
//	ProgWeldManager.Clear();
	C_OBJECT_MANAGER(ProgWeldManager)->AddActiveObject(&ProgWeldManager, 3, OG_PARTS);
	return true;
}
int CNCMDoc::UpdateProgWeldManager()
{
	return ProgWeldManager.MakeAll(-1);
}

int CNCMDoc::UpdateProgWeldTraject()
{
	return ProgWeldManager.MakeAll(-3);
}

int CNCMDoc::UpdateProgWeldSections()
{
	return ProgWeldManager.MakeAll(-2);
}

NProgram *CNCMDoc::NewProgram(void)
{
	NProgram *pProg = (NCMComData::GetType() == AT_WELD) ? new NProgramWeld : new NProgram;
	pProg->SetColor(NColor().SetRGB(NToolCombined::Palet.GetRecurColor()));
	return pProg;
}

cadrID CNCMDoc::GetCurCadr(double &t) const
{
	// method is used for partial program draw
	// t is geometric (not time) parameter
	cadrID CadrID;
	CadrID.Prog = -1;
	CadrID.Cadr = 0;
	t = -1.; // Draw whole last cadr
	if(pSliderRRCadr)
	{
		CadrID = pSliderRRCadr->GetCadrIDhist();
		++CadrID.Cadr;
	}
	else if (pCurChannel->IsAnimWalkerStartedCn())
	{
		t = pCurChannel->GetRunAnimateT();
		auto Val = pCurChannel->GetRunAnimateCadr();
		CadrID.Cadr = Val < 0 ? 0 : Val; // Need for correct processing of emty value(-1)
		CString ActiveProgName = GetUnit()->GetState()->GetCurProg();
		int ActiveProgNum = max(0, NGetProgramList()->GetIndex(ActiveProgName));
		CadrID.Prog = ActiveProgNum;
	}
	else 
	{
		GDynamicCadr *pDCadr = GetCurChannel()->GetDynamicCadr();
		if(pDCadr)
		{
			CadrID = pDCadr->GetCadrIDhist();
			++CadrID.Cadr;
			t = pDCadr->GetCurParam();
		}
		if(CadrID.Prog < 0)
		{
			CString ActiveProgName = GetUnit()->GetState()->GetCurProg();
			int ActiveProgNum = max(0, NGetProgramList()->GetIndex(ActiveProgName));
			CadrID.Prog = ActiveProgNum;
			CadrID.Cadr = GetUnit()->GetState()->GetCurHCadr();
		}
	}
	return CadrID;
}

cadrID CNCMDoc::GetCurCadrAbs()
{
	cadrID CadrID;
	CadrID.Prog = -1;
	CadrID.Cadr = 0;
	if (pCurChannel != nullptr)
	{
		GDynamicCadr* pDCadr = GetCurChannel()->GetDynamicCadr();
		if (pDCadr)
		{
			CadrID = pDCadr->GetCadrIDhist();
			CMapStringToNProgram& ProgList = *NGetProgramList();
			if (CadrID.Prog >= 0)
			{
				const BProgram* pProg = ProgList.GetProg(CadrID);
				if (pProg != nullptr)
					CadrID.Cadr = pProg->GetHistByHCadr(CadrID.Cadr);
			}
		}
	}
	if(CadrID.Prog < 0) // Not found on the previous step
	{
		const CString &ActiveProgName = GetUnit()->GetState()->GetCurProg();
		int ActiveProgNum = max(0, NGetProgramList()->GetIndex(ActiveProgName));
		CadrID.Prog = ActiveProgNum;
		CadrID.Cadr = max(0, GetUnit()->GetState()->GetCurHCadrAbs());
	}
	return CadrID;
}

const NCadrGeom* CNCMDoc::GetAbsCadrGeom(cadrID AbsID, int StockPos)
{
	const BProgram* pProg = GetProgram(AbsID);
	if (pProg == nullptr)
		return nullptr;
	if (pProg->GetGeomArraySize() == 0)
		return nullptr;
	const BGeomArray& GeomArray = pProg->GetGeomArrayByStPos(StockPos);
	if (AbsID.Cadr < 0 || AbsID.Cadr >= GeomArray.GetSize())
		return nullptr;
	return GeomArray.GetAt(AbsID.Cadr);
}

const NCadrGeom* CNCMDoc::GetAbsCadrCompGeom(cadrID AbsID, int StockPos)
{
	const BProgram* pProg = GetProgram(AbsID);
	if (pProg == nullptr)
		return nullptr;
	if (pProg->GetGeomArraySize() == 0)
		return nullptr;
	const BGeomArray& GeomArray = pProg->GetCompGeomArrayByStPos(StockPos);
	if (AbsID.Cadr < 0 || AbsID.Cadr >= GeomArray.GetSize())
		return nullptr;
	return GeomArray.GetAt(AbsID.Cadr);
}

const NCadrGeom* CNCMDoc::GetHCadrCompGeom(cadrID HID, int StockPos)
{
	const BProgram* pProg = GetProgram(HID);
	if (pProg == nullptr)
		return nullptr;
	if (pProg->GetGeomArraySize() == 0)
		return nullptr;
	cadrID AbsID = HID;
	AbsID.Cadr = pProg->GetHistByHCadr(HID.Cadr);
	const BGeomArray& GeomArray = pProg->GetCompGeomArrayByStPos(StockPos);
	if (AbsID.Cadr < 0 || AbsID.Cadr >= GeomArray.GetSize())
		return nullptr;
	return GeomArray.GetAt(AbsID.Cadr);
}

int CNCMDoc::GetGlobPos() const
{
	double t = -1.;
	cadrID CadrID = GetCurCadr(t);
	return GetGlobPos(CadrID);
}

int CNCMDoc::GetGlobPos(cadrID CadrID) const
{
	if (CadrID.Prog < 0)
		return 0;
	int PartProgramsSize = 0;
	const CMapStringToNProgram& ProgList = *NGetProgramList();
	for (int i = 1; i < ProgList.GetMainProgCount() && i <= CadrID.Prog; ++i)
	{
		CObject* pProg;
		CString key;
		ProgList.GetAt(i - 1, key, pProg);
		int Size = ((BProgram*)pProg)->GetHCadr2HistorySize();
		if (CadrID.Prog >= i)
			PartProgramsSize += Size;
	}
	return CadrID.Cadr + PartProgramsSize;
}


int CNCMDoc::GetToolsPosCol(int MaxNum, NColor *Colors, int *Positions)
{
	// Colors[i] should be the color for the part from Positions[i-1] to Positions[i]
	// Last Positions value must be equal to AllProgramsSize
	if(MaxNum <= 0)
		return 0;
	CMapStringToNProgram &ProgList = *NGetProgramList();
	NColor DefColor(0., 1., 0., 1.);
	NColor *pColor = &DefColor;
	int PosInd = 0;
	int PartProgramsSize = 0;
	for(int i = 0; i < ProgList.GetSize(); ++i)
	{
		CObject *pProg;
		CString key;
		ProgList.GetAt(i, key, pProg);
		NMapStringToNTool &ToolList = *NGetToolList();
		for(int k = 0; k < ((BProgram *)pProg)->GetTCHistSize(); ++k)
		{
			Colors[PosInd] = *pColor;
			NToolID ToolID;
			int Position = 0;
			pColor = NULL;
			if(((BProgram *)pProg)->GetTCHist(k, Position, ToolID))
			{
				NToolCombined *pTool;
				if(NGetToolList()->Lookup(ToolID, pTool))
					pColor = &pTool->Color;
			}
			Positions[PosInd] = Position + PartProgramsSize + 1;
			if(!pColor)
				pColor = &DefColor;
			if(PosInd >= MaxNum - 1)
				break; // Max size exceeded
			++PosInd;
		}
		PartProgramsSize += ((BProgram *)pProg)->GetHCadr2HistorySize();
	}
	Colors[PosInd] = *pColor;
	Positions[PosInd] = PartProgramsSize - 1;
	Positions[0] = 0;
	return PosInd + 1;
}

const NToolCombined *CNCMDoc::GetActiveTool(void)
{
	const NToolCombined *pTool = NULL;
	if(pSliderRRCadr)
		pTool = pSliderRRCadr->GetLastTool();
	if(pTool)
		return pTool;
	const auto pDynCadr = GetCurChannel()->GetDynamicCadr();
	if (pDynCadr != nullptr)
	{
		if (pDynCadr->GetLastTool() != nullptr)
			return pDynCadr->GetLastTool();
		return &DummyTool;
	}

	pTool = GetCurTool();
	if(!pTool)
		return &DummyTool;
	return pTool;
}

bool CNCMDoc::IsToolActiveStatic(const NToolCombined* pComTool) const
{
	for (int ChInd = 0; ChInd < GetChannelsCount(); ++ChInd)
	{
		const auto pChannel = GetChannel(ChInd);
		if (pComTool == pChannel->GetCurToolCn())
			return true;
	}
	return false;
}

bool CNCMDoc::GetFullToolMatr(const NToolCombined* pComTool, bool StockView, BMatr& ToolMatr)
{
	bool ToolFound = true;
	if (IsToolActiveStatic(pComTool))
	{
		if (!NTool::IsShapeVisible())
			return false;
		ToolMatr = GetMachTool().GetToolMatr(pComTool->Turret, pComTool->Attach, true);
	}
	else
	{
		if (!GetMachTool().GetDrawAllTools() || !GetMachTool().IsVisible() || StockView)
			return false;
		ToolMatr = pComTool->GetCompMatr() * GetMachTool().GetToolPlaceMatr(pComTool->Position, pComTool->Turret, pComTool->Attach, &ToolFound);
	}
	if (pComTool->GetClass() == AT_TURN)
		ToolMatr = BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.) * ToolMatr;
	ToolMatr = ToolMatr * GetMachTool().GetStockMatr(0, true).invr();
	return ToolFound;
}

void CNCMDoc::StopSlider(int GlobPos)
{
	delete pSliderRRCadr;
	pSliderRRCadr = NULL;
	if(GlobPos <= 0)
	{
		GetUnit()->JumpToProjStart();
	}
	else
	{
		cadrID CadrID = GetIDByGlobPos(GlobPos);
		BProgram *pProg = GetProgram(CadrID);
		if(!pProg)
			return;
		GetUnit()->JumpToHCadrRel(pProg, CadrID.Cadr, false);
	}
	UpdateAllViews(NULL, N_TEXT | N_LEFT | N_OGL | N_STATE);
}

void CNCMDoc::SetSliderPos(int GlobPos)
{
	cadrID CadrID = GetIDByGlobPos(max(1, GlobPos));
	BProgram *pProg = GetProgram(CadrID);
	if(!pProg)
		return;
	if(!pSliderRRCadr)
	{
		pSliderRRCadr = new GReRenderCadr(nullptr);
		pSliderRRCadr->SetMT(&GetMachTool(), GetUnit(), &GetCollManager().GetCollTimeLine());
	}
	pSliderRRCadr->SetProg(pProg);
	int HistPos = 0;
	NToolID ToolID = pProg->GetToolByHist(CadrID.Cadr, &HistPos);
	NToolCombined *pTool = nullptr;
	NGetToolList()->Lookup(ToolID, pTool);
	if(!pTool)
		pTool = &DummyTool;
	if (pSliderRRCadr->GetCadrID().Prog != CadrID.Prog)
		pSliderRRCadr->SetProg(GetProgram(CadrID.Prog));
	pSliderRRCadr->SetA(NAnimState(nullptr, pTool, NCadrGeom(), CadrID, CadrID));
	pSliderRRCadr->FindGeom();
	pSliderRRCadr->NextStep();

	// Set status params
	StatusX = pSliderRRCadr->GetXYZ().GetX();
	StatusY = pSliderRRCadr->GetXYZ().GetY();
	StatusZ = pSliderRRCadr->GetXYZ().GetZ();

	// Set text pos
//	CadrID = GetIDByGlobPos(GlobPos);
	StatusCadr = CadrID;
	if(int(CadrID.Cadr) >= pProg->GetHCadr2HistorySize())
		CadrID.Cadr = pProg->GetHistByHCadr(pProg->GetHCadr2HistorySize() - 1) + 1;
	else
		CadrID.Cadr = pProg->GetHistByHCadr(CadrID.Cadr);
	GotoAbsCadr(pCurChannel, CadrID);
}

BProgram *CNCMDoc::GetProgram(cadrID CadrID)
{
	return GetProgram(CadrID.Prog);
}

BProgram *CNCMDoc::GetProgram(int ProgNum)
{
	CString key;
	BProgram *pProg = NULL;
	NGetProgramList()->GetAt(ProgNum, key, (CObject *&)pProg);
	return pProg;
}

BProgram *CNCMDoc::GetProgram(const CString &ProgName)
{
	BProgram *pProg = NULL;
	for (int i = 0; i < NGetProgramList()->GetSize(); ++i)
	{
		CString Key;
		NProgram *pProg = NULL;
		NGetProgramList()->GetAt(i, Key, (CObject *&)pProg);
		if (Key.CompareNoCase(ProgName) == 0)
			return pProg;
	}
	return nullptr;
}

void CNCMDoc::ReadJobFile(LPCTSTR lpszPathName)
{
	ENSURE(lpszPathName);

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName, CFile::modeRead, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return;
	}

//	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			ReadJobFile(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
		GetUndoHistory().Clear();
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
//		DELETE_EXCEPTION(e);
		return;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified
}

PropList * CNCMDoc::Create(PropList * List)
{
	List->Clear();
	if (GetUnit() == nullptr)
		return List;
	const NCUnitState &State = GetUnit()->GetStartState();
	PropSingle * h = new PropSingle();
	// Position
	h->ID=7;
	h->Name=IDS_STATE_NAME_CurToolN;
	h->Type=5;
	h->Comment=IDS_COMM_PROP_POSITION;
	h->Group=IDS_PROP_START_COORD;
	h->V_Value = State.CurToolN.GetToolNum(); 
	List->Add(h);
	// Turret
	h = new PropSingle();
	h->ID=8;
	h->Name=IDS_PROP_TURRET;
	h->Type=5;
	h->Comment=IDS_COMM_PROP_TURRET;
	h->Group=IDS_PROP_START_COORD;
	h->V_Value = State.CurToolN.GetTurretNum();
	List->Add(h);
	//Start X
	h = new PropSingle();
	h->ID=1;
	h->Name=IDS_START_X;
	h->Type=4;
	h->Comment=IDS_COMM_START_COORD;
	h->Group=IDS_PROP_START_COORD;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.MashX;
	List->Add(h);
	//Start Y
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_START_Y;
	h->Type=4;
	h->Comment=IDS_COMM_START_COORD;
	h->Group=IDS_PROP_START_COORD;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.MashY;
	List->Add(h);
	//Start Z
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_START_Z;
	h->Type=4;
	h->Comment=IDS_COMM_START_COORD;
	h->Group=IDS_PROP_START_COORD;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.MashZ;
	List->Add(h);
	//Start A
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_START_A;
	h->Type=4;
	h->Comment=IDS_COMM_START_COORD;
	h->Group=IDS_PROP_START_COORD;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.MashA;
	List->Add(h);
	//Start B
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_START_B;
	h->Type=4;
	h->Comment=IDS_COMM_START_COORD;
	h->Group=IDS_PROP_START_COORD;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.MashB;
	List->Add(h);
	//Start C
	h = new PropSingle();
	h->ID=6;
	h->Name=IDS_START_C;
	h->Type=4;
	h->Comment=IDS_COMM_START_COORD;
	h->Group=IDS_PROP_START_COORD;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.MashC;
	List->Add(h);
	// Feed mode
	h = new PropSingle();
	h->ID=9;
	h->Name=IDS_STATE_NAME_Feed;
	h->Type=1;
	h->Comment=IDS_COMM_IS_FEED;
	h->Group=IDS_PROP_INITIAL_STATE;
	h->ST_Value = State.FeedM;
	h->For_Enum.Add(IDS_STATE_NOTE_MMpMIN);
	h->For_Enum.Add(IDS_STATE_NOTE_MMpREV);
	h->For_Enum.Add(IDS_STATE_NOTE_MMpTOOTH);
	h->Color = 1;
	List->Add(h);
	//Feed value
	h = new PropSingle();
	h->ID=10;
	h->Name=IDS_IS_FEED_VALUE;
	h->Type=4;
	h->Comment=IDS_COMM_IS_FEED_VALUE;
	h->Group=IDS_PROP_INITIAL_STATE;
//	h->Min=0.;
//	h->Max=360.;
	h->V_Value = State.Feed;
	List->Add(h);
	// Speed mode
	if(NCMComData::GetType() == AT_MILL)
	{
		h = new PropSingle();
		h->ID=11;
		h->Name=IDS_STATE_NAME_SpindleDir;
		h->Type=1;
		h->Comment=IDS_COMM_IS_SPEED;
		h->Group=IDS_PROP_INITIAL_STATE;
		h->ST_Value = State.Spindle[SPINDLE_MILL].SpindleDir;
		h->For_Enum.Add(IDS_STATE_VALUE_R_UNDEF);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CW);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CCW);
		h->Color = 1;
		List->Add(h);
	}
	else if(NCMComData::GetType() == AT_TURN)
	{
		h = new PropSingle();
		h->ID=11;
		h->Name=IDS_STATE_NAME_SpindleDirTurn;
		h->Type=1;
		h->Comment=IDS_COMM_IS_SPEED_TURN;
		h->Group=IDS_PROP_INITIAL_STATE;
		h->ST_Value = State.Spindle[SPINDLE_TURN].SpindleDir;
		h->For_Enum.Add(IDS_STATE_VALUE_R_UNDEF);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CW);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CCW);
		h->Color = 1;
		List->Add(h);
	}
	else 
	{
		h = new PropSingle();
		h->ID=11;
		h->Name=IDS_STATE_NAME_SpindleDirTurn;
		h->Type=1;
		h->Comment=IDS_COMM_IS_SPEED_TURN;
		h->Group=IDS_PROP_INITIAL_STATE;
		h->ST_Value = State.Spindle[SPINDLE_TURN].SpindleDir;
		h->For_Enum.Add(IDS_STATE_VALUE_R_UNDEF);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CW);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CCW);
		h->Color = 1;
		List->Add(h);
		h = new PropSingle();
		h->ID=18;
		h->Name=IDS_STATE_NAME_SpindleDir;
		h->Type=1;
		h->Comment=IDS_COMM_IS_SPEED;
		h->Group=IDS_PROP_INITIAL_STATE;
		h->ST_Value = State.Spindle[SPINDLE_MILL].SpindleDir;
		h->For_Enum.Add(IDS_STATE_VALUE_R_UNDEF);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CW);
		h->For_Enum.Add(IDS_STATE_VALUE_R_CCW);
		h->Color = 1;
		List->Add(h);
	}
	//Speed value
	if(NCMComData::GetType() == AT_MILL)
	{
		h = new PropSingle();
		h->ID=12;
		h->Name=IDS_STATE_NAME_Speed;
		h->Type=4;
		h->Comment=IDS_COMM_IS_SPEED_VALUE;
		h->Group=IDS_PROP_INITIAL_STATE;
	//	h->Min=0.;
	//	h->Max=360.;
		h->V_Value = State.Spindle[SPINDLE_MILL].Speed;
		List->Add(h);
	}
	else if(NCMComData::GetType() == AT_TURN)
	{
		h = new PropSingle();
		h->ID=12;
		h->Name=IDS_STATE_NAME_SpeedTurn;
		h->Type=4;
		h->Comment=IDS_COMM_IS_SPEED_VALUE_TURN;
		h->Group=IDS_PROP_INITIAL_STATE;
	//	h->Min=0.;
	//	h->Max=360.;
		h->V_Value = State.Spindle[SPINDLE_TURN].Speed;
		List->Add(h);
	}
	else 
	{
		h = new PropSingle();
		h->ID=12;
		h->Name=IDS_STATE_NAME_Speed;
		h->Type=4;
		h->Comment=IDS_COMM_IS_SPEED_VALUE;
		h->Group=IDS_PROP_INITIAL_STATE;
	//	h->Min=0.;
	//	h->Max=360.;
		h->V_Value = State.Spindle[SPINDLE_MILL].Speed;
		List->Add(h);

		h = new PropSingle();
		h->ID=19;
		h->Name=IDS_STATE_NAME_SpeedTurn;
		h->Type=4;
		h->Comment=IDS_COMM_IS_SPEED_VALUE_TURN;
		h->Group=IDS_PROP_INITIAL_STATE;
	//	h->Min=0.;
	//	h->Max=360.;
		h->V_Value = State.Spindle[SPINDLE_TURN].Speed;
		List->Add(h);
	}
	//Programing mode
	h = new PropSingle();
	h->ID=14;
	h->Name=IDS_IS_PROGMODE;
	h->Type=1;
	h->Comment=IDS_COMM_IS_PROGMODE;
	h->Group=IDS_PROP_INITIAL_STATE;
	h->ST_Value = State.AbsoluteCoord ? IDS_IS_ABS : IDS_IS_REL;
	h->For_Enum.Add(IDS_IS_ABS);
	h->For_Enum.Add(IDS_IS_REL);
	h->Color = 1;
	List->Add(h);
	//MM-INCH
	h = new PropSingle();
	h->ID=15;
	h->Name=IDS_IS_MM_INCH;
	h->Type=1;
	h->Comment=IDS_COMM_IS_MM_INCH;
	h->Group=IDS_PROP_INITIAL_STATE;
	h->ST_Value = State.MetricCoord ? IDS_IS_MM : IDS_IS_INCH;
	h->For_Enum.Add(IDS_IS_MM);
	h->For_Enum.Add(IDS_IS_INCH);
	h->Color = 1;
	List->Add(h);
	// debugging 
	h = new PropSingle();
	h->ID=16;
	h->Name=IDS_IS_DEBUG;
	h->Type=1;
	h->Comment=IDS_COMM_IS_DEBUG;
	h->Group=IDS_PROP_INITIAL_STATE;
	h->ST_Value = State.Debug ? IDS_TRUE : IDS_FALSE;
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	List->Add(h);
	// File path
	h = new PropSingle();
	h->ID=17;
	h->Name=IDS_PROP_STLFILE;
	h->Type=2;
	h->Comment= IDS_COMM_PROP_STLFILE;
	h->Group=IDS_PROP_INITIAL_STATE;
	h->Value = NCM_PROJECT.GetUnitsPath() + GetUnit()->GetConfFileName();
	h->Color = 0;
	List->Add(h);

	return List;
}

PropList * CNCMDoc::Change(PropList *List, PropSingle *Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;
	NCUnitState &State = GetUnit()->GetStartState();
	switch(id)
	{
	case 1:
		State.MashX = Single->V_Value;
		break;
	case 2:
		State.MashY = Single->V_Value;
		break;
	case 3:
		State.MashZ = Single->V_Value;
		break;
	case 4:
		State.MashA = Single->V_Value;
		break;
	case 5:
		State.MashB = Single->V_Value;
		break;
	case 6:
		State.MashC = Single->V_Value;
		break;
	case 7: // Position
		State.CurToolN.Set(int(Single->V_Value + 0.5), State.CurToolN.GetTurretNum());
		break;
	case 8: // Turret
		State.CurToolN.Set(State.CurToolN.GetToolNum(), int(Single->V_Value + 0.5));
		break;
	case 9: 
		State.FeedM = FeedMeasure(Single->ST_Value);
		break;
	case 10: 
		State.Feed = Single->V_Value;
		break;
	case 11: 
		if(NCMComData::GetType() == AT_MILL)
			State.Spindle[SPINDLE_MILL].SpindleDir = RotDir(Single->ST_Value);
		else if(NCMComData::GetType() == AT_TURN)
			State.Spindle[SPINDLE_TURN].SpindleDir = RotDir(Single->ST_Value);
		else
			State.Spindle[SPINDLE_TURN].SpindleDir = RotDir(Single->ST_Value);
		break;
	case 18:
		State.Spindle[SPINDLE_MILL].SpindleDir = RotDir(Single->ST_Value);
	break;

	case 12: 
		if(NCMComData::GetType() == AT_MILL)
			State.Spindle[SPINDLE_MILL].Speed = Single->V_Value;
		else if(NCMComData::GetType() == AT_TURN)
			State.Spindle[SPINDLE_TURN].Speed = Single->V_Value;
		else
			State.Spindle[SPINDLE_MILL].Speed = Single->V_Value;
		break;
	case 19:
		State.Spindle[SPINDLE_MILL].Speed = Single->ST_Value;
	break;
	case 14:
		State.AbsoluteCoord = (Single->ST_Value == IDS_IS_ABS);
		break;
	case 15:
		State.MetricCoord =	(Single->ST_Value == IDS_IS_MM);
		break;
	case 16:
		State.Debug = (State.Debug == IDS_TRUE);
		break;
	}
	RegeneratePrograms();
	GetUnit()->Reset();
//// Restore previos position
//	GetUnit()->JumpToProg(CurProg);
//	GetUnit()->JumpToCadr(CurHCadr);

	SetModifiedFlag();
	UpdateAllViews(NULL,N_OGL | N_STATE | N_LEFT | N_PROGRAMCHANGED, NULL);
	return 	Create(List);
}
int CNCMDoc::FindUnProtGlobPos(int GlobPos, bool Next/* = false*/)
{// METHOD IS DEVELOPED BUT NOT TESTED
	// Returns nearest unprotected global position
	// If Next - find next position else find previous position
	GlobPos = max(0, GlobPos);
	if(!NCUnit::IsFileProtectionEnabled())
		return GlobPos;
		
	cadrID CadrID(-1);
	int PartProgramsSize = 0;
	CMapStringToNProgram &ProgList = *NGetProgramList();
	// Find running program
	int i = 0, Size = 0;
	for(; i < ProgList.GetMainProgCount(); ++i)
	{
		BProgram *pProg = ProgList.GetProg(cadrID(i));
		int Size = 1 + pProg->GetHCadr2HistorySize();
		PartProgramsSize += Size;
		if(PartProgramsSize > GlobPos)
		{
			CadrID.Prog = i;
			CadrID.Cadr = GlobPos - PartProgramsSize + Size;
			break;
		}
	}
	if(CadrID.Prog < 0)
		return 0;

	int j = int(CadrID.Cadr);// Index to iterate withing a running program
	if(Next)
	{
		bool FirstProg = true;
		for(; i < ProgList.GetMainProgCount(); ++i)
		{
			BProgram *pProg = ProgList.GetProg(cadrID(i));
			if(!FirstProg)
				j = 0;
			for(; j < pProg->GetHCadr2HistorySize(); ++j, ++GlobPos)
			{
				cadrID ID = pProg->GetCadrByHist(pProg->GetHistByHCadr(int(j)));
				if(!NGetProgramList()->IsCadrProtected(CadrID))
					return GlobPos;
			}
			FirstProg = false;
		}
	}
	else
	{
		for(; i >= 0; --i)
		{
			BProgram *pProg = ProgList.GetProg(cadrID(i));
			if(j < 0) 
				j = pProg->GetHCadr2HistorySize();
			for(; j >= 0; --j, --GlobPos) 
			{
				cadrID ID = pProg->GetCadrByHist(pProg->GetHistByHCadr(int(j)));
				if(!NGetProgramList()->IsCadrProtected(CadrID))
					return GlobPos;
			}
		}
	}
	return 0;// Position not found
}

cadrID CNCMDoc::GetIDByGlobPos(int GlobPos, int DeltaPos /* = 0*/) const
{
	// Returns ID with running program
	// works with DeltaPos <= 0 only now
	GlobPos = max(0, GlobPos);
		
	cadrID CadrID;
	CadrID.Prog = -1;
	CadrID.Cadr = 0;
	int PartProgramsSize = 0;
	const CMapStringToNProgram &ProgList = *NGetProgramList();
	// Find running program
	int i = 0, Size = 0;
	for(; i < ProgList.GetMainProgCount(); ++i)
	{
		BProgram *pProg = ProgList.GetProg(cadrID(i));
		Size = pProg->GetHCadr2HistorySize();
		PartProgramsSize += Size;
		if(PartProgramsSize > GlobPos)
		{
			CadrID.Prog = i;
			CadrID.Cadr = GlobPos - PartProgramsSize + Size;
			break;
		}
	}
	if(CadrID.Prog < 0)
	{
		--i;
		CadrID.Prog = i;
		if(CadrID.Prog < 0)
			return CadrID;
		CadrID.Cadr = Size;
	}

	if(DeltaPos < 0)
	{
		int k = 0;// Index to compare with DeltaPos
		int j = CadrID.Cadr;// Index to iterate withing a running program
		bool ProtectionEnabled = NCUnit::IsFileProtectionEnabled();
		for(; i >= 0; --i)
		{
			BProgram *pProg = ProgList.GetProg(cadrID(i));
			if(j < 0)
				j = pProg->GetHCadr2HistorySize();
			cadrID ID;
			if(j == 0)
				j = -1;
			else
				ID = pProg->GetCadrByHist(pProg->GetHistByHCadr(j - 1));
			for (; j >= 0 && k > DeltaPos;)
			{
				--j;
				cadrID PrevID = (j > 0) ? pProg->GetCadrByHist(pProg->GetHistByHCadr(j - 1)) : cadrID(-1);
				if(PrevID != ID)
				{
					if(ProtectionEnabled)
					{
						if(!NGetProgramList()->IsCadrProtected(ID))
							--k;
					}
					else
						--k;
				}
				ID = PrevID;
			}
			if(k == DeltaPos)
				break;
		}
		CadrID.Prog = i;
		CadrID.Cadr = j;
	}
	else if(DeltaPos > 0)
	{
	}

	if (CadrID.Cadr == Size)
		CadrID.Cadr--;

	return CadrID;
}
NToolCombined * CNCMDoc::MakeNewTool(const NToolID &ToolID, const CString &Name)
{
	NToolCombined *pNewTool = NULL;
	if(NCMComData::IsTurnEnabled())
	{
		if(Name.IsEmpty())
		{
			pNewTool = new NToolCombinedTurn(_T("AutoTurn"));
		}
		else
		{
			pNewTool = new NToolCombinedTurn(Name);
			pNewTool->IsNameAuto = false;
		}
	}
	else
	{
		if(Name.IsEmpty())
		{
			pNewTool = new NToolCombinedMill(_T("AutoMill"));
		}
		else
		{
			pNewTool = new NToolCombinedMill(Name);
			pNewTool->IsNameAuto = false;
		}
	}
	pNewTool->Position = ToolID.GetToolNum();
	pNewTool->Turret = ToolID.GetTurretNum();
	pNewTool->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
	NGetToolList()->SetAt(ToolID, pNewTool);
	return pNewTool;
}

void CNCMDoc::CopyTools2Clipboard(const CStringArray &CheckedTools)
{
	if(!pClipboard)
		return;
	for(int i = 0; i < CheckedTools.GetSize(); ++i)
	{
		NToolCombined *pSourceTool = NULL;
		NGetToolList()->Lookup(CheckedTools[i], pSourceTool);
		if(pSourceTool)
		{
			NToolCombined *pDestTool = pSourceTool->CreateCopyTool();
			pClipboard->NToolList.SetAt(CheckedTools[i], pDestTool); 
		}
	}
}

void CNCMDoc::PrepClipboard()
{
	if(!pClipboard)
		pClipboard = new NDocClipboard;
	else
		pClipboard->NToolList.FullRemoveAll();
	pClipboard->AppType = NCMComData::GetType();
	pClipboard->BaseDocPath = GetPathName();
}

void CNCMDoc::PasteToolsFrClipboard(void)
{
	if(!pClipboard)
		return;

	if(pClipboard->BaseDocPath.IsEmpty())
	{
		switch(pClipboard->AppType)
		{
		default:
		case AT_MILL:
			theApp.OnFileNewMill();
			break;
		case AT_TURN:
			theApp.OnFileNewTurn();
			break;
		case AT_MILL_TURN:
			theApp.OnFileNewMillturn();
			break;
		case AT_WELD:
			theApp.OnFileNewWeld();
			break;
		}
	}
	else
		GetDocTemplate()->OpenDocumentFile(pClipboard->BaseDocPath);
	for(int i = 0; i < pClipboard->NToolList.GetSize(); ++i)
	{
		NToolCombined *pSourceTool = NULL;
		CString Key;
		pClipboard->NToolList.GetAt(i, Key, (CObject *&)pSourceTool);
		NToolCombined *pDestTool = pSourceTool->CreateCopyTool();
		if ( pDestTool == NULL )
			continue;
		switch(pDestTool->GetClass())
		{
		case AT_MILL:
			if(!NCMComData::IsMillEnabled())
				continue;
			break;
		case AT_TURN:
			if(!NCMComData::IsTurnEnabled())
				continue;
			break;
		case AT_WELD:
			if(!NCMComData::IsWeldEnabled())
				continue;
			break;
		default:
			continue;
			break;
		}
		NToolID ToolID;
		while(1)
		{
			ToolID.Set(pDestTool->Position, pDestTool->Turret);
			NToolCombined *pOldTool;
			if(NGetToolList()->Lookup(ToolID, pOldTool))
				++pDestTool->Position;
			else
				break;
		}
		
		(*NGetToolList())[ToolID] = pDestTool;
		SetModifiedFlag();
		pDestTool->SetGroup(OG_TOOLS);
	}
}

void CNCMDoc::DeleteUnUsedTools(void)
{
	NGetToolList()->ClearUsed();
	RegeneratePrograms();
	NGetToolList()->DeleteUnUsed();
	SetModifiedFlag();
}

void CNCMDoc::ReportSaveLoadException(LPCTSTR lpszPathName,
				CException* e, BOOL bSaving, UINT nIDPDefault)
{
	if(!IsSilent())
		CDocument::ReportSaveLoadException(lpszPathName, e, bSaving, nIDPDefault);
}


cadrID CNCMDoc::FindNextToolHID(bool *pProjEnd) const
{
	cadrID StartCadrID = GetUnit()->GetState()->GetCurCadrIDEx();
	cadrID DestHID = NGetProgramList()->GetNearestTCFor(StartCadrID, pProjEnd);
	++DestHID.Cadr;
	return DestHID;
}
cadrID CNCMDoc::FindPrevToolHID() const
{
	cadrID StartCadrID = GetUnit()->GetState()->GetCurCadrIDEx();
	cadrID DestHID = NGetProgramList()->GetNearestTCBack(StartCadrID);
	++DestHID.Cadr;
	return DestHID;
}

void CNCMDoc::JumpToNextTool()
{
// Jump to the next tool change
	ClearAllCollisions();
	bool ProjEnd = false;
	cadrID DestHID = FindNextToolHID(&ProjEnd);
	GetUnit()->JumpForToHCadr(DestHID);
	SetProgrTDT(DestHID.Prog);
	UpdateAllViews(NULL, N_CADRCHANGED | N_STATE | N_OGL);
}

void CNCMDoc::JumpToPrevTool()
{
// Jump to the previous tool change
	ClearAllCollisions();
	cadrID DestHID = FindPrevToolHID();
	GetUnit()->JumpForToHCadr(DestHID);
	SetProgrTDT(DestHID.Prog);
	UpdateAllViews(NULL, N_CADRCHANGED | N_OGL);
}

BBox CNCMDoc::GetExObjGabar(void)
{
	return NExObjList.GetGabar();
}

void CNCMDoc::ResetCutting(void)
{
	ResetInterrupt();
	for (int i = 0; SetNextChannel(i);)
	{
		pCurChannel->GetProg2BSP().Init(false);
		pCurChannel->FillMachCopy();
		pCurChannel->GetCutting().Init();
		pCurChannel->GetAnimateDisp().ClearQueues();
	}
}

const BBox CNCMDoc::GetModifStockGabar(int index) // # TODO значение по умолчанию
{
	BBox StockG;
	NStock * pS = GetStock(index);
	if(pS)
		StockG = pS->GetGabar();
	else
		StockG.Expand(0., 0., 0.);
		
	BBox ExObjGabar = GetExObjGabar();
	if(ExObjGabar.IsDefined())
	{
		StockG.Zmax = max(ExObjGabar.Zmax, StockG.Zmax);
		StockG.Zmin = min(ExObjGabar.Zmin, StockG.Zmin);
	}
	return StockG;
}

const BBox CNCMDoc::GetModifStocksGabar()
{
	BBox StockG;
	for (int index = 0; index < NStocksList.GetSize(); ++index)
		StockG.Expand(GetModifStockGabar(index));
	return StockG;
}

void CNCMDoc::StoreCurrent2UCS(int UCS)
{
	GetUnit()->StoreCurrent2UCS(UCS);
	RegeneratePrograms();
	GetUnit()->Reset();
	UpdateAllViews(NULL, N_OGL);

}

void CNCMDoc::GetToolMatr(BMatr &M)
{
	const NToolCombined *pTool = GetActiveTool();
	int Tur = 0, Att = 0;
	if(pTool)
	{
		Att = pTool->Attach;
		Tur = pTool->Turret;
	}
	M = GetMachTool().GetStock2ToolMatr(GetUnit()->GetCurStockPosition(), Tur, Att);
}

bool CNCMDoc::SetBoxPointApply(const BPoint &P, const BPoint &N)
{
	bool Res = GetUnit()->PlaceTool(P, N);
	GetMachTool().ProcCadrStep(*GetUnit(), *GetUnit()->GetState());
	BMatr PosToolMatr;
	GetToolMatr(PosToolMatr);
	GetPosTool().Activate(PosToolMatr);
	UpdateAllViews(NULL, N_OGL | N_CADRCHANGED | N_STATE);
	return Res;
}

int CNCMDoc::SetUCSOnBoxApply(const BMatr &CS, int Solution)
{
	int Res = GetUnit()->PlaceTool(CS, Solution);
	GetMachTool().ProcCadrStep(*GetUnit(), *GetUnit()->GetState());
	//BMatr PosToolMatr;
	//GetToolMatr(PosToolMatr);
	GetPosTool().Activate(CS);
	UpdateAllViews(NULL, N_OGL | N_CADRCHANGED | N_STATE);
	return Res;
}

void CNCMDoc::SetBoxPointEnd()
{
	GetPosTool().DeActivate();
	GetUnit()->Reset();
	UpdateAllViews(NULL, N_OGL | N_CADRCHANGED);
}

void CNCMDoc::JumpToProjStart(void)
{
	ClearAllCollisions();
	GetUnit()->JumpToProjStart();
}

void CNCMDoc::LoadSingleStock(CArchive& ar)
{
	NStock * pS;
	ar >> (CObject *&)pS;
	if(pS)
	{
		NStock::SerializeExt(ar);
		pS->WFrame = TRUE;
		if(pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
		{
			if(!((NStockStl *)pS)->IsCreated)
			{
				delete pS;
				pS = NULL;
			}
		}
	}
	if (pS)
	{
		NStocksList.SetSharpEdges(pS->GetEdgesS() == TRUE);
		NStocksList.SetSharpEdgesDX(pS->GetEdgesDX() == TRUE);
		NStocksList.SetColor(pS->GetColor());
		NStocksList.SetVisible(pS->IsVisible());
		NStocksList.SetWFrame(pS->GetWFrame() == TRUE);
		NStocksList.SetRMode(pS->GetRMode());
		AddStock(pS);
	}
}

void CNCMDoc::SerializeStocks(CArchive& ar)
{
	NStocksList.Serialize(ar);
	NStocksList.PostSerialize();
}

void CNCMDoc::SerializeTraj(CArchive& ar)
{
	if (ar.IsLoading())
	{
		int Size;
		SerializeElements(ar, &Size, 1);
		BYTE* pBuf = new BYTE[Size];
		SerializeElements(ar, pBuf, Size);
		delete[] pBuf;
	}
	else
	{
		BProgram* pProg = GetCurChannel()->GetProgram(0);
		if (pProg == nullptr)
		{
			int Size = 0;
			SerializeElements(ar, &Size, 1);
		}
		else
		{
			pProg->SerializeLines(ar);
		}
	}
}

void CNCMDoc::FillEnvironment(void)
{
	NCM_PROJECT.UnitConfs.SetSize(GetChannelsCount());
	for each(auto pCh in Channels)
		pCh->GetUnitCn()->FillUnitFile();
	FillStls(*NGetPartList(), NCM_PROJECT.PartsFile);
	FillStls(*NGetExObjList(), NCM_PROJECT.ExObjsFile);
	GetMachTool().FillMachineFile(NCM_PROJECT.MachineFile);
	FillStocks();
	FillTools();
	FillDFNFile();
}

void CNCMDoc::FillStocks(void)
{
	for (int i = 0; i < NStocksList.GetSize(); ++i)
	{
		CString STLStockName;
		const NStock *pStock = GetStock(i);
		if (pStock)
			if (pStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
				STLStockName = ((NStockStl *)pStock)->GetFileName();
		const int index = STLStockName.ReverseFind('\\');
		STLStockName.Delete(0, index + 1);
		NCM_PROJECT.StockFile.Add(STLStockName);
		NCM_PROJECT.StockFile.Add(_T(""));
		if (!STLStockName.IsEmpty())
		{
			NCMProject::LoadFile2Str(MakeInputFileName(((NStockStl *)pStock)->GetFileName()), NCM_PROJECT.StockFile[2 * i + 1]);
		}
	}
}

CString CNCMDoc::MakeInputFileName(const CString& FileName)
{
	CString NewFileName = FileName;
	// Read stl file
	CStdioFile f;
	if (!f.Open(NewFileName, CFile::typeText | CFile::modeRead))
	{//Try to find stl file in DEMO subdirectory
		const int pos = NewFileName.ReverseFind('\\');
		CString ShortFileName = NewFileName.Right(NewFileName.GetLength() - pos - 1);
		NewFileName = NCM_PROJECT.GetDemoPath() + ShortFileName;
		if (!f.Open(NewFileName, CFile::typeText | CFile::modeRead))
		{//Try to find stl file in the project subdirectory
			CString ProjFileName = GetPathName();
			const int pos = ProjFileName.ReverseFind('\\');
			CString ProjPath;
			if (pos >= 0)
				ProjPath = ProjFileName.Left(pos + 1);
			if (!ProjPath.IsEmpty() && f.Open(NewFileName, CFile::typeText | CFile::modeRead))
				NewFileName = ProjPath + ShortFileName;
			else
				NewFileName.Empty();
		}
	}
	return NewFileName;
}

void CNCMDoc::FillTools(void)
{
	CString key;
	NToolCombined *pToolCom = NULL;
	for (int i = 0; i < NToolList.GetSize(); ++i)
	{
		NToolList.GetAt(i, key, (CObject *&)pToolCom);
		if (pToolCom)
		{
			for (int k = 0; k < pToolCom->GetToolsNum(); ++k)
			{
				NTool *pTool = pToolCom->GetTool(k);
				if (pTool->IsKindOf(RUNTIME_CLASS(NToolTurnHolderSTL)))
				{
					CString STLName = static_cast<NToolTurnHolderSTL *>(pTool)->GetFileName();
					int index = STLName.ReverseFind('\\');
					STLName.Delete(0, index + 1);
					NCM_PROJECT.ToolFile.Add(STLName);
					NCM_PROJECT.ToolFile.Add(_T(""));
					if (!STLName.IsEmpty())
					{
						NCMProject::LoadFile2Str(MakeInputFileName(static_cast<NToolTurnHolderSTL *>(pTool)->GetFileName())
							, NCM_PROJECT.ToolFile[NCM_PROJECT.ToolFile.GetSize() - 1]);
					}
				}
			}
		}
	}
}

void CNCMDoc::FillDFNFile()
{
	CString path = NCM_PROJECT.GetSettingsPath();
	CString ext;
	ext.LoadString(IDS_SETTINGS_EXT);
	if (DFNFileName.IsEmpty())
	{
		DFNFileName.LoadString(IDS_SETTINGS_FILENAME);
		DFNFileName += ext;
	}
	NCMProject::LoadFile2Str(path + "\\" + DFNFileName, NCM_PROJECT.DFNFile);
	NCM_PROJECT.DFNFile = DFNFileName + "\n" + NCM_PROJECT.DFNFile; // change in 2025
}

void CNCMDoc::FillStls(BMapStringToNStock &Map, CStringArray &StringArray)
{
	int k = -1;
	StringArray.RemoveAll();
	for each(const auto& el in Map)
	{
		const NStock *pS = static_cast<NStock*>(el.second);
		if(!pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
			continue;
		CString FileName = ((NStockStl*)pS)->GetFileName();
		int index = FileName.ReverseFind( '\\' );
		CString ShortFileName = FileName.Right(FileName.GetLength() - index - 1);
		ShortFileName = ShortFileName.Left(ShortFileName.GetLength() - 4); // remove extension

		StringArray.Add(ShortFileName);
		StringArray.Add(_T(""));
		k += 2;
		NCMProject::LoadFile2Str(MakeInputFileName(FileName), StringArray[k]);
	}
}

void CNCMDoc::ClearAnimateQueues()
{
	for each (auto pEl in Channels)
	{
		pEl->SetChainEnded(true);
		pEl->GetAnimateDisp().ClearQueues();
	}
}
bool CNCMDoc::SaveProgText(const CString &ProgName, const CString &NewName)
{
	// Save to ProgName if NewName is empty
	CMapStringToNProgram &ProgList = *NGetProgramList();
	auto *pProg = ProgList.GetProg(ProgName);
	if (pProg == nullptr)
		return false;
	if (NewName.IsEmpty())
		SaveProgText(ProgName, static_cast<NProgram *>(pProg));
	else
		SaveProgText(NewName, static_cast<NProgram *>(pProg));
	return true;
}

void CNCMDoc::SaveProgText(const CString &filename, NProgram *pNProg) const
{
//	ASSERT(FALSE);
	CStdioFile f(filename,
		CFile::typeText | CFile::modeWrite | CFile::modeCreate);

	// Translit
	CString Val;
	if (GetUnit()->GetOtherValue("Translit", Val))
	{
		if (Val == "Yes")
		{
			if (GetUnit()->GetWordValue("CommentStart", Val))
			{
				char Cs = Val[0];
				if (GetUnit()->GetWordValue("CommentEnd", Val))
				{
					char Ce = Val[0];
					pNProg->TranslitText(false, Cs, Ce);
				}
			}
		}
	}
	// Remove additional text if any
	CString Symbol;
	GetUnit()->GetWordValue("AdditionalTextStart", Symbol);
	for (int i = 0; i < pNProg->GetSize(); i++)
	{
		CString str(pNProg->GetCadrText(i));
		if (!Symbol.IsEmpty())// Remove additional text
			if (str == Symbol)
				break;
		f.WriteString(str + "\n");
	}
	f.Close();
}

void CNCMDoc::SaveAllProgramsText()
{
	CMapStringToNProgram &ProgList = *NGetProgramList();
	for (int i = 0; i < ProgList.GetSize(); ++i)
	{
		CObject *pProg;
		CString key;
		ProgList.GetAt(i, key, pProg);
		SaveProgText(key, static_cast<NProgram *>(pProg));
	}
}

bool CNCMDoc::ShowProgrText(int ind)
{
	bool Res = SetProgrTDT(ind);
	SetModifiedFlag();
	UpdateAllViews(NULL, N_NEW_TEXT | N_CADRCHANGED, NULL);
	return Res;
}

bool CNCMDoc::ShowProgrText(const CString &ProgName)
{
	CMapStringToNProgram *pPrL = NGetProgramList();
	int ind = pPrL->GetIndex(ProgName);
	if (ind < 0)
		return false;
	return ShowProgrText(ind);
}

INT_PTR CNCMDoc::CalcSubObjects() const
{
	const MTPartArray &Parts = GetMachTool().GetParts();
	INT_PTR res = NGetProgramList()->GetSize() + NGetExObjList()->GetSize() + NGetPartList()->GetSize() + NGetToolList()->GetSize() + Parts.GetSize();
	if (IsThereMoreThanOneStock())
		res += NGetStockList()->GetSize();
	for (int i = 0; i < NGetToolList()->GetSize(); ++i)
	{
		NToolCombined *pTool = nullptr;
		CString key;
		NGetToolList()->GetAt(i, key, (CObject *&)pTool);
		if (pTool != nullptr)
			if (pTool->num_tools > 1)
				res += pTool->num_tools;
	}
	return res;
}

bool CNCMDoc::CheckProgExist()
{
	ClearAllCollisions();
	if (IsThereAnyProgr())
		return true;
	AfxMessageBox(IDS_COMP_NOPROG);
	return false;
}

bool CNCMDoc::IsCadrVisible(cadrID CadrID) const
{
	int ChanCount = GetChannelsCount();
	int ProgInd = CadrID.Prog;
	int ChanInd = ProgInd % ChanCount;
	ProgInd /= ChanCount;
	auto pChannel = GetChannel(ChanInd);
	// Cadr is cadrID in history
	const CMapStringToNProgram &ProgramList = *pChannel->NGetProgramListCn();
	if (!ProgramList.Visible)
		return false;
	CString ProgName = ProgramList.GetKey(ProgInd);
	const BProgram *pProg = ProgramList.GetProg(ProgName);
	if (pProg == nullptr)
		return false;
	if (!pProg->IsVisible())
		return false;
//	CadrID = pProg->GetCadrByHist(CadrID.Cadr);
	int HistPos = 0;
	NToolID ToolID = pProg->GetToolByHist(CadrID.Cadr, &HistPos);
	NToolCombined *pTc;
	if (NGetToolList()->Lookup(ToolID, pTc))
	{
		if (!pTc->IsVisible())
			return false;
	}
	if (ProgramList.PartialDraw == TrajPart::TP_PART || ProgramList.PartialDraw == TrajPart::TP_WORM || ProgramList.GetDrawOnlyActiveToolPart())
	{
		CString ActiveProgName = pChannel->GetUnitCn()->GetState()->GetCurProg();
		cadrID ActiveCadrID;
		ActiveCadrID.Prog = max(0, ProgramList.GetIndex(ActiveProgName));
		ActiveCadrID.Cadr = pChannel->GetUnitCn()->GetState()->GetCurHCadr() - 1;
		unsigned int ActiveHistInd = pProg->GetHistByHCadr(ActiveCadrID.Cadr);
		if (ProgramList.PartialDraw == TrajPart::TP_PART)
		{
			if (ActiveCadrID.Prog < ProgInd)
				return false;
			if (ActiveCadrID.Prog == ProgInd && ActiveHistInd < CadrID.Cadr)
				return false;
		}
		if (ProgramList.PartialDraw == TrajPart::TP_WORM)
		{
			if (ActiveCadrID.Prog != ProgInd)
				return false;
			if (ActiveHistInd + ProgramList.WormAfter < CadrID.Cadr || ActiveHistInd > CadrID.Cadr + ProgramList.WormBefore) // ActiveCadrID.Cadr - ProgramList.WormBefore > CadrID.Cadr - incorrect
				return false;
		}
		if (ProgramList.GetDrawOnlyActiveToolPart())
		{
			const BProgram *pActiveProg = ProgramList.GetProg(ActiveProgName);
			NToolID ActiveToolID = pActiveProg->GetToolByHist(ActiveCadrID.Cadr, &HistPos);
			if (ActiveToolID != ToolID)
				return false;
		}
	}
	return true;
}

bool CNCMDoc::GetCadrIDStrings(cadrID IDabs, CString& Chan, CString& Prog, CString& Line, CString& Step)
{
	NChannel* pChannel = GetCurChannel();
	int ChanCount = GetChannelsCount();
	if (ChanCount > 1)
	{
		int ProgInd = IDabs.Prog;
		int ChanInd = ProgInd % ChanCount;
		IDabs.Prog /= ChanCount;
		Chan.Format(_T("%d"), ChanInd);
		pChannel = GetChannel(ChanInd);
	}
	else
		Chan.Empty();

	cadrID ID0, ID1;
	IDabs.SplitMix(ID0, ID1);
	const CMapStringToNProgram& ProgramList = *pChannel->NGetProgramListCn();
	Prog = ProgramList.GetKey(ID0.Prog);
	int index = Prog.ReverseFind('\\');
	Prog = Prog.Right(Prog.GetLength() - index - 1);
	const BProgram* pProg = ProgramList.GetProg(ID0);
	if (pProg == nullptr)
		return false;
	if (IDabs.Prog < -1)
	{
		Step.Format("%d : %d", ID0.Cadr, ID1.Cadr);
		ID0 = pProg->GetCadrByHist(ID0.Cadr);
		ID1 = pProg->GetCadrByHist(ID1.Cadr);
		Line.Format("%d : %d", ID0.Cadr + 1, ID1.Cadr + 1);
	}
	else
	{
		Step.Format("%d", IDabs.Cadr);
		IDabs = pProg->GetCadrByHist(IDabs.Cadr);
		Line.Format("%d", IDabs.Cadr + 1);
	}
	return true;
}

const NCadrGeom * CNCMDoc::GetCadrIfVisible(cadrID ID, bool CompCadr, int ChInd, int StPos) const
{
	const auto pChannel = GetChannel(ChInd);
	// Cadr is cadrID in history
	const CMapStringToNProgram &ProgramList = *pChannel->NGetProgramListCn();
	if (!ProgramList.Visible)
		return nullptr;
	CString ProgName = ProgramList.GetKey(ID.Prog);
	const BProgram *pProg = ProgramList.GetProg(ProgName);
	if (pProg == nullptr)
		return nullptr;
	if (!pProg->IsVisible())
		return nullptr;
	cadrID CadrID = pProg->GetCadrByHist(ID.Cadr);
	int HistPos = 0;
	NToolID ToolID = pProg->GetToolByHist(ID.Cadr, &HistPos);
	NToolCombined *pTc;
	if (NGetToolList()->Lookup(ToolID, pTc))
	{
		if (!pTc->IsVisible())
			return nullptr;
	}
	if (ProgramList.PartialDraw == TrajPart::TP_PART || ProgramList.PartialDraw == TrajPart::TP_WORM || ProgramList.GetDrawOnlyActiveToolPart())
	{
		CString ActiveProgName = GetUnit()->GetState()->GetCurProg();
		cadrID ActiveCadrID;
		ActiveCadrID.Prog = max(0, ProgramList.GetIndex(ActiveProgName));
		ActiveCadrID.Cadr = GetUnit()->GetState()->GetCurHCadr() - 1;
		unsigned int ActiveHistInd = pProg->GetHistByHCadr(ActiveCadrID.Cadr);
		if (ProgramList.PartialDraw == TrajPart::TP_PART)
		{
			if (ActiveCadrID.Prog < ID.Prog)
				return nullptr;
			if (ActiveCadrID.Prog == ID.Prog && ActiveHistInd < ID.Cadr)
				return nullptr;
		}
		if (ProgramList.PartialDraw == TrajPart::TP_WORM)
		{
			if (ActiveCadrID.Prog != ID.Prog)
				return nullptr;
			if (ActiveHistInd + ProgramList.WormAfter < ID.Cadr || ActiveHistInd > ID.Cadr + ProgramList.WormBefore) // ActiveCadrID.Cadr - ProgramList.WormBefore > CadrID.Cadr - incorrect
				return nullptr;
		}
		if (ProgramList.GetDrawOnlyActiveToolPart())
		{
			const BProgram *pActiveProg = ProgramList.GetProg(ActiveProgName);
			if(pActiveProg == nullptr)
				return nullptr;
			NToolID ActiveToolID = pActiveProg->GetToolByHist(ActiveCadrID.Cadr, &HistPos);
			if (ActiveToolID != ToolID)
				return nullptr;
		}
	}
	return CompCadr ? pProg->GetCompGeomArrayByStPos(StPos)[ID.Cadr] : pProg->GetGeomArrayByStPos(StPos)[ID.Cadr];
}

void CNCMDoc::DrawTool(RenderMode GlobRMode, const NToolCombined * pTool)
{
	if (GlobRMode == RM_TRANSLUCENT && !NTool::IsTranslucent())
		return;
	if (!pTool)
		return;
	bool SectionActive = false;
	if (pStockSection)
		if (pStockSection->IsActive() && pStockSection->NeedCutAll())
			SectionActive = true;
	if (SectionActive)
		pStockSection->EnableClipPlane();
	pTool->Draw(GlobRMode);
	if (SectionActive)
		pStockSection->DisableClipPlane();
}

MTMachineTool& CNCMDoc::GetMachTool(int MTNum /* = -1*/)// -1 - base machine; 0... - single configuration
{
	return MachTool;
}

const MTMachineTool& CNCMDoc::GetMachTool(void) const
{
	return MachTool;
}

bool CNCMDoc::LoadMachTool(const CString& path, const CString& name)
{
	bool Ret = MachTool.LoadXML(path, name);
	return Ret;
}

bool CNCMDoc::SetProgrTDT(const CString& str)
{
	return pCurChannel->SetProgrTDTCn(str);
}

bool CNCMDoc::SetProgrTDT(int ProgInd)
{
	return pCurChannel->SetProgrTDTCn(ProgInd);
}

void CNCMDoc::SetCurProgrTDT()
{
	pCurChannel->SetCurProgrTDTCn();
}

const CString& CNCMDoc::GetProgrTDT() const
{
	return pCurChannel->GetProgrTDTCn();
}

void CNCMDoc::ImplRepl(const BProgram* pProg) const
{
	pCurChannel->ImplReplCn(pProg);
}

bool CNCMDoc::SetCurChannel(int i)
{
	bool res = false;
	if (i >= 0 && i < Channels.size())
	{
		if (pCurChannel != Channels.at(i))
		{
			pCurChannel = Channels.at(i);
			FillMUPair();
			res = true;
		}
	}
	if (res)
	{
		StatusCadr.Prog = -1;
	}
	return res;
}

void CNCMDoc::AddNewChannel()
{
	Channels.push_back(new NChannel());
	NCM_PROJECT.SetUseQuadTreeParallel(!IsMultiChannel());
	if(pCurChannel == nullptr)
		pCurChannel = Channels[0];
	Channels.back()->MakeNewChannel(this, int(Channels.size() - 1));
}

void CNCMDoc::DeleteChannel(int ChannelInd)
{
	if (GetChannelsCount() < 2)
		return;
	if (ChannelInd < 0 || ChannelInd >= GetChannelsCount())
		return;
	NChannel* pChan2Delete = GetChannel(ChannelInd);

	if (GetCurChannel()->GetChannelInd() == ChannelInd)
		SetCurChannel((ChannelInd + 1) % GetChannelsCount());

	delete pChan2Delete;
	for (int i = ChannelInd; i < GetChannelsCount() - 1; ++i)
	{
		Channels.at(i) = Channels.at(i + 1);
		GetChannel(i)->MakeNewChannel(this, i);
	}
	Channels.resize(Channels.size() - 1);
	UpdateAllViews(nullptr, N_UNIT_CHANGED | N_TEXT | N_LEFT | N_STATE | N_TOOLCHANGED | N_PROGRAMCHANGED);
}

bool CNCMDoc::SetNextChannel(int& Iter)
{
	static int BaseInd = 0;
	if (Channels.size() == 0)
		return false;
	if (Iter >= Channels.size())
	{
		pCurChannel = Channels.at(BaseInd);
		FillMUPair();
		return false;
	}
	if (Iter == 0)
	{
		BaseInd = pCurChannel->GetChannelInd();
		Iter++;
		return true;
	}
	pCurChannel = Channels.at((BaseInd + Iter) % Channels.size());
	FillMUPair();
	Iter++;
	return true;
}

NChannel* CNCMDoc::GetNextCuttingChannel(const NChannel& Chan) const
{
	NChannel* pNextChannel = Channels.at((Chan.GetChannelInd() + 1) % int(Channels.size()));
	for (int i = 0
		; i < Channels.size()
		; pNextChannel = Channels.at((pNextChannel->GetChannelInd() + 1) % int(Channels.size())), ++i)
	{
		if (!pNextChannel->GetCutting().IsStopped())
			return pNextChannel;
	}
	return nullptr;
}

bool CNCMDoc::RunAnimateStep(int& MaxRunStepsNum)
{
	int End = -1;
	int Start = 0;
	int HStart = 0;
	int HEnd = -1;
//	bool DoAnimateUpdate = false;
	RunStepsStopReason CommonRes = SR_OK;
	std::vector<TimeSlot> TimeSlots(Channels.size());
	GetTimeSlots(TimeSlots);
	for each(auto pCh in Channels)
	{
//		GetMachTool().MTBase::Reset();
		int N1 = -1;
		int* pN = pCh == pCurChannel ? &MaxRunStepsNum : &N1;// we need -1 for all channels except current
		if (NeedRunAnimStep(TimeSlots, pCh))
		{
			RunStepsStopReason res = pCh->RunAnimateStepCn(this, *pN);
//			DoAnimateUpdate &= pCh->IsInsideCadrCn();
			if (res == SR_EMPTY)// internal error
				return false;
			switch (res)
			{
			case SR_BREAK:
				if (pCh->GetChannelInd() == 0)
					CommonRes = SR_BREAK;
				break;
			}
		}
	}
	bool Res = true;
	if (CommonRes == SR_BREAK && MaxRunStepsNum < 0)
	{
		Res = false;
	}
	else
	{
		//if (DoAnimateUpdate)// ISSUE 54
		//	pMainFrame->AnimateUpdate();
		//else
//			UpdateAllViews(nullptr, N_STATE | N_CADRCHANGED | N_SELECTTEXT | N_OGL, nullptr);
	}

	return Res;
}

void CNCMDoc::StartAnimWalker(bool UseSetHStart)
{
	BBox StockGabar;
	if (IsThereAnyStock())
		StockGabar = GetStocksGabar();
	for each(auto pCh in Channels)
	{
		pCh->StartAnimWalkerCn(UseSetHStart, StockGabar, GetMachTool());
	}
}

void CNCMDoc::Reset()
{
	bool NeedRegen = false;
	Comparison.DeActivate();
	if (GetAutoUpdateProg())
	{
		ReloadAllProg();
	}
	else
	{
		for each (auto pCh in Channels)
		{
			NeedRegen |= pCh->IsProbePresent() || pCh->IsTouchPresent();
		}
		if(NeedRegen)
			RegeneratePrograms();
	}
	for each(auto pCh in Channels)
	{
		pCh->GetUnitCn()->JumpToProjStart();
		pCh->SetCurProgrTDTCn();
	}
	if (NCM_PROJECT.GetGlobalState().GetModelMode() != NGlobalState::BSP)
		MFOREST.Compress(true);
	ResetStocks();
	ResetCutting();
	if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		MFOREST.Compress(true);

	if (CalcVolume)
	{
		double volume = 0.0;
		if (IsThereAnyStock())
		{
			CMapStringToNStock* list = NGetStockList();
			for each (const auto & el in *list)
			{
				NStock* pS = static_cast<NStock*>(el.second);
				volume += pS->GetVolume();
			}
		}
		GetAnalyseState().CurVolume = volume;
	}

	MachToolEditor.ResetMach();
	GetCollManager().ClearCollided();
	MileStones.Activate();
	if (GetAutoUpdateProg())
		UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED | N_ANALYSE | N_LEFT | N_NEW_TEXT | N_PROGRAMCHANGED);
	else
		UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED | N_ANALYSE | (NeedRegen ? N_PROGRAMCHANGED : 0));
}

void CNCMDoc::DrawAllStocks(RenderMode GlobRMode, int Type, bool DrawSurf)
{
	CStocksCollection& Stocks = *NGetStockList();
	bool NeedDrawOrStocks = true;
	if (theApp.IsInCuttingMode() && NCM_PROJECT.GetGlobalState().GetAnimMode() != NGlobalState::Result)
	{
		auto pDynC = Channels[0]->GetDynamicCadr();
		GAnimateCadr* pAnimateCadr = dynamic_cast<GAnimateCadr*>(pDynC);
		
		if (pAnimateCadr)
		{
			if (Stocks.NeedDraw(GlobRMode, Stocks.GetRMode()))
			{
				glColor4fv(Stocks.GetColor().array);
				bool ChangeAttrib = Stocks.GetDrawSharpEdges() && Stocks.GetStocksState() == AT_TURN;
				if (ChangeAttrib)
				{
					glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(1., 1.);
				}
				std::vector<GAnimateCadrMill*> DynBSPCadrs;
				if (theApp.GetGlobalState().GetModelMode() == NGlobalState::BSP)
				{
					DynBSPCadrs.reserve(Channels.size());
					for (int ChInd = 0; ChInd < Channels.size(); ++ChInd)
					{
						const auto pDCadr = Channels[ChInd]->GetDynamicCadr();
						const auto pBSPCadr = dynamic_cast<GAnimateCadrMill*>(pDCadr);
						if(pBSPCadr != nullptr)
							DynBSPCadrs.push_back(pBSPCadr);
					}
				}
				;
				pAnimateCadr->RenderTop(GlobRMode, Type, DynBSPCadrs, GetStocksList().IsCollided());
//				pAnimateCadr->Render(GlobRMode, Type, DynBSPCadrs);

				if (ChangeAttrib)
				{
					glLineWidth(GLfloat(GQuadRender::GetEdgesWidth()));
					glColor3fv(GQuadRender::GetEdgesColor().array);
					Stocks.RenderSect(GetMachTool());
					glPopAttrib();
				}
			}
			NeedDrawOrStocks = false;
		}
	}
	if(NeedDrawOrStocks)
		Stocks.DrawStocks(GlobRMode, Type, theApp.IsInCuttingMode(), DrawSurf, GetMachTool());
}

bool CNCMDoc::InterruptAnimation(void)
{
	Interrupt = true;
	return true;
}


bool CNCMDoc::IsInterrupted(void) const
{
	return Interrupt;
}

void CNCMDoc::FinishAnimation(void)// MCH
{
	for each (auto pChannel in Channels)
	{
		pChannel->GetCutting().Started = false;
		SetCuttingEnabled(*pChannel, false);
		pChannel->StopDynamicCadr();
		if (pChannel->GetCutting().Started)
			SetCuttingEnabled(*GetChannel(0), true);
	}
}

void CNCMDoc::ProcCuttingStopped(int ChanInd)
{
	bool AllChannelsStopped = true;
	for each (auto pCh in Channels)
		AllChannelsStopped &= pCh->GetCutting().IsStopped();

	NChannel* pChannel = GetChannel(ChanInd);
	if ((pChannel->GetCuttingResult() & CUT_INTERRUPTED) != 0)
	{
//		AfxMessageBox(IDS_MES_ERR_TOOL);
		switch (NCM_PROJECT.GetGlobalState().GetAnimMode())
		{
		case NGlobalState::Result:
			pChannel->GetCutting().Started = false;
			EnableNextChannel(*pChannel);
			// Set machine tool to its right position
			GetMachTool().ProcCadrStep(*pChannel->GetUnitCn(), *pChannel->GetUnitCn()->GetState());
			break;
		case NGlobalState::Animation:
			break;
		}
	}
	if ((pChannel->GetCuttingResult() & CUT_OK) != 0)
	{
		EnableNextChannel(*pChannel);
		pChannel->GetUnitCn()->JumpToHCadrRel(pChannel->GetCutting().GetLastProg(), pChannel->GetCutting().GetLastHCadr());
	}
	if (AllChannelsStopped)
	{
		bool BadToolFound = false;
		bool MS = MileStones.IsActive();
		MileStones.DeActivate();
		for each (auto pCh in Channels)
		{
			BadToolFound |= pCh->GetCuttingResult() & CUT_INTERRUPTED;
		}
		if (MS)
			MileStones.Activate();

		if (BadToolFound)
		{
			NGetStockList()->ClearStocksRender();
			UpdateAllViews(nullptr, N_OGL | N_STATE | N_CADRCHANGED | N_SELECTTEXT);
		}

		switch (NCM_PROJECT.GetGlobalState().GetAnimMode())
		{
		case NGlobalState::Result:
		{
			bool NeedProcCadrStep = true;
			if (NCMComData::GetType() == AT_MILL_TURN)
			{
				bool ChannelsHaveCutting = false;
				for each (auto pCh in Channels)
				{
					pCh->WaitCuttingThreadCn();
					ChannelsHaveCutting |= pCh->GetCutting().HasCutting();
				}

				if (ChannelsHaveCutting && !IsInterrupted())
					NeedProcCadrStep = false;
			}
			if(NeedProcCadrStep)
			{
				CreateModifStocksRender();
				// Set machine tool to its right position
				for each (auto pCh in Channels)
					GetMachTool().ProcCadrStep(*pCh->GetUnitCn(), *pCh->GetUnitCn()->GetState());
			}
			break;
		}
		}

		bool AllChanProjEnd = true;
		for each (auto pCh in Channels)
			AllChanProjEnd &= ((pCh->GetCuttingResult() & CUT_PROJ_END) != 0);
//		static_cast<CMainFrame*>(AfxGetMainWnd())->AnimateUpdate();
		if (AllChanProjEnd && NCM_PROJECT.Defaults.GetBool(_T("Defaults/Stock/AutoSaveStockFile@ENABLE"), false))
		{
			if(StocksSaveThread.joinable())
				StocksSaveThread.join();
			StocksSaveThread = std::thread([this] {SaveAllStocks(); });
			SetThreadPriority(StocksSaveThread.native_handle(), THREAD_PRIORITY_IDLE);
		}
	}
	//if (pDoc->CalcVolume)
	//	UpdateVolumeParams();
}

bool CNCMDoc::EnableNextChannel(NChannel& CurChan)
{
	// returns false if all channels are disabled now and true if one channel is enabled
	if (!IsMultiChannel())
		return true;
	auto pNextChannel = GetNextCuttingChannel(CurChan);
	if (pNextChannel != &CurChan)
	{
		CurChan.GetCuttingEnabled().reset();
		if (pNextChannel != nullptr)
		{
			pNextChannel->GetCuttingEnabled().set();
			return true;
		}
	}
	return false;
}

bool CNCMDoc::GetCutPermission(int ChanInd)
{
	if (IsInterrupted())
		return true;
	MainTime MyTime = Channels[ChanInd]->GetCurCutTime();
	for each (auto pCh in Channels)
	{
		if (pCh->GetCutting().IsStoppedFull())
			continue;
		if (pCh->GetCurCutTime() < MyTime)
			return false;
	}
	return true;
}

void CNCMDoc::WaitAllChannels()
{
	for each (auto pChan in Channels)
		pChan->WaitCuttingThreadCn();
}

void CNCMDoc::Wait4Permission(NChannel& ActChannel, MainTime CurTime)
{
	if (!IsMultiChannel())
		return;
	ActChannel.GetCuttingEnabled().wait();
	ActChannel.SetCurCutTime(CurTime);
	while (!GetCutPermission(ActChannel.GetChannelInd()))
	{
		EnableNextChannel(ActChannel);
		ActChannel.GetCuttingEnabled().wait();
	}
}

void CNCMDoc::WaitCuttingEnabled(NChannel& ActChannel)
{
	if (!IsMultiChannel())
		return;
	ActChannel.GetCuttingEnabled().wait();
}

void CNCMDoc::SetCuttingEnabled(NChannel& ActChannel, bool Set)
{
	if (!IsMultiChannel())
		return;
	Set ? ActChannel.GetCuttingEnabled().set() : ActChannel.GetCuttingEnabled().reset();
}

void CNCMDoc::GetErrorsVector(NChannel::ErrVector& ErrorsVector)
{
	GetCurChannel()->PrepErrorsVector(ErrorsVector);
}

void CNCMDoc::StopAnimation(void)
{
	if (IsInterrupted())
		return;
	if (theApp.IsInCuttingMode())
	{
		for (int ChInd = 0; ChInd < GetChannelsCount(); ++ChInd)
			GetChannel(ChInd)->ResetNeedProbe();
		InterruptAnimation();
		WaitAllChannels();
	}
}

void CNCMDoc::JumpAfterStop(void)
{
	for (int ChInd = 0; ChInd < GetChannelsCount(); ++ChInd)
	{
		const auto pChannel = GetChannel(ChInd);
		pChannel->GetUnitCn()->JumpToHCadrRel(pChannel->GetCutting().GetLastProg(), pChannel->GetCutting().GetLastHCadr());
	}
}
void CNCMDoc::LoadSettings()
{
	CString OpenError;
	OpenError.LoadString(IDS_FN_ERR);
	CString FormatError;
	FormatError.LoadString(IDS_FORM_ERR);
	CString Success = _T("");
	CStringArray StateViewElements;
	CStringArray AnalyseViewElements;
	try
	{
		CString path = NCM_PROJECT.GetSettingsPath();
		CString ext;
		ext.LoadString(IDS_SETTINGS_EXT);
		if (DFNFileName.IsEmpty())
		{
			DFNFileName.LoadString(IDS_SETTINGS_FILENAME);
			DFNFileName += ext;
		}
		CStdioFile f;
		if (!f.Open(path + "\\" + DFNFileName, CFile::modeRead))
		{
			AfxMessageBox(IDS_MES_NOTOPENDFN);
			DFNFileName.LoadString(IDS_SETTINGS_FILENAME);
			DFNFileName += ext;
			if (!f.Open(path + "\\" + DFNFileName, CFile::modeRead))
				throw OpenError;
		}
		CString str;
		BOOL FEnd;
		// StateView elements
		for (FEnd = f.ReadString(str);
			str != "NCM Part end" && FEnd;
			FEnd = f.ReadString(str))
			StateViewElements.Add(str);
		if (str != "NCM Part end")
			throw FormatError;
		// AnalyseView elements
		for (FEnd = f.ReadString(str);
			str != "NCM Part end" && FEnd;
			FEnd = f.ReadString(str))
			AnalyseViewElements.Add(str);
		if (str != "NCM Part end")
			throw FormatError;
		CString key;
		// reading complete successfully
		throw Success;
	}

	catch (CString Message)
	{
		CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		if (pMainFrame == nullptr)
			return;
		NStateUnitPane* pUnitPane = (NStateUnitPane*)pMainFrame->GetUnitPane();

		CStringArray& VisElements = pUnitPane->GetVisElemList();
		if (VisElements.GetSize() != 0)
			VisElements.RemoveAll();
		for (int i = 0; i < StateViewElements.GetSize(); i++)
			VisElements.Add(StateViewElements[i]);

		NStateAnalysePane* pAnalysePane = (NStateAnalysePane*)pMainFrame->GetAnalysePane();

		CStringArray& VisElementsAnalyse = pAnalysePane->GetVisElemList();
		if (VisElementsAnalyse.GetSize() != 0)
			VisElementsAnalyse.RemoveAll();
		for (int i = 0; i < AnalyseViewElements.GetSize(); i++)
			VisElementsAnalyse.Add(AnalyseViewElements[i]);
	}
}

bool CNCMDoc::CutFirstWord(CString& src, CString& dst)
{
	// Needed because sscanf(%s) stops on the "€" symbol (VS2015)
	dst = src.SpanExcluding(_T(" \n\t"));
	if (dst.GetLength() == 0)
		return false;
	src.Delete(0, dst.GetLength() + 1);
	return true;
}

void CNCMDoc::ApplyAppearSettings(void)
{
	CMapStringToNProgram* pPList = NGetProgramList();
	for (int i = 0; i < pPList->GetMainProgCount(); ++i)
	{
		CString Key;
		NProgram* pProg = NULL;
		pPList->GetAt(i, Key, (CObject*&)pProg);
		pProg->SetAuxColors();
	}
	bool NeedRegen = false;
	NColor* c;
	if (NCM_PROJECT.Colors.Lookup("ќсевое—ечение«аготовок", c))
	{
		if (*c != GQuadRenderRot::AxialSectionColorStock)
		{
			GetStocksList().SetAxialSectionColor(*c);// For a future use
			GQuadRenderRot::AxialSectionColorStock = *c;
			NeedRegen = true;
		}
	}
	if (NCM_PROJECT.Colors.Lookup("ќсевое—ечениеƒеталей", c))
	{
		if (*c != GQuadRenderRot::AxialSectionColorPart)
		{
			NGetPartList()->SetAxialSectionColor(*c);// For a future use
			GQuadRenderRot::AxialSectionColorPart = *c;
			NeedRegen = true;
		}
	}
	if (NeedRegen)
		Regenerate();
}

bool CNCMDoc::RemoveTempStock(void)
{
	CString key;
	NStock* pS = nullptr;
	if (GetStocksList().FindTempStock(key, pS))
	{
		DeleteStockWU(pS);
		return true;
	}
	return false;
}

void CNCMDoc::AddPart(const CString& name, int& ind, NStock* pStock)
{
	if (pStock == nullptr)
		return;
	CString TrunName = name.Right(name.GetLength() - CMapStringToNStock::PrefixStock.GetLength());
	NGetPartList()->AddObj(TrunName, ind, pStock);
	if (!TrunName.IsEmpty())
	{
		pStock->SetEdgesAttrs(NGetPartList()->GetEdgesAttrs());
		pStock->SetGroup(OG_PARTS);
	}
}

void CNCMDoc::AddFixture(const CString& name, int& ind, NStock* pStock)
{
	if (pStock == nullptr)
		return;
	CString TrunName = name.Right(name.GetLength() - CMapStringToNStock::PrefixStock.GetLength());
	NGetExObjList()->AddObj(TrunName, ind, pStock);
	if (!TrunName.IsEmpty())
	{
		pStock->SetEdgesAttrs(NGetExObjList()->GetEdgesAttrs());
		pStock->SetGroup(OG_FIXTURES);
	}
}

void CNCMDoc::DeletePartWU(NStock* pstock)
{
	GetUndoHistory().AddAndDo(new UCAddDelPart(this, pstock));
}

void CNCMDoc::DeleteFixtWU(NStock* pstock)
{
	GetUndoHistory().AddAndDo(new UCAddDelFixture(this, pstock));
}

void CNCMDoc::TransMovObjWU(NMovableObj* pS, const BMatr& M)
{
	GetUndoHistory().AddAndDo(new UCTransMovObj(this, pS, M));
}

bool CNCMDoc::UHUndoGroup()
{
	UHPrepUndoRedo();
	return GetUndoHistory().UndoGroup();
}

bool CNCMDoc::UHDoGroup()
{
	UHPrepUndoRedo();
	return GetUndoHistory().DoGroup();
}

bool CNCMDoc::UHCanRedo() const
{
	return GetUndoHistory().CanRedo();
}

bool CNCMDoc::UHCanUndo() const
{
	return GetUndoHistory().CanUndo();
}

bool CNCMDoc::UHPrepUndoRedo()
{
	Comparison.DeActivate();
	return false;
}

void CNCMDoc::SetMesureDistanceActive(bool Set)
{
	Distance.SetActive(Set);
}

bool CNCMDoc::IsMesureDistanceActive() const
{
	return Distance.IsActive();
}

bool CNCMDoc::IsNCMAll() const
{
	const CString& Path = GetPathName();
	if (Path.GetLength() > 6)
		return Path.Right(6).MakeUpper() == _T("NCMALL");
	return false;
}

void CNCMDoc::SetModifiedFlag(BOOL bModified)
{
	if (SnapsGlob.IsActivated())
		SnapsGlob.SetRestartFlag();
	CDocument::SetModifiedFlag(bModified);

}

const NCadrGeom* CNCMDoc::GetCadrGeomIDost(IDost ID)
{
	static NCadrGeom BufGeom;
	const auto& ToolList = *NGetToolList();
	if (ToolList.empty())
		return nullptr;
	MTMachineTool& Mach = GetMachTool();
	NToolCombined* pComTool = nullptr;
	CString key;
	ToolList.GetAt(ID.GetToolInd(), key, (CObject*&)pComTool);
	if (pComTool == nullptr)
		return nullptr;
	const BGeomArray& GeomArray = pComTool->GetSkeleton();
	if (ID.GetCadrInd() < 0 || ID.GetCadrInd() >= GeomArray.GetSize())
		return nullptr;
	BMatr Matr;
	bool ToolFound = GetFullToolMatr(pComTool, false, Matr);// false means that all tools should be valid here 
	if (!ToolFound)
		return nullptr;
	BufGeom = *GeomArray[ID.GetCadrInd()];
	BufGeom.Tr(Matr);
	return &BufGeom;
}

void CNCMDoc::ShowCollidedAnim(const NCollisionID* pCollID, bool Val)
{
	if (pCollID == nullptr)
		return;
	if (pCollID->CollType != LE_COLLISION)
		return;
	auto* pObjID = &pCollID->IDObj1;
	if (pObjID->GetGroup() == OG_STOCKS)
		GetStocksList().SetCollided(Val);
	else
	{
		auto pCO = pObjID->GetCollidableObj();
		if(pCO != nullptr)
			pCO->SetCollided(Val);
	}
	pObjID = &pCollID->IDObj2;
	if (pObjID->GetGroup() == OG_STOCKS)
		GetStocksList().SetCollided(Val);
	else
	{
		auto pCO = pObjID->GetCollidableObj();
		if (pCO != nullptr)
			pCO->SetCollided(Val);
	}
}

void CNCMDoc::ShowCollided(bool RunAnimateActive)
{
	// highlight collided elements on static or run animate state
	if (pCurChannel == nullptr)
		return;

	ClearAllCollisions();

	double t = -1;
	const NCadrGeom* pGeom = nullptr; ;// NGetProgramList()->GetCadrCompGeom(ID, 0);
	if (RunAnimateActive)
	{
		t = pCurChannel->GetRunAnimateT();
		pGeom = pCurChannel->GetRunAnimateCadrGeom();
	}
	else
	{
		t = 1.;
		cadrID ID = GetCurCadrAbs();
		pGeom = GetAbsCadrCompGeom(ID, 0);
	}
	if (pGeom == nullptr)
		return;
	MainTime CurTime = (t == 1.) ? pGeom->GetEndTime() : pGeom->GetMidTime(t);

	ShowCollidedAnim(&GetCollManager().GetCollTimeLine().GetCollIDByTime(CurTime), true);
}

void CNCMDoc::ClearAllCollisions()
{
	GetCollManager().ClearCollided();
	GetStocksList().SetCollided(false);
}

bool CNCMDoc::IsCollWOStock() const
{
	if(!GetCollManager().IsForeType())
		return false;
	if (!IsThereAnyVisStock())
		return true;
	if (!GetCollManager().MaskHasStocks())
		return true;
	return false;
}

void CNCMDoc::GetTimeSlots(std::vector<TimeSlot>& TimeSlots) const
{
	TimeSlots.resize(0);
	for each (auto pCh in Channels)
		TimeSlots.push_back(pCh->GetWireAnimWalker().GetCurTimeSlot());
}

bool CNCMDoc::NeedRunAnimStep(const std::vector<TimeSlot>& TimeSlots, const NChannel* pCh)
{
	auto Ind = pCh->GetChannelInd();
	if(Ind < 0 || Ind >= TimeSlots.size())
		return false;
	// return true if Ind slot starts before all other slots ends
	bool Ret = true;
	for (int i = 0; i < TimeSlots.size(); ++i)
	{
		if (i == Ind)
			continue;
		if (TimeSlots.at(Ind).first > TimeSlots.at(i).second)
			Ret = false;
	}
	return Ret;
}

