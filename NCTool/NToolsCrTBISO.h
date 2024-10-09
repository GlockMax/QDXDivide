// NToolsCrTBISO dialog
#pragma once
#include "NToolTurnBladeISO.h"
#include "NToolsCrTurn.h"

class NToolsCrTBISO : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTBISO)

public:
	NToolsCrTBISO();
	virtual ~NToolsCrTBISO();

// Dialog Data

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolTurnBladeISO *CreateTool();
	virtual double GetRad();
public:
//	double m_AngleP;
	double m_L1;
	double m_L2;
	NFloatEdit m_L1_ctrl;
	NFloatEdit m_L2_ctrl;
	int m_Edge;
	BOOL m_RB1;
	CString m_Form;
	CString m_BackAngle;
	CString m_SThickness;
	CString m_SRad;
	CComboBox m_CForm;
	virtual BOOL OnInitDialog();
	CComboBox m_CBackAngle;
	CComboBox m_CSThikness;
	CComboBox m_CSRad;
	CStatic m_Pict6;
	CStatic m_Pict;
	CStatic m_EdgePict;
	CBitmap Pict6BMP;
	void DefEdgePicture(void);
	afx_msg void OnCbnSelchangeFormiso();
	afx_msg void OnBnClickedE1();
	afx_msg void OnBnClickedE2();
	CBitmap PictBMP;
};
