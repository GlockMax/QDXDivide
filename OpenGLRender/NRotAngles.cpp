// NRotAngles.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "BOpenGLWndVD.h"
#include "NRotAngles.h"


// NRotAngles dialog

IMPLEMENT_DYNAMIC(NRotAngles, CDialogEx)

NRotAngles::NRotAngles(CWnd* pParent /*=NULL*/)
	: CDialogEx(NRotAngles::IDD, pParent)
	, m_X(90)
	, m_Y(90)
	, m_Z(90)
{
	BufRotCenterVis = false;
}

NRotAngles::~NRotAngles()
{
}

void NRotAngles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROTANG_X, m_X);
	DDX_Text(pDX, IDC_ROTANG_Y, m_Y);
	DDX_Text(pDX, IDC_ROTANG_Z, m_Z);
	DDX_Text(pDX, IDC_ROTANG_Z, m_Z);
}

void NRotAngles::Apply(void)
{
}

BEGIN_MESSAGE_MAP(NRotAngles, CDialogEx)
	ON_BN_CLICKED(IDC_ROTANGCOUNT_X_MINUS, &NRotAngles::OnBnClickedRotangcountXMinus)
	ON_BN_CLICKED(IDC_ROTANGCOUNT_X, &NRotAngles::OnBnClickedRotangcountX)
	ON_BN_CLICKED(IDC_ROTANGCOUNT_Y_MINUS, &NRotAngles::OnBnClickedRotangcountYMinus)
	ON_BN_CLICKED(IDC_ROTANGCOUNT_Y, &NRotAngles::OnBnClickedRotangcountY)
	ON_BN_CLICKED(IDC_ROTANGCOUNT_Z_MINUS, &NRotAngles::OnBnClickedRotangcountZMinus)
	ON_BN_CLICKED(IDC_ROTANGCOUNT_Z, &NRotAngles::OnBnClickedRotangcountZ)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// NRotAngles message handlers


void NRotAngles::OnBnClickedRotangcountXMinus()
{
	UpdateData();
	pActiveWnd->ApplyRotAngle('X', -m_X);
}


void NRotAngles::OnBnClickedRotangcountX()
{
	UpdateData();
	pActiveWnd->ApplyRotAngle('X', m_X);
}


void NRotAngles::OnBnClickedRotangcountYMinus()
{
	UpdateData();
	pActiveWnd->ApplyRotAngle('Y', -m_Y);
}


void NRotAngles::OnBnClickedRotangcountY()
{
	UpdateData();
	pActiveWnd->ApplyRotAngle('Y', m_Y);
}


void NRotAngles::OnBnClickedRotangcountZMinus()
{
	UpdateData();
	pActiveWnd->ApplyRotAngle('Z', -m_Z);
}


void NRotAngles::OnBnClickedRotangcountZ()
{
	UpdateData();
	pActiveWnd->ApplyRotAngle('Z', m_Z);
}


void NRotAngles::OnClose()
{
	BOpenGLWnd::NRotCenter RotCenter;
	pActiveWnd->GetRotCenter(RotCenter);
	RotCenter.Visible = BufRotCenterVis;
	pActiveWnd->SetRotCenter(RotCenter);

	CDialogEx::OnClose();
}


INT_PTR NRotAngles::DoModal(BOpenGLWnd* pWnd)
{
	pActiveWnd = pWnd;
	BOpenGLWnd::NRotCenter RotCenter;
	pActiveWnd->GetRotCenter(RotCenter);
	BufRotCenterVis = RotCenter.Visible;
	RotCenter.Visible = true;
	pActiveWnd->SetRotCenter(RotCenter);

	return CDialogEx::DoModal();
}
