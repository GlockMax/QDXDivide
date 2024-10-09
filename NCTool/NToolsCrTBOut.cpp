// NToolsCrTBOut.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTBOut.h"
#include "ConstDef.h"
#include "NTiParams.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTBOut dialog

IMPLEMENT_DYNAMIC(NToolsCrTBOut, NToolsCrTurn)
NToolsCrTBOut::NToolsCrTBOut()
	: NToolsCrTurn(NToolsCrTBOut::IDD)
	, m_FormOut(0)
{
	CString Key = "Defaults/Tools/Turn/Blade/General";
	NTiParams &Par = NCM_PROJECT.Defaults;

	m_AngleA = Par.GetDouble((Key + "@A").GetBuffer(), 90.);
	m_AngleB = Par.GetDouble((Key + "@B").GetBuffer(), 10.);
	m_AngleP = Par.GetDouble((Key + "@C").GetBuffer(), 5.);
	if(MM_INCH == M_MM)
	{
		Key += "/mm";
		m_L1 = Par.GetDouble((Key + "@L1").GetBuffer(), 12.);
		m_L2 = Par.GetDouble((Key + "@L2").GetBuffer(), 8.);
		m_R = Par.GetDouble((Key + "@R").GetBuffer(), 0.5);
		m_Thickness = Par.GetDouble((Key + "@H").GetBuffer(), 4.76);
	}
	else
	{
		Key += "/inch";
		m_L1 = Par.GetDouble((Key + "@L1").GetBuffer(), 0.61);
		m_L2 = Par.GetDouble((Key + "@L2").GetBuffer(), 0.433);
		m_R = Par.GetDouble((Key + "@R").GetBuffer(), 0.0156);
		m_Thickness = Par.GetDouble((Key + "@H").GetBuffer(), 0.125);
	}

}

NToolsCrTBOut::~NToolsCrTBOut()
{
}

void NToolsCrTBOut::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERT_ANGLMAIN, m_AngleA);
	DDX_Text(pDX, IDC_INSERT_ANGLADD, m_AngleB);
	DDX_Text(pDX, IDC_INSERT_ANGLFRONT, m_AngleP);
	DDX_Text(pDX, IDC_INSERT_LENGTHCUT, m_L1);
	DDX_Text(pDX, IDC_INSERT_LENGTHADD, m_L2);
	DDX_Text(pDX, IDC_INSERT_RADIUS, m_R);
	DDX_Text(pDX, IDC_INSERT_THICKNESS, m_Thickness);
	DDX_Control(pDX, IDC_INSERT_ANGLMAIN, m_AngleA_ctrl);
	DDX_Control(pDX, IDC_INSERT_ANGLADD, m_AngleB_ctrl);
	DDX_Control(pDX, IDC_INSERT_ANGLFRONT, m_AngleP_ctrl);
	DDX_Control(pDX, IDC_INSERT_LENGTHCUT, m_L1_ctrl);
	DDX_Control(pDX, IDC_INSERT_LENGTHADD, m_L2_ctrl);
	DDX_Control(pDX, IDC_INSERT_RADIUS, m_R_ctrl);
	DDX_Control(pDX, IDC_INSERT_THICKNESS, m_Thickness_ctrl);
	//DDV_MinMaxDouble(pDX, m_AngleA, 0, 180);
	DDV_MinMaxDouble(pDX, m_AngleB, 0, 180);
	DDV_MinMaxDouble(pDX, m_AngleP, 0, 180);
	DDV_MinMaxDouble(pDX, m_L1, 0, 1000);
	DDV_MinMaxDouble(pDX, m_L2, 0, 1000);
	DDV_MinMaxDouble(pDX, m_R, 0, 1000);
	DDX_CBIndex(pDX, IDC_FORMOUT, m_FormOut);
	DDX_Control(pDX, IDC_TURNOUTR, m_Pict);
}


BEGIN_MESSAGE_MAP(NToolsCrTBOut, NToolsCrTurn)
	ON_CBN_SELCHANGE(IDC_FORMOUT, OnCbnSelchangeFormout)
END_MESSAGE_MAP()


// NToolsCrTBOut message handlers

void NToolsCrTBOut::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnBladeOut *NToolsCrTBOut::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnBladeOut *pTool = new NToolTurnBladeOut;

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

void NToolsCrTBOut::OnCbnSelchangeFormout()
{
	UpdateData(TRUE);
	SetPicture();
}

BOOL NToolsCrTBOut::OnInitDialog()
{
	NToolsCrTurn::OnInitDialog();

	SetPicture();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NToolsCrTBOut::SetPicture(void)
{
	PictBMP.DeleteObject();

	int ResourceID = IDB_TOOL_TURNOUTR;
	
	switch( m_FormOut )
	{
	case 0:
		ResourceID = IDB_TOOL_TURNOUTR;
		break;
	case 1:
		ResourceID = IDB_TOOL_TURNINR;
		break;
	case 2:
		ResourceID = IDB_TOOL_TURNOUTL;
		break;
	case 3:
		ResourceID = IDB_TOOL_TURNINL;
		break;
	}
	PictBMP.LoadBitmap(ResourceID);

	m_Pict.SetBitmap(HBITMAP(PictBMP));
}
