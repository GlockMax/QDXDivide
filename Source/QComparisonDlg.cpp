// QComparisonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BConst.h"
#include "qcomparisondlg.h"
#include "QComparison.h"
#include "MBSPForest.h"
#include "WM_APP_messages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// QComparisonDlg dialog

IMPLEMENT_DYNAMIC(QComparisonDlg, CDialog)
QComparisonDlg::QComparisonDlg(CWnd* pParent)
	: CDialog(IDD_QCOMPARISON, pParent)
{
	k_Color = 8;
	m_ColorCtrl = new QColorButton [k_Color];
	clrInit = RGB(128,128,255);//цвет по умолчанию при выборе цвета	
	m_Lim = new double [k_Color - 1];//полей ввода на 1 меньше чем цветов
	SetDefaultLim();
	MaxNumber = 1;
}

QComparisonDlg::~QComparisonDlg()
{
	delete [] m_ColorCtrl;
	delete [] m_Lim;
}

void QComparisonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LIM01, m_Lim[0]);
	DDX_Text(pDX, IDC_LIM02, m_Lim[1]);
	DDX_Text(pDX, IDC_LIM03, m_Lim[2]);
	DDX_Text(pDX, IDC_LIM04, m_Lim[3]);
	DDX_Text(pDX, IDC_LIM05, m_Lim[4]);
	DDX_Text(pDX, IDC_LIM06, m_Lim[5]);
	DDX_Text(pDX, IDC_LIM07, m_Lim[6]);

	DDX_Text(pDX, IDC_MAXNUMBER, MaxNumber);
	DDV_MinMaxInt(pDX, MaxNumber, 1, 50);

	DDX_Control(pDX, IDC_MAXNUMBER, MaxNumberCtrl);
	DDX_Control(pDX, IDC_COLOR01, m_ColorCtrl[0]);
	DDX_Control(pDX, IDC_COLOR02, m_ColorCtrl[1]);
	DDX_Control(pDX, IDC_COLOR03, m_ColorCtrl[2]);
	DDX_Control(pDX, IDC_COLOR04, m_ColorCtrl[3]);
	DDX_Control(pDX, IDC_COLOR05, m_ColorCtrl[4]);
	DDX_Control(pDX, IDC_COLOR06, m_ColorCtrl[5]);
	DDX_Control(pDX, IDC_COLOR07, m_ColorCtrl[6]);
	DDX_Control(pDX, IDC_COLOR08, m_ColorCtrl[7]);

	DDX_Text(pDX, IDC_PARTANDSTOCKDISTANCE, m_Dist);
	DDX_Text(pDX, IDC_LINE_NUMBER_STEP2, Cadr);
	DDX_Text(pDX, IDC_PROGRAM_NAME, ProgName);
}

BEGIN_MESSAGE_MAP(QComparisonDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_COLOR01, OnBnClickedColor01)
	ON_BN_CLICKED(IDC_COLOR02, OnBnClickedColor02)
	ON_BN_CLICKED(IDC_COLOR03, OnBnClickedColor03)
	ON_BN_CLICKED(IDC_COLOR04, OnBnClickedColor04)
	ON_BN_CLICKED(IDC_COLOR05, OnBnClickedColor05)
	ON_BN_CLICKED(IDC_COLOR06, OnBnClickedColor06)
	ON_BN_CLICKED(IDC_COLOR07, OnBnClickedColor07)
	ON_BN_CLICKED(IDC_COLOR08, OnBnClickedColor08)
	ON_BN_CLICKED(IDC_DEFAULT_COLOR, OnBnClickedDefaultColor)
	ON_BN_CLICKED(IDC_DEFAULT_LIM, OnBnClickedDefaultLim)
	ON_BN_CLICKED(IDC_GOTO_LINE, OnBnClickedGotoLine)
    ON_MESSAGE(WM_APP_COMPARERDY, OnDataReady)
    ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void QComparisonDlg::OnBnClickedApply()
{
    while(pParComp->IsActiveThread())
        pParComp->TerminateThread();
	double OldMaxDist = pParComp->GetMaxDist();
	int OldMaxNumber = pParComp->GetMaxNumber();
	UpdateData();
	/*Если границы диапазонов идут не в возрастающей последовательности*/
	for(unsigned int i = 0; i < k_Color - 2; i++)
	{
		if(m_Lim[i] >= m_Lim[i+1])
		{
			AfxMessageBox(IDS_WRONG_DIAP);
			return;
		}
	}
	StartProgressBar();
	pParComp->Verification(OldMaxDist < pParComp->GetMaxDist(), OldMaxNumber != pParComp->GetMaxNumber());
	GetParent()->Invalidate();
}
LRESULT QComparisonDlg::OnDataReady(WPARAM wParam, LPARAM lParam)
{
    pParComp->VeriFinish();
    KillTimer(ProgTimer);
    //Добавить перерисовку рабочего окна!!!
    GetDlgItem(IDC_CMPPROGRESS)->ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL);
    return 0;
}
void QComparisonDlg::OnCancel()
{
    while(pParComp->IsActiveThread())
        pParComp->TerminateThread();
//	OnBnClickedDefaultLim();
	for(unsigned int ix = 0; ix < k_Color - 1; ++ix)
		pParComp->Lim_tmp[ix] = m_Lim[ix];
//	OnBnClickedDefaultColor();
	for(unsigned int i = 0; i < k_Color; i++)
		pParComp->Color_tmp[i] = pParComp->def_Color[i];
	pParComp->DeActivate();

	MFOREST.Compress(true);

	CDialog::OnCancel();
}
void QComparisonDlg::SetParComp(QComparison * pComp)
{
	pParComp = pComp;
}
void QComparisonDlg::UpdateColorButton(COLORREF NewColor, QColorButton &m_ColorCtrl)
{
	m_ColorCtrl.SetColor(NewColor);
}
BOOL QComparisonDlg::OnInitDialog()
{
    changed = false;
	CDialog::OnInitDialog();
	/*задаем начальные цвета кнопок*/
	for(unsigned int i = 0; i < k_Color; i++)
		UpdateColorButton(pParComp->def_Color[i], m_ColorCtrl[i]);
	ClearDistance();
	GetDlgItem(IDC_GOTO_LINE)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void QComparisonDlg::OnBnClickedColor01()
{
    clrInit=m_ColorCtrl[0].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[0]);
	UpdateColorButton(NewColor, m_ColorCtrl[7]);
}
void QComparisonDlg::OnBnClickedColor02()
{
    clrInit=m_ColorCtrl[1].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[1]);
}

void QComparisonDlg::OnBnClickedColor03()
{
    clrInit=m_ColorCtrl[2].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[2]);
}

void QComparisonDlg::OnBnClickedColor04()
{
    clrInit=m_ColorCtrl[3].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[3]);
}

void QComparisonDlg::OnBnClickedColor05()
{
    clrInit=m_ColorCtrl[4].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[4]);
}

void QComparisonDlg::OnBnClickedColor06()
{
    clrInit=m_ColorCtrl[5].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[5]);
}

void QComparisonDlg::OnBnClickedColor07()
{
    clrInit=m_ColorCtrl[6].GetColor();
	CColorDialog ColorDialog(clrInit, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	COLORREF NewColor = ColorDialog.GetColor();
	UpdateColorButton(NewColor,m_ColorCtrl[6]);
}

void QComparisonDlg::OnBnClickedColor08()
{
 //   clrInit=m_ColorCtrl[7].GetColor();
	//CColorDialog ColorDialog(clrInit, 0, this);
	//ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	//if(ColorDialog.DoModal() != IDOK)
	//	return;
	//COLORREF NewColor = ColorDialog.GetColor();
	//UpdateColorButton(NewColor,m_ColorCtrl[7]);
}

void QComparisonDlg::OnBnClickedDefaultColor()
{
	for(unsigned int i = 0; i < k_Color; i++)
		UpdateColorButton(pParComp->def_Color[i], m_ColorCtrl[i]);
}
void QComparisonDlg::OnBnClickedDefaultLim()
{
	SetDefaultLim();
	UpdateData(false);
}
void QComparisonDlg::OnBnClickedGotoLine()
{
	pParComp->GotoCurrentCadr(CadrID);
}
void QComparisonDlg::SetDefaultLim()
{
	m_Lim[0] = -2.0;
	m_Lim[1] = -0.5;
	m_Lim[2] = -0.1;
	m_Lim[3] = 0.01;
	m_Lim[4] = 0.5;
	m_Lim[5] = 1.0;
	m_Lim[6] = 5.0;
}
void QComparisonDlg::OnTimer(UINT_PTR nIDEvent)
{
     CProgressCtrl *a = (CProgressCtrl *)GetDlgItem(IDC_CMPPROGRESS);
    a->SetPos(pParComp->progress);
    CDialog::OnTimer(nIDEvent);
}

void QComparisonDlg::StartProgressBar()
{
	CProgressCtrl *a = (CProgressCtrl *)GetDlgItem(IDC_CMPPROGRESS);
	a->SetPos(0);
	ProgTimer = SetTimer(1, 500, NULL);
	GetDlgItem(IDC_CMPPROGRESS)->ShowWindow(SW_SHOW);
}

void QComparisonDlg::OnOK()
{
//	CDialog::OnOK();
}

void QComparisonDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	MaxNumberCtrl.EnableWindow(pParComp->QRender != nullptr);
	UpdateData(FALSE);
}

void QComparisonDlg::SetDistance(const BPoint& P, double D)
{
	m_Dist.Format((MM_INCH == M_MM) ? IDS_DIST_XYZ_MM : IDS_DIST_XYZ_INCH, D, P.GetX(), P.GetY(), P.GetZ());
	UpdateData(FALSE);
}

void QComparisonDlg::ClearDistance()
{
	m_Dist.Empty();
	ProgName.Empty();
	Cadr.Empty();
	CadrID = cadrID(-1);
	UpdateData(FALSE);
}
