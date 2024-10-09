// NStockCrContMDial.cpp : implementation file
//

#include "stdafx.h"
#include "StContOut.h"
#include "TBladeCont.h"
#include "NStockSweep.h"
#include "NStockCrContMDial.h"


// NStockCrContMDial dialog

IMPLEMENT_DYNAMIC(NStockCrContMDial, CDialog)
NStockCrContMDial::NStockCrContMDial(CWnd* pParent /*=NULL*/)
	: CDialog(NStockCrContMDial::IDD, pParent)
	, m_FileName(_T(""))
	, m_len(0)
	, m_wid(0)
	, m_heig(1.)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_a(0)
	, m_b(0)
	, m_c(0)
	, Vector(0., 0., 1., 0.)
{
	m_Pos = 0;
}

NStockCrContMDial::~NStockCrContMDial()
{
}

void NStockCrContMDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Text(pDX, IDC_STOCK_HEIG, m_heig);
	DDX_Text(pDX, IDC_STOCK_LEN, m_len);
	DDX_Text(pDX, IDC_STOCK_WID, m_wid);
	DDX_Text(pDX, IDC_STOCK_X, m_x);
	DDX_Text(pDX, IDC_STOCK_Y, m_y);
	DDX_Text(pDX, IDC_STOCK_Z, m_z);
	DDX_Text(pDX, IDC_STOCK_A, m_a);
	DDX_Text(pDX, IDC_STOCK_B, m_b);
	DDX_Text(pDX, IDC_STOCK_C, m_c);
	DDX_Control(pDX, IDC_STOCK_HEIG, m_heig_ctrl);
	DDX_Control(pDX, IDC_STOCK_LEN, m_len_ctrl);
	DDX_Control(pDX, IDC_STOCK_WID, m_wid_ctrl);
	DDX_Control(pDX, IDC_STOCK_X, m_x_ctrl);
	DDX_Control(pDX, IDC_STOCK_Y, m_y_ctrl);
	DDX_Control(pDX, IDC_STOCK_Z, m_z_ctrl);
	DDV_MinMaxDouble(pDX, m_heig, 0.01, 10000.);
	DDX_Text(pDX, IDC_STOCK_POS, m_Pos);
}


BEGIN_MESSAGE_MAP(NStockCrContMDial, CDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()


NStock * NStockCrContMDial::GetNewStock()
{
	return pStock;
}
// NStockCrContMDial message handlers

void NStockCrContMDial::OnBnClickedBrowse()
{
	if( ! UpdateData() )
		return;
	CString FileName;
	LoadFile(FileName);
	if(pStock)
		if (pStock->GetGabar().IsDefined())
		{
			m_len = pStock->GetGabar().dX();
			m_wid = pStock->GetGabar().dY();
		}
	UpdateData(FALSE);
}
void NStockCrContMDial::UnSet()
{
	m_FileName = "";
}

BOOL NStockCrContMDial::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(pStock)
	{
		OriginalStock = TRUE;
		if (pStock->GetGabar().IsDefined())
		{
			m_len = pStock->GetGabar().dX();
			m_wid = pStock->GetGabar().dY();
//			m_heig = pStock->Gabar.dZ();
		}
		m_a = pStock->GetABC().GetX();
		m_b = pStock->GetABC().GetY();
		m_c = pStock->GetABC().GetZ();
		m_heig = sqrt(pStock->GetVector().D2());
		m_x		= -pStock->GetBase().GetX();
		m_y		= -pStock->GetBase().GetY();
		m_z		= -pStock->GetBase().GetZ();
	}
	else
	{
		OriginalStock = FALSE;
		UnSet();
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR NStockCrContMDial::DoModal(NStock *pOldStock) 
{
	pStock = (NStockSweep *)pOldStock;
	m_Pos = (pOldStock == nullptr) ? 0 : pOldStock->GetStockGr().Get();
	return CDialog::DoModal();
}

void NStockCrContMDial::OnOK()
{
	if( ! UpdateData() )
		return;

	LoadFile(m_FileName);

	if(!pStock)
	{
		CString mes;
		mes.Format(IDS_MES_FILEO, m_FileName);
		AfxMessageBox(mes);
		return ;
	}
	if (!pStock->GetGabar().IsDefined())
	{
		CString mes;
		mes.Format(IDS_MES_FILEO, m_FileName);
		AfxMessageBox(mes);
		return ;
	}

	pStock->MoveBase(- m_x, - m_y, - m_z);
	pStock->SetStockGr(BStockGrID().Set(m_Pos));
	CDialog::OnOK();
}

int NStockCrContMDial::LoadFile(CString & FileName)
{
	StContOut Import;
	NCadrGeom *GeomArr;
	int Num = Import.Load(GeomArr, FileName);
	if(Num < 0)
		return -1;
    if(Num == 0)
	{
		AfxMessageBox(IDS_STOCK_ERREAD);
		return -1;
	}

	NStockSweep *pBufStock = new NStockSweep;
	Vector = BPoint(0., 0., fabs(m_heig), 0.); 
	pBufStock->Create(FileName, GeomArr, Num, BPoint(0.,0.,0.,1.), Vector, BPoint(m_a, m_b, m_c, 1.));
	delete [] GeomArr;
	if(pBufStock->IsCreated)
	{
		if(OriginalStock)
			OriginalStock = FALSE;
		else
			delete pStock;
		pStock = pBufStock;
		m_FileName = FileName;
	}
	else
	{
		delete pBufStock;
	}
	return 0;
}
