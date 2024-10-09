#pragma once

#include "MTEditTreeCtrl.h"
#include "MTMashineTool.h"
#include "MTEdit.h"
#include <vector>

class CTreeToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }

	virtual BOOL OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const;
public:
	CTreeToolBar():
	CMFCToolBar()
	{
		m_bLocked = TRUE;
	};
};

// MTEditTreePane

/// Панель с деревом станка
class AFX_EXT_CLASS MTEditTreePane : public CDockablePane
{
	DECLARE_DYNAMIC(MTEditTreePane)
	class AxisStackNode : public CObject
	{
	public:
		HTREEITEM hItem;
		int level;
	};//class AxisStackNode : public CObject

	class AxisStack : public CTypedPtrList< CObList, AxisStackNode* >
	{
	public:
		// Add element to top of stack
		void Push(AxisStackNode* newTray)
		{
			AddHead(newTray);
		}

		// Peek at top element of stack
		AxisStackNode* Peek()
		{
			return IsEmpty() ? NULL : GetHead();
		}

		// Pop top element off stack
		AxisStackNode* Pop()
		{
			return RemoveHead();
		}
	};

public:
	MTEditTreePane();
	virtual ~MTEditTreePane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPressCloseButton();
	afx_msg void OnPaMachEditEnd(void);
	afx_msg void OnToolBarButtonExpand(void);
	afx_msg void OnToolBarButtonSave(void);
	afx_msg void OnToolBarButtonUndo(void);
	afx_msg void OnToolBarButtonRedo(void);
	afx_msg void OnToolBarButtonSaveAs(void);
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	afx_msg void OnToolBarLoadBranch(void);
 
	virtual BOOL AllowShowOnPaneMenu() const override;
	void ShowPropPane();


	void OnUpdateStateTBUndo(CCmdUI *pCmdUI);
	void OnUpdateStateTBRedo(CCmdUI *pCmdUI);
	void OnUpdateStateTBSave(CCmdUI *pCmdUI);
	void OnUpdateStateTBSaveAs(CCmdUI *pCmdUI);
	//ToolBar management
	void TBEnableUndo(bool Enable = true);
	void TBEnableRedo(bool Enable = true);
	void TBEnableSave(bool Enable = true);

	void SetMachineTool(MTMachineTool *MT);
	void SetMTEdit(MTEdit *MTE, CString path);
	bool ShowMachineTool(MTMachineTool *MT, int SelIndex = 0);
	bool GetImagesNums(MTMachineTool* MT, int i, int& AxisImage, int& AxisSelImage);
	bool SelectAxis(const CString& AxisName);
	bool SelectNode(const CString& NodeName);
	bool SelectAndShowPropNode(const CString& NodeName);
	CString GetSelectedNode();
	bool RenameTreeNode(const CString &OldName, const CString &NewName);
	bool ShowAxisProp(const CString& AxisName);//Вызывается дочерним контролом СClassTreeWnd при выделении элемента
	bool ShowColProp(const CString& Axis1, const CString& Axis2);//Или вызывается это в зависимости от типа узла
	bool IsDescendant(CString &BaseAxis, CString &Axis);
	bool IsChangingNodeAllowed();///<Вызывается для при смене выделенной оси, показывает диалог применения изменений.
	void ShowEmptyProp();
	
	void GetMachinePath(CString &path, CString &name);

	//Функции редактирования
	//====================================================================================
	//MTEditTreeCtrl -> MTEdit
	void MoveBranch(const CString& Axis1, const CString& Axis2);
	void MoveCollision(const CString& Axis1, const CString& Axis2, const CString& NewAxis1);
	void CreateSChain(const CString& ParentName);
	void CreateArray(const CString& ParentName);
	void CreateTrans(const CString& ParentName);
	void CreateRot(const CString& ParentName);
	void CreateSpin(const CString& ParentName);
	void CreatePart(const CString& ParentName);
	void CreateParts(const CString& ParentName, std::vector<CString> &list);
	void CreateTool(const CString& ParentName);
	void CreateToolVirt(const CString& ParentName);
	void CreateStock(const CString& ParentName);
	void CreateCol(const CString& ParentName);
	void CreateX(const CString& ParentName);
	void CreateY(const CString& ParentName);
	void CreateZ(const CString& ParentName);
	void CreateA(const CString& ParentName);
	void CreateB(const CString& ParentName);
	void CreateC(const CString& ParentName);
	void DeleteBranch(const CString& AxisName);
	void DeleteCollision(const CString& Axis1, const CString& Axis2);
	//Изменения в MTEditTreeCtrl после выполнения операций редактирования
	void MoveBranchCtrl(const CString& AxisName, const CString& DestName);
	void MoveCollisionCtrl(const CString& Axis1, const CString& Axis2, const CString& NewAxis1);
	HTREEITEM CreateAxisCtrl(const MTAxis& Axis, const CString& ParentName);
	void CreateTransCtrl(const CString& AxisName, const CString& ParentName);
	void CreateRotCtrl(const CString& AxisName, const CString& ParentName);
	void CreateSpinCtrl(const CString& AxisName, const CString& ParentName);
	void CreatePartCtrl(const CString& AxisName, const CString& ParentName);
	void CreateToolCtrl(const CString& AxisName, const CString& ParentName);
	void CreateToolVirtCtrl(const CString& AxisName, const CString& ParentName);
	void CreateStockCtrl(const CString& AxisName, const CString& ParentName);
	void CreateColCtrl(const CString& AxisName, const CString& ParentName);
	void DeleteBranchCtrl(const CString& NodeName);
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

protected:
	MTMachineTool *EditingMT;
	CTreeToolBar mtTreeToolBar;
	MTEditTreeCtrl mtTreeTree;
	CImageList mtTreeViewImages;
	MTEdit *MTEditPtr;   ///< Указатель на редактор, для вызова функций
	CString MachinePath;
	char fUndo;
	char fRedo;
	char fSave;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void AdjustLayout();

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	bool ShowMachineTool(void) { return ShowMachineTool(EditingMT);}
	void RenameRoot(const CString &NewRoot);
	bool SaveBranch(const CString& AxisName) const;
	const auto& GetmtTreeTree() const { return mtTreeTree; }
	afx_msg void OnClose();
};


