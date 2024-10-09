// QDivideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NObjEnums.h"
#include "WM_APP_messages.h"
#include "QDivideDlg.h"


// QDivideDlg dialog

IMPLEMENT_DYNAMIC(QDivideDlg, CDialog)

QDivideDlg::QDivideDlg(CWnd* pParent, QDivideBase*& pDivide)
	: CDialog(QDivideDlg::IDD, pParent), pQDivide(pDivide)
{
}

QDivideDlg::~QDivideDlg()
{
}

void QDivideDlg::SetQDivide(QDivideBase*& pDivide)
{
	pQDivide = pDivide;
}

void QDivideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void QDivideDlg::OnCancel()
{
	DestroyWindow();
}

void QDivideDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete pQDivide;
	pQDivide = nullptr;
	delete this;
}

BEGIN_MESSAGE_MAP(QDivideDlg, CDialog)
	ON_BN_CLICKED(IDC_REVERT, &QDivideDlg::OnBnClickedRevert)
	ON_BN_CLICKED(IDC_DEL1, &QDivideDlg::OnBnClickedDel1)
	ON_BN_CLICKED(IDC_DEL2, &QDivideDlg::OnBnClickedDel2)
END_MESSAGE_MAP()


// QDivideDlg message handlers

void QDivideDlg::OnBnClickedRevert()
{
	pQDivide->Revert();
	AfxGetMainWnd()->SendMessage(WM_APP_REGENERATE, N_OGL, OG_STOCKS);
}

void QDivideDlg::OnBnClickedDel1()
{
	// keep object which was picking
	pQDivide->ChangeCuttingMode(true);
}

void QDivideDlg::OnBnClickedDel2()
{
	// delete object which was picking
	pQDivide->ChangeCuttingMode(false);
}