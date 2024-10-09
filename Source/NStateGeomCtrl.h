#pragma once
#include "MTreeCtrl.h"
#include "NCadrGeom.h"
/////////////////////////////////////////////////////////////////////////////
// NStateGeomCtrl window

class NStateGeomCtrl : public MTreeCtrl
{
	DECLARE_DYNAMIC(NStateGeomCtrl)
// Construction
public:
	NStateGeomCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NStateGeomCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Update(const NCadrGeom &PrevGeom, const NCadrGeom &NextGeom, const BPoint &ToolAxis);
	void UpdateDist(const BPoint& p0, const BPoint& p1, const BPoint& ToolAxis);
	virtual ~NStateGeomCtrl();
	void CreateOtr();

	// Generated message map functions
protected:

	HTREEITEM BXItem, BYItem, BZItem;// Start point
	HTREEITEM EXItem, EYItem, EZItem;// End point
	HTREEITEM AXItem, AYItem, AZItem;// Angles
	HTREEITEM CXItem, CYItem, CZItem, CRItem;// Center and radius
	HTREEITEM TXItem, TYItem, TZItem;// Tool axis angles
	HTREEITEM LenItem;
	HTREEITEM CenterItem;

	//{{AFX_MSG(NStateGeomCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	CString GetBackName() const override { return CString(_T("ФонПанелиКонтроля")); }
	CString GetFontName() const override { return CString(_T("Defaults/PaneParameters/Font")); }
	CString GetColorName() const override { return CString(_T("ТекстПанелиКонтроля")); }
};
