#pragma once
#include <set>
#include "MListCtrl.h"

class AFX_EXT_CLASS NReportCtrl : public MListCtrl
{
	DECLARE_DYNAMIC(NReportCtrl)
// Construction
public:
	NReportCtrl();

// Implementation
public:
	void Update(const double *Regs);
	virtual ~NReportCtrl();
	void AddCollItem(UINT StrID, DWORD_PTR Data, const CString &ProgName, cadrID LocID, int StepNum, const CString& CollNote);
	void AddCollItem(UINT StrID, DWORD_PTR Data, const CString &ProgName, cadrID LocID, int StepNum, int StartLineNum, int StartStepNum, const CString& CollNote);
	void AddCollItem(const CString& Str, DWORD_PTR Data, const CString& ProgName, cadrID LocID, int StepNum, const CString& CollNote);
	void AddErrorItem(const CString& Str, DWORD_PTR Data, cadrID LocID, const CString& ProgName, const CString& Prim);
	DWORD_PTR GetDataFromSelectedItem(void) const;  
	CString GetTextFromSelectedItem(int FieldInd) const;
	COLORREF OnGetCellTextColor(int nRow, int nColumn) override;

	// Generated message map functions
protected:
	//{{AFX_MSG(NReportCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	CString GetBackName() const override { return CString(_T("‘онѕанелиќшибки")); }
	CString GetFontName() const override { return CString(_T("Defaults/PaneErrors/Font")); }
	CString GetColorName() const override { return CString(_T("“екстѕанелиќшибки")); }

public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnListItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnReportRunto();
	afx_msg void OnReportClear();
	int OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn) override;
	void Sort(int iColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE) override;
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void DeleteMessages(const std::set<int>& Inds);
	void SortBySettings();
};

/////////////////////////////////////////////////////////////////////////////
