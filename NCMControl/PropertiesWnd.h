
#pragma once
#include "NCUnitDef.h"
#include "NCUnitPropCtrl.h"

class CNCEditor2Doc;

/// Тулбар окна свойств
class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
	virtual BOOL OnUserToolTip(CMFCToolBarButton *pButton, CString & strTTText) const
	{
		switch (pButton->GetImage())
		{
		case 0:
			strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP0);
			return TRUE;
			break;
		case 1:
			strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP1);
			return TRUE;
			break;
		case 2:
			strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP2);
			return TRUE;
			break;
		case 3:
			strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP3);
			return TRUE;
			break;
		case 4:
			strTTText.LoadString(IDS_PROP_TOOLBAR_TOOLTIP4);
			return TRUE;
			break;
		default:
			return FALSE;
			break;
		}//switch
	return FALSE;
	};
};

enum PropObjectType
{
	PO_SECTION,
	PO_GROUP,
	PO_DEFINITION,
	PO_WREPL
};

/// Плавающая панель со списком свойств
class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CPropertiesToolBar m_wndToolBar;
	CNCUnitPropCtrl m_wndPropList;
	PropObjectType editingObjectType;
	void * editingObject;

// Implementation
public:
	virtual ~CPropertiesWnd();
	/// Отображение секции
	void ShowSection(Section *section);
	/// Отображение группы
	void ShowGroup(Section *section, CString group);
	/// Отображение определения
	void ShowDefinition(Def *def);
	/// Отображение read-only определения
	void ShowDefinitionUnknown(Def *def);
	/// Отображение элемента Word Replacement
	void ShowWRepl(WRep *wrep);

	CNCEditor2Doc * GetDocument();

	/// Вызывается при изменении свойств параметров
	/// Из CMFCPropertyGridCtrl
	void OnPropertyChanged(CMFCPropertyGridProperty *pProp);

	void OnChangeSelection(CMFCPropertyGridProperty *pNewSel);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	/// Создание примерного списка
	/** \deprecated */
	void InitPropList();
	void SetPropListFont();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

