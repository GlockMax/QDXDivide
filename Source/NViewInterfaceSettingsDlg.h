#pragma once
#include "ResLocal.h"
#include "NCMProject.h"

// NViewInterfaceSettingsDlg dialog

class NViewInterfaceSettingsDlg : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(NViewInterfaceSettingsDlg)

public:
	NViewInterfaceSettingsDlg();
	virtual ~NViewInterfaceSettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWINTERFACE_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeNamesCtrl();

	BOOL m_Reverse;
	CComboBox m_NamesCtrl;
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
