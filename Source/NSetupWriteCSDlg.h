#pragma once
#include "ResLocal.h"

// NSetupWriteCSDlg dialog

class NSetupWriteCSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NSetupWriteCSDlg)

public:
	NSetupWriteCSDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NSetupWriteCSDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_WRITECS };
	CComboBox &GetUCSCtrl() { return m_UCSCtrl; }

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void OnOK() override;
	void OnCancel() override;
protected:
	int Ind;
	CComboBox m_UCSCtrl;
};
