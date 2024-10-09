#pragma once
#include "afxpropertygridctrl.h"

/// Перегруженный список свойств
class CNCUnitPropCtrl :
	public CMFCPropertyGridCtrl
{
protected:
	DECLARE_MESSAGE_MAP()
public:
	CNCUnitPropCtrl(void);
	~CNCUnitPropCtrl(void);

	/// Обработчик изменения параметра
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	virtual void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
