#pragma once
#include "MListCtrl.h"

// NStateUnitCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NStateUnitCtrl window

class NStateUnitCtrl : public MListCtrl
{
public:
	NStateUnitCtrl();

public:
	COLORREF OnGetCellTextColor(int /*nRow*/, int /*nColum*/) override;
	virtual ~NStateUnitCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(NStateUnitCtrl)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	CString GetBackName() const override { return CString(_T("ФонПанелиКонтроля")); }
	CString GetFontName() const override { return CString(_T("Defaults/PaneParameters/Font")); }
	CString GetColorName() const override { return CString(_T("ТекстПанелиКонтроля")); }
};
