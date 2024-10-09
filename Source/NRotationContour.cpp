#include "stdafx.h"
#include "NRotationContour.h"

NRotationContour::NRotationContour(CWnd* pParent) : CDialog(IDD, pParent)
{
	StartMessage = false;
	FindStartEnd = FALSE;
	CorrectNormal = FALSE;
}

NRotationContour::~NRotationContour()
{
}


BOOL NRotationContour::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_THISASSEMBLY);
	if (pWnd != nullptr)
		if (!StartMessage)
			pWnd->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void NRotationContour::DoDataExchange(CDataExchange* pDX)
{
	DDX_Radio(pDX, IDC_FINDSTARTEND, FindStartEnd);
	DDX_Radio(pDX, IDC_CORRECTNORMAL, CorrectNormal);

	CDialog::DoDataExchange(pDX);
}
