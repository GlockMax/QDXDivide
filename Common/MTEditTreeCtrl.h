#pragma once

#include "stdafx.h"
#include "MTEditDropTarget.h"


/////////////////////////////////////////////////////////////////////////////
// MTEditTreeCtrl window


/// Перегруженный CTreeCtrl для отображения станка.
/**
\sa MTEditTreePane, MTEdit
*/
class MTEditTreeCtrl : public CTreeCtrl
{
	/// Узел дерева, стуктура для вспомогательного алгоритма
	struct TNode
	{
		CString Name;
		CString Parent;
		DWORD_PTR Data;
		int Image, SelImage;
	};
	// Construction
public:
	MTEditTreeCtrl();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~MTEditTreeCtrl();

	/// В зависимости от текущего состояния дерева выполняет expand или collapse для всего дерева
	void ExpandCollapseAll();
	/// Разворачивает всё дерево
	void ExpandAll();
private:
	void ExpandCollapseAllInternal(HTREEITEM hItem, UINT nCode);

public:

	HTREEITEM GetItemByName(HTREEITEM hItem, LPCTSTR szItemName) const;
	CString GetAxis1ofCollision(HTREEITEM hItem);
	CString GetAxis2ofCollision(HTREEITEM hItem);
	void InsertCollision(HTREEITEM hParent, const CString &Axis2);
	void MoveBranch(HTREEITEM hDest);
	void MoveBranch(HTREEITEM hSrc, HTREEITEM hDest);
	void MoveBrahcnCtrl(const CString& AxisName, const CString& DestName);
	void MoveCollisionCtrl(const CString& Axis1, const CString& Axis2, const CString& NewAxis1);
	int GetBranchAmount(HTREEITEM hBaseItem);
	void GetBranch(IN OUT TNode * Branch, IN int &pos, IN HTREEITEM hBase);
	void ShowItemProp(HTREEITEM hItem);
	HTREEITEM GetCutNode(HTREEITEM hNode = NULL);
	void RemoveCutHighlight();

	//context menu handlers
	void OnMTEditShowProp();
	void OnCMPaste();
	void OnCMCut();
	void OnCMCreateTrans();
	void OnCMCreateSChain();
	void OnCMCreateArray();
	void OnCMCreateRot();
	void OnCMCreatePart();
	void OnCMCreateTool();
	void OnCMCreateToolVirt();
	void OnCMCreateStock();
	void OnCMCreateCol();
	void OnCMCreateX();
	void OnCMCreateY();
	void OnCMCreateZ();
	void OnCMCreateA();
	void OnCMCreateB();
	void OnCMCreateC();
	void OnCMCreateSpin();
	void OnCMDeleteBranch();
	void OnCMSaveBranch();
	/// Массовое добавление деталей с загрузкой stl файлов
	void OnCMCreateParts();
	void OnCMCreateParts3D();

public:
	bool IsDropAllowed(HTREEITEM htiTarget);
	bool IsMoveAllowed(HTREEITEM hSrc, HTREEITEM hDest);
protected:
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	HTREEITEM hDraged;
	HTREEITEM hContextMenu;
protected:

public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
private:
	/// Объект для OLE drag and drop
	MTEditDropTarget m_CTreeDropTarget;

public:
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
