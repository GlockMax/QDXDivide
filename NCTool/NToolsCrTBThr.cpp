// NToolsCrTBThr.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTBThr.h"
#include "ConstDef.h"
#include "ntoolscrtbthr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTBThr dialog

IMPLEMENT_DYNAMIC(NToolsCrTBThr, NToolsCrTurn)
NToolsCrTBThr::NToolsCrTBThr()
	: NToolsCrTurn(NToolsCrTBThr::IDD)
	, m_R(0.)
	, m_Thickness((MM_INCH == M_MM) ? 4.76 : 0.188)
	, m_ThreadAngle(60.)
	, m_CutWhide((MM_INCH == M_MM) ? 4.76 : 0.188)
	, m_CutLength((MM_INCH == M_MM) ? 10. : 0.33)
	, m_HalfT((MM_INCH == M_MM) ? 2.38 : 0.094)
	, m_AngleP(0)
	, m_FormTread(0)
{
}

NToolsCrTBThr::~NToolsCrTBThr()
{
}

void NToolsCrTBThr::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERT_RADIUS, m_R);
	DDX_Text(pDX, IDC_INSERT_THICKNESS, m_Thickness);
	DDX_Text(pDX, IDC_INSERT_THREADANGL, m_ThreadAngle);
	DDX_Text(pDX, IDC_INSERT_CUTWHIDE, m_CutWhide);
	DDX_Text(pDX, IDC_INSERT_CUTLENGTH, m_CutLength);
	DDX_Text(pDX, IDC_INSERT_HALFT, m_HalfT);
	DDX_Text(pDX, IDC_INSERT_ANGLFRONT, m_AngleP);
	DDX_Control(pDX, IDC_INSERT_RADIUS, m_R_ctrl);
	DDX_Control(pDX, IDC_INSERT_THICKNESS, m_Thickness_ctrl);
	DDX_Control(pDX, IDC_INSERT_THREADANGL, m_ThreadAngle_ctrl);
	DDX_Control(pDX, IDC_INSERT_CUTWHIDE, m_CutWhide_ctrl);
	DDX_Control(pDX, IDC_INSERT_CUTLENGTH, m_CutLength_ctrl);
	DDX_Control(pDX, IDC_INSERT_HALFT, m_HalfT_ctrl);
	DDX_Control(pDX, IDC_INSERT_ANGLFRONT, m_AngleP_ctrl);
	DDV_MinMaxDouble(pDX, m_R, 0, 1000);
	DDX_CBIndex(pDX, IDC_FORMTREAD, m_FormTread);
	DDX_Control(pDX, IDC_TURNTHR1, m_Pict);
}


BEGIN_MESSAGE_MAP(NToolsCrTBThr, NToolsCrTurn)
	ON_CBN_SELCHANGE(IDC_FORMTREAD, OnCbnSelchangeFormtread)
END_MESSAGE_MAP()


// NToolsCrTBThr message handlers

void NToolsCrTBThr::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnBladeThr *NToolsCrTBThr::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnBladeThr *pTool = new NToolTurnBladeThr;

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

void NToolsCrTBThr::OnCbnSelchangeFormtread()
{
	UpdateData(TRUE);
	SetPicture();
}

BOOL NToolsCrTBThr::OnInitDialog()
{
	NToolsCrTurn::OnInitDialog();

	SetPicture();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NToolsCrTBThr::SetPicture(void)
{
	PictBMP.DeleteObject();

	int ResourceID = IDB_TOOL_THREAD;
	
	switch( m_FormTread )
	{
	case 0:
		ResourceID = IDB_TOOL_THREAD;
		break;
	case 1:
		ResourceID = IDB_TOOL_THREAD2;
		break;
	case 2:
		ResourceID = IDB_TOOL_THREAD3;
		break;
	case 3:
		ResourceID = IDB_TOOL_THREAD4;
		break;
	}
	PictBMP.LoadBitmap(ResourceID);

	m_Pict.SetBitmap(HBITMAP(PictBMP));
}
