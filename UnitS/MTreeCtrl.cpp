#include "stdafx.h"
#include "NCMProject.h"
#include "NColor.h"
#include "NFont.h"
#include "MTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(MTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(MTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(MTreeCtrl)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL MTreeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags & MK_CONTROL)
	{
		ZoomText(zDelta);
	}

	return CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void MTreeCtrl::ZoomText(int Dir)
{
	if (Dir == 0)
		return;
	// from http://support.microsoft.com/kb/85518
	LOGFONT lf;// Used to create the CFont.
	m_Font.GetLogFont(&lf);
	if (Dir > 0 || std::abs(lf.lfHeight) >= 3)
	{
		LONG Delta = Dir > 0 ? 1 : -1;
		lf.lfHeight += lf.lfHeight > 0 ? Delta : -Delta;
	}
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&lf);    // Create the font.

	// Use the font to paint a control.
	SetFont(&m_Font);
	RedrawWindow();
}

void MTreeCtrl::SetSettings()
{
	NColor* pCol;
	if (NCM_PROJECT.Colors.Lookup(GetBackName(), pCol))
	{
		SetBkColor(pCol->GetRGB());
	}

	if (NCM_PROJECT.Colors.Lookup(GetColorName(), pCol))
	{
		SetTextColor(pCol->GetRGB());
		SetInsertMarkColor(pCol->GetRGB());
	}

	LOGFONT lf;
	lf = NCM_PROJECT.Defaults.GetFont(GetFontName(), lf);
	::DeleteObject(m_Font.Detach());
	m_Font.CreateFontIndirect(&lf);
	SetFont(&m_Font);
}


