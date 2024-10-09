#pragma once
#include "nunitnamedlg.h"

class AFX_EXT_CLASS NMachNameDlg :
	public NUnitNameDlg
{
public:
	NMachNameDlg(void);
	virtual ~NMachNameDlg(void);
	virtual BOOL OnInitDialog();
	CString GetName();
};
