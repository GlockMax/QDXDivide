// NStockCrCylDial.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "NStockCyl.h"
#include "NCMProject.h"
#include "NStockCrCylDial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStockCrCylDial dialog


NStockCrCylDial::NStockCrCylDial(CWnd* pParent /*=NULL*/)
	: NSetupSetBoxPointDlg(NStockCrCylDial::IDD, pParent, NULL)
	, m_AxisInt(2)
{
	//{{AFX_DATA_INIT(NStockCrCylDial)
	m_diam = 0.0;
	m_heig = 0.0;
	m_shift = 0.0;
	m_len = 0.;
	m_wid = 0.;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	//}}AFX_DATA_INIT
	but = 0;
	m_Pos = 0;
}


void NStockCrCylDial::DoDataExchange(CDataExchange* pDX)
{
	NSetupSetBoxPointDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NStockCrCylDial)
	DDX_Control(pDX, IDC_STOCK_CYL_S, m_Pict_s);
	DDX_Text(pDX, IDC_STOCK_DIAMETER, m_diam);
	DDX_Text(pDX, IDC_STOCK_HEIG, m_heig);
	DDX_Text(pDX, IDC_STOCK_SHIFT, m_shift);
	DDX_Control(pDX, IDC_STOCK_DIAMETER, m_diam_ctrl);
	DDX_Control(pDX, IDC_STOCK_SHIFT, m_shift_ctrl);
	DDV_MinMaxDouble(pDX, m_diam, 0., 10000.);
	DDV_MinMaxDouble(pDX, m_heig, 0., 10000.);
	DDX_Control(pDX, IDC_CYL_COMBO1, m_AxisBox);
	DDX_CBIndex(pDX, IDC_CYL_COMBO1, m_AxisInt);
	DDX_Text(pDX, IDC_STOCK_POS, m_Pos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NStockCrCylDial, NSetupSetBoxPointDlg)
	//{{AFX_MSG_MAP(NStockCrCylDial)
	ON_BN_CLICKED(IDC_CALC_DIM, OnCalcDim)
	//}}AFX_MSG_MAP
//	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStockCrCylDial message handlers

void NStockCrCylDial::OnOK() 
{
 	if( ! UpdateData() )
		return;
 	NStockCyl *pCyl = new NStockCyl();

	pCyl->Create( 0., 0., 0., m_diam, m_heig, m_AxisBox.GetCurSel());
	pCyl->SetStockGr(BStockGrID().Set(m_Pos));
	pStock = pCyl;

	NSetupSetBoxPointDlg::OnToolApply();
	NSetupSetBoxPointDlg::OnOK();
}

BOOL NStockCrCylDial::OnInitDialog() 
{
	NSetupSetBoxPointDlg::OnInitDialog();

	COLORMAP Map;
	Map.from = RGB(255, 255, 255);
	Map.to = GetSysColor(COLOR_3DFACE);
	
	
//	CheckRadioButton(IDC_R0,IDC_R1,but);

//	if(m_AxisBox.GetCurSel() < 0)
//		m_AxisBox.SetCurSel(2);

// Set visible bitmap according to system font size	
// and change white color to the background color
//	int dpi = GetDC()->GetDeviceCaps(LOGPIXELSX);
	//if(dpi == 96) // "SmallFonts"
	{
		CBitmap *pBMP,t;
		pBMP = t.FromHandle(m_Pict_s.GetBitmap());
		pBMP->Detach();
		pBMP->LoadMappedBitmap(IDB_CYL_SMALL, 0, &Map, 1);
		m_Pict_s.SetBitmap(*pBMP);
		pBMP->Detach();
	}
	//else
	//{
	//	m_Pict_s.ShowWindow(SW_HIDE);

	//	CBitmap *pBMP,t;
	//	pBMP = t.FromHandle(m_Pict_l.GetBitmap());
	//	pBMP->Detach();
	//	pBMP->LoadMappedBitmap(IDB_CYL_LARGE, 0, &Map, 1);
	//	m_Pict_l.SetBitmap(*pBMP);
	//	pBMP->Detach();
	//}
	std::vector<int> IDs;
	std::vector<CPoint> Positions;
	IDs.push_back(IDC_R0);
	IDs.push_back(IDC_R1);
	IDs.push_back(IDC_R2);
	IDs.push_back(IDC_R3);
	IDs.push_back(IDC_R4);
	IDs.push_back(IDC_R5);
	IDs.push_back(IDC_R6);
	IDs.push_back(IDC_R7);
	IDs.push_back(IDC_R8);
	IDs.push_back(IDC_R9);
	Positions.push_back(CPoint(55, 146));
	Positions.push_back(CPoint(182, 128));
	Positions.push_back(CPoint(20, 110));
	Positions.push_back(CPoint(55, 72));
	Positions.push_back(CPoint(140, 91));
	Positions.push_back(CPoint(182, 54));
	Positions.push_back(CPoint(20, 36));
	Positions.push_back(CPoint(140, 16));
	Positions.push_back(CPoint(99, 118));
	Positions.push_back(CPoint(99, 44));
	PlaceControlsOnPict(m_Pict_s, IDs, Positions);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NStockCrCylDial::UnSet()
{
	m_diam = 0.0;
	m_heig = 0.0;
	m_shift = 0.0;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	but = 0;
}

INT_PTR NStockCrCylDial::DoModal(const BBox &cyldim)
{
	CylDim = cyldim;
	m_Pos = 0;
	return CDialog::DoModal();
}

void NStockCrCylDial::OnCalcDim() 
{
	if(!CylDim.IsDefined())
		CylDim.Expand(0.,0.,0.);
	UpdateData(TRUE);

	// Вычислить по габаритам
	m_x = -(CylDim.Xmin+CylDim.Xmax)/2.;
	m_y = -(CylDim.Ymin+CylDim.Ymax)/2.;
	m_z = -(CylDim.Zmin+CylDim.Zmax)/2.;
	switch(m_AxisBox.GetCurSel())
	{
	case 0:// X
		m_diam = sqrt(CylDim.dZ()*CylDim.dZ()+
				CylDim.dY()*CylDim.dY())+2*m_shift;
		m_heig = CylDim.dX() + 2*m_shift;
		m_x += m_heig * 0.5;
		break;
	case 1:// Y
		m_diam = sqrt(CylDim.dX()*CylDim.dX()+
				CylDim.dZ()*CylDim.dZ())+2*m_shift;
		m_heig = CylDim.dY() + 2*m_shift;
		m_y += m_heig * 0.5;
		break;
	case 2:// Z
		m_diam = sqrt(CylDim.dX()*CylDim.dX()+
				CylDim.dY()*CylDim.dY())+2*m_shift;
		m_heig = CylDim.dZ() + 2*m_shift;
		m_z += m_heig * 0.5;
		break;
	}
	UpdateData(FALSE);
	
}

NStock * NStockCrCylDial::GetNewStock()
{
	return pStock;
}

//int NStockCrCylDial::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialog::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	m_AxisBox.SetCurSel(2);
//
//	return 0;
//}

void NStockCrCylDial::Serialize(CArchive& ar)
{
	m_AxisBox.Serialize(ar);
	SerializeElements(ar,&but,1);
	SerializeElements(ar,&m_AxisInt,1);
	SerializeElements(ar,&m_diam,1);
	SerializeElements(ar,&m_heig,1);
	SerializeElements(ar,&m_shift,1);
	SerializeElements(ar,&m_x,1);
	SerializeElements(ar,&m_y,1);
	SerializeElements(ar,&m_z,1);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")))
	{
		SerializeElements(ar, &m_Pos, 1);
	}
}
void NStockCrCylDial::GetBaseShift(int &x, int &y, int &z) const
{
	//x=0;y=0;z=0;
	//but = GetCheckedRadioButton(IDC_R0,IDC_R1);	
	//int value = 0;
	//switch(but)
	//{
	//case IDC_R0:
	//	break;
	//case IDC_R1:
	//	value = 2;
	//	break;
	//}
	//switch(m_AxisBox.GetCurSel())
	//{
	//case 0:// X
	//	x = value;
	//	break;
	//case 1:// Y
	//	y = value;
	//	break;
	//case 2:// Z
	//	z = value;
	//	break;
	//}
}
