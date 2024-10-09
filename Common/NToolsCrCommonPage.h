#pragma once
#include <afxdlgs.h>
#include <NCTOOL_API.h>

class NCTOOL_API NToolsCrCommonPage : public CPropertyPage
{
public:
	NToolsCrCommonPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate) {};
	virtual void OnOK();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	virtual void OnCancel();
};

