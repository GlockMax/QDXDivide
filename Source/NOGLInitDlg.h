#pragma once

#include "ResLocal.h"
// NOGLInitDlg dialog

class NOGLInitDlg : public CDialog
{
	DECLARE_DYNAMIC(NOGLInitDlg)

public:
	NOGLInitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NOGLInitDlg();

// Dialog Data
	enum { IDD = IDD_OGL_INIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
protected:
	CButton CheckBox;
};
