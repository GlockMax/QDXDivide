#include "stdafx.h"
#include "NFont.h"
#include "NCMProject.h"
#include "PropEditor.h"
#include "PropGridCtrl.h"

IMPLEMENT_DYNAMIC(PropGridProp, CMFCPropertyGridProperty)

BEGIN_MESSAGE_MAP(PropGridCtrl, CMFCPropertyGridCtrl)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

PropGridCtrl::PropGridCtrl()
{
	propEditor = NULL;
}

void PropGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
    CString value;
    value = pProp->GetValue();
	propEditor->Change(int(pProp->GetData()), value);
	propEditor->Put(propEditor->pabstractObject);
}

PropGridCtrl::~PropGridCtrl()
{
}

void PropGridCtrl::PrEdit(PropEditor* edit)
{
	propEditor = edit;
}

PropGridProp::~PropGridProp()
{
}

void PropGridCtrl::SetEnabled(bool Enable)
{
	if((!(GetStyle() & WS_DISABLED)) == Enable)
		return;
	EnableWindow(Enable);
	SetCustomColors(-1, Enable ? RGB(0, 0, 0) : RGB(127, 127, 127), -1, -1, -1, -1, -1);
	RedrawWindow();
}

BOOL PropGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags & MK_CONTROL)
	{
		ZoomText(zDelta);
	}

	return CMFCPropertyGridCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void PropGridCtrl::ZoomText(int Dir)
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

void PropGridCtrl::SetPropListFont()
{
	LOGFONT lf;
	lf = NCM_PROJECT.Defaults.GetFont(_T("Defaults/PaneProperties/Font"), lf);
	::DeleteObject(m_Font.Detach());
	m_Font.CreateFontIndirect(&lf);
	SetFont(&m_Font);
	//else
	//{
	//	::DeleteObject(m_Font.Detach());

	//	LOGFONT lf;
	//	afxGlobalData.fontRegular.GetLogFont(&lf);

	//	NONCLIENTMETRICS info;
	//	info.cbSize = sizeof(info);

	//	afxGlobalData.GetNonClientMetrics(info);

	//	lf.lfHeight = info.lfMenuFont.lfHeight;
	//	lf.lfWeight = info.lfMenuFont.lfWeight;
	//	lf.lfItalic = info.lfMenuFont.lfItalic;

	//	m_Font.CreateFontIndirect(&lf);

	//	SetFont(&m_Font);
	//}
}

