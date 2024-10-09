// NToolsCrTHBar.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTHBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTHBar dialog

IMPLEMENT_DYNAMIC(NToolsCrTHBar, NToolsCrTurn)
NToolsCrTHBar::NToolsCrTHBar()
	: NToolsCrTurn(NToolsCrTHBar::IDD)
	, m_Dx((MM_INCH == M_MM) ? 100. : 4.)
	, m_Dy((MM_INCH == M_MM) ? 10. : 0.75)
	, m_Dz((MM_INCH == M_MM) ? 16. : 0.75)
	, m_Y((MM_INCH == M_MM) ? 18. : 0.8)
{
	m_Xdisp = (MM_INCH == M_MM) ? 1. : 0.04;
	m_Zdisp = (MM_INCH == M_MM) ? 1. : 0.04;
}

NToolsCrTHBar::~NToolsCrTHBar()
{
}

void NToolsCrTHBar::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOLD_DX, m_Dx);
	DDX_Text(pDX, IDC_HOLD_DY, m_Dy);
	DDX_Text(pDX, IDC_HOLD_DZ, m_Dz);
	DDX_Text(pDX, IDC_HOLD_Y, m_Y);
	DDX_Control(pDX, IDC_HOLD_DX, m_Dx_ctrl);
	DDX_Control(pDX, IDC_HOLD_DY, m_Dy_ctrl);
	DDX_Control(pDX, IDC_HOLD_DZ, m_Dz_ctrl);
	DDX_Control(pDX, IDC_HOLD_Y, m_Y_ctrl);
}


BEGIN_MESSAGE_MAP(NToolsCrTHBar, NToolsCrTurn)
END_MESSAGE_MAP()


// NToolsCrTHBar message handlers

void NToolsCrTHBar::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnHolderBar *NToolsCrTHBar::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnHolderBar *pTool = new NToolTurnHolderBar;

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
