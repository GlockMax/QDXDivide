// RInsParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ncm.h"
#include "resource.h"
#include "RInsParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RInsParamDlg dialog


RInsParamDlg::RInsParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RInsParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RInsParamDlg)
	m_Sym = _T("");
	//}}AFX_DATA_INIT
}


void RInsParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RInsParamDlg)
	DDX_CBString(pDX, IDC_COMBO_PARAM2, m_Sym);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RInsParamDlg, CDialog)
	//{{AFX_MSG_MAP(RInsParamDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RInsParamDlg message handlers
