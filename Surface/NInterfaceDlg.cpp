// NInterfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NInterfaceDlg.h"
#include "NCEnums.h"
#include "surEnums.h"
#include "MBSPForest.h"
#include "..\common\ninterfacedlg.h"
#include "Gl\Glu.h"
#include "..\bsp_model\TestDlg.h"

// NInterfaceDlg dialog

IMPLEMENT_DYNAMIC(NInterfaceDlg, CDialog)
NInterfaceDlg::NInterfaceDlg(CWnd* pParent)
	: CDialog(NInterfaceDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_nID = NInterfaceDlg::IDD;

	m_bBuildState = false;
	m_bBuildMesh = true;
	m_bBuildSur = true;
	m_bViewMesh = true;
	m_bViewSur = false;
	m_bViewNum = false;
	m_bViewPoint = false;
	m_bViewNorm = false;
	m_bSound = false;
}

NInterfaceDlg::~NInterfaceDlg()
{
}

void NInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(NInterfaceDlg, CDialog)
	ON_BN_CLICKED(IDC_BUILD_TREE, OnBnClickedBuildMesh)
	ON_BN_CLICKED(IDC_BUILD_SUR, OnBnClickedBuildSur)
	ON_BN_CLICKED(IDC_TYPE_BSP, OnBnClickedTypeMesh)
	ON_BN_CLICKED(IDC_TYPE_SUR, OnBnClickedTypeSur)
	ON_BN_CLICKED(IDC_TYPE_PL, OnBnClickedTypePl)
	ON_BN_CLICKED(IDC_TYPE_TR, OnBnClickedTypeTr)
	ON_BN_CLICKED(IDC_BREAK, OnBnClickedBreak)
	ON_BN_CLICKED(IDC_BUILD, OnBnClickedBuild)
	ON_BN_CLICKED(IDC_SOUND, OnBnClickedSound)
	ON_BN_CLICKED(IDC_NUM, OnBnClickedNum)
	ON_BN_CLICKED(IDC_NORM, OnBnClickedNorm)
END_MESSAGE_MAP()


// NInterfaceDlg message handlers

BOOL NInterfaceDlg::Create(void)
{
	return CDialog::Create(m_nID, m_pParent);
}

BOOL NInterfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	NUpdate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NInterfaceDlg::OnBnClickedBreak()
{	
	m_bBuildState = false;
	NUpdate();
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_BREAK);
}

void NInterfaceDlg::OnBnClickedBuild()
{
	m_bBuildState = true;
	NUpdate();
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_REBUILD);
}

void NInterfaceDlg::OnOK()
{
	CDialog::OnOK();
}
void NInterfaceDlg::OnCancel()
{
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_CONTROLDLG_DONE);
	DestroyWindow();
}

void NInterfaceDlg::OnBnClickedBuildMesh()
{
	m_bBuildMesh = !m_bBuildMesh;
}

void NInterfaceDlg::OnBnClickedBuildSur()
{
	m_bBuildSur = !m_bBuildSur;
}
void NInterfaceDlg::OnBnClickedTypeMesh()
{
	m_bViewMesh = !m_bViewMesh;
	NUpdate();
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_VIEW);
}
void NInterfaceDlg::OnBnClickedTypeSur()
{
	m_bViewSur = !m_bViewSur;
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_VIEW);
}
void NInterfaceDlg::OnBnClickedTypePl()
{
	m_bViewPoint = false;
	NUpdate();
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_VIEW);
}

void NInterfaceDlg::OnBnClickedTypeTr()
{
	m_bViewPoint = true;
	NUpdate();
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_VIEW);
}
void NInterfaceDlg::OnBnClickedNorm()
{
	m_bViewNorm = !m_bViewNorm;
	NUpdate();
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_VIEW);
}

LPARAM NInterfaceDlg::GetJobType()
{
	LPARAM res;

	if (m_bBuildMesh)
		res = bMESH;
	if (m_bBuildSur)
		res = res | bSURFACE; 

	return res;
}

LPARAM NInterfaceDlg::GetViewType(void)
{
	LPARAM res;
	
	if (m_bViewMesh)
		res = bVIEW_MESH;
	if (m_bViewSur)
		res = res | bVIEW_SUR;
	if (m_bViewNum)
		res = res | bVIEW_NUM;
	if (m_bViewPoint)
		res = res | bVIEW_POINT;
	if (m_bViewNorm)
		res = res | bVIEW_NORM;

	return res;
}

bool NInterfaceDlg::IsSound(void)
{
	return m_bSound;
}

void NInterfaceDlg::OnBnClickedSound()
{
	m_bSound = !m_bSound;
}

void NInterfaceDlg::OnBnClickedNum()
{
	m_bViewNum = !m_bViewNum;
	((CView*) m_pParent)->GetDocument()->UpdateAllViews(NULL, USER_VIEW);
}

void NInterfaceDlg::NUpdate(void)
{
	GetDlgItem(IDC_BREAK)->EnableWindow(m_bBuildState);
	GetDlgItem(IDC_BUILD)->EnableWindow(!m_bBuildState);

	if (m_bBuildState)
	{
		GetDlgItem(IDC_BUILD_SUR)->EnableWindow(m_bBuildSur);
		GetDlgItem(IDC_BUILD_MESH)->EnableWindow(m_bBuildMesh);
	}
	else
	{
		GetDlgItem(IDC_BUILD_SUR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUILD_MESH)->EnableWindow(TRUE);	
	}

	GetDlgItem(IDC_TYPE_SUR)->EnableWindow(m_bBuildSur);
	GetDlgItem(IDC_TYPE_PL)->EnableWindow(m_bBuildMesh && m_bViewMesh);
	GetDlgItem(IDC_TYPE_TR)->EnableWindow(m_bBuildMesh && m_bViewMesh);
	GetDlgItem(IDC_NORMALS)->EnableWindow(m_bBuildMesh && m_bViewMesh);

	CheckDlgButton(IDC_BUILD_SUR, m_bBuildSur ? 1: 0);
	CheckDlgButton(IDC_BUILD_MESH, m_bBuildMesh ? 1: 0);
	CheckDlgButton(IDC_TYPE_SUR, m_bViewSur ? 1: 0);
	CheckDlgButton(IDC_TYPE_MESH, m_bViewMesh ? 1: 0);
	CheckRadioButton(IDC_TYPE_PL, IDC_TYPE_TR, 
		!m_bViewPoint ? IDC_TYPE_PL: IDC_TYPE_TR);
	CheckDlgButton(IDC_NUM, m_bViewNum ? 1: 0);

}

void NInterfaceDlg::SetInitSate(bool VolumeOn)
{
	m_bBuildState = VolumeOn;
}

