#pragma once
#include "MTreeCtrl.h"
#include "QMeasure.h"
//////////////////////////////////////////////////////////////////////////
// NStateMeasureCtrl


class NStateMeasureCtrl : public MTreeCtrl
{
	DECLARE_DYNAMIC(NStateMeasureCtrl)
	// Construction
public:
	NStateMeasureCtrl();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateMeasureCtrl)

	//}}AFX_VIRTUAL

	// Implementation
public:
	void CreateOtr(void);
	void Update(void);		
	virtual ~NStateMeasureCtrl();

	// Generated message map functions
protected:
	QMeasure * pMeasure;
	HTREEITEM LineIDItem;// program line ID								for all
	HTREEITEM SurfTypeItem;// Type of surface							for all
	HTREEITEM BXItem, BYItem, BZItem;// Center (Start point)			for all
	HTREEITEM NXItem, NYItem, NZItem;// Direction (normal vector)		Cyl, Pl, Tor, Conus
	HTREEITEM ANXItem, ANYItem, ANZItem;// E_Cyl directtion
	HTREEITEM Angle;// Angles N to axis of coordinate	
//	HTREEITEM ERItem;// External radius									spher, tor, Cyl, ECyl
//	HTREEITEM IRItem;// OInternal radius								tor
	HTREEITEM RItem;//  radius				spher, tor, Cyl, ECyl
	HTREEITEM R1Item;// radius				tor

	//{{AFX_MSG(NStateMeasureCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	virtual void PreSubclassWindow();
public:
	void SetMeasure(QMeasure * pM);
protected:
	CString GetBackName() const override { return CString(_T("ФонПанелиКонтроля")); }
	CString GetFontName() const override { return CString(_T("Defaults/PaneParameters/Font")); }
	CString GetColorName() const override { return CString(_T("ТекстПанелиКонтроля")); }
};

