#include "stdafx.h"
#include "NToolsConCrPage.h"
#include "WM_APP_messages.h"
#include "NToolCommonSheet.h"

NToolCommonSheet::NToolCommonSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

NToolCommonSheet::NToolCommonSheet(CWnd* pParentWnd, UINT iSelectPage) : CPropertySheet(_T(""), pParentWnd, iSelectPage)
{
}

BOOL NToolCommonSheet::OnInitDialog()
{
	m_bModeless = FALSE;
	m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CPropertySheet::OnInitDialog();

	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;
	GetDlgItem(IDOK)->EnableWindow(true);
	return bResult;
}

void NToolCommonSheet::PostNcDestroy()
{
	for (int i = 0; i < GetPageCount(); ++i)
		if (i != GetActiveIndex())
			static_cast<NToolsConCrPage*>(GetPage(i))->ClearNewTool();

	CPropertySheet::PostNcDestroy();
}

BOOL NToolCommonSheet::DestroyWindow()
{
	AfxGetMainWnd()->SendMessage(WM_APP_STOP_TOOL);

	return CPropertySheet::DestroyWindow();
}

