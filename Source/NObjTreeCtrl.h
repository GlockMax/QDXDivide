#pragma once
#include "MTreeCtrl.h"
#include "NObjDropTarget.h"
#include "NLeftViewToolBar.h"

enum 
{
	TOOL_CHECKED = 3,
	TOOL_UNCHECKED = 2,
	TOOL_PARTIAL = 1
};
// NObjTreeCtrl

class NObjTreeCtrl : public MTreeCtrl
{
	friend class CLeftView;
	DECLARE_DYNAMIC(NObjTreeCtrl)

public:
	NObjTreeCtrl(CLeftView *pLeftView);
	virtual ~NObjTreeCtrl();

protected:
// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	HTREEITEM NProgramItem;
	HTREEITEM NProgramItemAux;
	HTREEITEM NToolItem;
	HTREEITEM NExObjItem;
	HTREEITEM NPartItem;
	HTREEITEM NStockItem;
	HTREEITEM NStocksItem;
	HTREEITEM NCUnitItem;
	HTREEITEM NMachineItem;
	CImageList * DragIL;
	HTREEITEM SourceItem;
	HTREEITEM TargetItem;
	CPoint MousePos;
	NObjDropTarget ObjDropTarget;
	static const int ItemTypesNum = 9;
	NLeftViewToolBar ItemToolbars[ItemTypesNum];
	std::vector<NLeftViewToolBar> SubItemToolBars;
	NLeftViewToolBarBase ContextToolBar;
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseLeave();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void ShowContextToolbar(const CPoint &point);
	void CloseContextToolBar();
	void LoadTBFromIcons(UINT MenuID);
	void ClearTB();
	void MakeTB(class CNCMDoc* pDoc);
	void Make1TB(CToolBar &TB, UINT *IDs0, int ButtonsCount, UINT *IDsIcon, int IconsCount, bool SetBkColor = true);
	void MakeChangable(CToolBar &TB, int ButtonID, int NewIconInd);
	void ProcessItemPosChanged();
	void ProcessSibling(HTREEITEM Parent, size_t *pGlobalInd);
	void ProcImportMode(bool IsInImportMode);
	bool SetItemLeftTB(HTREEITEM item, CToolBar & TB);
	static void SetColorIcon(int ind, CToolBar &TB, const class NColor &col);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool IsItemExpanded(HTREEITEM item) const;
	void SetItemExpanded(HTREEITEM item);
	void SetHighlights(class CNCMDoc* pDoc);
	void SetPosChecks(class CNCMDoc* pDoc, bool SetChecks);
	void SetPosChecks1Sect(class CNCMDoc* pDoc, bool SetChecks, HTREEITEM SectItem, const CString& Prefix);
	void InvertPosCheck(class CMapStringToNStock* pStL, HTREEITEM SectItem, const CString& Prefix);
	void ProcPaPosCheck(class CMapStringToNStock* pStL, HTREEITEM SectItem, const CString& Prefix);
	void MergeStL2PosChecks(class CMapStringToNStock* pStL, const CString& Prefix, bool Add);
	bool SetItemBold(HTREEITEM item, bool Set);
	bool SetItemCut(HTREEITEM item, bool Set);
	void ShowAllTB(bool Show);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	HTREEITEM SelectItemByPoint(const CPoint & point);
	bool IsChecked(HTREEITEM item) const;
	bool IsPartial(HTREEITEM item) const;
	void ProcPosClick(HTREEITEM item);
protected:
	CString GetBackName() const override { return CString(_T("ФонПанелиОбъектов")); }
	CString GetFontName() const override { return CString(_T("Defaults/PaneObjects/Font")); }
	CString GetColorName() const override { return CString(_T("ТекстПанелиОбъектов")); }
};


