#pragma once
#include "NCM.h"


// ConfigList

class ConfigList : public CListBox
{
	DECLARE_DYNAMIC(ConfigList)

public:
	ConfigList();
	virtual ~ConfigList();
	CString Modules;
	CString Enabled;
	CString Disabled;
	NCMConfig Config;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


