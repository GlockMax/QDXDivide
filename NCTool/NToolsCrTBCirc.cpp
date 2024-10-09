// NToolsCrTBCirc.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTBCirc.h"
#include "ConstDef.h"
#include "ntoolscrtbcirc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTBCirc dialog

IMPLEMENT_DYNAMIC(NToolsCrTBCirc, NToolsCrTurn)
NToolsCrTBCirc::NToolsCrTBCirc()
	: NToolsCrTurn(NToolsCrTBCirc::IDD)
{
	CString Key = "Defaults/Tools/Turn/Blade/Round";
	NTiParams &Par = NCM_PROJECT.Defaults;

	m_AngleP = Par.GetDouble((Key + "@C").GetBuffer(), 7.);
	if(MM_INCH == M_MM)
	{
		Key += "/mm";
		m_R = Par.GetDouble((Key + "@R").GetBuffer(), 4.);
		m_Thickness = Par.GetDouble((Key + "@H").GetBuffer(), 3.18);
	}
	else
	{
		Key += "/inch";
		m_R = Par.GetDouble((Key + "@R").GetBuffer(), 0.25);
		m_Thickness = Par.GetDouble((Key + "@H").GetBuffer(), 0.125);
	}
}

NToolsCrTBCirc::~NToolsCrTBCirc()
{
}

void NToolsCrTBCirc::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERT_ANGLFRONT, m_AngleP);
	DDV_MinMaxDouble(pDX, m_AngleP, 0, 180);
	DDX_Text(pDX, IDC_INSERT_RADIUS, m_R);
	DDV_MinMaxDouble(pDX, m_R, 0, 1000);
	DDX_Text(pDX, IDC_INSERT_THICKNESS, m_Thickness);
	DDX_Control(pDX, IDC_INSERT_RADIUS, m_R_ctrl);
	DDX_Control(pDX, IDC_INSERT_ANGLFRONT, m_C_ctrl);
	DDX_Control(pDX, IDC_INSERT_THICKNESS, m_H_ctrl);
//	DDX_Control(pDX, IDC_MANGLE, m_D_ctrl);
}


BEGIN_MESSAGE_MAP(NToolsCrTBCirc, NToolsCrTurn)
END_MESSAGE_MAP()


// NToolsCrTBCirc message handlers

void NToolsCrTBCirc::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnBladeCirc *NToolsCrTBCirc::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnBladeCirc *pTool = new NToolTurnBladeCirc;

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

