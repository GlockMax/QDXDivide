// TEXTView.cpp : implementation of the NTextView class
//

#include "stdafx.h"
#include <math.h>
#include "BSpacePos.h"

#include "NCMDoc.h"
#include "NCUnit.h"
#include "LeftView.h"
#include "MainFrm.h"
#include "NCadr.h"
#include "NProgram.h"
#include "NColor.h"
#include "NFont.h"
#include "curve.h"
#include "NToolCombined.h"
#include "NTextPropertySheet.h"
#include "NModePainterDlg.h"
#include "NTextRenumDlg.h"
#include "cadrID.h"
#include "NCadrEditDlg.h"
#include "NTextsComp.h"

#include "NTextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//TEXTSTATUS const int STATUSBAR_HEIGHT = 20;

bool EDITOR_COLORS_USE = true;
/////////////////////////////////////////////////////////////////////////////
// NTextView
IMPLEMENT_DYNCREATE(NTextView, EditorView)

BEGIN_MESSAGE_MAP(NTextView, EditorView)
	//{{AFX_MSG_MAP(NTextView)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_UPDATE, OnEditUpdate)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_RENUM, OnEditRenum)
	ON_WM_CONTEXTMENU( )
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_USER_CHANGE_OBJECT_PROPERTIES, OnApplyChanges)
	ON_COMMAND(ID_EDIT_AUTO_UPDATE, OnAutoUpdate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AUTO_UPDATE, OnUpdateAutoUpdate)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_CALL_MACRO, OnMacro)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_ADD_BOOKMARK, OnAddBookmark)
	ON_COMMAND(ID_CLEARALL_BOOKMARK, OnClearAllBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UPDATE, &NTextView::OnUpdateEditUpdate)
	ON_COMMAND(ID_TOOLS_LIST, OnInsertToolsStr)
	ON_COMMAND(ID_SHOW_REPLACED, OnShowReplaced)
	ON_COMMAND(ID_COMPARE_WITH_CHANGE, OnCompP)
	ON_COMMAND(ID_COMPARE_WITH_ORIGINAL, OnCompF)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NTextView construction/destruction

NTextView::NTextView()
{
	CurrFeedID = editor_cfg::ID_STYLE_FEED_SECOND;
	ChosenMode = 0;
	m_DispExecute = false;

	m_pTextPropertySheet = NULL;

	ShowReplaced = false;
	LoadedProgInd = -1;
}

NTextView::~NTextView()
{
}


int NTextView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	LoadParams();
	if (EditorView::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef NC_DEMO
	::RevokeDragDrop(m_wndEditor.GetSafeHwnd());
#endif
//	if (GetInterfaceStyle() != _T("Ribbon"))
	{
		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, ID_VIEW_TOOLBAR_NTEXT);
		LoadTB();

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

		m_wndToolBar.SetOwner(this);

		// All commands will be routed via this control , not via the parent frame:
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

		m_wndToolBar.EnableCustomizeButton(TRUE, -1, "");
		AdjustLayout();
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// NTextView diagnostics

#ifdef _DEBUG
void NTextView::AssertValid() const
{
  	EditorView::AssertValid();
}

void NTextView::Dump(CDumpContext& dc) const
{
	EditorView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NTextView message handlers

void NTextView::OnUpdate(CPane* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint & N_DELETECONT)
	{
		ClearLoadedProg();
		return;
	}
	if (lHint & N_PROGRAMCHANGED)
	{
		NProgram* pProg;
		CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
		if (!LoadedProg.IsEmpty() && pDoc->NGetProgramList()->Lookup(LoadedProg, pProg))
			SetErrorArr(pProg);
	}
	if(lHint & N_NEW_TEXT)
	{
		CWaitCursor w; 
//		LoadedProg = "";// Removed for multiple undo buffer
		NUpdate();
	}
	if(lHint & N_SETTINGS)// Color Update
	{
		CWaitCursor w; 
		ReadColors();	
		ReColor();
		NUpdate();
	}
	if (lHint & N_UNIT_CHANGED)
	{
		CWaitCursor w; 
		ReadColors();
		ReadWordSettings();
		NUpdate();
	}
	if(lHint & N_TEXT)
	{
		NUpdate();
	}
	if (GetSafeHwnd() == NULL)
		return;
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	if(pDoc)
	{
		if (pDoc->GetProgrTDT() != LoadedProg)
		{
			NUpdate();
		}
	}
	if(lHint & N_CADRCHANGED || lHint & N_SELECTTEXT)
		SetCadrPos(false);
	if(!(lHint & N_NEW_TEXT)&&!(lHint & N_SELECTTEXT)&&!(lHint & N_CADRCHANGED))
	{
		RefreshMarkers();
	}
}



void NTextView::NUpdate()
{
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	const CString &buf = pDoc->GetProgrTDT();	
	//////////////////////////////////////////////////////////////////////////

	// сохранение закладок в текущей программе 
	SaveBookMarkInProgram();
	
	// загрузка новой программы
	NProgram *pProg = nullptr;
	if(buf.IsEmpty() || !pDoc->NGetProgramList()->Lookup(buf,pProg))
	{
		DeleteAllText();
		ClearLoadedProg();
		return;
	}

	EnsureProperProg();

	if (pProg)
		if (!pProg->HaveErr())
			ClearErrorMarkers();
	UpdateStat(pDoc->GetProgrTDT(), pDoc->GetCurChannel()->GetChannelInd(), pDoc->GetChannelsCount());
}


// отметка ис
void NTextView::SetCadrPos(bool fBind)
{
	if (!IsPaneVisible())
		return;
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	cadrID CurrentCadr = pDoc->GetUnit()->GetState()->GetCurCadrID();
	if (LoadedProg == "" || 
		LoadedProg != pDoc->NGetProgramList()->GetProgName(CurrentCadr))
	{
		return;
	}
	if(fBind)
		SetImageAndState(CurrentCadr.Cadr, GADRPOS_IMAGE);
	else
		SetImageAndState(CurrentCadr.Cadr, STATE_IMAGE);
		
	//auto ID = pDoc->GetUnit()->GetState()->GetCurHCadrAbsEx();
	//const auto pProg = pDoc->GetCurChannel()->NGetProgramListCn()->GetProg(ID);
	//if (pProg != nullptr)
	//{
	//	const auto& Hist = pProg->GetErrorHistory();
	//	NProgError ProgError;
	//	if (Hist.Lookup(ID.Cadr + 1, ProgError))
	//	{
	//		SetWindowText(ProgError.GetErrorString());
	//	}
	//	//else
	//	//	SetWindowText("");
	//}
}

// установка положения и иконки выделяющей строку
// type:
// 1 - отметка при управлении кнопками проекта
// 2 - ошибка
void NTextView::SetImageAndState(int CurentCadr, ImageType type)
{
	if (type < 1 || type > 3)
		return;
	
	if (CurentCadr == -1)
		return;
	
	//выбор положения

	// картинка для метки (установить)
	if (type == GADRPOS_IMAGE)
	{
		if(fBind)
		{
			SetBindMarker(CurentCadr);
			GoToLine(CurentCadr);
		}
	}
	if (type == ERROR_IMAGE)
	{
		ClearErrorMarkers();
		SetMarkerError(CurentCadr - 1);
	}
	
	if (type == STATE_IMAGE)
	{
		SetMarkerCurrLine(CurentCadr);
		GoToLine(CurentCadr);
	}
}

void NTextView::OnEditUpdate() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	((CLeftView *)((CMainFrame *)AfxGetMainWnd())->GetLeftPane())->CloseSectProgDlg();
// Save program text to document
	
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	NProgram *pProg;
	CString name;
	name.LoadString(IDS_CH_NAME);
	pDoc->DeleteProg(name);
	if(!pDoc->NGetProgramList()->Lookup(LoadedProg, pProg))
	{
		AfxMessageBox(IDS_MES_PR_DEL);
		return;//error
	}
	pProg->Clear();
	const int NStr = GetLineCount();
	for(int i = 0; i < NStr; ++i)
		pProg->AddCadr(new NCadr(GetLine(i)));
	pDoc->ImplRepl(pProg);
	pDoc->RegeneratePrograms();
	pDoc->GetUnit()->JumpHere();
	pDoc->SetModifiedFlag();
	pProg->SetManChanged();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_PROGRAMCHANGED | N_TEXT, NULL);
	if(ChosenMode != 0) 
		Colorise(0, GetDocLength());
	SetSavePoint();
	// Subscription
	NCM_PROJECT.GetGlobalState().ProcProgChangeSubscr(LoadedProg);
}

// выполнить программу до кадра на котором установлен курсор
void NTextView::CompleteCurCadr()
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	if(!pDoc->GetUnit())
		return;
	CMapStringToNProgram *pPList = pDoc->NGetProgramList();
	cadrID DestID;
	DestID.Cadr = GetCurLineWOAnchor();
	DestID.Prog = pPList->GetIndex(LoadedProg);
	double t;
	cadrID SourceHID = pDoc->GetCurCadr(t);
	cadrID DestHID = pPList->GetNearest(SourceHID, DestID);
	if (DestHID.Prog < 0)
		return;
	pDoc->ClearAllCollisions();
	pDoc->GetUnit()->JumpForToHCadr(DestHID);
	pDoc->UpdateAllViews(NULL, N_OGL | N_STATE | N_CADRCHANGED);
}

/////////////////////////////////////////////////
// операции по поиску текста и установка маркеров

void NTextView::SaveBookMarkInProgram()
{
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	NProgram *pProg;
	if (!LoadedProg.IsEmpty() && pDoc->NGetProgramList()->Lookup(LoadedProg, pProg))
	{
		NArray arr;
		GetBookmarkArr(arr);
		pProg->Copy2BookMarkArray(arr);
		pDoc->SetModifiedFlag();
	}
}

/////////////////////////////////////////////////
// раскраска текста программы
void NTextView::OnModePainter()
{
	if (!m_pTextPropertySheet)
	{
		CString tmp;
		tmp.LoadString(IDS_PAR_TITLE);
		m_pTextPropertySheet = new NTextPropertySheet (tmp, this);
	}
	FillSheet(m_pTextPropertySheet);

	if( m_pTextPropertySheet->DoModal() == IDOK )
		NUpdateProperty(m_pTextPropertySheet);	

	delete m_pTextPropertySheet;
	m_pTextPropertySheet = NULL;
}

void NTextView::Colorise(ptrdiff_t startPos, ptrdiff_t position)
{

	if(!fEnableColorise)
		return;
	if(ChosenMode == 0) 
	{
		EditorView::Colorise(startPos,position);
		return;
	}
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	class NCUnit NewUnit(pDoc, *pDoc->GetUnit());//NCUnit constructor
	NewUnit.SetCurProg(pDoc->GetProgrTDT());
	NProgram *pProg = NewUnit.GetCurRunProg();
	if(!pProg)
		return;
	if(pProg->HaveErr())
		return;

	NToolID CurTool(NewUnit.GetState()->CurToolN);
	CStringArray NTArray;
	if (ChosenMode == 2)
	{
		NMapStringToNTool *pToolList = pDoc->NGetToolList();
		for each(const auto& el in *pToolList)
		{
			NTArray.Add(el.first);
		}
	}
	NColor *pCol=NULL;
	NColor *pColStart=NULL, *pColEnd=NULL; // две точки задающие отрезок изменения цвета
//	CHARFORMAT cf, cf_previos;
	double Feed = 0.;
//	float u;
	int CurentCadr=0, LastColoredCadr=0, VisCadr=0,k=0,AttachedCadr=0;
	bool IsPaint = false, Attached = false;

	// определение MAX и MIN значений подачи
	if (ChosenMode == 5) 
	{
		FeedMax = 0;
		while (NewUnit.EvaluateCurrent(true) > -1)
		{
			Feed = NewUnit.GetState()->Feed;
			if (Feed > FeedMax)
				FeedMax = Feed;
		}
		if (FeedMax == 0)
		{
			CurTool.LoadString(IDS_MES_FEED);
			AfxMessageBox(CurTool);
			ChosenMode = 0;
			return;
		}
		NewUnit.Reset();
		pColStart = new NColor;
		pColEnd = new NColor;
	}
	// на время раскраски привязка отключается
	if (m_DispExecute)
	{
		m_DispExecute = false;
		Attached = true;
		AttachedCadr = NewUnit.GetState()->GetCurCadr();
	}
	BeginWaitCursor();

	UINT ID = 0;
	//int lineCounter = 0;
	//int slpos;
	int ssize;
	editor_cfg cfg = GetConfig();
	StartStyling(0);
	//Назначаем стиль по умолчанию
	ssize = GetDocLength();
	SetStyle(ssize,ID);
	if(NewUnit.EvaluateCurrent(true) < 0)
	{
		EndWaitCursor();
		return;
	}
	bool flag = false;
	int lineNum = 0;
	int pos = 0;
	while(1)
	{
		
		ID = STYLE_DEFAULT;
		switch (ChosenMode)
		{
		case 1:	// По типу движения инструмента
			// холостой ход
			if (NewUnit.GetState()->CurCurveType == fast)
				ID = cfg.StyleCurveType[0].ID;
			//отрезок
			if (NewUnit.GetState()->CurCurveType == line)
				ID = cfg.StyleCurveType[1].ID;
			// дуга ПЧС
			if (NewUnit.GetState()->CurCurveType == cwarc)
				ID = cfg.StyleCurveType[2].ID;
			// дуга ПрЧС
			if(NewUnit.GetState()->CurCurveType == ccwarc)
				ID = cfg.StyleCurveType[3].ID;
			break;
		case 2: // По периодам работы разных инструментов
//			double Zdisp_min;
			{
				if(CurTool != NewUnit.GetState()->CurToolN)
				{
					CurTool = NewUnit.GetState()->CurToolN;
					//pDoc->NGetToolList()->Lookup(CurTool, pTool);
				}
				int index = 0;
				for(int i = 0; i < NTArray.GetSize(); i++)
				{
					if(NTArray[i] == CurTool)
					{
						index = i;
						break;
					}
				}
				
				ID = editor_cfg::ID_STYLE_TOOL + index;
			}
			break;
		case 4:	// По типу охлаждения инструмента
			if (NewUnit.GetState()->Coolant == COOLOFF)
				ID = editor_cfg::ID_STYLE_COOL_OFF;
			if (NewUnit.GetState()->Coolant == MIST)
				ID = editor_cfg::ID_STYLE_COOL_MIST;
			if (NewUnit.GetState()->Coolant == FLOOD)
				ID = editor_cfg::ID_STYLE_COOL_FLOOD;
			break;
		case 5: // по величине подачи
			Feed = NewUnit.GetState()->Feed;
			if (NewUnit.GetState()->CurCurveType == fast)
				ID = editor_cfg::ID_STYLE_FEED_RAPID;
			else if(Feed == FeedMax)
				ID = editor_cfg::ID_STYLE_FEED_MAX;
			else if(Feed == CurrentFeed)
				ID = CurrFeedID;
			else
			{
				CurrentFeed = Feed;
				if(CurrFeedID == editor_cfg::ID_STYLE_FEED_FIRST)
					ID = editor_cfg::ID_STYLE_FEED_SECOND;
				else
					ID = editor_cfg::ID_STYLE_FEED_SECOND;
				CurrFeedID = ID;
			}
			IsPaint = true;
			break;
		case 6:	// По циклам работы программы
			if (NewUnit.GetState()->CurCycle == DRILL ||
				NewUnit.GetState()->CurCycle == DRILLDWELL ||
				NewUnit.GetState()->CurCycle == DRILLPECK)
				ID = editor_cfg::ID_STYLE_CYCLE_DRILL;
			if (NewUnit.GetState()->CurCycle == RHTAPPINGX ||
				NewUnit.GetState()->CurCycle == RHTAPPINGZ ||
				NewUnit.GetState()->CurCycle == RHTAPPINGPLANE ||
				NewUnit.GetState()->CurCycle == RIGHTRHTAPPING)
				ID = editor_cfg::ID_STYLE_CYCLE_TAPPING;
			if (NewUnit.GetState()->CurCycle == REAM ||
				NewUnit.GetState()->CurCycle == REAMDWELL ||
				NewUnit.GetState()->CurCycle == BORE ||
				NewUnit.GetState()->CurCycle == BACKBORE ||
				NewUnit.GetState()->CurCycle == BOREDWELL)
				ID = editor_cfg::ID_STYLE_CYCLE_REAM;
			break;
		case 31: // По способу коррекции - коррекция на длину
			if (NewUnit.GetState()->CurZComp == UP)
				ID = editor_cfg::ID_STYLE_COMP_UP;
			if (NewUnit.GetState()->CurZComp == DOWN)
				ID = editor_cfg::ID_STYLE_COMP_DOWN;
			if (NewUnit.GetState()->CurZComp == UD_OFF)
				ID = editor_cfg::ID_STYLE_COMP_OFF;
			break;
		case 32: // По способу коррекции - контурная коррекция
			if (NewUnit.GetState()->CurDirComp == LR_OFF)
				ID = editor_cfg::ID_STYLE_LRCOMP_OFF;
			if (NewUnit.GetState()->CurDirComp == LEFT)
				ID = editor_cfg::ID_STYLE_LRCOMP_LEFT;
			if (NewUnit.GetState()->CurDirComp == RIGHT)
				ID = editor_cfg::ID_STYLE_LRCOMP_RIGHT;
			break;
		}//switch
		//slpos = GetStartLinePos(lineCounter);
		//lineCounter++;
		lineNum = NewUnit.GetState()->GetCurCadr() - 1;
		pos = GetStartLinePos(lineNum);
		StartStyling(pos);
		ssize = GetLineLength(lineNum);
		SetStyle(ssize,ID);
		if(NewUnit.EvaluateCurrent(true) < 0)
		{
			if(flag)
			{
				break;
			}
			else
				flag = true;
		}
	}//while
	EndWaitCursor();

}

bool NTextView::GotoCadr(int CadrNum)
{
	EnsureProperProg();
	SetBindMarker(CadrNum);
	GoToLine(CadrNum);
	return true;
}

bool NTextView::GotoCadrFast(int CadrNum)
{
	EnsureProperProg();
	GoToLine(CadrNum);
	SetBindMarker(CadrNum);
	return true;
}

void NTextView::OnAutoUpdate()
{
	AutoUpdate = !AutoUpdate;
}

void NTextView::OnUpdateAutoUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(AutoUpdate);
}

void NTextView::OnEditPaste()
{
#ifndef NC_DEMO
	EditorView::Paste();
#endif
#ifdef NC_DEMO
	AfxMessageBox(IDS_MES_DEMO);
#endif
}

void NTextView::OnEditCopy()
{
#ifndef NC_DEMO
	EditorView::Copy();
#endif
#ifdef NC_DEMO
	AfxMessageBox(IDS_MES_DEMO);
#endif
}
void NTextView::OnEditCut()
{
#ifndef NC_DEMO
	EditorView::Cut();
#endif
#ifdef NC_DEMO
	AfxMessageBox(IDS_MES_DEMO);
#endif
}

void NTextView::OnTextNotPaint() 
{
	NUpdatePainter(0);	
}


// --- ОБРАБОТКА ПОЛЯ С ЗАКЛАДКАМИ ------


// заполнение значений диалога свойств текстового окна
void NTextView::FillSheet(CPropertySheet *pDial)
{
	NTextPropertySheet *pD = (NTextPropertySheet *)pDial;

	// диалог раскраски
	pD->m_painterpage.ModeType = ChosenMode;
}

//----------- UPDATE полей-------------
LRESULT NTextView::OnApplyChanges(WPARAM, LPARAM)
{
	// The user-defined WM_USER_CHANGE_OBJECT_PROPS message is sent
	// by the modal CModalShapePropSheet when the user chooses Apply Now,
	// or by the modeless CModelessShapePropSheet when the user
	// changes any setting in a property page.

	if (!m_pTextPropertySheet)
		return 0;

	NUpdateProperty(m_pTextPropertySheet);
	return 0;
}


// обновление параметров текстового окна
void NTextView::NUpdateProperty(CPropertySheet *pDial)
{
	NTextPropertySheet *pD = (NTextPropertySheet *)pDial;

	// раскраска
	NUpdatePainter(pD->m_iPainterMode);
}

void NTextView::NUpdatePainter(int mode)
{
	if (mode == ChosenMode)
		return;

// раскраска
	ChosenMode = mode;
	ReColor();
}

void NTextView::MarkErrCadr(int CadrNum, const CString &ProgName)
{
// Mark error cadr if ProgName program is loaded
	if (ProgName == LoadedProg)
		SetImageAndState(CadrNum + 1, ERROR_IMAGE);
}


void NTextView::OnEditRenum()
{
	NTextRenumDlg TRD(this);
	if(TRD.DoModal() != IDOK)
		return;
	int width = 0;
	if(TRD.m_LeadZero)
		width = TRD.m_Width;
	int step = 0;
	if(!TRD.m_RemoveNumbers)
		step = TRD.m_Step;
	CString s = TRD.m_Sym;
	if(TRD.m_AddBlank)
		s += " ";
	Renumeration(s,TRD.m_StartNum,TRD.m_MaxNum,step,width,"%");
}

void NTextView::NewText(void)
{
	SendToScentilla(SCI_SETREADONLY, false);
	SendToScentilla(SCI_SETCODEPAGE, 0);
	SendToScentilla(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_RUSSIAN);// Нет необходимости. SCI_SETCODEPAGE, 0 рещает проблему <- неправда
	CString key;
	NProgram *pProg;
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	const CString &buf = pDoc->GetProgrTDT();
	EnableUndo(false);
	// загрузка новой программы
	if(buf.IsEmpty() || !pDoc->NGetProgramList()->Lookup(buf,pProg))
	{
		return;
	}
	auto bAutoUpdate = AutoUpdate;
	AutoUpdate = false;

	DeleteAllText();
	// Disable colorize for huge program
	if (pProg->GetSize() > 50000)
		EnableColorise(false);
	else
		LoadParams();
	bool bEnableColorize = fEnableColorise;
	EnableColorise(false);
	SendToScentilla(SCI_CLEARALL);
	CString str;
	for(int i = 0; i < pProg->GetSize(); ++i)
	{
		const CString &s = ShowReplaced ? (*pProg)[i]->GetReText() : pProg->GetCadrText(i);
		str += s + _T("\n");
		if((i % 500000) == 0)
		{
			AppendText(str);
			str.Empty();
		}
	}
	AppendText(str);
	if (ShowReplaced)
	{
		int lineCount = (int)SendToScentilla(SCI_GETLINECOUNT);
		for (int i = 0; i < min(pProg->GetSize(), lineCount); ++i)
		{
			SendToScentilla(SCI_SETINDICATORCURRENT, (*pProg)[i]->IsTextChanged() ? 1 : 2);
			auto LineEnd = SendToScentilla(SCI_GETLINEENDPOSITION, i);
			auto LineStart = SendToScentilla(SCI_POSITIONFROMLINE, i);
			SendToScentilla(SCI_INDICATORFILLRANGE, LineStart, LineEnd - LineStart);
		}
	}
	EnableUndo(true);
	EnableColorise(bEnableColorize);
	SetPXScreenWidth();
	SetMarginWidth(pProg->GetSize());
	UpdateStat(LoadedProg, pDoc->GetCurChannel()->GetChannelInd(), pDoc->GetChannelsCount());
	SendToScentilla(SCI_SETREADONLY, ShowReplaced);
	AutoUpdate = bAutoUpdate;
}

void NTextView::ChangeTextReplaced()
{
	NProgram* pProg = nullptr;
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	const CString& buf = pDoc->GetProgrTDT();
	if (buf.IsEmpty() || !pDoc->NGetProgramList()->Lookup(buf, pProg))
	{
		return;
	}

	ShowReplaced = !ShowReplaced;
	auto bAutoUpdate = AutoUpdate;
	AutoUpdate = false;

	// Disable colorize for huge program
	if (pProg->GetSize() > 50000)
		EnableColorise(false);
	else
		LoadParams();
	bool bEnableColorize = fEnableColorise;
	auto firstLine = static_cast<int>(SendToScentilla(SCI_GETFIRSTVISIBLELINE));
	EnableColorise(false);
	void* pNewSciDoc = nullptr;
	if (ShowReplaced)
	{
		SaveSciDoc();
		SendToScentilla(SCI_SETDOCPOINTER, 0, (LPARAM)nullptr);
		SendToScentilla(SCI_SETREADONLY, false);
		SendToScentilla(SCI_SETCODEPAGE, 0);
		SendToScentilla(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_RUSSIAN);// Нет необходимости. SCI_SETCODEPAGE, 0 рещает проблему <- неправда
		EnableUndo(false);
		CString str;
		for (int i = 0; i < pProg->GetSize(); ++i)
		{
			const CString& s = (*pProg)[i]->GetReText();
			str += s + _T("\n");
			if ((i % 500000) == 0)
			{
				AppendText(str);
				str.Empty();
			}
		}
		AppendText(str);
		int lineCount = (int)SendToScentilla(SCI_GETLINECOUNT);
		SendToScentilla(SCI_SETINDICATORCURRENT, 0);
		for (int i = 0; i < min(pProg->GetSize(), lineCount); ++i)
		{
			if ((*pProg)[i]->IsTextChanged())
			{
				const auto LineEnd = SendToScentilla(SCI_GETLINEENDPOSITION, i);
				const auto LineStart = SendToScentilla(SCI_POSITIONFROMLINE, i);
				SendToScentilla(SCI_INDICATORFILLRANGE, LineStart, LineEnd - LineStart);
			}
		}
	}
	else
	{
		// Restore SciDoc from NProgram
		pNewSciDoc = pProg->GetSciDoc();
		pProg->SetSciDoc(nullptr); // SciDoc is owned by scintilla now
		SendToScentilla(SCI_SETDOCPOINTER, 0, (LPARAM)pNewSciDoc);
		if (pNewSciDoc != nullptr)
			SendToScentilla(SCI_RELEASEDOCUMENT, 0, (LPARAM)pNewSciDoc);
		else
		{
			EnableUndo(false);
			CString str;
			for (int i = 0; i < pProg->GetSize(); ++i)
			{
				const CString& s = pProg->GetCadrText(i);
				str += s + _T("\n");
				if ((i % 500000) == 0)
				{
					AppendText(str);
					str.Empty();
				}
			}
			AppendText(str);
		}
		EnableUndo(true);
	}
	SetMarginWidth(GetLineCount());
	SetBookmarkArr(pProg->GetBookMarkArray());
	SetErrorArr(pProg);
	SendToScentilla(SCI_SETREADONLY, ShowReplaced);
	EnableColorise(bEnableColorize);
	SetCadrPos(false);
	SendToScentilla(SCI_SETFIRSTVISIBLELINE, firstLine);
	AutoUpdate = bAutoUpdate;
}

bool NTextView::IsWordDef(const CString& str)
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	CString tmp;
	if(pDoc->GetUnit()->GetWordValue(str,tmp))
		return false;
	return true;
}

void NTextView::OnEditUndo()
{
	Undo();
}

void NTextView::OnInsertToolsStr()
{
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	CString ToolsStr;
	pDoc->FillToolsStr(ToolsStr);
	ToolsStr += '\0';
	SendToScentilla(SCI_INSERTTEXT, 0, LPARAM((LPCTSTR)ToolsStr));
}

void NTextView::OnShowReplaced()
{
	ChangeTextReplaced();
}

void NTextView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	if(CanPaste())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void NTextView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	if(CanUndo())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void NTextView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	if(CanCutCopy())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void NTextView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	if(CanCutCopy())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void NTextView::OnEditSelectAll()
{
	SelectAll();
}

void NTextView::OnMacro()
{
	StartMacro();
}

void NTextView::OnChangeLine(int lineNum)
{
	CompleteCurCadr();
}
void NTextView::OnEditorDblClick ()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CompleteCurCadr();
}
void NTextView::OnContextMenu( CWnd* pWnd, CPoint point)
{	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_EDIT));
	((CWinAppEx *)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(*menu.GetSubMenu(0), point.x, point.y, this, TRUE);
	menu.DestroyMenu();
}

void NTextView::RefreshMarkers(void)
{
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc)
		return;
	SetMarkerCurrLine(pDoc->GetUnit()->GetState()->GetCurCadr());
}

void NTextView::OnEditRedo()
{
	Redo();
}

void NTextView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CanRedo());
}

void NTextView::ReadWordSettings(void)
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	editor_cfg cfg = GetConfig();
	pDoc->GetUnit()->GetWordValue("FunctionCode",cfg.General);
	pDoc->GetUnit()->GetWordValue("MiscFunctionCode",cfg.Misc);
	pDoc->GetUnit()->GetWordValue("CommentStart",cfg.BlockCommentStart);
	pDoc->GetUnit()->GetWordValue("CommentEnd",cfg.BlockCommentEnd);
	pDoc->GetUnit()->GetWordValue("ModalCommentOn",cfg.ModalCommentStart);
	pDoc->GetUnit()->GetWordValue("ModalCommentOff",cfg.ModalCommentEnd);
	pDoc->GetUnit()->GetWordValue("BlockSkip",cfg.StringComment);
	pDoc->GetUnit()->GetWordValue("BlockDebug",cfg.BlockDebug);

	pDoc->GetUnit()->GetWordValue("LineNumber",cfg.LineNumber);
	pDoc->GetUnit()->GetWordValue("FeedCode",cfg.Feed);
	pDoc->GetUnit()->GetWordValue("SpeedCode",cfg.Speed);
	pDoc->GetUnit()->GetWordValue("ToolCode",cfg.Tool);
	pDoc->GetUnit()->GetWordValue("ProgramStartEnd",cfg.ProgramSE);
	pDoc->GetUnit()->GetWordValue("ProgramBeginEnd",cfg.ProgramBE);

	pDoc->GetUnit()->GetWordValue("HorizontalAxis",cfg.HorizontalAxis);
	pDoc->GetUnit()->GetWordValue("VerticalAxis",cfg.VerticalAxis);
	pDoc->GetUnit()->GetWordValue("SpindleAxis",cfg.SpindleAxis);

	pDoc->GetUnit()->GetWordValue("HorizontalRotationAxis",cfg.HorizontalRotationAxis);
	pDoc->GetUnit()->GetWordValue("VerticalRotationAxis",cfg.VerticalRotationAxis);
	pDoc->GetUnit()->GetWordValue("SpindleRotationAxis",cfg.SpindleRotationAxis);

	pDoc->GetUnit()->GetWordValue("CircleHorzCenter",cfg.CircleHorzCenter);
	pDoc->GetUnit()->GetWordValue("CircleVerticalCenter",cfg.CircleVerticalCenter);
	pDoc->GetUnit()->GetWordValue("CircleDepthCenter",cfg.CircleDepthCenter);
	pDoc->GetUnit()->GetWordValue("CircleRadius",cfg.CircleRadius);

	pDoc->GetUnit()->GetWordValue("DiaCompOffset",cfg.DiaCompOffset);
	pDoc->GetUnit()->GetWordValue("LengthCompOffset",cfg.LengthCompOffset);
	SetConfig(cfg);
	//NUpdate();
}

void NTextView::ReadColors(void)
{
	CString key;
	editor_cfg cfg = GetConfig();
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);


	//////////////////////////////////////////////////////////////////////////
	// Настройки языка
	//////////////////////////////////////////////////////////////////////////


	NColor *pCol = NULL;

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleCmdNumber.short_name,pCol))
	{
		cfg.StyleCmdNumber.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleCmdNumber.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleCmdNumber.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleComment.short_name,pCol))
	{
		cfg.StyleComment.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleComment.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleComment.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleDigit.short_name,pCol))
	{
		cfg.StyleDigit.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleDigit.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleDigit.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleGCode.short_name,pCol))
	{
		cfg.StyleGCode.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleGCode.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleGCode.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleMCode.short_name,pCol))
	{
		cfg.StyleMCode.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleMCode.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleMCode.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleOther.short_name,pCol))
	{
		cfg.StyleOther.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleOther.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleOther.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleReg.short_name,pCol))
	{
		cfg.StyleReg.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleReg.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleReg.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if(NCM_PROJECT.Colors.Lookup(cfg.StyleText.short_name,pCol))
	{
		cfg.StyleText.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleText.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleText.color.blue = (DWORD)(pCol->array[2]*255);
	}

	LOGFONT lf;
	cfg.font = NCM_PROJECT.Defaults.GetFont(_T("Defaults/PaneText/Font"), lf);

	NColor *pBGCol;
	if (NCM_PROJECT.Colors.Lookup(_T("ФонТекстовогоОкна"), pBGCol))
	{
		cfg.wbkcolor.red = (DWORD)(pBGCol->r());
		cfg.wbkcolor.green = (DWORD)(pBGCol->g());
		cfg.wbkcolor.blue = (DWORD)(pBGCol->b());
	}

	if (NCM_PROJECT.Colors.Lookup(_T("ФонДляНомеровСтрок"), pBGCol))
	{
		cfg.numbkcolor.red = (DWORD)(pBGCol->r());
		cfg.numbkcolor.green = (DWORD)(pBGCol->g());
		cfg.numbkcolor.blue = (DWORD)(pBGCol->b());
	}

	if (NCM_PROJECT.Colors.Lookup(_T("ТекстНомеровСтрок"), pBGCol))
	{
		cfg.numfrcolor.red = (DWORD)(pBGCol->r());
		cfg.numfrcolor.green = (DWORD)(pBGCol->g());
		cfg.numfrcolor.blue = (DWORD)(pBGCol->b());
	}

	if (NCM_PROJECT.Colors.Lookup(_T("СтрелкаАктивногоКадра"), pBGCol))
	{
		cfg.arr1color.red = (DWORD)(pBGCol->r());
		cfg.arr1color.green = (DWORD)(pBGCol->g());
		cfg.arr1color.blue = (DWORD)(pBGCol->b());
	}

	if (NCM_PROJECT.Colors.Lookup(_T("СтрелкаУказанногоКадра"), pBGCol))
	{
		cfg.arr2color.red = (DWORD)(pBGCol->r());
		cfg.arr2color.green = (DWORD)(pBGCol->g());
		cfg.arr2color.blue = (DWORD)(pBGCol->b());
	}

	for(int i = 0; i < 4; i++)
	{
		if(NCM_PROJECT.Colors.Lookup(cfg.StyleCurveType[i].short_name,pCol))
		{
			cfg.StyleCurveType[i].fcolor.red = (DWORD)(pCol->array[0]*255);
			cfg.StyleCurveType[i].fcolor.green = (DWORD)(pCol->array[1]*255);
			cfg.StyleCurveType[i].fcolor.blue = (DWORD)(pCol->array[2]*255);
		}
	}

	NMapStringToNTool *pToolList = pDoc->NGetToolList();
	int i = 0;
	for each(const auto&el in *pToolList)
	{
		if (i == NUM_TOOL)
			break;
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);
		cfg.StyleTool[i].fcolor.red = (DWORD)(pTool->Color.array[0]*255);
		cfg.StyleTool[i].fcolor.green = (DWORD)(pTool->Color.array[1]*255);
		cfg.StyleTool[i].fcolor.blue = (DWORD)(pTool->Color.array[2]*255);
		i++;
	}
	for(int i = 0; i < NUM_FEED; i++)
	{
		if(!NCM_PROJECT.Colors.Lookup(cfg.StyleFeed[i].short_name,pCol))
			continue;
		cfg.StyleFeed[i].fcolor.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleFeed[i].fcolor.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleFeed[i].fcolor.blue = (DWORD)(pCol->array[2]*255);
	}
	for (int i = 0; i < NUM_CYCLE; i++)
	{
		if(NCM_PROJECT.Colors.Lookup(cfg.StyleCycle[i].short_name,pCol))
		{
			cfg.StyleCycle[i].fcolor.red = (DWORD)(pCol->array[0]*255);
			cfg.StyleCycle[i].fcolor.green = (DWORD)(pCol->array[1]*255);
			cfg.StyleCycle[i].fcolor.blue = (DWORD)(pCol->array[2]*255);
		}
	}
	for (int i = 0; i < NUM_COMP; i++)
	{
		if(NCM_PROJECT.Colors.Lookup(cfg.StyleComp[i].short_name,pCol))
		{
			cfg.StyleComp[i].fcolor.red = (DWORD)(pCol->array[0]*255);
			cfg.StyleComp[i].fcolor.green = (DWORD)(pCol->array[1]*255);
			cfg.StyleComp[i].fcolor.blue = (DWORD)(pCol->array[2]*255);
		}
	}
	for (int i = 0; i < NUM_COOL; i++)
	{
		if(NCM_PROJECT.Colors.Lookup(cfg.StyleCool[i].short_name,pCol))
		{
			cfg.StyleCool[i].fcolor.red = (DWORD)(pCol->array[0]*255);
			cfg.StyleCool[i].fcolor.green = (DWORD)(pCol->array[1]*255);
			cfg.StyleCool[i].fcolor.blue = (DWORD)(pCol->array[2]*255);
		}
	}
	for (int i = 0; i < NUM_LR; i++)
	{
		if(NCM_PROJECT.Colors.Lookup(cfg.StyleLRComp[i].short_name,pCol))
		{
			cfg.StyleLRComp[i].fcolor.red = (DWORD)(pCol->array[0]*255);
			cfg.StyleLRComp[i].fcolor.green = (DWORD)(pCol->array[1]*255);
			cfg.StyleLRComp[i].fcolor.blue = (DWORD)(pCol->array[2]*255);
		}
	}
	if (NCM_PROJECT.Colors.Lookup(cfg.StyleLineNumber.short_name,pCol))
	{
		cfg.StyleLineNumber.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleLineNumber.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleLineNumber.color.blue = (DWORD)(pCol->array[2]*255);
	}
	if (NCM_PROJECT.Colors.Lookup(cfg.StyleFeedCode.short_name,pCol))
	{
		cfg.StyleFeedCode.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleFeedCode.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleFeedCode.color.blue = (DWORD)(pCol->array[2]*255);
	}
	if (NCM_PROJECT.Colors.Lookup(cfg.StyleSpeedCode.short_name,pCol))
	{
		cfg.StyleSpeedCode.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleSpeedCode.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleSpeedCode.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleToolCode.short_name,pCol))
	{
		cfg.StyleToolCode.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleToolCode.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleToolCode.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleProgramSE.short_name,pCol))
	{
		cfg.StyleProgramSE.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleProgramSE.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleProgramSE.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleProgramBE.short_name,pCol))
	{
		cfg.StyleProgramBE.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleProgramBE.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleProgramBE.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleLinear.short_name,pCol))
	{
		cfg.StyleLinear.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleLinear.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleLinear.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleAngle.short_name,pCol))
	{
		cfg.StyleAngle.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleAngle.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleAngle.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleArc.short_name,pCol))
	{
		cfg.StyleArc.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleArc.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleArc.color.blue = (DWORD)(pCol->array[2]*255);
	}

	if (NCM_PROJECT.Colors.Lookup(cfg.StyleSpeedCode.short_name,pCol))
	{
		cfg.StyleOffset.color.red = (DWORD)(pCol->array[0]*255);
		cfg.StyleOffset.color.green = (DWORD)(pCol->array[1]*255);
		cfg.StyleOffset.color.blue = (DWORD)(pCol->array[2]*255);
	}
	SetConfig(cfg);
}
void NTextView::OnAddBookmark()
{
	EditorView::OnAddBookmark();
	SaveBookMarkInProgram();
}
void NTextView::OnClearAllBookmark()
{
	EditorView::OnClearAllBookmark();
	SaveBookMarkInProgram();
}

void NTextView::LoadTB(void)
{
	CMFCToolBarInfo TBInfo;
	TBInfo.m_uiColdResID = IDB_TEXT_16;
	TBInfo.m_uiDisabledResID = IDB_TEXT_16_H;
	TBInfo.m_uiHotResID = IDB_TEXT_16;
	TBInfo.m_uiLargeColdResID = IDB_TEXT_24;
	TBInfo.m_uiLargeDisabledResID = IDB_TEXT_24_H;
	TBInfo.m_uiLargeHotResID = IDB_TEXT_24;
	TBInfo.m_uiMenuDisabledResID = IDB_TEXT_16_H;
	TBInfo.m_uiMenuResID = IDB_TEXT_16;
//	if (GetInterfaceStyle() != _T("Ribbon"))
	{
		m_wndToolBar.LoadToolBarEx(IDR_TEXT_16, TBInfo, TRUE);
		m_wndToolBar.SetLockedSizes(CSize(20, 16), CSize(16, 16));
	}


}

int NTextView::LoadParams(void)
{
	NTiParams &Par = NCM_PROJECT.Defaults;

	CString Key = "Defaults/Editor/";
	CString str = Par.GetString((Key + "Colors@Use").GetBuffer(), "Yes");
	EDITOR_COLORS_USE = (str == "Yes");
	fEnableColorise = EDITOR_COLORS_USE;
	int Slop = int(Par.GetDouble((Key + "CaretSlop@Size"), 2.));
	EDITOR_CARET_SLOP = Slop;
	return 0;
}

void NTextView::AdjustLayout(void)
{

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	//TEXTSTATUS	if(::IsWindow(wndStatusBar.GetSafeHwnd()))
	//TEXTSTATUS		wndStatusBar.SetWindowPos(NULL, rectClient.left, rectClient.bottom - STATUSBAR_HEIGHT, rectClient.Width(), rectClient.bottom, SWP_NOACTIVATE | SWP_NOZORDER);
	//TEXTSTATUS CRect rect;
	//TEXTSTATUS	wndStatusBar.GetWindowRect(&rect);
	int cyTlb = 0;
	cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndEditor.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb /*TEXTSTATUS- rect.Height()*/ + 1, SWP_NOACTIVATE | SWP_NOZORDER);
	//TEXTSTATUS	int wArr[] = {0,50,60};
	//TEXTSTATUS	wArr[0] = rectClient.Width() - wArr[1] - wArr[2];
	//TEXTSTATUS	wArr[1] += wArr[0];
	//TEXTSTATUS	wArr[2] += wArr[1];
	//TEXTSTATUS	wndStatusBar.SetParts(3,wArr);
}
void NTextView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
void NTextView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndEditor.SetFocus();
}

void NTextView::OnUpdateEditUpdate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetModify() && ShowReplaced == false);
}

void NTextView::OnUpdateDispExecute(CCmdUI *pCmdUI)
{
	((CMainFrame *)AfxGetMainWnd())->OnUpdateDispExecute(pCmdUI);
}
bool NTextView::HasFocus(void)
{
	return (GetFocus() == &m_wndEditor);
}

void NTextView::SaveSciDoc()
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	if (!LoadedProg.IsEmpty())
	{// Save SciDoc in NProgram
		void* pSciDoc = (void*)SendToScentilla(SCI_GETDOCPOINTER);
		NProgram* pOldProg = nullptr;
		pDoc->NGetProgramList()->Lookup(LoadedProg, pOldProg);
		if (pOldProg != nullptr)
		{
			SendToScentilla(SCI_ADDREFDOCUMENT, 0, (LPARAM)pSciDoc); // to prevent doc deletion
			pOldProg->SetSciDoc(pSciDoc);
		}
	}
}

void NTextView::EnsureProperProg()
{
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	// загрузка текста программы
	const CString &buf = pDoc->GetProgrTDT();
	if (LoadedProg != buf)
	{
		void* pNewSciDoc = nullptr;
		if (!ShowReplaced)
			SaveSciDoc();
		NProgram* pProg = nullptr;
		if (!buf.IsEmpty() && pDoc->NGetProgramList()->Lookup(buf, pProg))
		{// Restore SciDoc from NProgram
			if (!ShowReplaced)
			{
				pNewSciDoc = pProg->GetSciDoc();
				pProg->SetSciDoc(nullptr); // SciDoc is owned by scintilla now
				SendToScentilla(SCI_SETDOCPOINTER, 0, (LPARAM)pNewSciDoc);
				if (pNewSciDoc != nullptr)
					SendToScentilla(SCI_RELEASEDOCUMENT, 0, (LPARAM)pNewSciDoc);
			}
		}
		else
		{
			DeleteAllText();
			ClearLoadedProg();
			SendToScentilla(SCI_SETDOCPOINTER, 0, (LPARAM)nullptr);
			return;
		}
		UpdateStat(buf, pDoc->GetCurChannel()->GetChannelInd(), pDoc->GetChannelsCount());
		LoadedProg = buf;
		LoadedProgInd = pDoc->NGetProgramList()->GetIndex(LoadedProg);
		if(pNewSciDoc == nullptr)
			NewText();
		SetMarginWidth(GetLineCount());
		SendToScentilla(SCI_MARKERDELETEALL, MARKER_BIND);
		SendToScentilla(SCI_MARKERDELETEALL, MARKER_ARROW);
		SetBookmarkArr(pProg->GetBookMarkArray());
		SetErrorArr(pProg);
		SetCollisionArr();
	}
}

void NTextView::OnDestroy()
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	for (int i = 0; i < pDoc->NGetProgramList()->GetSize(); ++i)
	{
		CObject* pObj = nullptr;
		CString Key;
		pDoc->NGetProgramList()->GetAt(i, Key, pObj);
		pDoc->ReleaseSciDoc((NProgram*)pObj);
	}
	EditorView::OnDestroy();
}

void NTextView::SetErrorArr(const BProgram* pProg)
{
	if (pProg == nullptr)
		return;

	ClearErrorMarkersMin();
	auto &Hist = pProg->GetErrorHistory();

	int Pos = 0;
	NProgError ProgError;
	for (POSITION cur = Hist.GetStartPosition(); cur != NULL;)
	{
		Hist.GetNextAssoc(cur, Pos, ProgError);
		if(ProgError.GetCadr().Prog == LoadedProgInd)
			SetMarkerErrorMin(ProgError.GetCadr().Cadr);
	}
}

CString NTextView::GetErrorMinText(int curline)
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	auto pProg = pDoc->NGetProgramList()->GetProg(cadrID(LoadedProgInd, 0));
	if(pProg == nullptr)
		return CString();
	auto& Hist = pProg->GetErrorHistory();

	int Pos = 0;
	NProgError ProgError;
	for (POSITION cur = Hist.GetStartPosition(); cur != NULL;)
	{
		Hist.GetNextAssoc(cur, Pos, ProgError);
		if (ProgError.GetCadr().Cadr == curline)
			return pDoc->ProgErrHandler.MakeErrorMessage(ProgError);
	}
	return CString();
}

void NTextView::SetCollisionArr()
{
	ClearCollisionMarkers();
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	const auto& CollM = pDoc->GetCollManager();
	const NCollisionLog& Log = *CollM.GetForeLog();
	for (int i = 0; i < Log.GetSize(); ++i)
		SetCollision(Log.GetElem(i));
}

cadrID NTextView::CalcCollLocID(const NCollLogElem& LogElem, cadrID& StartID) const
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	CString ProgName;
	BProgram* pProg;
	if (!pDoc->NGetProgramList()->GetAt(LogElem.GetCadrID().Prog, ProgName, (CObject*&)pProg))
		return cadrID(-1);
	cadrID NewID = LogElem.PTPoint;
	StartID = LogElem.StartPTPoint;
	if (LogElem.StartPTPoint.Prog != -1)
	{
//		NewID.Cadr = pProg->GetHistByHCadr(NewID.Cadr);
//		StartID.Cadr = pProg->GetHistByHCadr(StartID.Cadr);
		StartID = pProg->GetCadrByHist(StartID.Cadr);
	}
	const cadrID LocID = pProg->GetCadrByHist(NewID.Cadr);
	ProgName = pDoc->NGetProgramList()->GetProgName(LocID);
	if (ProgName != LoadedProg)
		return cadrID(-1);
	if (StartID.Prog == -1)
		StartID = LocID;
	return LocID;
}

bool NTextView::SetCollision(const NCollLogElem& LogElem)
{
	cadrID StartID;
	cadrID LocID = CalcCollLocID(LogElem, StartID);
	if (LocID.IsEmpty())
		return false;
	for (auto i = StartID.Cadr; i <= LocID.Cadr; ++i)
		SetMarkerCollision(i);
	return true;
}

CString NTextView::GetCollisionText(int curline) const
{
	CNCMDoc* pDoc = (CNCMDoc*)GetDocument();
	const auto& CollM = pDoc->GetCollManager();
	const NCollisionLog& Log = *CollM.GetForeLog();
	for (int i = 0; i < Log.GetSize(); ++i)
	{
		const NCollLogElem& Elem = Log.GetElem(i);
		cadrID buf;
		const cadrID LocID = CalcCollLocID(Elem, buf);
		if (LocID.Cadr == curline)
		{
			CString Str;
			Str.LoadString(Elem.Type);
			return CString( Str + " " + Elem.GetCollNote().data());
		}
	}
	return CString();
}

void NTextView::ApplyMacro()
{
	OnEditUpdate();
	CString NumSym;
	const int Num = GetLastCadrNum(NumSym);
	GenMacro.InserCadrNums(NumSym, Num);
	SendToScentilla(SCI_INSERTTEXT, -1, LPARAM((LPCTSTR)GenMacro.GetResult()));
}

int NTextView::GetLastCadrNum(CString& NumSym) const
{
	CNCMDoc* pDoc = static_cast<CNCMDoc*>(GetDocument());
	auto pProg = pDoc->NGetProgramList()->GetProg(cadrID(LoadedProgInd, 0));
	if (pProg == nullptr)
		return 0;
	const auto CurPos = SendToScentilla(SCI_GETCURRENTPOS);
	const auto CurLine = SendToScentilla(SCI_LINEFROMPOSITION, CurPos);
	int CadrNum = -1;
	for (int i = int(CurLine) - 1; i >= 0 && CadrNum < 0; --i)
	{
		const CString& str = pProg->GetCadrReTextSafe(i);
		CadrNum = pDoc->GetUnit()->GetCadrNum(str, NumSym);
	}
	return CadrNum < 0 ? 0 : CadrNum;
}

bool NTextView::StartMacro()
{
	GenMacro.Generate();
	return true;
}

void NTextView::OnCompP()
{
	NTextsComp TComp(*static_cast<CNCMDoc*>(GetDocument()));
	int ProgInd = GetLoadedProgInd();
	if (ProgInd < 0)
		return;
	if (!TComp.MakeArg1())
		return;
	if (!TComp.MakeArg2(ProgInd))
		return;
	if (!TComp.DoComp())
		AfxMessageBox(IDS_TXTCOMPARE_NOTSTART);// can't start WinMerge
}

void NTextView::OnCompF()
{
	NTextsComp TComp(*static_cast<CNCMDoc*>(GetDocument()));
	int ProgInd = GetLoadedProgInd();
	if (ProgInd < 0)
		return;
	if (!TComp.MakeArg1())
		return;
	if (!TComp.MakeArgOrig2(ProgInd))
		return;
	if (!TComp.DoComp())
		AfxMessageBox(IDS_TXTCOMPARE_NOTSTART);// can't start WinMerge
}

