// NToolsCrTBIn.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTurnSheet.h"
#include "NToolTurnBladeIn.h"
#include "NToolsCrTBIn.h"


// NToolsCrTBIn dialog

IMPLEMENT_DYNAMIC(NToolsCrTBIn, NToolsCrTurn)
NToolsCrTBIn::NToolsCrTBIn()
	: NToolsCrTurn(NToolsCrTBIn::IDD)
	, m_AngleA(0)
	, m_AngleB(0)
	, m_AngleP(0)
	, m_L1(0)
	, m_L2(0)
	, m_R(0)
	, m_PointPos(0)
{
}

NToolsCrTBIn::~NToolsCrTBIn()
{
}

void NToolsCrTBIn::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERT_ANGLMAIN, m_AngleA);
	DDV_MinMaxDouble(pDX, m_AngleA, 0, 180);
	DDX_Text(pDX, IDC_INSERT_ANGLADD, m_AngleB);
	DDV_MinMaxDouble(pDX, m_AngleB, 0, 180);
	DDX_Text(pDX, IDC_INSERT_ANGLFRONT, m_AngleP);
	DDV_MinMaxDouble(pDX, m_AngleP, 0, 180);
	DDX_Text(pDX, IDC_INSERT_LENGTHCUT, m_L1);
	DDV_MinMaxDouble(pDX, m_L1, 0, 1000);
	DDX_Text(pDX, IDC_INSERT_LENGTHADD, m_L2);
	DDV_MinMaxDouble(pDX, m_L2, 0, 1000);
	DDX_Text(pDX, IDC_INSERT_RADIUS, m_R);
	DDV_MinMaxDouble(pDX, m_R, 0, 1000);
	DDX_Radio(pDX, IDC_RB1, m_PointPos);
}


BEGIN_MESSAGE_MAP(NToolsCrTBIn, NToolsCrTurn)
END_MESSAGE_MAP()


// NToolsCrTBIn message handlers

void NToolsCrTBIn::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnBladeIn *NToolsCrTBIn::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnBladeIn *pTool = new NToolTurnBladeIn;

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
