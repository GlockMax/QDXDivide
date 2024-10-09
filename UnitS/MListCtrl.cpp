#include "stdafx.h"
#include "NCMProject.h"
#include "NColor.h"
#include "NFont.h"
#include "MListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(MListCtrl, CMFCListCtrl)

BEGIN_MESSAGE_MAP(MListCtrl, CMFCListCtrl)
	//{{AFX_MSG_MAP(MListCtrl)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

int MListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetExtendedStyle(LVS_EX_LABELTIP | LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
	InitHeader();
	return 0;
}

BOOL MListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags & MK_CONTROL)
	{
		ZoomText(zDelta);
	}

	return CMFCListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void MListCtrl::ZoomText(int Dir)
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

void MListCtrl::InitHeader()
{
	// Initialize header control:
	m_headerCtrl.SubclassDlgItem(0, this);
}

void MListCtrl::SetSettings()
{
	NColor* pCol;
	if (NCM_PROJECT.Colors.Lookup(GetBackName(), pCol))
	{
		SetBkColor(pCol->GetRGB());
		SetTextBkColor(pCol->GetRGB());
		m_headerCtrl.SetBkColor(pCol->GetRGB());
	}

	if (NCM_PROJECT.Colors.Lookup(GetColorName(), pCol))
	{
		SetTextColor(pCol->GetRGB());
		m_headerCtrl.SetTextColor(pCol->GetRGB());
	}

	LOGFONT lf;
	lf = NCM_PROJECT.Defaults.GetFont(GetFontName(), lf);
	::DeleteObject(m_Font.Detach());
	m_Font.CreateFontIndirect(&lf);
	SetFont(&m_Font);
}

bool MListCtrl::CopyToClipboard(const CString& str)
{
	// Open the clipboard, and empty it. 
	if (!OpenClipboard())
		return false;
	EmptyClipboard();
	auto CodePage = GetThreadLocale();
	HGLOBAL Data = GlobalAlloc(GMEM_MOVEABLE + GMEM_DDESHARE, 4);  // Назначить кодовую страницу для буфера обмена
	char* DataPtr = (char*)GlobalLock(Data);
	memcpy(DataPtr, (const VOID*)&CodePage, 4);
	GlobalUnlock(Data);
	SetClipboardData(CF_LOCALE, Data);
	// Allocate a global memory object for the text. 
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, str.GetLength() + 1);

	if (hglbCopy == NULL)
	{
		CloseClipboard();
		return false;
	}
	// Lock the handle and copy the text to the buffer. 
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
	memcpy_s(lptstrCopy, str.GetLength(), str, str.GetLength());
	lptstrCopy[str.GetLength()] = '\x0';    // null character 
	GlobalUnlock(hglbCopy);
	// Place the handle on the clipboard. 
	SetClipboardData(CF_TEXT, hglbCopy);

	CloseClipboard();
	return true;
}

bool MListCtrl::CopySelectionToClipboard()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
		return true;
	CString str;
	while (pos)
	{
		int nItem = GetNextSelectedItem(pos);
		for (int k = 0; k < GetHeaderCtrl().GetItemCount(); ++k)
		{
			CString part = GetItemText(nItem, k);
			str += part + _T("  ");
		}
		str +=_T("\r\n");
	}
	if (str.IsEmpty())
		return true;
	return CopyToClipboard(str);
}

void MListCtrl::OnEditCopy()
{
	CopySelectionToClipboard();
}

void MListCtrl::OnEditSelectAll()
{
	int Count = GetItemCount();
	for (int i = 0; i < Count; ++i)
	{
		SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

BEGIN_MESSAGE_MAP(MHeaderCtrl, CMFCHeaderCtrl)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(MHeaderCtrl, CMFCHeaderCtrl)

MHeaderCtrl::MHeaderCtrl()
{
	BkColor = RGB(255, 255, 255);
	TextColor = RGB(0, 0, 0);
}

void MHeaderCtrl::SetBkColor(COLORREF cr)
{
	BkColor = cr;
}

void MHeaderCtrl::SetTextColor(COLORREF cr)
{
	TextColor = cr;
}

void MHeaderCtrl::OnFillBackground(CDC* pDC)
{
	CRect rect;
	MHeaderCtrl::GetClientRect(rect);
	pDC->FillSolidRect(rect, BkColor);
	CMFCHeaderCtrl::OnFillBackground(pDC);
}

void MHeaderCtrl::OnDrawItem(CDC* pDC, int iItem, CRect rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	pDC->SetTextColor(TextColor);
	CMFCHeaderCtrl::OnDrawItem(pDC, iItem, rect, bIsPressed, bIsHighlighted);
}

