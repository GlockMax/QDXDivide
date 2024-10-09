// NToolsCrTHBar dialog
#pragma once
#include "ResLocal.h"
#include "NToolTurnHolderBar.h"
#include "NToolsCrTurn.h"

class NToolsCrTHBar : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTHBar)

public:
	NToolsCrTHBar();
	virtual ~NToolsCrTHBar();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_HOLDBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnHolderBar *CreateTool();
	double m_Dx;
	double m_Dy;
	double m_Dz;
	double m_Y;
	NFloatEdit m_Dx_ctrl;
	NFloatEdit m_Dy_ctrl;
	NFloatEdit m_Dz_ctrl;
	NFloatEdit m_Y_ctrl;
};
