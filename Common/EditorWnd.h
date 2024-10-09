#pragma once

// EditorWnd


class EditorWnd : public CWnd
{
	DECLARE_DYNAMIC(EditorWnd)

public:
	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,
		UINT nID,CCreateContext* pContext);
	EditorWnd();
	virtual ~EditorWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


