#if !defined(AFX_NCSTATIC_H__A6BB8B99_1D08_11D4_A0EC_000000000000__INCLUDED_)
#define AFX_NCSTATIC_H__A6BB8B99_1D08_11D4_A0EC_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NCStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NCStatic window

class AFX_EXT_CLASS NCStatic : public CStatic
{
// Construction
public:
	NCStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NCStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~NCStatic();

	// Generated message map functions
protected:
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;
	CBrush m_brBkgnd;
	//{{AFX_MSG(NCStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCSTATIC_H__A6BB8B99_1D08_11D4_A0EC_000000000000__INCLUDED_)
