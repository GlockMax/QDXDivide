#pragma once
#include "afxpropertygridctrl.h"

/// ������������� ������ �������
class CNCUnitPropCtrl :
	public CMFCPropertyGridCtrl
{
protected:
	DECLARE_MESSAGE_MAP()
public:
	CNCUnitPropCtrl(void);
	~CNCUnitPropCtrl(void);

	/// ���������� ��������� ���������
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	virtual void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
