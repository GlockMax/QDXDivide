// GAnimationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ganimationdlg.h"
#include "GAnimateCadr.h"
#include "..\common\ganimationdlg.h"


// GAnimationDlg dialog
//extern class GAnimateCadr;

IMPLEMENT_DYNAMIC(GAnimationDlg, CDialog)
GAnimationDlg::GAnimationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GAnimationDlg::IDD, pParent)
	, StepLength(0)
{
	Updated = false;
}

GAnimationDlg::~GAnimationDlg()
{
}

void GAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_ANIM_SLD, StepLength);
//	DDV_MinMaxSlider(pDX, StepLength, 1, 1000);
}


BEGIN_MESSAGE_MAP(GAnimationDlg, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ANIM_SLD, OnNMReleasedcaptureAnimSld)
END_MESSAGE_MAP()


// GAnimationDlg message handlers

void GAnimationDlg::OnNMReleasedcaptureAnimSld(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	GAnimateCadr::SetLengthStep(StepLength/1. + 1.);
	Updated = true;
	*pResult = 0;
}

bool GAnimationDlg::IsUpdated(void)
{
	bool b = Updated;
	if( Updated )
		Updated = false;
	return b;
}
