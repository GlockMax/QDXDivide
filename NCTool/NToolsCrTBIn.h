// NToolsCrTBIn dialog
#pragma once
#include "NCToolRes.h"
#include "NToolTurnBladeIn.h"
#include "NToolsCrTurn.h"

class NToolTurnBladeIn;

class NToolsCrTBIn : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTBIn)

public:
	NToolsCrTBIn();
	virtual ~NToolsCrTBIn();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_TURNIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnBladeIn *CreateTool();
public:
	double m_AngleA;
	double m_AngleB;
	double m_AngleP;
	double m_L1;
	double m_L2;
	double m_R;
	int m_PointPos;
	BOOL m_RB1;
};
