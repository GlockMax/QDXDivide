// NStockCrPipDial.cpp : implementation file
//

#include "stdafx.h"
#include "NStockCrPipDial.h"
#include "NStockBar.h"
#include "NCMProject.h"


// NStockCrPipDial dialog

IMPLEMENT_DYNAMIC(NStockCrPipDial, CDialog)
NStockCrPipDial::NStockCrPipDial(CWnd* pParent /*=NULL*/)
	: CDialog(NStockCrPipDial::IDD, pParent)
	, m_ExtRad0(0)
	, m_ExtRad1(0)
	, m_IntRad0(0)
	, m_IntRad1(0)
	, m_PolyNum(0)
	, m_Length(0)
	, m_RotAngle(0)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_shift(0)
	, m_AxisInt(2)
	, Auto(FALSE)
	, FillG54(TRUE)
{
	MillDialog = false;
	m_Pos = 0;
}

NStockCrPipDial::NStockCrPipDial(int ID, CWnd* pParent /*=NULL*/)
	: CDialog(ID, pParent)
	, m_ExtRad0(0)
	, m_ExtRad1(0)
	, m_IntRad0(0)
	, m_IntRad1(0)
	, m_PolyNum(0)
	, m_Length(0)
	, m_RotAngle(0)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_shift(0)
	, m_AxisInt(2)
	, Auto(FALSE)
	, FillG54(TRUE)
{
	MillDialog = true;
	m_Pos = 0;
}

NStockCrPipDial::~NStockCrPipDial()
{
}

void NStockCrPipDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STOCK_POLYNUM, m_PolyNum);
	DDX_Text(pDX, IDC_STOCK_EXTRAD0, m_ExtRad0);
	DDX_Text(pDX, IDC_STOCK_EXTRAD1, m_ExtRad1);
	DDX_Text(pDX, IDC_STOCK_INTRAD0, m_IntRad0);
	DDX_Text(pDX, IDC_STOCK_INTRAD1, m_IntRad1);
	DDX_Text(pDX, IDC_STOCK_LENGTH, m_Length);
	DDX_Text(pDX, IDC_STOCK_ROTANGLE, m_RotAngle);
	DDX_Text(pDX, IDC_STOCK_X, m_x);
	DDX_Text(pDX, IDC_STOCK_Y, m_y);
	DDX_Text(pDX, IDC_STOCK_Z, m_z);
	DDX_Text(pDX, IDC_STOCK_SHIFT, m_shift);
	DDX_Control(pDX, IDC_STOCK_EXTRAD0, m_ExtRad0_ctrl);
	DDX_Control(pDX, IDC_STOCK_EXTRAD1, m_ExtRad1_ctrl);
	DDX_Control(pDX, IDC_STOCK_INTRAD0, m_IntRad0_ctrl);
	DDX_Control(pDX, IDC_STOCK_INTRAD1, m_IntRad1_ctrl);
	DDX_Control(pDX, IDC_STOCK_LENGTH, m_Length_ctrl);
	DDX_Control(pDX, IDC_STOCK_ROTANGLE, m_RotAngle_ctrl);
	DDX_Control(pDX, IDC_STOCK_X, m_x_ctrl);
	DDX_Control(pDX, IDC_STOCK_Y, m_y_ctrl);
	DDX_Control(pDX, IDC_STOCK_Z, m_z_ctrl);
	DDX_Control(pDX, IDC_STOCK_SHIFT, m_shift_ctrl);
	DDX_Text(pDX, IDC_STOCK_POS, m_Pos);
	if (MillDialog == true)
	{
		DDX_CBIndex(pDX, IDC_CYL_COMBO1, m_AxisInt);
		DDX_Check(pDX, IDC_STOCK_AUTO, Auto);
		DDX_Check(pDX, IDC_STOCK_WRITEPROG0, FillG54);
	}
}


BEGIN_MESSAGE_MAP(NStockCrPipDial, CDialog)
	ON_BN_CLICKED(IDC_CALC_DIM, OnBnClickedCalcDim)
END_MESSAGE_MAP()


// NStockCrPipDial message handlers
void NStockCrPipDial::UnSet(void)
{
	m_ExtRad0 = 0.;
	m_ExtRad1 = 0.;
	m_IntRad0 = 0.;
	m_IntRad1 = 0.;
	m_PolyNum = 0;
	m_Length = 0.;
	m_RotAngle = 0.;
}
NStock * NStockCrPipDial::GetNewStock()
{
	return pStock;
}

BOOL NStockCrPipDial::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (NCMComData::IsMillEnabled() && !NCMComData::IsTurnEnabled())
	{//Mill project
		CWnd *pWnd = GetDlgItem(IDC_STOCK_AUTO);
		if (pWnd != nullptr)
			pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STOCK_WRITEPROG0);
		if (pWnd != nullptr)
			pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_CYL_COMBO1);
		if (pWnd != nullptr)
			pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STOCK_AXISWORLD);
		if (pWnd != nullptr)
			pWnd->ShowWindow(SW_SHOW);
		m_x_ctrl.SetReadOnly(FALSE);
		m_y_ctrl.SetReadOnly(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NStockCrPipDial::OnOK()
{
 	if( ! UpdateData() )
		return;
 	NStockBar *pBar = new NStockBar();

	pBar->Create(m_PolyNum, m_ExtRad0, m_ExtRad1
		, m_IntRad0, m_IntRad1
		, m_Length
		, m_RotAngle
		, BPoint(-m_x, -m_y, -m_z, 1.)
		, m_AxisInt);

	pBar->SetStockGr(BStockGrID().Set(m_Pos));
	pStock = pBar;

	CDialog::OnOK();
}

void NStockCrPipDial::Serialize(CArchive& ar)
{
	SerializeElements(ar,&m_ExtRad0,1);
	SerializeElements(ar,&m_ExtRad1,1);
	SerializeElements(ar,&m_IntRad0,1);
	SerializeElements(ar,&m_IntRad1,1);
	SerializeElements(ar,&m_PolyNum,1);
	SerializeElements(ar,&m_Length,1);
	SerializeElements(ar,&m_RotAngle,1);
	SerializeElements(ar,&m_x,1);
	SerializeElements(ar,&m_y,1);
	SerializeElements(ar,&m_z,1);
	SerializeElements(ar,&m_shift,1);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")))
	{
		SerializeElements(ar, &m_Pos, 1);
	}
	CDialog::Serialize(ar);
}

void NStockCrPipDial::OnBnClickedCalcDim()
{
	if(!Gabar.IsDefined())
		Gabar.Expand(0.,0.,0.);
	UpdateData(TRUE);
	m_Length = Gabar.dZ() + 2*m_shift;	
	m_ExtRad0 = 2. * (max(Gabar.Xmax, - Gabar.Xmin) + m_shift);// m_ExtRad0 is a diameter not radius
	m_ExtRad1 = m_ExtRad0;
	m_z = -Gabar.Zmin + m_shift;
	UpdateData(FALSE);
}

INT_PTR NStockCrPipDial::DoModal(const BBox &gabar, class NStock *pExStock)
{
	pStock = pExStock;
	if (pStock)
	{
		if (pStock->IsKindOf(RUNTIME_CLASS(NStockBar)))
		{
			NStockBar *pStockBar = static_cast<NStockBar *>(pStock);
			m_ExtRad0 = 2 * pStockBar->ExtRad0;
			m_ExtRad1 = 2 * pStockBar->ExtRad1;
			m_IntRad0 = 2 * pStockBar->IntRad0;
			m_IntRad1 = 2 * pStockBar->IntRad1;
			m_PolyNum = GetID() == IDD_STOCK_CREATE_CONE ? 0 : pStockBar->PolyNum;
			m_Length = pStockBar->Length;
			m_RotAngle = pStockBar->RotAngle;
			m_x = pStockBar->BaseCenter.GetX();
			m_y = pStockBar->BaseCenter.GetY();
			m_z = -pStockBar->BaseCenter.GetZ();
			m_shift = 0;
		}
		m_Pos = pStock->GetStockGr().Get();
	}
	else
		m_Pos = 0;
	Gabar = gabar;
	return CDialog::DoModal();
}
