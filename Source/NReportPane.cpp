// NReportPane.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NCMDoc.h"
#include "cadrID.h"
#include "MainFrm.h"
#include "NCUnitState.h"
#include "WM_APP_messages.h"
#include "BProgram.h"
#include "NControlPanel.h"
#include "NReportPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NReportPane

#define AFX_REG_SECTION_FMT    _T("%sReportPane-%d")
#define AFX_REG_SECTION_FMT_EX _T("%sReportPane-%d%x")

IMPLEMENT_DYNCREATE(NReportPane, CDockablePane)

NReportPane::NReportPane()
{
	Blocked = false;
}

NReportPane::~NReportPane()
{
}


BEGIN_MESSAGE_MAP(NReportPane, CDockablePane)
	//{{AFX_MSG_MAP(NReportPane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_APP_REPORT_CLEAR, &NReportPane::OnReportClear)
	ON_MESSAGE(WM_APP_REPORT_RUNTO, &NReportPane::OnReportRunto)
	ON_MESSAGE(WM_APP_REP_ITEM_CHANGE, &NReportPane::OnItemChanged)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// NReportPane diagnostics

#ifdef _DEBUG
void NReportPane::AssertValid() const
{
	CDockablePane::AssertValid();
}

void NReportPane::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NReportPane message handlers

void NReportPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint & N_PROGRAMCHANGED)
	{
		CNCMDoc* pDoc = GetDocument();
		if (!pDoc)
			return;
		Clear(false, true);
		NChannel::ErrVector ErrorsVector;
		pDoc->GetErrorsVector(ErrorsVector);
		for (int i = 0; i < ErrorsVector.size(); ++i)
			AddErrorItem(ErrorsVector.at(i).first, &ErrorsVector.at(i).second);
		ReportCtrl.SortBySettings();
	}
	// Update colors
	if (lHint & N_SETTINGS)
	{
		ReportCtrl.SetSettings();
		return;
	}
	if (lHint & N_COLLLOG)
	{
		Invalidate();
		return;
	}
}

void NReportPane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect Rect;
	GetClientRect(&Rect);
	ReportCtrl.MoveWindow(Rect, TRUE);
}

int NReportPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect Rect;
	GetClientRect(&Rect);
	ReportCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | /*LVS_SINGLESEL |*/ LVS_SHOWSELALWAYS,
				Rect, this, ID_REPORTCTRL);
	RedrawCaption();
	return 0;
}


CNCMDoc* NReportPane::GetDocument() 
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}



void NReportPane::OnSetFocus(CWnd* pOldWnd) 
{
	CDockablePane::OnSetFocus(pOldWnd);

	ReportCtrl.SetFocus();
}

void NReportPane::AddCollItem(const NCollLogElem *pCollLogElem)
{
	if(!pCollLogElem || Blocked)
		return;
	CNCMDoc *pDoc = GetDocument();
	CString ProgName;
	BProgram *pProg;
	if (!pDoc->NGetProgramList()->GetAt(pCollLogElem->PTPoint.Prog, ProgName, (CObject *&)pProg))
		return;
	cadrID NewID = pCollLogElem->PTPoint;
	cadrID LocID = pProg->GetCadrByHist(NewID.Cadr);
	ProgName = pDoc->NGetProgramList()->GetProgName(LocID);
	int pos = ProgName.ReverseFind('\\');
	if (pos > 0)
		ProgName.Delete(0, pos + 1);
	int StepNum = NewID.Cadr;
	if(pCollLogElem->StartPTPoint.Prog == -1)
	{
		ReportCtrl.AddCollItem(pCollLogElem->Type, NewID.ID, ProgName, LocID
			, StepNum, CString(pCollLogElem->GetCollNote().data()));
	}
	else
	{
		if (pCollLogElem->PTPoint == pCollLogElem->StartPTPoint)
		{
			ReportCtrl.AddCollItem(pCollLogElem->Type, NewID.ID, ProgName, LocID
				, StepNum, CString(pCollLogElem->GetCollNote().data()));
		}
		else
		{
			cadrID StartNewID = pCollLogElem->StartPTPoint;
			cadrID StartLocID = pProg->GetCadrByHist(StartNewID.Cadr);
			int StartStepNum = StartNewID.Cadr;
			ReportCtrl.AddCollItem(pCollLogElem->Type, NewID.ID, ProgName, LocID
				, StepNum, StartLocID.Cadr, StartStepNum, CString(pCollLogElem->GetCollNote().data()));
		}
	}
	ReportCtrl.SortBySettings();
	RedrawCaption();
	// Subscription
	if (NCM_PROJECT.GetGlobalState().GetSubStColl().Wnd != NULL)
	{
		CollLogElemTypeE CollisionType = LEE_NOTDEFINED;
		switch (pCollLogElem->Type)
		{
		case LE_NOTDEFINED:
			CollisionType = LEE_NOTDEFINED;
			break;
		case LE_COLLISION:
			CollisionType = LEE_COLLISION;
			break;
		case LE_OUTOFRANGE:
			CollisionType = LEE_OUTOFRANGE;
			break;
		case LE_COLLTOOL:
			CollisionType = LEE_COLLTOOL;
			break;
		case LE_COLLHOLD:
			CollisionType = LEE_COLLHOLD;
			break;
		case LE_COLLSHANK:
			CollisionType = LEE_COLLSHANK;
			break;
		case LE_COLLVERT:
			CollisionType = LEE_COLLVERT;
			break;
		case LE_EMPTYTOOL:
			CollisionType = LEE_EMPTYTOOL;
			break;
		}
		CollisionData CollD;
		CollD.CollisionType = CollisionType;
		CollD.ProgramNum = pCollLogElem->PTPoint.Prog;
		CollD.LineNum = NewID.Cadr;
		CollD.StepNum = StepNum;
		NCM_PROJECT.GetGlobalState().ProcCollSubscr(CollD);
	}
}

void NReportPane::AddErrorItem(int HistPos, const NProgError* pErrorLogElem)
{
	if (pErrorLogElem == nullptr || Blocked)
		return;
	const CNCMDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;
	CString ProgName;
	BProgram* pProg = nullptr;
	if (!pDoc->NGetProgramList()->GetAt(pErrorLogElem->GetCadr().Prog, ProgName, (CObject*&)pProg))
		return;
	int ProgInd = pDoc->NGetProgramList()->GetIndex(pErrorLogElem->GetProg());
	if (ProgInd < 0)
		return;
	const int pos = ProgName.ReverseFind('\\');
	if (pos > 0)
		ProgName.Delete(0, pos + 1);
	cadrID AbsID(ProgInd, HistPos);// working program and it's history position
	if (HistPos < 0)
		AbsID = cadrID(-1, 0);
	const cadrID LocID(pErrorLogElem->GetCadr());// program or subprogram and it's line number
	CString Message = pDoc->ProgErrHandler.MakeErrorMessage(*pErrorLogElem);
	ReportCtrl.AddErrorItem(Message, AbsID.ID, LocID, ProgName, pProg->GetCadrText(LocID.Cadr));
	ReportCtrl.SortBySettings();
	RedrawCaption();
}

void NReportPane::AddCollLog(const NCollisionLog& Log, int StartInd)
{
	ReportCtrl.SetRedraw(FALSE);
	for (int i = StartInd; i < Log.GetSize() && i <= MAX_LINES; ++i)
	{
		AddCollItem(&Log.GetElem(i));
	}
	ReportCtrl.SetRedraw(TRUE);
	RedrawCaption();
}

void NReportPane::Clear(bool Collisions, bool Errors)
{
	std::set<int> Inds;
	if (Errors)
	{
		Inds.insert(0);
		Inds.insert(1);
		Inds.insert(2);
		Inds.insert(3);
	}
	if (Collisions)
	{
		Inds.insert(4);
		Inds.insert(5);
		Inds.insert(6);
		Inds.insert(7);
		Inds.insert(8);
		Inds.insert(9);
		Inds.insert(10);
		Inds.insert(11);
	}
	ReportCtrl.DeleteMessages(Inds);
	RedrawCaption();
}

int NReportPane::GetItemCount(void) const
{
	return ReportCtrl.GetItemCount();
}

void NReportPane::RedrawCaption()
{
	CString Caption;
	Caption.LoadString(ID_REPORTPANE);
	const auto Count = GetItemCount();
	bool NewBlocked = (Count >= MAX_LINES);
	if (NewBlocked != Blocked)
	{
		Blocked = NewBlocked;
		COLORREF Col = Blocked ? RGB(192, 0, 0) : RGB(0, 0, 0);
		ReportCtrl.GetHeaderCtrl().SetTextColor(Col);
	}
	CString Add;
	if (Blocked)
		Add.Format(_T(" (%d+!)"), Count);
	else
		Add.Format(_T(" (%d)"), Count);
	SetWindowText(Caption + Add);
}

void NReportPane::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ReportRunto();
}

LRESULT NReportPane::OnReportClear(WPARAM wParam, LPARAM lParam)
{
	Clear(true, false);
	CNCMDoc *pDoc = GetDocument();
	if (!pDoc)
		return 0;
	pDoc->GetCollManager().ClearLogs();
	RedrawCaption();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	((NTextView*)pMainFrame->GetTextView())->ClearCollisionMarkers();
	return 0;
}

LRESULT NReportPane::OnReportRunto(WPARAM wParam, LPARAM lParam)
{
	ReportRunto();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pMainFrame)
		return 0;
//	pMainFrame->OnRunHereResult();
	pMainFrame->OnRunHere();
	return 0;
}

LRESULT NReportPane::OnItemChanged(WPARAM wParam, LPARAM lParam)
{
	cadrID CadrIDabs;
	CadrIDabs.ID = unsigned int(ReportCtrl.GetDataFromSelectedItem());
	if (CadrIDabs.ID == 0)
		return 0;
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc)
		return 0;
	if (CadrIDabs.Prog == -1)
	{// group error
		CString text = ReportCtrl.GetTextFromSelectedItem(2);
		CString pr = text.SpanExcluding(".");
		text.Delete(0, pr.GetLength() + 1);
		cadrID LocID(atoi(pr), atoi(text) - 1);
		pDoc->GotoCadrFast(pDoc->GetCurChannel(), LocID);
	}
	else
		pDoc->GotoAbsCadr(pDoc->GetCurChannel(), CadrIDabs);
	return 0;
}

bool NReportPane::HasFocus(void)
{
	return (GetFocus() == &ReportCtrl);
}

BOOL NReportPane::LoadState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	//CString strProfileName = ::AFXGetRegPath(strControlBarProfile, lpszProfileName);

	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID();
	}

	CString strSection;
	if (uiID == (UINT)-1)
	{
		strSection.Format(AFX_REG_SECTION_FMT, lpszProfileName, nIndex);
	}
	else
	{
		strSection.Format(AFX_REG_SECTION_FMT_EX, lpszProfileName, nIndex, uiID);
	}

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, TRUE);

	if (!reg.Open(strSection))
	{
		return FALSE;
	}

	int Width;
	reg.Read(_T("Width0"), Width);
	GetCtrlW().SetColumnWidth(0, Width);
	reg.Read(_T("Width1"), Width);
	GetCtrlW().SetColumnWidth(1, Width);
	reg.Read(_T("Width2"), Width);
	GetCtrlW().SetColumnWidth(2, Width);
	reg.Read(_T("Width3"), Width);
	GetCtrlW().SetColumnWidth(3, Width);
	reg.Read(_T("Width4"), Width);
	GetCtrlW().SetColumnWidth(4, Width);

	return CDockablePane::LoadState(lpszProfileName, nIndex, uiID);
}
BOOL NReportPane::SaveState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{

	//CString strProfileName = ::AFXGetRegPath(strBaseControlBarProfile, lpszProfileName);

	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID();
	}

	CString strSection;
	if (uiID == (UINT)-1)
	{
		strSection.Format(AFX_REG_SECTION_FMT, lpszProfileName, nIndex);
	}
	else
	{
		strSection.Format(AFX_REG_SECTION_FMT_EX, lpszProfileName, nIndex, uiID);
	}

	CSettingsStoreSP regSP;
	CSettingsStore& reg = regSP.Create(FALSE, FALSE);

	if (reg.CreateKey(strSection))
	{
		reg.Write(_T("Width0"), GetCtrl().GetColumnWidth(0));
		reg.Write(_T("Width1"), GetCtrl().GetColumnWidth(1));
		reg.Write(_T("Width2"), GetCtrl().GetColumnWidth(2));
		reg.Write(_T("Width3"), GetCtrl().GetColumnWidth(3));
		reg.Write(_T("Width4"), GetCtrl().GetColumnWidth(4));
	}

	return CDockablePane::SaveState(lpszProfileName, nIndex, uiID);
}

void NReportPane::ReportRunto()
{
	cadrID CadrIDabs;
	CadrIDabs.ID = unsigned int(ReportCtrl.GetDataFromSelectedItem());
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	pDoc->Jump2AbsCadr(CadrIDabs, true);
	return;
}
