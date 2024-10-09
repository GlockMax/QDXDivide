#pragma once

#include "MTAxis.h"
#include "MTEdit.h"
#include "MTEditPropList.h"
#include "MTEditPropToolBar.h"

// MTEditPane

//! ��������� ���� �� ������� ������� ���. 
/** ���� �� ���� ������� ��������� ������.
*/
class AFX_EXT_CLASS MTEditPane : public CDockablePane
{
	DECLARE_DYNAMIC(MTEditPane)

public:
	MTEditPane();
	virtual ~MTEditPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPressCloseButton();
	afx_msg void OnToolBarButtonExpand(void);
	afx_msg void OnToolBarButtonApply(void);
	afx_msg void OnToolBarButtonAddsp(void);
	virtual BOOL AllowShowOnPaneMenu() const;
	afx_msg void OnSlider();
	void OnUpdateStateTBSlider(CCmdUI *pCmdUI);
	void OnUpdateStateTBApply(CCmdUI *pCmdUI);
	void OnUpdateToolBarButtonAddsp(CCmdUI *pCmdUI);

	void SetMTEdit(MTEdit *MTE, const CString &MachinePath);
	void SetMachineTool(MTMachineTool *MT);

	void ShowAxisProp(int index);
	void ShowColProp(int index);
	void ShowEmptyProp();
	void MoveAxis(double MovePortion);

	void ApplyChanges();
	void AddSpecPoint();
	void RevertChanges();
	bool IsChangingNodeAllowed();
	bool IsClosingAllowed(void);
	void Deactivate();

protected:
	MTEditPropToolBar mtPropToolBar;
	bool fSlider; ///< ���� ��������� ��������
	MTEditPropList mtPropList;

	//! ��������� �� �������� ����� ���������
	MTEdit *MTEditPtr;   /*!< ����� ���� ����� ���������� ����� ���������� � ������� MTEditTreePane */
	MTMachineTool *EditingMT;
	//! ������ ���, �������� ������� �������� 
	int EditingAxisIndex;
	//! ������ ������� ������������, �������� �������� �������� 
	int EditingColIndex;
	/// 1 - axis, 0 - collision object
	/** ����������, ����������� �� ��� �������, �������� �������� �������� */
	int EditingObject; 

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void AdjustLayout();  //!< ���������� �� OnSize
};
