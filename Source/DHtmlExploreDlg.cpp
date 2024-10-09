// DHtmlExploreDlg.cpp : implementation file
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"
//#include "DHtmlExplore.h"
#include "DHtmlExploreDlg.h"
//#include "PropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDHtmlExploreDlg dialog

CDHtmlExploreDlg::CDHtmlExploreDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDHtmlExploreDlg::IDD, 0, pParent)
{
	//{{AFX_DATA_INIT(CDHtmlExploreDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

void CDHtmlExploreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDHtmlExploreDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_DHtml_ElementInnerText(pDX, _T("currDir"), m_strCurrDir);
}

BEGIN_MESSAGE_MAP(CDHtmlExploreDlg, CDHtmlDialog)
	//{{AFX_MSG_MAP(CDHtmlExploreDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CDHtmlExploreDlg message handlers

BOOL CDHtmlExploreDlg::OnInitDialog()
{
	SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR);

	CDHtmlDialog::OnInitDialog();

	HINSTANCE bInst = AfxGetResourceHandle();
	AfxSetResourceHandle(GetModuleHandle(_T("Resources.dll")));

	LoadFromResource(IDR_EXPLORE);

	AfxSetResourceHandle(bInst);

	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CDHtmlExploreDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	RefreshDir();
}

void CDHtmlExploreDlg::RefreshDir()
{
 	UpdateData(FALSE);

	// recreate the table
	CComPtr<IHTMLElement> spfileList;
	GetElementInterface(_T("fileList"), IID_IHTMLElement, (void **) &spfileList);
	if (spfileList == NULL)
		return;
	
	CString strBuf;
	strBuf.LoadString(IDS_STAT_HEAD);

	CString strTextProgs;
	strTextProgs.LoadString(IDS_STAT_TEXTPROG);

	CString strTextTools;
	strTextTools.LoadString(IDS_STAT_TEXTTOOL);

	CComBSTR bstrTable = strBuf + strAll + _T("</TABLE>")
		+ strTextProgs + strBuf + strProgs + _T("</TABLE>")
		+ strTextTools + strBuf + strTools + _T("</TABLE>");

	spfileList->put_innerHTML(bstrTable);
}




