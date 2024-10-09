// NToolsCrTBThr dialog
#pragma once
#include "ResLocal.h"
#include "NToolTurnBladeThr.h"
#include "NToolsCrTurn.h"

class NToolsCrTBThr : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTBThr)

public:
	NToolsCrTBThr();
	virtual ~NToolsCrTBThr();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_TURNTHR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnBladeThr *CreateTool();
	virtual double GetRad() { return m_R; };
public:
	double m_R;
	double m_Thickness;
	double m_ThreadAngle;
	double m_CutWhide;
	double m_CutLength;
	double m_HalfT;
	double m_AngleP;
	NFloatEdit m_R_ctrl;
	NFloatEdit m_Thickness_ctrl;
	NFloatEdit m_ThreadAngle_ctrl;
	NFloatEdit m_CutWhide_ctrl;
	NFloatEdit m_CutLength_ctrl;
	NFloatEdit m_HalfT_ctrl;
	NFloatEdit m_AngleP_ctrl;
	int m_PointPos;
	BOOL m_RB1;
	int m_FormTread;
protected:
	CStatic m_Pict;
	CBitmap PictBMP;
public:
	afx_msg void OnCbnSelchangeFormtread();
	virtual BOOL OnInitDialog();
protected:
	void SetPicture(void);
};
