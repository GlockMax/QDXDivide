// NStockCrContDial.cpp : implementation file
//

#include "stdafx.h"
#include "StContOut.h"
#include "TBladeCont.h"
#include "NStockCont.h"
#include "NStockBSP.h"
#include "BFileDialogWPath.h"
#include "NStockCrContDial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NStockCrContDial dialog

IMPLEMENT_DYNAMIC(NStockCrContDial, CDialog)
NStockCrContDial::NStockCrContDial(CWnd* pParent /*=NULL*/)
	: CDialog(NStockCrContDial::IDD, pParent)
	, m_FileName(_T(""))
	, m_len(0)
	, m_wid(0)
	, m_heig(0)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_inv(FALSE)
	, m_FlipXY(TRUE)
	, m_FlipXYInv(FALSE)

{
	m_Pos = 0;
}

NStockCrContDial::~NStockCrContDial()
{
}

void NStockCrContDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Text(pDX, IDC_STOCK_HEIG, m_heig);
	DDX_Text(pDX, IDC_STOCK_LEN, m_len);
	DDX_Text(pDX, IDC_STOCK_WID, m_wid);
	DDX_Text(pDX, IDC_STOCK_X, m_x);
	DDX_Text(pDX, IDC_STOCK_Y, m_y);
	DDX_Text(pDX, IDC_STOCK_Z, m_z);
	DDX_Check(pDX, IDC_STCONT_180, m_inv);
	DDX_Control(pDX, IDC_STOCK_HEIG, m_heig_ctrl);
	DDX_Control(pDX, IDC_STOCK_LEN, m_len_ctrl);
	DDX_Control(pDX, IDC_STOCK_WID, m_wid_ctrl);
	DDX_Control(pDX, IDC_STOCK_X, m_x_ctrl);
	DDX_Control(pDX, IDC_STOCK_Y, m_y_ctrl);
	DDX_Control(pDX, IDC_STOCK_Z, m_z_ctrl);
	DDX_Check(pDX, IDC_STCONT_AXIS_X, m_FlipXY);
	DDX_Check(pDX, IDC_STCONT_AXIS_Y, m_FlipXYInv);
	DDX_Text(pDX, IDC_STOCK_POS, m_Pos);
}


BEGIN_MESSAGE_MAP(NStockCrContDial, CDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_STCONT_AXIS_X, OnBnClickedFlipXY)
	ON_BN_CLICKED(IDC_STCONT_AXIS_Y, OnBnClickedFlipXY)
END_MESSAGE_MAP()


NStock * NStockCrContDial::GetNewStock()
{
	return pStock;
}
// NStockCrContDial message handlers

void NStockCrContDial::OnBnClickedBrowse()
{
	if (!UpdateData())
		return;
	CString FileName;
	LoadFile(FileName, false);
	if (pStock)
		if (pStock->GetGabar().IsDefined())
		{
			m_len = pStock->GetGabar().dX();
			m_wid = pStock->GetGabar().dY();
			m_heig = pStock->GetGabar().dZ();
		}
	UpdateData(FALSE);
}

void NStockCrContDial::OnBnClickedFlipXY()
{
	if (!UpdateData())
		return;
	if (pStock == nullptr)
		return;
	CString FileName = pStock->GetFileName();
	LoadFile(FileName, false);
	if (pStock)
		if (pStock->GetGabar().IsDefined())
		{
			m_len = pStock->GetGabar().dX();
			m_wid = pStock->GetGabar().dY();
			m_heig = pStock->GetGabar().dZ();
		}
	UpdateData(FALSE);
}

void NStockCrContDial::UnSet()
{
	m_FileName = "";
/*	m_heig = 0.0;
	m_len = 0.0;
	m_wid = 0.0;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;*/
}

BOOL NStockCrContDial::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(pStock)
	{
		OriginalStock = TRUE;
		if (pStock->GetGabar().IsDefined())
		{
			m_len = pStock->GetGabar().dX();
			m_wid = pStock->GetGabar().dY();
			m_heig = pStock->GetGabar().dZ();
		}
		if (pStock->IsKindOf(RUNTIME_CLASS(NStockBSP)))
		{
			m_x = -((NStockBSP *)pStock)->GetBase().GetX();
			m_y = -((NStockBSP *)pStock)->GetBase().GetY();
			m_z = -((NStockBSP *)pStock)->GetBase().GetZ();
			m_inv = ((NStockBSP *)pStock)->GetInvert();
		}
		else if (pStock->IsKindOf(RUNTIME_CLASS(NStockCont)))
		{
			m_x = -((NStockCont *)pStock)->GetBase().GetX();
			m_y = -((NStockCont *)pStock)->GetBase().GetY();
			m_z = -((NStockCont *)pStock)->GetBase().GetZ();
			m_inv = ((NStockCont *)pStock)->GetInvert();
			m_FlipXY = ((NStockCont*)pStock)->GetFlipXY();
			m_FlipXYInv = !m_FlipXY;
		}
		m_FileName = pStock->GetFileName();
		LoadFile(m_FileName, false);
		UpdateData(FALSE);
	}
	else
	{
		if(!m_FileName.IsEmpty())
			LoadFile(m_FileName, false);
		if (pStock)
			if (pStock->GetGabar().IsDefined())
			{
				m_len = pStock->GetGabar().dX();
				m_wid = pStock->GetGabar().dY();
				m_heig = pStock->GetGabar().dZ();
			}
		UpdateData(FALSE);
		OriginalStock = FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR NStockCrContDial::DoModal(NStock *pOldStock) 
{
	pStock = (NStockCont *)pOldStock;
	m_Pos = (pOldStock == nullptr) ? 0 : pOldStock->GetStockGr().Get();
	return CDialog::DoModal();
}

void NStockCrContDial::OnOK()
{
	if( ! UpdateData() )
		return;

	LoadFile(m_FileName, true);

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

	if (pStock->IsKindOf(RUNTIME_CLASS(NStockBSP)))
		((NStockBSP *)pStock)->MoveBase(-m_x, -m_y, -m_z, m_inv == TRUE);
	else if (pStock->IsKindOf(RUNTIME_CLASS(NStockCont)))
		((NStockCont *)pStock)->MoveBase(- m_x, - m_y, - m_z, m_inv == TRUE);
	pStock->SetStockGr(BStockGrID().Set(m_Pos));

	CDialog::OnOK();
}

int NStockCrContDial::LoadFile(CString & FileName, bool FullRead)
{
	if (FileName.IsEmpty())
	{
		CString filter;
		filter.LoadString(IDS_FILESIMPORT_FILTER);	// создание фильтра для ограничения на расширение
		BFileDialogWPath fd(_T("LastStockPath"), TRUE, NULL, NULL, NULL, filter);

		if (fd.DoModal() == IDOK)
			FileName = fd.GetPathName();
		else
			return -1;
	}
	int index = FileName.ReverseFind('.');
	if (index <= 0)
		return 0;
	CString RecExt = FileName.Right(FileName.GetLength() - index - 1);
	RecExt.MakeUpper();
	NStock *pBufStock = nullptr;
	if (RecExt == _T("MFR"))
	{
		pBufStock = new NStockBSP;
		if (FullRead)
			((NStockBSP *)pBufStock)->CreateEx(FileName);
		else
			((NStockBSP *)pBufStock)->ReadGabarOnly(FileName);
	}
	else
	{
		StContOut Import;// ADD ROT AXIS
		NCadrGeom *GeomArr;
		size_t Num = Import.ReadFromFile(GeomArr, FileName);
		if (Num == 0)
		{
			AfxMessageBox(Import.GetErrorCode());
			return -1;
		}
		if (m_FlipXY)
			NCadrGeom::FlipXY(GeomArr, int(Num));
		NStockCont *pBufCont = new NStockCont();
		pBufCont->Create(GeomArr, int(Num), BPoint(0., 0., 0., 1.), FileName, m_FlipXY == TRUE);
		delete[] GeomArr;
		pBufStock = pBufCont;
	}
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
		AfxMessageBox(IDS_STOCK_WRONG_CONTOUR);//INSERT ID
		delete pBufStock;
		delete pStock;
		pStock = nullptr;
	}
	return 0;
}
