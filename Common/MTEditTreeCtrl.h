#pragma once

#include "stdafx.h"
#include "MTEditDropTarget.h"


/////////////////////////////////////////////////////////////////////////////
// MTEditTreeCtrl window


/// ������������� CTreeCtrl ��� ����������� ������.
/**
\sa MTEditTreePane, MTEdit
*/
class MTEditTreeCtrl : public CTreeCtrl
{
	/// ���� ������, �������� ��� ���������������� ���������
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

	/// � ����������� �� �������� ��������� ������ ��������� expand ��� collapse ��� ����� ������
	void ExpandCollapseAll();
	/// ������������� �� ������
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
	/// �������� ���������� ������� � ��������� stl ������
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
	/// ������ ��� OLE drag and drop
	MTEditDropTarget m_CTreeDropTarget;

public:
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
