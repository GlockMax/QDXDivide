// NToolsCrTBOut dialog
#pragma once
#include "ResLocal.h"
#include "NToolTurnBladeOut.h"
#include "NToolsCrTurn.h"

class NToolsCrTBOut : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTBOut)

public:
	NToolsCrTBOut();
	virtual ~NToolsCrTBOut();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_TURNOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnBladeOut *CreateTool();
	virtual double GetRad() { return m_R; };

public:
	double m_AngleA;
	double m_AngleB;
	double m_AngleP;
	double m_L1;
	double m_L2;
	double m_R;
	double m_Thickness;
	NFloatEdit m_AngleA_ctrl;
	NFloatEdit m_AngleB_ctrl;
	NFloatEdit m_AngleP_ctrl;
	NFloatEdit m_L1_ctrl;
	NFloatEdit m_L2_ctrl;
	NFloatEdit m_R_ctrl;
	NFloatEdit m_Thickness_ctrl;
	int m_PointPos;
	BOOL m_RB1;
	int m_FormOut;
	afx_msg void OnCbnSelchangeFormout();
protected:
	CStatic m_Pict;
	CBitmap PictBMP;
public:
	virtual BOOL OnInitDialog();
protected:
	void SetPicture(void);
};
