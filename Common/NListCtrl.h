#pragma once
// NListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NListCtrl window

class NListCtrl : public CListCtrl
{
// Construction
public:
	NListCtrl();

// Attributes
public:
	

protected:
	DWORD	m_newStyle;
	BOOL	m_bColumnLines;
	CArray <int, int> m_arChangeCol;
	CTypedPtrArray<CPtrArray, CEdit*> m_arLVEdit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CEdit* GetEditCtrl(int index);
	int GetNumEditColumn();
	void InsertEditLAbels();
	void SetRectStyle(BOOL bRect);
	void AddNewStyle(DWORD dwNewStyle);
	void AddChangingColumn(int col);
	virtual ~NListCtrl();

	// Generated message map functions
protected:

	//{{AFX_MSG(NListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

