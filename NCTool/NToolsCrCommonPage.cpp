#include "stdafx.h"
#include "NToolsCrDiskSheet.h"
#include "NToolsCrCommonPage.h"

void NToolsCrCommonPage::OnOK()
{
	CPropertyPage::OnOK();
 
}

BOOL NToolsCrCommonPage::OnApply()
{
	auto Res = CPropertyPage::OnApply();
	SetModified(TRUE);
	return Res;
}

BOOL NToolsCrCommonPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetModified(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void NToolsCrCommonPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	SetModified(TRUE);
}

void NToolsCrCommonPage::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CPropertyPage::OnCancel();
}
