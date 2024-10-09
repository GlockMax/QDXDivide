// NTextPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "NTextView.h"
#include "NTextPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NTextPropertySheet

IMPLEMENT_DYNAMIC(NTextPropertySheet, CPropertySheet)

NTextPropertySheet::NTextPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages(pParentWnd);

	m_iPainterMode = 0;
	//m_bFix = FALSE;
	//m_bScroll = FALSE;
	//m_bMark1 = FALSE;
	//m_bMark2 = FALSE;
	//m_bError = FALSE;
	//m_iTopLine = 0;
	//m_iBottomLine = 0;
	//m_iFontSize = 0;
	//m_sFontName = _T("");
	//m_VisLine = 0;
}

NTextPropertySheet::NTextPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddControlPages(pParentWnd);

	m_iPainterMode = 0;
	//m_bFix = FALSE;
	//m_bScroll = FALSE;
	//m_bMark1 = FALSE;
	//m_bMark2 = FALSE;
	//m_bError = FALSE;
	//m_iTopLine = 0;
	//m_iBottomLine = 0;
	//m_iFontSize = 0;
	//m_sFontName = _T("");
}

NTextPropertySheet::~NTextPropertySheet()
{
}

BEGIN_MESSAGE_MAP(NTextPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(NTextPropertySheet)
	ON_COMMAND(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NTextPropertySheet message handlers

void NTextPropertySheet::AddControlPages(CWnd* pParentWnd)
{
//	m_psh.dwFlags |= SWP_NOACTIVATE ;//PSH_NOAPPLYNOW;    // Lose the Apply Now button
//	m_psh.dwFlags &= ~PSH_HASHELP;  // Lose the Help button
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

//	m_painterpage.m_psp.dwFlags &= ~PSP_HASHELP;  // Lose the Help button
	AddPage(&m_painterpage);
//	AddPage(&m_findpage);
//	m_findpage.SetNTextView((NTextView*)pParentWnd);
//	AddPage(&m_fontpage);
//	AddPage(&m_markerspage);
}

void NTextPropertySheet::OnOK()
{
	NUpdateData();
		
	EndDialog( IDOK );
}

void NTextPropertySheet::NUpdateData()
{
	if (m_painterpage)
		m_iPainterMode = m_painterpage.GetMode();

/*	if (m_fontpage)
	{
		m_fontpage.UpdateData();
		m_sFontName = m_fontpage.m_FontName;
		m_iFontSize = m_fontpage.m_FontSize;
		m_bBold = m_fontpage.m_bBold;
		m_bItalic = m_fontpage.m_bItalic;
		m_bUnderLine = m_fontpage.m_bUnderLine;
		m_FontColor.SetRGB(m_fontpage.m_Color.GetRGB());
	}
*/	
/*	if (m_markerspage)
	{
		m_markerspage.UpdateData();
		m_bFix = m_markerspage.m_fix;
		m_bScroll = m_markerspage.m_scroll;
		m_bMark1 = m_markerspage.m_mark1;
		m_bMark2 = m_markerspage.m_mark2;
		m_bError = m_markerspage.m_error;
		m_iTopLine = m_markerspage.m_top_line;
		m_iBottomLine = m_markerspage.m_bottom_line;
		if (m_iTopLine + m_iBottomLine > m_VisLine)
		{
			AfxMessageBox(IDS_MES_STR);
			return;
		}
	}
*/
}
