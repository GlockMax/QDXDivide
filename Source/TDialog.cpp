// TDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TDialog.h"


// TDialog dialog

IMPLEMENT_DYNAMIC(TDialog, CDialog)
TDialog::TDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TDialog::IDD, pParent)
	, m_CellSize(0)
	, m_DebugMode(false)
	, m_Smooth(true)
	, m_DrawFaces(false)
	, m_Step(0)//!!!
	, m_Maxl(0)//!!!
	, m_Dich(0)//!!!
	, m_UseSlicedTree(false)
	, m_SurfelVis(false)
	, m_MarchCubes(true)
	, m_Async(true)
	, m_MultiThread(true)
{
}

TDialog::~TDialog()
{
}

void TDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CellSize);
	DDX_Text(pDX, IDC_DXNUMBER, m_DXNumber);
	DDX_Text(pDX, IDC_EDIT2, m_Step);//!!!
	DDX_Text(pDX, IDC_EDIT3, m_Maxl);//!!!
	DDX_Text(pDX, IDC_EDIT4, m_Dich);//!!!
	DDX_Check(pDX, IDC_CHECK1, m_DebugMode);
	DDX_Check(pDX, IDC_CHECK2, m_Smooth);
	DDX_Check(pDX, IDC_CHECK3, m_DrawFaces);
	DDX_Check(pDX, IDC_CHECK4, m_MultiThread);
	DDX_Check(pDX, IDC_CHECK5, m_UseSlicedTree);
	DDX_Check(pDX, IDC_CHECK_SURFEL, m_SurfelVis);
	DDX_Check(pDX, IDC_CHECK16, m_MarchCubes);
	DDX_Check(pDX, IDC_CHECK17, m_Async);
}


BEGIN_MESSAGE_MAP(TDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// TDialog message handlers

void TDialog::OnBnClickedOk()
{
	UpdateData(true);
	OnOK();
}


BOOL TDialog::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
