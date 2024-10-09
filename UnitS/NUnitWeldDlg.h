#pragma once
#include "ResLocal.h"
#include "NFloatEdit.h"

// NUnitWeldDlg dialog

class NUnitWeldDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(NUnitWeldDlg)

public:
	NUnitWeldDlg();
	virtual ~NUnitWeldDlg();

// Dialog Data
	enum { IDD = IDD_UNIT_FOCUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	NFloatEdit m_ElectrT1_ctrl;
	double m_ElectrT1;
	NFloatEdit m_ElectrA_ctrl;
	double m_ElectrA;
	NFloatEdit m_ElectrB_ctrl;
	double m_ElectrB;
	NFloatEdit m_ElectrR_ctrl;
	double m_ElectrR;
};
