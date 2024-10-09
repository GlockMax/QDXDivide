#pragma once

#include "ResLocal.h"

/////////////////////////////////////////////////////////////////////////////
// NTextFindDlg dialog
//EXTERN_PROCESS_LOCAL(_AFX_RICHEDIT_STATE, _afxRichEditState)

class EditorView;
//class _AFX_RICHEDIT_STATE;

class NTextFindDlg : public CDialog 
{
// Construction
public:
	NTextFindDlg(); // standard constructor
	void SetNTextView(EditorView *);

// Dialog Data
	//{{AFX_DATA(NTextFindDlg)
	enum { IDD = IDD_FIND_TEXT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NTextFindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeToolsTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void MoveButton(bool init = false);
	void EnableButton();
	bool	m_bCase;
	bool	m_bWord;
	bool	m_bExpr;
	bool	m_IsFindPage;
	CString m_strFind;
	CString m_strReplace;
	CTabCtrl	*m_Tab;
	EditorView	*m_ParentWnd;

// Generated message map functions
	//{{AFX_MSG(NTextFindDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFindNext();
	afx_msg void OnMarkAll();
	afx_msg void OnReplace();
	afx_msg void OnChangeFindEdit();
	afx_msg void OnChangeReplaceEdit();
	afx_msg void OnCase();
	afx_msg void OnWord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnExpr();
protected:
	virtual void PostNcDestroy();
};

