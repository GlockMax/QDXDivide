// NToolsCrTHCyl.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTHCyl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTHCyl dialog

IMPLEMENT_DYNAMIC(NToolsCrTHCyl, NToolsCrTurn)
NToolsCrTHCyl::NToolsCrTHCyl()
	: NToolsCrTurn(NToolsCrTHCyl::IDD)
	, m_Dx((MM_INCH == M_MM) ? 20. : 0.75)
	, m_Y(0.)
	, m_D1((MM_INCH == M_MM) ? 12. : 0.5)
	, m_D2((MM_INCH == M_MM) ? 12. : 0.5)
	, m_FH(1)
{
	m_Xdisp = (MM_INCH == M_MM) ? 1. : 0.04;
	m_Zdisp = (MM_INCH == M_MM) ? 1. : 0.04;
}

NToolsCrTHCyl::~NToolsCrTHCyl()
{
}

void NToolsCrTHCyl::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOLD_DX, m_Dx);
	DDX_Text(pDX, IDC_HOLD_Y, m_Y);
	DDX_Text(pDX, IDC_HOLD_R1, m_D1);
	DDX_Text(pDX, IDC_HOLD_R2, m_D2);
	DDX_Control(pDX, IDC_HOLD_DX, m_Dx_ctrl);
	DDX_Control(pDX, IDC_HOLD_Y, m_Y_ctrl);
	DDX_Control(pDX, IDC_HOLD_R1, m_D1_ctrl);
	DDX_Control(pDX, IDC_HOLD_R2, m_D2_ctrl);
	DDX_Radio(pDX, IDC_TURN_HOLD_D, m_FH);
}


BEGIN_MESSAGE_MAP(NToolsCrTHCyl, NToolsCrTurn)
END_MESSAGE_MAP()


// NToolsCrTHCyl message handlers

void NToolsCrTHCyl::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnHolderCyl *NToolsCrTHCyl::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnHolderCyl *pTool = new NToolTurnHolderCyl;

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
