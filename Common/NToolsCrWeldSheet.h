#pragma once
#include "NToolWeld.h"


// NToolsCrWeldSheet

class AFX_EXT_CLASS NToolsCrWeldSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(NToolsCrWeldSheet)

public:
	NToolsCrWeldSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~NToolsCrWeldSheet();
	NToolWeld *GetNewTool(){ return pTool; };
	void SetNewTool(NToolWeld *pT){ pTool = pT; };

protected:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
	NToolWeld *pTool;
	CImageList ToolPics;
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


