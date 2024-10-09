#pragma once
#include "NCMProject.h"
#include "NFloatEdit.h"
#include "ResLocal.h"
#include "NToolTurn.h"


// NToolsCrTurn dialog

class NToolsCrTurn : public CPropertyPage
{
	DECLARE_DYNAMIC(NToolsCrTurn)

public:
	NToolsCrTurn(UINT nIDTemplate);
	virtual ~NToolsCrTurn();
	virtual double GetRad() { return 0; };

// Dialog Data
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_name_ctrl;
	CString m_name;
	double m_Xdisp;
	double m_Zdisp;
	double m_MainAngle;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual NToolTurn *CreateTool();
	afx_msg void OnBnClickedUnitInfo();
	afx_msg void OnBnClickedPdz();
	afx_msg void OnBnClickedMdz();
	afx_msg void OnBnClickedPdx();
	afx_msg void OnBnClickedMdx();
	afx_msg void OnBnClickedD0();
	NFloatEdit m_Xdisp_ctrl;
	NFloatEdit m_Zdisp_ctrl;
	NFloatEdit m_MainAngle_ctrl;
};
