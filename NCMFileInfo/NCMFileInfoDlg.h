// NCMFileInfoDlg.h : header file
//

#if !defined(AFX_NCMFILEINFODLG_H__679A9CCE_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_)
#define AFX_NCMFILEINFODLG_H__679A9CCE_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNCMFileInfoDlgAutoProxy;
interface INCMGetData;

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoDlg dialog

class CNCMFileInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CNCMFileInfoDlg);
	friend class CNCMFileInfoDlgAutoProxy;

// Construction
public:
	void Update(const CString &name);
	CNCMFileInfoDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CNCMFileInfoDlg();
	void Update();

// Dialog Data
	//{{AFX_DATA(CNCMFileInfoDlg)
	enum { IDD = IDD_NCMFILEINFO_DIALOG };
	CListCtrl	m_ToolsCtrl;
	CListCtrl	m_ProgramsCtrl;
	long	m_ProgNum;
	long	m_ToolNum;
	CString	m_UnitDescr;
	CString	m_StockDescr;
	CString	m_FileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCMFileInfoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	INCMGetData * m_pIServerData;
	CNCMFileInfoDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CNCMFileInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCMFILEINFODLG_H__679A9CCE_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_)
