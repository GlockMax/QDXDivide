// NStockCrStlDial.cpp : implementation file
//

#include "stdafx.h"
#include "DocElements.h"
#include "MainFrm.h"
#include "NOCImport.h"
#include "NStockStlMillTurn.h"
#include "NStockStlTurn.h"
#include "NWeldPart.h"
#include "BFileDialogWPath.h"
#include "NStockBSP.h"
#include "DocElements.h"
#include "NStockCrStlDial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStockCrStlDial dialog
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


NStockCrStlDial::NStockCrStlDial(CWnd* pParent /*=NULL*/)
	: NSetupSetBoxPointDlg(NStockCrStlDial::IDD, pParent, NULL)
{
	//{{AFX_DATA_INIT(NStockCrStlDial)
	m_FileName = _T("");
	//}}AFX_DATA_INIT
	OriginalStock = FALSE;
	FillG54 = FALSE;
	m_Pos = 0;
}


void NStockCrStlDial::DoDataExchange(CDataExchange* pDX)
{
	NSetupSetBoxPointDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NStockCrStlDial)
	DDX_Text(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Text(pDX, IDC_STOCK_POS, m_Pos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NStockCrStlDial, NSetupSetBoxPointDlg)
	//{{AFX_MSG_MAP(NStockCrStlDial)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStockCrStlDial message handlers

void NStockCrStlDial::OnBrowse() 
{
	UpdateData();
	class NStock *pBufStock = nullptr;
// Get file name
	CString filter, FileName;
	filter.LoadString(IDS_STOCK_FILTER);
	BFileDialogWPath fd(_T("LastStockPath"), TRUE, NULL, NULL, NULL, filter);
	if(fd.DoModal() == IDOK)
	{
		FileName = fd.GetPathName();
		const StockFileType StockType = MTEdit::Conv2STL(FileName, NStocksList);// FileName can be changed here
		if (StockType != SFT_UNKNOWN)// ignore file if error while convertion
		{
			if (StockType == SFT_MFR)
			{
				pBufStock = new NStockBSP;
				((NStockBSP*)pBufStock)->ReadGabarOnly(FileName);
			}
			else
			{
				pBufStock = MakeNewStock();
				NStockStl* pBufStockStl = dynamic_cast<NStockStl*>(pBufStock);
				if (pBufStockStl != nullptr)
				{
					pBufStockStl->CreateEx(FileName);
					//if (!pBufStockStl->GetSTL().Is1Connected())
					//{
					//	AfxMessageBox(IDS_MES_STL_IMPOSSIBLE_BUILD_STOCK);
					//	pBufStock->IsCreated = false;
					//}
				}
			}
		}
	}
	if (pBufStock != nullptr)
	{
		if (pBufStock->IsCreated)
		{
			if (OriginalStock)
				OriginalStock = FALSE;
			else
				delete pStock;
			pStock = pBufStock;
			m_FileName = pStock->GetFileName();
		}
		else
		{
			delete pBufStock;
		}
	}
	UpdateStockGabar();
}

void NStockCrStlDial::UnSet()
{
	m_FileName = "";
	m_heig = 0.0;
	m_len = 0.0;
	m_wid = 0.0;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
}

BOOL NStockCrStlDial::OnInitDialog() 
{
	NSetupSetBoxPointDlg::OnInitDialog();
	UpdateStockGabar();
	if(pStock)
	{
		OriginalStock = TRUE;
		m_FileName = pStock->GetFileName();
	}
	else
	{
		OriginalStock = FALSE;
		pStock = MakeNewStock();

		UnSet();
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR NStockCrStlDial::DoModal(class NStock *pOldStock) 
{
	pStock = pOldStock;
	m_Pos = (pOldStock == nullptr) ? 0 : pOldStock->GetStockGr().Get();
	return CDialogEx::DoModal();
}

class NStock *NStockCrStlDial::MakeNewStock(void)
{
	switch(NCMComData::GetType())
	{
	case AT_WELD:
		return new NWeldPart;
	case AT_MILL_TURN:
		return new NStockSTLMillTurn;
	case AT_TURN:
		return new NStockSTLTurn;
	default:
		return new NStockStl;
	}
}

void NStockCrStlDial::OnOK() 
{
 	if( ! UpdateData() )
		return;
	class NStock* pBufStock = nullptr;
	if(pStock->IsKindOf(RUNTIME_CLASS(NStockBSP)))
	{
		pBufStock = new NStockBSP;
		pBufStock->SetStockGr(BStockGrID().Set(m_Pos));// for right stock pos in bodies
		((NStockBSP *)pBufStock)->CreateEx(pStock->GetFileName());
	}
	else
	{
		pBufStock = MakeNewStock();
		pBufStock->SetStockGr(BStockGrID().Set(m_Pos));// for right stock pos in bodies
		((NStockStl *)pBufStock)->CreateEx(pStock->GetFileName());
	}
	if(pBufStock->IsCreated)
	{
		if(OriginalStock)
			OriginalStock = FALSE;
		else
			delete pStock;
		pStock = pBufStock;
	}
	else
	{
		delete pBufStock;
		CString Mes;
		if (pStock->IsKindOf(RUNTIME_CLASS(NStockBSP)))
			Mes.Format(IDS_MES_DX_ON, pStock->GetFileName());
		else
			Mes.Format(IDS_MES_FILEO, pStock->GetFileName());;

		AfxMessageBox(Mes);
		return;
	}
	NSetupSetBoxPointDlg::OnToolApply();
	NSetupSetBoxPointDlg::OnOK();
}

class NStock * NStockCrStlDial::GetNewStock()
{
	return pStock;
}

void NStockCrStlDial::OnCancel() 
{
	if(!OriginalStock)
		delete pStock;
	NSetupSetBoxPointDlg::OnCancel();
}

void NStockCrStlDial::Serialize(CArchive& ar)
{
	SerializeElements(ar,&m_FileName,1);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")))
	{
		SerializeElements(ar, &m_Pos, 1);
	}
}

void NStockCrStlDial::UpdateStockGabar(void)
{
	m_len  = 0.;
	m_wid  = 0.;
	m_heig = 0.;

	if(pStock)
	{
		if (pStock->GetGabar().IsDefined())
		{
			StockGabar = pStock->GetGabar();
			m_len  = StockGabar.dX();
			m_wid  = StockGabar.dY();
			m_heig = StockGabar.dZ();
		}
	}
	UpdateData(FALSE);
}

