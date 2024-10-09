#pragma once
#include "OPENGLRENDER_API.h"

// NViewToolBar

class OPENGLRENDER_API NViewToolBar : public CToolBar
{
	DECLARE_DYNAMIC(NViewToolBar)

public:
	NViewToolBar();
	NViewToolBar(const NViewToolBar & IN) : CToolBar(IN) {}
	virtual ~NViewToolBar();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgToolBar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};


