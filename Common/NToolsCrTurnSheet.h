#pragma once
#include "NToolTurn.h"


// NToolsCrTurnSheet

class AFX_EXT_CLASS NToolsCrTurnSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(NToolsCrTurnSheet)

public:
	NToolsCrTurnSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~NToolsCrTurnSheet();
	NToolTurn *GetNewTool(){ return pTool; };
	void SetNewTool(NToolTurn *pT){ pTool = pT; };

protected:
	afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
	NToolTurn *pTool;
	CImageList ToolPics;
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


