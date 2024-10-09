
#pragma once
#include "TreeCtrlColor.h"


enum ENCTreeItemType
{
	I_SECTION,
	I_DEFINITION,
	I_WREPL,
	I_GROUP
};

struct IDataSect
{
};

struct IDataDef
{
	CString sectName;
	bool known;
};

struct IDataWRepl
{
	CString srcStr;
	CString dstStr;
};

struct IDataGroup
{
	CString sectName;
};

/// Информация, прикрепленная к каждому элементу в дереве
class NCTreeItemData: public ItemDataABSTRACT
{
public:
	IDataSect sect;
	IDataDef def;
	IDataWRepl wrep;
	IDataGroup group;
	ENCTreeItemType type;
	CString description;

	virtual CString GetToolTipString()
	{
		return description;
	};
};

/////////////////////////////////////////////////////////////////////////////
// CNCUnitTreeCtrl window

/// Перегруженный CTreeCtrl
class CNCUnitTreeCtrl : public CTreeCtrlColor
{
	DECLARE_DYNAMIC(CNCUnitTreeCtrl)
// Construction
public:
	CNCUnitTreeCtrl();

	HTREEITEM hSource;
	HTREEITEM hTarget;
	CImageList *DragIL;
	CPoint MousePos;
	HTREEITEM hPopup;

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CNCUnitTreeCtrl();
	/// Очистка дерева (в т.ч. динамических данных)
	BOOL DeleteAllItems();

	/// Если установлен флаг, то событие OnTvnSelchanged не выполняется
	bool InternalSelect;

	HTREEITEM m_hActiveItem;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//http://support.microsoft.com/kb/222905
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddWrepl();
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
	afx_msg void OnDelWrepl();
};
