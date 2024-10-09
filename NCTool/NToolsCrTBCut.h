// NToolsCrTBCut dialog
#pragma once
#include "ResLocal.h"
#include "NToolTurnBladeCut.h"
#include "NToolsCrTurn.h"

class NToolsCrTBCut : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTBCut)

public:
	NToolsCrTBCut();
	virtual ~NToolsCrTBCut();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_TURNCUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnBladeCut *CreateTool();
	virtual double GetRad() { return (m_Edge == 1) ? m_R1 : m_R; };
public:
	double m_AngleB;
	double m_AngleP;
	double m_R;
	double m_Thickness;
	double m_Whidth;
	double m_Length;
	double m_R1;
	NFloatEdit m_AngleB_ctrl;
	NFloatEdit m_AngleP_ctrl;
	NFloatEdit m_R_ctrl;
	NFloatEdit m_Thickness_ctrl;
	NFloatEdit m_Whidth_ctrl;
	NFloatEdit m_Length_ctrl;
	NFloatEdit m_R1_ctrl;
	BOOL m_RB1;
	int m_Edge;
	afx_msg void OnBnClickedE1();
	afx_msg void OnBnClickedE2();
	CBitmap PictBMP;
	CStatic m_Pict;
	void DefEdgePicture(void);
	virtual BOOL OnInitDialog();
};
