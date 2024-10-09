// NStockSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "constdef.h"
#include "math.h"
#include "WM_APP_Messages.h"
#include "nstocksectiondlg.h"

const COLORREF TranspColor = RGB(0,255,0);
// NStockSectionDlg dialog

IMPLEMENT_DYNAMIC(NStockSectionDlg, CDialog)
NStockSectionDlg::NStockSectionDlg(CWnd* pParent, NStockSection *pSect)
	: CDialog(NStockSectionDlg::IDD, pParent)
	, m_SldValue(0)
	, m_Dist(0.)
	, m_Angle(0.)
	, m_VStep((MM_INCH == M_MM) ? 1. : 0.03)
{
	pSection = pSect;
}

NStockSectionDlg::~NStockSectionDlg()
{
}

void NStockSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SldCtrl);
	DDX_Slider(pDX, IDC_SLIDER1, m_SldValue);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Control(pDX, IDC_P_BUTTON, m_PickButton);
	DDX_Text(pDX, IDC_DIST, m_Dist);
	DDX_Text(pDX, IDC_VSTEP2, m_Angle);
	DDX_Text(pDX, IDC_VSTEP, m_VStep);
	DDX_Control(pDX, IDC_SECT_FILL, m_FillSect);
	DDX_Control(pDX, IDC_SECT_SHOW, m_CutAll);
}


BEGIN_MESSAGE_MAP(NStockSectionDlg, CDialog)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_P_BUTTON, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_UP_BUT, OnBnClickedUpBut)
	ON_BN_CLICKED(IDC_DW_BUT, OnBnClickedDwBut)
ON_EN_KILLFOCUS(IDC_DIST, OnEnKillfocusDist)
ON_BN_CLICKED(IDC_SECT_FILL, OnBnClickedSectFill)
ON_BN_CLICKED(IDC_SECT_SHOW, OnBnClickedCutAll)
ON_EN_KILLFOCUS(IDC_VSTEP2, OnEnKillfocusVstep2)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// NStockSectionDlg message handlers

void NStockSectionDlg::OnCancel()
{
	pSection->Deactivate();
	InvalidateView();

	CDialog::OnCancel();
}

void NStockSectionDlg::OnOK()
{
	UpdateData();
	DistChanged();
	InvalidateView();
}

void NStockSectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	double v = m_SldValue;
	UpdateData();
	if(v != m_SldValue)
	{
		SldValueChanged();
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void NStockSectionDlg::OnCbnSelchangeCombo1()
{
	m_Angle = 0.;
	pSection->SetPlane(m_ComboBox.GetCurSel());

	ResetSlider();

	InvalidateView();
}

BOOL NStockSectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	bmp.DeleteObject();
	COLORMAP Map;
	Map.from = TranspColor;
	Map.to = GetSysColor(COLOR_3DFACE);
	bmp.LoadMappedBitmap(IDB_DISP_OFF, 0, &Map, 1);
	BITMAP BitMap;
	bmp.GetBitmap(&BitMap);
	CRect rect;
	m_PickButton.GetWindowRect(rect);
	rect.right = rect.left + BitMap.bmWidth;
	rect.bottom = rect.top + BitMap.bmWidth;
	ScreenToClient(&rect);
	m_PickButton.MoveWindow(rect);
	m_PickButton.SetBitmap(HBITMAP(bmp));

	m_ComboBox.SetCurSel(1);
	pSection->SetPlane(m_ComboBox.GetCurSel());
	m_FillSect.SetCheck(pSection->FillSection);
	m_CutAll.SetCheck(!pSection->CutAll);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NStockSectionDlg::OnBnClickedButton1()
{
	pSection->WaitPoint = (m_PickButton.GetCheck() == BST_CHECKED);

	bmp.DeleteObject();
	UINT res = pSection->WaitPoint ? IDB_DISP_ON : IDB_DISP_OFF;
	COLORMAP Map;
	Map.from = TranspColor;
	Map.to = GetSysColor(COLOR_3DFACE);
	bmp.LoadMappedBitmap(res, 0, &Map, 1);
	m_PickButton.SetBitmap(HBITMAP(bmp));

}

void NStockSectionDlg::InvalidateView(void)
{
	m_pParentWnd->SendMessage(WM_APP_UPDATE_PANES, N_OGL);
}

int NStockSectionDlg::GetPlaneNum(void)
{
	return m_ComboBox.GetCurSel();
}

void NStockSectionDlg::ResetSlider(void)
{
	m_Dist = pSection->P * pSection->N;
	m_Dist = int(m_Dist * 1000) / 1000.;
	double t = (m_Dist - pSection->Vmin) / (pSection->Vmax - pSection->Vmin);
	m_SldValue = int(t * 100.);
	UpdateData(0);
}

void NStockSectionDlg::OnBnClickedUpBut()
{
	UpdateData();
	m_Dist = min(m_Dist+m_VStep, pSection->Vmax);
	DistChanged();
}

void NStockSectionDlg::OnBnClickedDwBut()
{
	UpdateData();
	m_Dist = max(m_Dist-m_VStep, pSection->Vmin);
	DistChanged();
}


void NStockSectionDlg::OnEnKillfocusDist()
{
	UpdateData();
	DistChanged();
}

void NStockSectionDlg::CalcSldValue(void)
{
	m_SldValue = int((m_Dist-pSection->Vmin)*100./(pSection->Vmax-pSection->Vmin));
}

void NStockSectionDlg::SldValueChanged(void)
{
	double v = m_SldValue/100.;
	v = pSection->Vmin * (1. - v) + pSection->Vmax * v;
	pSection->P = pSection->P - pSection->N * ((pSection->P*pSection->N) - v);
	InvalidateView();
	m_Dist = v;
	m_Dist = int(m_Dist * 1000)/1000.;
	UpdateData(0);
}

void NStockSectionDlg::DistChanged(void)
{
	m_Dist = int(m_Dist * 1000)/1000.;
	CalcSldValue();
	pSection->P = pSection->P - pSection->N * ((pSection->P*pSection->N) - m_Dist);
	InvalidateView();
	UpdateData(0);
}

void NStockSectionDlg::OnBnClickedSectFill()
{
	pSection->FillSection = (m_FillSect.GetCheck() == BST_CHECKED);
	InvalidateView();
}

void NStockSectionDlg::OnBnClickedCutAll()
{
	pSection->CutAll = (m_CutAll.GetCheck() != BST_CHECKED);
	InvalidateView();
}

void NStockSectionDlg::OnEnKillfocusVstep2()
{// Angle changed
	AngleChanged();
}

void NStockSectionDlg::AngleChanged()
{
	double OldAngle = m_Angle;
	UpdateData();
	BMatr rot;
	switch(m_ComboBox.GetCurSel())
	{
	case 0://XY
		rot.RotX(BPoint(0,0,0,1),m_Angle - OldAngle);
		break;
	case 1://ZX
		rot.RotZ(BPoint(0,0,0,1),m_Angle - OldAngle);
		break;
	case 2://YZ
		rot.RotY(BPoint(0,0,0,1),-m_Angle + OldAngle);
		break;
	case 3://Screen
		break;
	}
	pSection->N = pSection->N * rot;
	pSection->CalcVMinMax();
	ResetSlider();
	InvalidateView();
	UpdateData(0);
}

void NStockSectionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_SldCtrl.SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}
