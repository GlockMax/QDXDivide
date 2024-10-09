// NViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NViewDlg.h"


// NViewDlg dialog

IMPLEMENT_DYNAMIC(NViewDlg, CDialog)
// Dialog Data
enum { IDD = IDD_VIEW };

NViewDlg::NViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{

}

NViewDlg::~NViewDlg()
{
}

BOOL NViewDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


BEGIN_MESSAGE_MAP(NViewDlg, CDialog)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

//      Handles TTN_NEEDTEXT message to display tooltips for the toolbar.
//      This code is based on CFrameWnd::OnToolTipText
BOOL NViewDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// allow top level routing frame to handle the message
	if (GetRoutingFrame() != NULL)
		return FALSE;

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT_PTR nID = (UINT_PTR)pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT_PTR)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	if (nID != 0) // will be zero on a separator
	{
		AfxLoadString((UINT)nID, szFullText);
			// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_tcsncpy_s(pTTTA->szText, (sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0])),
			strTipText, _TRUNCATE);
	else
	{
		int n = MultiByteToWideChar(CP_ACP, 0, strTipText, -1, pTTTW->szText, 
			sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0]));
		if (n > 0)
			pTTTW->szText[n-1] = 0;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		int n = WideCharToMultiByte(CP_ACP, 0, strTipText, -1, 
			pTTTA->szText, sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0]),
			NULL, NULL);
		if (n > 0)
			pTTTA->szText[n-1] = 0;
	}
	else
		_tcsncpy_s(pTTTW->szText, (sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0])),
			strTipText, _TRUNCATE);
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;    // message was handled
}

BOOL NViewDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CWnd* pOwner = GetParent();
	if (pOwner != NULL)
	{
		TRACE(traceCmdRouting, 1, "Routing command id 0x%04X to owner window.\n", nID);

		ASSERT(pOwner != this);
		if (pOwner->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}