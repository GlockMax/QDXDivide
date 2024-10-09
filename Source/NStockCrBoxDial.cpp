// NStockCrBoxDial.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NStockBox.h"
#include "NStockCrBoxDial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStockCrBoxDial dialog
static DWORD aHelpIDs[]=
{
	IDC_STOCK_SHIFT,	HIDC_STOCK_SHIFT,
	IDC_CALC_DIM,		HIDC_CALC_DIM,
	IDC_STOCK_LEN,		HIDC_STOCK_LEN,
	IDC_STOCK_WID,		HIDC_STOCK_WID,
	IDC_STOCK_HEIG,		HIDC_STOCK_HEIG,
	IDC_STOCK_X,		HIDC_STOCK_X,
	IDC_STOCK_Y,		HIDC_STOCK_Y,
	IDC_STOCK_Z,		HIDC_STOCK_Z,
	IDC_BOX_SMALL,		HIDC_BOX_SMALL,
	IDC_BOX_LARGE,		HIDC_BOX_LARGE,
	IDC_R0,				HIDC_R0,
	IDC_R1,				HIDC_R1,
	IDC_R2,				HIDC_R2,
	IDC_R3,				HIDC_R3,
	IDC_R4,				HIDC_R4,
	IDC_R5,				HIDC_R5,
	IDC_R6,				HIDC_R6,
	IDC_R7,				HIDC_R7,
	IDC_R8,				HIDC_R8,	
	IDC_R9,				HIDC_R9,
	0,0
};

NStockCrBoxDial::NStockCrBoxDial(CWnd* pParent /*=NULL*/)
	: NSetupSetBoxPointDlg(NStockCrBoxDial::IDD, pParent, NULL)
{
	//{{AFX_DATA_INIT(NStockCrBoxDial)
	m_heig = 0.0;
	m_len = 0.0;
	m_shift = 0.0;
	m_wid = 0.0;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_Pos = 0;
	//}}AFX_DATA_INIT
}


void NStockCrBoxDial::DoDataExchange(CDataExchange* pDX)
{
	NSetupSetBoxPointDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NStockCrBoxDial)
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_SHIFT, m_shift);
	DDX_Control(pDX, IDC_STOCK_SHIFT, m_shift_ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NStockCrBoxDial, NSetupSetBoxPointDlg)
	//{{AFX_MSG_MAP(NStockCrBoxDial)
	ON_BN_CLICKED(IDC_CALC_DIM, OnCalcDim)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStockCrBoxDial message handlers

void NStockCrBoxDial::OnCalcDim() 
{
	if(!Gabar.IsDefined())
		Gabar.Expand(0.,0.,0.);
	UpdateData(TRUE);
	m_len = Gabar.dX() + 2*m_shift;	
	m_wid = Gabar.dY() + 2*m_shift;	
	m_heig = Gabar.dZ() + 2*m_shift;
	m_x = -Gabar.Xmin+m_shift;
	m_y = -Gabar.Ymin+m_shift;
	m_z = -Gabar.Zmin+m_shift;
	Recalc();
	UpdateData(FALSE);
}



INT_PTR NStockCrBoxDial::DoModal(const BBox &gabar, NStock *pExStock)
{
	pStock = pExStock;
	Gabar = gabar;
	m_Pos = (pExStock == nullptr) ? 0 : pExStock->GetStockGr().Get();
	return CDialogEx::DoModal();
}

NStock * NStockCrBoxDial::GetNewStock()
{
	return pStock;
}

void NStockCrBoxDial::OnOK() 
{
 	if( ! UpdateData() )
		return;
 	NStockBox *pBox = new NStockBox();
 	BBox b;
 	b.Expand(0., 0., 0.);
 	b.Expand(m_len, m_wid, m_heig);
 	if(!pBox->Create(b))
	{
		AfxMessageBox(IDS_MES_ERR_ST);
	}
	pBox->SetStockGr(BStockGrID().Set(m_Pos));
 	pStock = pBox;
 	
 	CDialogEx::OnOK();
}

BOOL NStockCrBoxDial::OnInitDialog() 
{

	NSetupSetBoxPointDlg::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NStockCrBoxDial::Recalc()
//Recalculate parameters due to radio button state
{
	int x,y,z;
	GetBaseShift(x,y,z);
	m_x -= x*m_len/2;
	m_y -= y*m_wid/2;
	m_z -= z*m_heig/2;
}

void NStockCrBoxDial::UnSet()
{
	m_heig = 0.0;
	m_len = 0.0;
	m_wid = 0.0;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_shift = 0.0;
}

void NStockCrBoxDial::Serialize(CArchive& ar)
{
	SerializeElements(ar, &but, 1);
	if((ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.2")))
	{// Version <= 4.1
		but = but - IDC_R0;
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")))
	{
		SerializeElements(ar, &m_Pos, 1);
	}
	SerializeElements(ar,&m_heig,1);
	SerializeElements(ar,&m_len,1);
	SerializeElements(ar,&m_shift,1);
	SerializeElements(ar,&m_wid,1);
	SerializeElements(ar,&m_x,1);
	SerializeElements(ar,&m_y,1);
	SerializeElements(ar, &m_z, 1);
}

void NStockCrBoxDial::UpdateStockGabar(void)
{
	if(pStock)
	{
		if (pStock->GetGabar().IsDefined())
		{
			StockGabar = pStock->GetGabar();
		}
	}
	UpdateData(FALSE);
}
