// NTextFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scintilla\include\scintilla.h"
#include "EditorView.h"
#include "NTextFindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOL_0 0
#define TOOL_1 1

/////////////////////////////////////////////////////////////////////////////
// NTextFindDlg dialog


NTextFindDlg::NTextFindDlg()
	:  CDialog(NTextFindDlg::IDD)
{
	//{{AFX_DATA_INIT(NTextFindDlg)
	m_bCase = false;
	m_bWord = false;
	m_bExpr = false;
	m_strFind = _T("");
	m_strReplace = _T("");
	//}}AFX_DATA_INIT
	m_IsFindPage = true;
}

void NTextFindDlg::SetNTextView(EditorView *pParent)
{
	m_ParentWnd = pParent;
	m_ParentWnd->m_bFirstSearch = TRUE;
	m_strFind = pParent->GetSelText();
	if (m_strFind.IsEmpty() || (m_strFind.FindOneOf(_T("\n\r")) != -1))
		m_strFind = _T("");
}

void NTextFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NTextFindDlg)
	DDX_Text(pDX, IDC_FIND_EDIT, m_strFind);
	DDX_Text(pDX, IDC_REPLACE_EDIT, m_strReplace);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(NTextFindDlg, CDialog)
	//{{AFX_MSG_MAP(NTextFindDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FIND_TAB, OnSelchangeToolsTab)
	ON_BN_CLICKED(ID_FIND_NEXT, OnFindNext)
	ON_BN_CLICKED(ID_MARK_ALL, OnMarkAll)
	ON_BN_CLICKED(ID_REPLACE, OnReplace)
	ON_EN_CHANGE(IDC_FIND_EDIT, OnChangeFindEdit)
	ON_EN_CHANGE(IDC_REPLACE_EDIT, OnChangeReplaceEdit)
	ON_BN_CLICKED(IDC_REG, OnCase)
	ON_BN_CLICKED(IDC_WHOLWORD, OnWord)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_REG_EXP, OnExpr)
END_MESSAGE_MAP()


BOOL NTextFindDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Tab = (CTabCtrl * )GetDlgItem(IDC_FIND_TAB);
	ASSERT(m_Tab != NULL);

	CString tmp;
	tmp.LoadString(IDS_STR_FIND);
	m_Tab->InsertItem(0,tmp,0);
	tmp.LoadString(IDS_STR_REPL);
	m_Tab->InsertItem(1,tmp,1);

	m_Tab->SetCurSel(m_IsFindPage ? TOOL_0 : TOOL_1);
/*
	CWnd * pButton;
	pButton = GetDlgItem(IDOK);
	pButton->ShowWindow(SW_SHOW);
	pButton = GetDlgItem(IDCANCEL);
	pButton->ShowWindow(SW_SHOW);
*/
	EnableButton();
	MoveButton(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NTextFindDlg::OnSelchangeToolsTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Tab = (CTabCtrl * )GetDlgItem(IDC_FIND_TAB);
	if( ! UpdateData() )
	{
		m_Tab->SetCurSel(m_IsFindPage ? TOOL_0 : TOOL_1);
		return;
	}
	m_IsFindPage = !m_IsFindPage;

	m_Tab->SetCurSel(m_IsFindPage ? TOOL_0 : TOOL_1);
	EnableButton();
	MoveButton();
}

void NTextFindDlg::OnMarkAll() 
{
	ASSERT (m_ParentWnd);
	
	if (m_IsFindPage)
		m_ParentWnd->NMarkAll(m_strFind, m_bCase, m_bWord,m_bExpr);
	else
		m_ParentWnd->NReplaceAll(m_strFind, m_bCase, m_bWord,m_bExpr, m_strReplace);

	CWnd * pButton = GetDlgItem(ID_MARK_ALL);
	pButton->EnableWindow(false);
}

void NTextFindDlg::OnReplace()
{	
	ASSERT (m_ParentWnd);

	m_ParentWnd->NReplaceText(m_strFind, m_bCase, m_bWord, m_bExpr, m_strReplace);
}

void NTextFindDlg::OnFindNext() 
{
	ASSERT(m_ParentWnd);

	m_ParentWnd->NFindText(m_strFind, m_bCase, m_bWord,m_bExpr);
}

void NTextFindDlg::OnChangeFindEdit() 
{
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_FIND_EDIT);
	DDX_Text(pDX, IDC_FIND_EDIT, m_strFind);
	EnableButton();
	delete pDX;
}

void NTextFindDlg::OnChangeReplaceEdit()
{
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_REPLACE_EDIT);
	DDX_Text(pDX, IDC_REPLACE_EDIT, m_strReplace);
	EnableButton();
	delete pDX;
}

void NTextFindDlg::OnCase() 
{
	if (m_bCase == true)
		m_bCase = false;
	else
		m_bCase = true;
}

void NTextFindDlg::OnWord() 
{
	if (m_bWord == true)
		m_bWord = false;
	else
		m_bWord = true;
}
void NTextFindDlg::OnExpr()
{
	if (m_bExpr == true)
		m_bExpr = false;
	else
		m_bExpr = true;
}

void NTextFindDlg::EnableButton()
{
	CWnd * pButton;

	if (m_IsFindPage)
	{
		pButton = GetDlgItem(IDC_REPLACE_EDIT);
		pButton->ShowWindow(SW_HIDE);
		pButton = GetDlgItem(IDC_REPLACE_STATIC);
		pButton->ShowWindow(SW_HIDE); // SW_SHOW		

		pButton = GetDlgItem(ID_FIND_NEXT);
		CString tmp;
		tmp.LoadString(IDS_FINDNEXT);
		pButton->SetWindowText(tmp);
		pButton->EnableWindow(m_strFind.IsEmpty() ? FALSE : TRUE);

		pButton = GetDlgItem(ID_REPLACE);
		pButton->ShowWindow(SW_HIDE);

		pButton = GetDlgItem(ID_MARK_ALL);

		tmp.LoadString(IDS_MARKALL);
		pButton->SetWindowText(tmp);
		pButton->EnableWindow(m_strFind.IsEmpty() ? FALSE : TRUE);
	}
	else
	{
		pButton = GetDlgItem(IDC_REPLACE_EDIT);
		pButton->ShowWindow(SW_SHOW);
		pButton = GetDlgItem(IDC_REPLACE_STATIC);
		pButton->ShowWindow(SW_SHOW);

		pButton = GetDlgItem(ID_FIND_NEXT);
		CString tmp;
		tmp.LoadString(IDS_FINDNEXT);
		pButton->SetWindowText(tmp);
		pButton->EnableWindow(m_strFind.IsEmpty() ? FALSE : TRUE);

		pButton = GetDlgItem(ID_REPLACE);
		pButton->ShowWindow(SW_SHOW);
		pButton->EnableWindow(m_strFind.IsEmpty() ? FALSE : TRUE);

		pButton = GetDlgItem(ID_MARK_ALL);
		tmp.LoadString(IDS_REPALL);
		pButton->SetWindowText(tmp);
		pButton->EnableWindow((m_strFind.IsEmpty() &&
							   m_strReplace.IsEmpty())? FALSE : TRUE);
	}
}

void NTextFindDlg::MoveButton(bool init /*false*/)
{
	CRect rect;
	CPoint point;
	CWnd * pButton;

	if (m_IsFindPage)
	{
		point.x = 0;
		point.y = -20;
		pButton = GetDlgItem(ID_MARK_ALL);
		pButton->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect += point;
		pButton->MoveWindow(&rect);
		
		point.x = 0;
		point.y = 20;
	}
	else
	{
		point.x = 0;	
		if (init) point.y = 0; else point.y = 20;
		pButton = GetDlgItem(ID_MARK_ALL);
		pButton->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect += point;
		pButton->MoveWindow(&rect);
		pButton = GetDlgItem(ID_MARK_ALL);
		point.x = 0;
		if (init) point.y = 0; else point.y = -20;
	}

	pButton = GetDlgItem(IDC_FIND_EDIT);
	pButton->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect += point;
	pButton->MoveWindow(&rect);

	pButton = GetDlgItem(IDC_FIND_STATIC);
	pButton->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect += point;
	pButton->MoveWindow(&rect);
}

void NTextFindDlg::OnOK() 
{
	CDialog::OnOK();
}

void NTextFindDlg::OnCancel() 
{
	CDialog::OnCancel();
}


void NTextFindDlg::PostNcDestroy()
{
	m_ParentWnd->RemoveFindDlg();
	CDialog::PostNcDestroy();
	delete this;
}
