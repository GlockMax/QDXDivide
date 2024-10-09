#pragma once


// CNCAddWReplD dialog

class CNCAddWReplD : public CDialog
{
	DECLARE_DYNAMIC(CNCAddWReplD)

public:
	CNCAddWReplD(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNCAddWReplD();

// Dialog Data
	enum { IDD = IDD_ADD_WREPL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString srcString;
	CString dstString;
protected:
	virtual void OnOK();
};
