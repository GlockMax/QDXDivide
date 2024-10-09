// NToolsCrTurn.cpp : implementation file
//

#include "stdafx.h"
#include "WM_APP_messages.h"
#include "NToolsCrTurn.h"
#include "NToolTurn.h"
#include ".\ntoolscrturn.h"


// NToolsCrTurn dialog

IMPLEMENT_DYNAMIC(NToolsCrTurn, CPropertyPage)
NToolsCrTurn::NToolsCrTurn(UINT nIDTemplate)
	: CPropertyPage(nIDTemplate)
	, m_name(_T("Tool"))
	, m_Xdisp(0.)
	, m_Zdisp(0.)
	, m_MainAngle(0.)
{
}

NToolsCrTurn::~NToolsCrTurn()
{
}

void NToolsCrTurn::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, m_name_ctrl);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_DISPLACEMENT_X, m_Xdisp);
	DDX_Text(pDX, IDC_DISPLACEMENT_Z, m_Zdisp);
	DDX_Text(pDX, IDC_MANGLE, m_MainAngle);
	DDX_Control(pDX, IDC_DISPLACEMENT_X, m_Xdisp_ctrl);
	DDX_Control(pDX, IDC_DISPLACEMENT_Z, m_Zdisp_ctrl);
	DDX_Control(pDX, IDC_MANGLE, m_MainAngle_ctrl);
}


BEGIN_MESSAGE_MAP(NToolsCrTurn, CPropertyPage)
	ON_BN_CLICKED(IDC_UNIT_INFO, OnBnClickedUnitInfo)
	ON_BN_CLICKED(IDC_PDZ, OnBnClickedPdz)
	ON_BN_CLICKED(IDC_MDZ, OnBnClickedMdz)
	ON_BN_CLICKED(IDC_PDX, OnBnClickedPdx)
	ON_BN_CLICKED(IDC_MDX, OnBnClickedMdx)
	ON_BN_CLICKED(IDC_D0, OnBnClickedD0)
END_MESSAGE_MAP()


// NToolsCrTurn message handlers

BOOL NToolsCrTurn::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NToolsCrTurn::OnOK()
{
	CPropertyPage::OnOK();
}

void NToolsCrTurn::OnCancel()
{

	CPropertyPage::OnCancel();
}
NToolTurn *NToolsCrTurn::CreateTool()
{
	return NULL;
}
void NToolsCrTurn::OnBnClickedUnitInfo()
{
	AfxGetMainWnd()->PostMessage(WM_APP_UNIT_INFO, 0);
}

void NToolsCrTurn::OnBnClickedPdz()
{
	UpdateData(TRUE);
	m_Zdisp = GetRad();
	UpdateData(FALSE);
}

void NToolsCrTurn::OnBnClickedMdz()
{
	UpdateData(TRUE);
	m_Zdisp = - GetRad();
	UpdateData(FALSE);
}

void NToolsCrTurn::OnBnClickedPdx()
{
	UpdateData(TRUE);
	m_Xdisp = GetRad();
	UpdateData(FALSE);
}

void NToolsCrTurn::OnBnClickedMdx()
{
	UpdateData(TRUE);
	m_Xdisp = - GetRad();
	UpdateData(FALSE);
}

void NToolsCrTurn::OnBnClickedD0()
{
	UpdateData(TRUE);
	m_Zdisp = 0.0;
	m_Xdisp = 0.0;
	UpdateData(FALSE);
}
