#if !defined(AFX_TREECTRLCH_H__EAE31712_77AA_4272_BC11_DFE44B3CD0EC__INCLUDED_)
#define AFX_TREECTRLCH_H__EAE31712_77AA_4272_BC11_DFE44B3CD0EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlCh.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlCh window
/// Абстрактный тип для данных элемента дерева, определяет способ получения строки,
/// отображаемой в тултипе элемента.
class ItemDataABSTRACT
{

	public:
	virtual CString GetToolTipString() = 0;
		
	virtual ~ItemDataABSTRACT(){};

	protected:
		ItemDataABSTRACT(){}; 
};

/// TreeCtrl, умеющий показывать разные тултипы для каждого элемента дерева
class CTreeCtrlCh : public CTreeCtrl
{
// Construction
public:
	DECLARE_DYNAMIC(CTreeCtrlCh)
	CTreeCtrlCh();
	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	void CleanItemData();
	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	void DeleteBranchData( HTREEITEM hti);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlCh)
	protected:
	virtual void PreSubclassWindow();

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlCh();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlCh)
	
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLCH_H__EAE31712_77AA_4272_BC11_DFE44B3CD0EC__INCLUDED_)
