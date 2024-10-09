#pragma once
#include "ResLocal.h"

// NDemoSettingsDlg dialog

class NDemoSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(NDemoSettingsDlg)

public:
	NDemoSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NDemoSettingsDlg();

// Dialog Data
	enum { IDD = IDD_DEMO_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
