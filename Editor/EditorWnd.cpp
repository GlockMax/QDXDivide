// EditorWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "WM_APP_messages.h"
#include "Scintilla\include\Accessor.h"
#include "Scintilla\include\PropSet.h"
#include "Scintilla\include\Scintilla.h"
#include "EditorWnd.h"

static HMODULE SCINTILLA_HMODULE = NULL;
static int SCI_REF = 0;




// EditorWnd

IMPLEMENT_DYNAMIC(EditorWnd, CWnd)
EditorWnd::EditorWnd()
{
}

EditorWnd::~EditorWnd()
{
}


BEGIN_MESSAGE_MAP(EditorWnd, CWnd)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// EditorWnd message handlers

BOOL EditorWnd::Create(LPCTSTR lpszWindowName, DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,
						UINT nID,CCreateContext* pContext)
{
	if(NULL == SCINTILLA_HMODULE)
	{
		SCINTILLA_HMODULE=::LoadLibrary(_T("SciLexer.dll"));
		SCI_REF++;
	}
	if(NULL == SCINTILLA_HMODULE)
	{
		AfxMessageBox(IDS_ERRMSG_LOADSCINTILLAFAILD,MB_OK|MB_ICONERROR);
		return FALSE;
	}
	if(!CWnd::CreateEx(0, _T("Scintilla"), lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		AfxMessageBox(IDS_ERRMSG_CREATEFAILD,MB_OK|MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

void EditorWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	Sleep(200);// this gives the user time to release mouse button
	int pos = (int)SendMessage(SCI_GETCURRENTPOS);
	SendMessage(SCI_SETSEL, pos, pos);

	CWnd::OnLButtonDblClk(nFlags, point);
}
