// NCTabCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ncm.h"
#include "NCTabCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NCTabCtrlDlg dialog


NCTabCtrlDlg::NCTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NCTabCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NCTabCtrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void NCTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NCTabCtrlDlg)
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NCTabCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(NCTabCtrlDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NCTabCtrlDlg message handlers

void NCTabCtrlDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int PageNum = m_TabCtrl.GetCurSel();
	*pResult = 0;
}
