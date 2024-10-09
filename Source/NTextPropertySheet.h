#pragma once
// NTextPropertySheet.h : header file
//
//#include "NTextFindDlg.h"
//#include "NTextMarkers.h"
//#include "NTextFonts.h"
#include "NModePainterDlg.h"

#define WM_USER_CHANGE_OBJECT_PROPERTIES (WM_USER + 1)
/////////////////////////////////////////////////////////////////////////////
// NTextPropertySheet

class NTextPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(NTextPropertySheet)

// Construction
public:
	NTextPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage = 0);
	NTextPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage = 0);

// Attributes
public:
	NModePainterDlg m_painterpage;
//	NTextFindPage	m_findpage;
//	NTextFonts		m_fontpage;
//	NTextMarkers	m_markerspage;

	int		m_iPainterMode;
	//int		m_iTopLine;
	//int		m_iBottomLine;
	//int		m_iFontSize;
	//BOOL	m_bFix;
	//BOOL	m_bScroll;
	//BOOL	m_bMark1;
	//BOOL	m_bMark2;
	//BOOL	m_bError;
	//
	//bool	m_bBold;
	//bool	m_bItalic;
	//bool	m_bUnderLine;

	//NColor  m_FontColor;
	//CString m_sFontName;
	//int		m_VisLine;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NTextPropertySheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~NTextPropertySheet();

	// Generated message map functions
protected:
	void NUpdateData();
	void AddControlPages(CWnd* pParentWnd);
	//{{AFX_MSG(NTextPropertySheet)
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

