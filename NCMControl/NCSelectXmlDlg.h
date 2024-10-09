#pragma once


// CNCSelectXmlDlg dialog

class CNCSelectXmlDlg : public CDialog
{
	DECLARE_DYNAMIC(CNCSelectXmlDlg)

public:
	CNCSelectXmlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNCSelectXmlDlg();

// Dialog Data
	enum { IDD = IDD_SELECT_XML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	CString xmlName;
	CString curName;
	CString workPath;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal(CString path, CString curname);
};
