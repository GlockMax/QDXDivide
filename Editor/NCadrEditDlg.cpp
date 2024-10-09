#include "stdafx.h"
#include "ResInv.h"
#include "NCadrEditDlg.h"


NCadrEditDlg::NCadrEditDlg(CWnd* pParent) : CDialogEx(IDD_ABOUTBOX, pParent)
{
	pCadrGeom = nullptr;
}

NCadrEditDlg::~NCadrEditDlg()
{
}

INT_PTR NCadrEditDlg::DoModal(const CString& text, const NCadrGeom* pGeom)
{
	if (pGeom == nullptr)
		return IDCANCEL;
	pCadrGeom = pGeom;
	cadrText = text;

	return CDialogEx::DoModal();
}
