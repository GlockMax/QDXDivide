#pragma once
#include "ResLocal.h"

// NImportDlg dialog

class NImportDlg : public CDialog
{
	DECLARE_DYNAMIC(NImportDlg)

public:
	NImportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NImportDlg();

// Dialog Data
	enum { IDD = IDD_IMPTOOLS_END };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void PostNcDestroy();
	virtual void OnCancel();
};
