// ..\NCTool\NToolsCrTurnHoldSTLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTurnHoldSTLDlg.h"
#include "NToolTurnHolderSTL.h"
#include "BFileDialogWPath.h"
#include "NOCImport.h"
#include "MTEdit.h"
#include "DocElements.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTurnHoldSTLDlg dialog

IMPLEMENT_DYNAMIC(NToolsCrTurnHoldSTLDlg, NToolsCrTurn)

NToolsCrTurnHoldSTLDlg::NToolsCrTurnHoldSTLDlg(CWnd* pParent /*=NULL*/)
	: NToolsCrTurn(NToolsCrTurnHoldSTLDlg::IDD)
	, FileName(_T(""))
	, m_Y(0.)
	, pTool(NULL)
	, m_DX(0.)
	, m_DY(0.)
	, m_DZ(0.)

{
		m_name = _T("HolderSTL");
}

NToolsCrTurnHoldSTLDlg::~NToolsCrTurnHoldSTLDlg()
{
}

void NToolsCrTurnHoldSTLDlg::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOLDSTL_NAME, FileName);
	DDX_Text(pDX, IDC_HOLD_Y, m_Y);
	DDX_Text(pDX, IDC_HOLDSTL_DX, m_DX);
	DDX_Text(pDX, IDC_HOLDSTL_DY, m_DY);
	DDX_Text(pDX, IDC_HOLDSTL_DZ, m_DZ);
}


BEGIN_MESSAGE_MAP(NToolsCrTurnHoldSTLDlg, NToolsCrTurn)
	ON_BN_CLICKED(IDC_LOAD_TURNHOLDER_STL, &NToolsCrTurnHoldSTLDlg::OnBnClickedLoadTurnholderStl)
END_MESSAGE_MAP()


// NToolsCrTurnHoldSTLDlg message handlers


void NToolsCrTurnHoldSTLDlg::OnBnClickedLoadTurnholderStl()
{
//	NStockStl *pBufStock = (NCMComData::GetType() == AT_WELD) ? new NWeldPart : new NStockStl;
// Get file name
	CString filter;
	filter.LoadString(IDS_PART_FILTER);
	BFileDialogWPath fd(_T("LastToolPath"), TRUE, NULL, NULL, NULL, filter);
	if(fd.DoModal() != IDOK)
		return;
	FileName = fd.GetPathName();
	const StockFileType StockType = MTEdit::Conv2STL(FileName, NToolList);// FileName can be changed here
	if (StockType == SFT_UNKNOWN)
		return;// ignore file if error while convertion
	UpdateData(FALSE);
	delete pTool;
	pTool = CreateTool();
	if(pTool)
	{
		const BBox &Gabar = pTool->GetGabar();
		m_DX = (float)Gabar.dX();
		m_DY = (float)Gabar.dY();
		m_DZ = (float)Gabar.dZ();
	}
	UpdateData(FALSE);
}


void NToolsCrTurnHoldSTLDlg::OnOK()
{
	delete pTool;
	pTool = CreateTool();
	if(!pTool)
		return;
	CDialog::OnOK();
}

NToolTurn *NToolsCrTurnHoldSTLDlg::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnHolderSTL *pTool = new NToolTurnHolderSTL;

	pTool->FillFromPage(this);

	if(pTool->GenInit() == 0)
	{
		return pTool;
	}
	else
	{
		delete pTool;
		AfxMessageBox(IDS_MES_PAR);
		return NULL;
	}
}


void NToolsCrTurnHoldSTLDlg::OnCancel()
{
	CDialog::OnCancel();
}
