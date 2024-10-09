// NToolsCrTBISO.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTBISO.h"
#include "ntoolscrtbiso.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsCrTBISO dialog

IMPLEMENT_DYNAMIC(NToolsCrTBISO, NToolsCrTurn)

BEGIN_MESSAGE_MAP(NToolsCrTBISO, NToolsCrTurn)
	ON_BN_CLICKED(IDC_UNIT_INFO, OnBnClickedUnitInfo)
	ON_CBN_SELCHANGE(IDC_FORMISO, OnCbnSelchangeFormiso)
	ON_BN_CLICKED(IDC_E1, OnBnClickedE1)
	ON_BN_CLICKED(IDC_E2, OnBnClickedE2)
END_MESSAGE_MAP()

NToolsCrTBISO::NToolsCrTBISO()
	: NToolsCrTurn((MM_INCH == M_MM) ? IDD_TOOLS_CREATE_TURNISO : IDD_TOOLS_CREATE_TURNISO_INCH )
{
	CString Key = "Defaults/Tools/Turn/Blade/ISO";
	NTiParams &Par = NCM_PROJECT.Defaults;

	m_Edge = int(Par.GetDouble((Key + "@Edge").GetBuffer(), 0));
	m_Form = Par.GetString((Key + "@InsertForm").GetBuffer(), "E");
	m_BackAngle = Par.GetString((Key + "@C").GetBuffer(), "A");
	if(MM_INCH == M_MM)
	{
		Key += "/mm";
		m_L1 = Par.GetDouble((Key + "@L1").GetBuffer(), 12.);
		m_L2 = Par.GetDouble((Key + "@L2").GetBuffer(), 20.);
		m_SRad = Par.GetString((Key + "@R").GetBuffer(), "0.4");
		m_SThickness = Par.GetString((Key + "@H").GetBuffer(), "3.18");
	}
	else
	{
		Key += "/inch";
		m_L1 = Par.GetDouble((Key + "@L1").GetBuffer(), 0.65);
		m_L2 = Par.GetDouble((Key + "@L2").GetBuffer(), 0.75);
		m_SRad = Par.GetString((Key + "@R").GetBuffer(), "0.0156");
		m_SThickness = Par.GetString((Key + "@H").GetBuffer(), "0.125");
	}

}

NToolsCrTBISO::~NToolsCrTBISO()
{
}

void NToolsCrTBISO::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrTurn::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERT_LENGTHCUT, m_L1);
	DDX_Text(pDX, IDC_INSERT_LENGTHADD, m_L2);
	DDX_Control(pDX, IDC_INSERT_LENGTHCUT, m_L1_ctrl);
	DDX_Control(pDX, IDC_INSERT_LENGTHADD, m_L2_ctrl);
	DDV_MinMaxDouble(pDX, m_L1, 0, 1000);
	DDV_MinMaxDouble(pDX, m_L2, 0, 1000);
	DDX_CBString(pDX, IDC_FORMISO, m_Form);
	DDX_CBString(pDX, IDC_BACKANGLE, m_BackAngle);
	DDX_CBString(pDX, IDC_COMBO43, m_SThickness);
	DDX_CBString(pDX, IDC_COMBO44, m_SRad);
	DDX_Control(pDX, IDC_FORMISO, m_CForm);
	DDX_Control(pDX, IDC_BACKANGLE, m_CBackAngle);
	DDX_Control(pDX, IDC_COMBO43, m_CSThikness);
	DDX_Control(pDX, IDC_COMBO44, m_CSRad);
	DDX_Control(pDX, IDC_TURNOUT6, m_Pict6);
	DDX_Control(pDX, IDC_INSERT_CORNER, m_EdgePict);
	DDX_Control(pDX, IDC_TURNOUT, m_Pict);
	DDX_Radio(pDX, IDC_E1, m_Edge);
}




// NToolsCrTBISO message handlers

void NToolsCrTBISO::OnOK()
{
	NToolsCrTurn::OnOK();
}
NToolTurnBladeISO *NToolsCrTBISO::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolTurnBladeISO *pTool = new NToolTurnBladeISO;

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

BOOL NToolsCrTBISO::OnInitDialog()
{
	NToolsCrTurn::OnInitDialog();

	if(m_Form.GetLength() < 1)
	{
		m_CForm.SetCurSel(4);
		m_Form = "T";
	}
	if(m_SThickness.GetLength() < 1)
		m_CSThikness.SetCurSel(7);
	if(m_SRad.GetLength() < 1)
		m_CSRad.SetCurSel(2);
	if(m_BackAngle.GetLength() < 1)
		m_CBackAngle.SetCurSel(1);

	Pict6BMP.DeleteObject();
	GetDlgItem(IDC_E1)->EnableWindow(FALSE);
	GetDlgItem(IDC_E2)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO44)->EnableWindow(TRUE);

	int ResourceID = IDB_TOOL_TRIANGLE;
	
	switch( m_Form[0] )
	{
	case 'H':
		m_Edge = 0;
		ResourceID = IDB_TOOL_HEX;
		break;
	case 'O':
		m_Edge = 0;
		ResourceID = IDB_TOOL_OCT;
		break;
	case 'P':
		m_Edge = 0;
		ResourceID = IDB_TOOL_PENT;
		break;
	case 'S':
		m_Edge = 0;
		ResourceID = IDB_TOOL_SQUARE;
		break;
	case 'T':
		m_Edge = 0;
		ResourceID = IDB_TOOL_TRIANGLE;
		break;
	case 'C':
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND80;
		break;
	case 'D':
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND55;
		break;
	case 'E':
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND75;
		break;
	case 'F':
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND50;
		break;
	case 'M':
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND86;
		break;
	case 'V':
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND35;
		break;
	case 'W':
		m_Edge = 0;
		ResourceID = IDB_TOOL_TRIGON;
		break;
	case 'L':
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_RECT;
		ResourceID = IDB_TOOL_RECT;
		break;
	case 'A':
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_RECT;
		ResourceID = IDB_TOOL_PARAL85;
		break;
	case 'B':
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_RECT;
		ResourceID = IDB_TOOL_PARAL82;
		break;
	case 'K':
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_RECT;
		ResourceID = IDB_TOOL_PARAL55;
		break;
	case 'R':
		m_Edge = 0;
		GetDlgItem(IDC_COMBO44)->EnableWindow(FALSE);
		ResourceID = IDB_TOOL_RND;
		break;
	}
	Pict6BMP.LoadBitmap(ResourceID);

	m_Pict.SetBitmap(HBITMAP(Pict6BMP));
//	UpdateData(FALSE);
	DefEdgePicture();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NToolsCrTBISO::OnCbnSelchangeFormiso()
{
//	UpdateData(TRUE);
	Pict6BMP.DeleteObject();
	GetDlgItem(IDC_E1)->EnableWindow(FALSE);
	GetDlgItem(IDC_E2)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO44)->EnableWindow(TRUE);

	int ResourceID = IDB_TOOL_TRIANGLE;
	
	switch( m_CForm.GetCurSel() )
	{
	case 0:
		m_Edge = 0;
		ResourceID = IDB_TOOL_HEX;
		break;
	case 1:
		m_Edge = 0;
		ResourceID = IDB_TOOL_OCT;
		break;
	case 2:
		m_Edge = 0;
		ResourceID = IDB_TOOL_PENT;
		break;
	case 3:
		m_Edge = 0;
		ResourceID = IDB_TOOL_SQUARE;
		break;
	case 4:
		m_Edge = 0;
		ResourceID = IDB_TOOL_TRIANGLE;
		break;
	case 5:
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND80;
		break;
	case 6:
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND55;
		break;
	case 7:
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND75;
		break;
	case 8:
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND50;
		break;
	case 9:
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND86;
		break;
	case 10:
		m_Edge = 0;
		ResourceID = IDB_TOOL_DIAMOND35;
		break;
	case 11:
		m_Edge = 0;
		ResourceID = IDB_TOOL_TRIGON;
		break;
	case 12:
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_RECT;
		break;
	case 13:
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_PARAL85;
		break;
	case 14:
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_PARAL82;
		break;
	case 15:
		GetDlgItem(IDC_E1)->EnableWindow(TRUE);
		GetDlgItem(IDC_E2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_LENGTHCUT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_INSERT_CORNER)->EnableWindow(TRUE);
		ResourceID = IDB_TOOL_PARAL55;
		break;
	case 16:
		m_Edge = 0;
		GetDlgItem(IDC_COMBO44)->EnableWindow(FALSE);
		ResourceID = IDB_TOOL_RND;
		break;
	}
	Pict6BMP.LoadBitmap(ResourceID);

	m_Pict.SetBitmap(HBITMAP(Pict6BMP));
//	UpdateData(FALSE);
	DefEdgePicture();
}

void NToolsCrTBISO::OnBnClickedE1()
{
	UpdateData(TRUE);
	DefEdgePicture();
}

void NToolsCrTBISO::OnBnClickedE2()
{
	UpdateData(TRUE);
	DefEdgePicture();
}

void  NToolsCrTBISO::DefEdgePicture(void)
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

	m_EdgePict.SetBitmap(HBITMAP(PictBMP));
}

double NToolsCrTBISO::GetRad()
{
	if(m_Form[0] != 'R')
		return NToolTurnBladeISO::GetVal(m_SRad); 
	else
		return m_L1; 

}
