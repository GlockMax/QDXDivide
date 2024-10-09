#pragma once
#include "UNITS_API.h"
#include <afxlistctrl.h>
class UNITS_API MHeaderCtrl : public CMFCHeaderCtrl
{
	DECLARE_DYNAMIC(MHeaderCtrl)
	DECLARE_MESSAGE_MAP()
public:
	MHeaderCtrl::MHeaderCtrl();
	void SetBkColor(COLORREF cr);
	void SetTextColor(COLORREF cr);
	void OnFillBackground(CDC* pDC) override;
	void OnDrawItem(CDC* pDC, int iItem, CRect rect, BOOL bIsPressed, BOOL bIsHighlighted);
protected:
	COLORREF BkColor;
	COLORREF TextColor;
};

class UNITS_API MListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(MListCtrl)
	DECLARE_MESSAGE_MAP()
public:
	void InitHeader() override;
	virtual MHeaderCtrl& GetHeaderCtrl() { return m_headerCtrl; }
public:
	void SetSettings();
	bool CopyToClipboard(const CString& str);
	bool CopySelectionToClipboard();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	MHeaderCtrl m_headerCtrl; // replaces CMFCListCtrl::m_wndHeader
	CFont m_Font;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void ZoomText(int Dir);
	afx_msg void OnEditCopy();
	afx_msg void OnEditSelectAll();
protected:
	virtual CString GetBackName() const = 0;
	virtual CString GetFontName() const = 0;
	virtual CString GetColorName() const = 0;
};

