#include "stdafx.h"
#include "NCMDoc.h"
#include "NTextCompDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(NTextCompDlg, CDialogEx)

BEGIN_MESSAGE_MAP(NTextCompDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_COMPARE_LIST, OnTcnSelChangeList)
	ON_BN_CLICKED(IDC_COMPARE_LOAD_FILE, OnLoadFile)
END_MESSAGE_MAP()

NTextCompDlg::NTextCompDlg(class CNCMDoc* pDoc, CWnd* pParent /*=NULL*/) : CDialogEx(NTextCompDlg::IDD, pParent), doc_(*pDoc)
{
}

NTextCompDlg::~NTextCompDlg()
{
}

void NTextCompDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPARE_LIST, listCtrl_);
}

BOOL NTextCompDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	listCtrl_.ResetContent();
	for (int i = 0, k = 0; i < doc_.NGetProgramList()->GetSize(); ++i)
	{
		if (i != selProgInd_)
		{
			CString name = doc_.NGetProgramList()->GetKey(i);
			int pos = name.ReverseFind('\\');
			listCtrl_.AddString(name.Mid(pos + 1));
			listCtrl_.SetItemData(k, i);
			k++;
		}
	}
	listCtrl_.SetCurSel(-1);
	AdjustListWidth();

	POINT p;
	GetCursorPos(&p);
	CRect myRect;
	GetWindowRect(myRect);
	MoveWindow(p.x, p.y, myRect.Width(), myRect.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NTextCompDlg::AdjustListWidth()
{
	// Find the longest string in the list box.
	CString str;
	CSize sz;
	int dx = 0;
	TEXTMETRIC tm;
	CDC* pDC = listCtrl_.GetDC();
	CFont* pFont = listCtrl_.GetFont();

	// Select the listbox font, save the old font
	CFont* pOldFont = pDC->SelectObject(pFont);
	// Get the text metrics for avg char width
	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < listCtrl_.GetCount(); i++)
	{
		listCtrl_.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		// Add the avg width to prevent clipping
		sz.cx += tm.tmAveCharWidth;

		if (sz.cx > dx)
			dx = sz.cx;
	}
	// Select the old font back into the DC
	pDC->SelectObject(pOldFont);
	listCtrl_.ReleaseDC(pDC);

	// Set the horizontal extent so every character of all strings
	// can be scrolled to.
	listCtrl_.SetHorizontalExtent(dx);
}

INT_PTR NTextCompDlg::DoModal(int selProgInd)
{
	selProgInd_ = selProgInd;
	fileSelected_ = false;;
	int progInd_ = -1;

	return CDialogEx::DoModal();
}

void NTextCompDlg::OnTcnSelChangeList()
{
	progInd_ = int(listCtrl_.GetItemData(listCtrl_.GetCurSel()));
	OnOK();
}

void NTextCompDlg::OnLoadFile()
{
	name_.Empty();
	CFileDialog fileDialog(TRUE, _T(""), _T(""), OFN_READONLY, _T(""));
	if (fileDialog.DoModal() != IDOK)
		return;
	name_ = fileDialog.GetPathName();
	fileSelected_ = true;
	OnOK();
}
