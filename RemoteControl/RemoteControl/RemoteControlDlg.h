
// RemoteControlDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CRemoteControlDlg dialog
class CRemoteControlDlg : public CDialogEx
{
// Construction
public:
	CRemoteControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_REMOTECONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGo();
	CString CadrText;

public:
	int Connect(void);

	int SendCommand(enum RemoteCommands Command, CString &Text = CString(_T("")));
	void MoveCommand(bool Plus, const CString &Name);
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedReset();
	afx_msg void OnClose();
protected:
	CString MakeGoAbsStr();
	CString MakeGoDeltaStr(double Delta);
	void SetDlgSize(void);
	virtual void OnOK();
	CBitmap bmp;
	CString PickStr;
public:
	double m_X;
	double m_Y;
	double m_Z;
	double m_I;
	double m_J;
	double m_K;
	afx_msg void OnBnClickedGoAbs();
	int m_Axes;
	int m_Variant;
	int m_SecondDecision;
	int m_FromPick;
	afx_msg void OnBnClickedVector();
	afx_msg void OnBnClickedProject();
	afx_msg void OnBnClickedTwoangl();
	double m_XZ;
	double m_YZ;
	double m_AbsA;
	double m_AbsC;
	int m_RotPart;
	double m_IncrA;
	double m_IncrC;
	int m_ShortDist;
	double m_Delta;
	CRichEditCtrl m_KadrList_Ctrl;
	CEdit m_CadrText;
	int m_ActiveCS;
	CButton m_PickButton;
	CButton m_PickButtonN;
	int m_FullSize;
	CRect FullRect;
public:
	afx_msg void OnBnClickedGoPr();
	afx_msg void OnBnClickedClearPr();
	afx_msg void OnBnClickedGoIncr();
	afx_msg void OnBnClickedGoDelta();
	afx_msg void OnBnClickedSelect();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedFromPick();
	afx_msg void OnBnClickedFullSize();
};
