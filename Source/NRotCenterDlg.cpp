// ..\source\NRotCenterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NRotCenterDlg.h"
#include "MainFrm.h"
#include "BOpenGLWnd.h"
#include "afxdialogex.h"


// NRotCenterDlg dialog

IMPLEMENT_DYNAMIC(NRotCenterDlg, CDialogEx)

NRotCenterDlg::NRotCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NRotCenterDlg::IDD, pParent)
{
	m_RotCenRSize = 5.;
	m_RotCentrX = 0.;
	m_RotCentrY = 0.;
	m_RotCentrZ = 0.;
	NTiParams &Par = NCM_PROJECT.Defaults;
	m_RotCentrActive = Par.GetBool("Defaults/View/RotCenter@Fixed", false);

	m_Color.SetColor(RGB(127, 255, 0));
}

NRotCenterDlg::~NRotCenterDlg()
{
}

void NRotCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROTCENRSIZE, m_RotCenRSize);
	DDX_Text(pDX, IDC_ROTCENTR_X, m_RotCentrX);
	DDX_Text(pDX, IDC_ROTCENTR_Y, m_RotCentrY);
	DDX_Text(pDX, IDC_ROTCENTR_Z, m_RotCentrZ);
	DDX_Check(pDX, IDC_ROTCENTRACTIVE, m_RotCentrActive);
	DDX_Control(pDX, IDC_ROTCENTRCOLOR, m_Color);
}


BEGIN_MESSAGE_MAP(NRotCenterDlg, CDialogEx)
ON_COMMAND(IDC_ROTCENTRACTIVE, &NRotCenterDlg::Apply)
ON_EN_CHANGE(IDC_ROTCENRSIZE, &NRotCenterDlg::Apply)
ON_EN_CHANGE(IDC_ROTCENTR_X, &NRotCenterDlg::Apply)
ON_EN_CHANGE(IDC_ROTCENTR_Y, &NRotCenterDlg::Apply)
ON_EN_CHANGE(IDC_ROTCENTR_Z, &NRotCenterDlg::Apply)
ON_BN_CLICKED(IDC_ROTCENTRCOLOR, &NRotCenterDlg::Apply)
ON_WM_SHOWWINDOW()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// NRotCenterDlg message handlers


BOOL NRotCenterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NRotCenterDlg::Apply(void)
{
	UpdateData();
	BOpenGLWnd::NRotCenter RotCenter;
	RotCenter.Color.SetRGB(m_Color.GetColor());
	RotCenter.Rad = m_RotCenRSize;
	RotCenter.RotBasePoint.Set(m_RotCentrX, m_RotCentrY, m_RotCentrZ, m_RotCentrActive ? 1. : -1.);
	RotCenter.Visible = true;
	((CMainFrame*)AfxGetMainWnd())->SetRotCenter(RotCenter);
}


void NRotCenterDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
		Apply();
	else
	{
		BOpenGLWnd::NRotCenter RotCenter;
		RotCenter.Color.SetRGB(m_Color.GetColor());
		RotCenter.Rad = m_RotCenRSize;
		RotCenter.RotBasePoint.Set(m_RotCentrX, m_RotCentrY, m_RotCentrZ, m_RotCentrActive ? 1. : -1.);
		RotCenter.Visible = false;
		((CMainFrame*)AfxGetMainWnd())->SetRotCenter(RotCenter);
	}
}


void NRotCenterDlg::OnClose()
{
	BOpenGLWnd::NRotCenter RotCenter;
	RotCenter.Color.SetRGB(m_Color.GetColor());
	RotCenter.Rad = m_RotCenRSize;
	RotCenter.RotBasePoint.Set(m_RotCentrX, m_RotCentrY, m_RotCentrZ, m_RotCentrActive ? 1. : -1.);
	RotCenter.Visible = false;
	((CMainFrame*)AfxGetMainWnd())->SetRotCenter(RotCenter);

	CDialogEx::OnClose();
}
