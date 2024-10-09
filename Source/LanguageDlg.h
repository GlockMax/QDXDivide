#pragma once
#include "ResInv.h"
#include "afxwin.h"

// LanguageDlg dialog

class LanguageDlg : public CDialog
{
	DECLARE_DYNAMIC(LanguageDlg)

public:
	LanguageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LanguageDlg();

// Dialog Data
	enum { IDD = IDD_LANGUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Lang;
	CComboBox LangCtrl;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
};
