// NToolsCrTHCyl dialog
#pragma once
#include "ResLocal.h"
#include "NToolTurnHolderCyl.h"
#include "NToolsCrTurn.h"

class NToolsCrTHCyl : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTHCyl)

public:
	NToolsCrTHCyl();
	virtual ~NToolsCrTHCyl();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_HOLDCON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnHolderCyl *CreateTool();
	double m_Dx;
	double m_Y;
	double m_D1;
	double m_D2;
	NFloatEdit m_Dx_ctrl;
	NFloatEdit m_Y_ctrl;
	NFloatEdit m_D1_ctrl;
	NFloatEdit m_D2_ctrl;
	int m_FH;
};
