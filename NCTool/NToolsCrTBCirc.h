// NToolsCrTBCirc dialog
#pragma once
#include "ResLocal.h"
#include "NToolTurnBladeCirc.h"
#include "NToolsCrTurn.h"
#include "nfloatedit.h"

class NToolsCrTBCirc : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTBCirc)

public:
	NToolsCrTBCirc();
	virtual ~NToolsCrTBCirc();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_TURNCIRC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnBladeCirc *CreateTool();
	virtual double GetRad() { return m_R; };
public:
	double m_AngleP;
	double m_R;
	double m_Thickness;
public:
	NFloatEdit m_R_ctrl;
	NFloatEdit m_C_ctrl;
	NFloatEdit m_H_ctrl;
};
