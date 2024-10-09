// NToolsCrTBCut.cpp : implementation file
//

#include "stdafx.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTBCut.h"
#include "ConstDef.h"
#include "ntoolscrtbcut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NToolsCrTBCut dialog

IMPLEMENT_DYNAMIC(NToolsCrTBCut, NToolsCrTurn)
NToolsCrTBCut::NToolsCrTBCut()
	: NToolsCrTurn(NToolsCrTBCut::IDD)
{
	CString Key = "Defaults/Tools/Turn/Blade/Slot";
	NTiParams &Par = NCM_PROJECT.Defaults;

	m_AngleB = Par.GetDouble((Key + "@A").GetBuffer(), 0.);
	m_AngleP = Par.GetDouble((Key + "@C").GetBuffer(), 3.);
	m_Edge = int(Par.GetDouble((Key + "@Edge").GetBuffer(), 0));
	if(MM_INCH == M_MM)
	{
		Key += "/mm";
		m_R = Par.GetDouble((Key + "@R1").GetBuffer(), 0.5);
		m_R1 = Par.GetDouble((Key + "@R2").GetBuffer(), 0.5);
		m_Length = Par.GetDouble((Key + "@L").GetBuffer(), 20.);
		m_Whidth = Par.GetDouble((Key + "@T").GetBuffer(), 5.);
		m_Thickness = Par.GetDouble((Key + "@H").GetBuffer(), 4.76);
	}
	else
	{
		Key += "/inch";
		m_R = Par.GetDouble((Key + "@R1").GetBuffer(), 0.016);
		m_R1 = Par.GetDouble((Key + "@R2").GetBuffer(), 0.016);
		m_Length = Par.GetDouble((Key + "@L").GetBuffer(), 0.75);
		m_Whidth = Par.GetDouble((Key + "@T").GetBuffer(), 0.188);
		m_Thickness = Par.GetDouble((Key + "@H").GetBuffer(), 0.188);
	}
}

NToolsCrTBCut::~NToolsCrTBCut()
{
}

void NToolsCrTBCut::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_INSERT_ANGLMAIN, m_AngleA);
//	DDV_MinMaxDouble(pDX, m_AngleA, 0, 180);
	DDX_Text(pDX, IDC_INSERT_ANGLADD, m_AngleB);
	DDX_Text(pDX, IDC_INSERT_ANGLFRONT, m_AngleP);
	DDX_Text(pDX, IDC_INSERT_RADIUS, m_R);
	DDX_Text(pDX, IDC_INSERT_THICKNESS, m_Thickness);
	DDX_Text(pDX, IDC_INSERT_CUTWHIDE, m_Whidth);
	DDX_Text(pDX, IDC_INSERT_CUTLENGTH, m_Length);
	DDX_Text(pDX, IDC_INSERT_RADIUS2, m_R1);
	DDX_Control(pDX, IDC_INSERT_ANGLADD, m_AngleB_ctrl);
	DDX_Control(pDX, IDC_INSERT_ANGLFRONT, m_AngleP_ctrl);
	DDX_Control(pDX, IDC_INSERT_RADIUS, m_R_ctrl);
	DDX_Control(pDX, IDC_INSERT_THICKNESS, m_Thickness_ctrl);
	DDX_Control(pDX, IDC_INSERT_CUTWHIDE, m_Whidth_ctrl);
	DDX_Control(pDX, IDC_INSERT_CUTLENGTH, m_Length_ctrl);
	DDX_Control(pDX, IDC_INSERT_RADIUS2, m_R1_ctrl);
	DDV_MinMaxDouble(pDX, m_AngleB, -89, 89);
	DDV_MinMaxDouble(pDX, m_AngleP, 0, 180);
	DDV_MinMaxDouble(pDX, m_R, 0, 1000);
	DDX_Radio(pDX, IDC_E1, m_Edge);
	DDX_Control(pDX, IDC_INSERT_CORNER, m_Pict);
}


BEGIN_MESSAGE_MAP(NToolsCrTBCut, NToolsCrTurn)
	ON_BN_CLICKED(IDC_E1, OnBnClickedE1)
	ON_BN_CLICKED(IDC_E2, OnBnClickedE2)
END_MESSAGE_MAP()


// NToolsCrTBCut message handlers

void NToolsCrTBCut::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnBladeCut *NToolsCrTBCut::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnBladeCut *pTool = new NToolTurnBladeCut;

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

void NToolsCrTBCut::OnBnClickedE1()
{
	UpdateData(TRUE);
	DefEdgePicture();
}

void NToolsCrTBCut::OnBnClickedE2()
{
	UpdateData(TRUE);
	DefEdgePicture();
}

void NToolsCrTBCut::DefEdgePicture(void)
{
	PictBMP.DeleteObject();
	
	int ResourceID = IDB_INSERT_CORNER1;
	
	switch( m_Edge )
	{
	case 0:
		ResourceID = IDB_INSERT_CORNER1;
		break;
	case 1:
		ResourceID = IDB_INSERT_CORNER2;
		break;
	}
	PictBMP.LoadBitmap(ResourceID);

	m_Pict.SetBitmap(HBITMAP(PictBMP));
}

BOOL NToolsCrTBCut::OnInitDialog()
{
	NToolsCrTurn::OnInitDialog();

	DefEdgePicture();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
