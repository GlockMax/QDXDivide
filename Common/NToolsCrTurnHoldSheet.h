#pragma once
#include "NToolTurn.h"


// NToolsCrTurnHoldSheet

class AFX_EXT_CLASS NToolsCrTurnHoldSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(NToolsCrTurnHoldSheet)

public:
	NToolsCrTurnHoldSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~NToolsCrTurnHoldSheet();
	NToolTurn *GetNewTool(){ return pTool; };
	void SetNewTool(NToolTurn *pT){ pTool = pT; };

protected:
	afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
	NToolTurn *pTool;
};


