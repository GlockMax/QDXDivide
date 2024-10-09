// NModePainterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NModePainterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// NModePainterDlg dialog


NModePainterDlg::NModePainterDlg()
	   :  CPropertyPage(NModePainterDlg::IDD)
{
	//{{AFX_DATA_INIT(NModePainterDlg)
	m_CurveType = FALSE;
	m_ToolN = FALSE;
	m_Correction = FALSE;
	m_Coolant = FALSE;
	m_Feed = FALSE;
	m_Cycle = FALSE;
	m_NotPaint = FALSE;
	//}}AFX_DATA_INIT
	ModeType = 0;
	ModeID = 0;
}


void NModePainterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NModePainterDlg)
	DDX_Check(pDX, IDC_RADIO1, m_CurveType);
	DDX_Check(pDX, IDC_RADIO2, m_ToolN);
	DDX_Check(pDX, IDC_RADIO3, m_Correction);
	DDX_Check(pDX, IDC_RADIO4, m_CorrectLine);
	DDX_Check(pDX, IDC_RADIO6, m_Coolant);
	DDX_Check(pDX, IDC_RADIO7, m_Feed);
	DDX_Check(pDX, IDC_RADIO8, m_Cycle);
	DDX_Check(pDX, IDC_RADIO9, m_NotPaint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NModePainterDlg, CDialog)
	//{{AFX_MSG_MAP(NModePainterDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnClickCurveType)
	ON_BN_CLICKED(IDC_RADIO2, OnClickToolN)
	ON_BN_CLICKED(IDC_RADIO3, OnClickCoorect)
	ON_BN_CLICKED(IDC_RADIO6, OnClickCoolant)
	ON_BN_CLICKED(IDC_RADIO4, OnClickCorrectLine)
	ON_BN_CLICKED(IDC_RADIO5, OnClickCorrectCont)
	ON_BN_CLICKED(IDC_RADIO7, OnClickFeed)
	ON_BN_CLICKED(IDC_RADIO8, OnClickCycle)
	ON_BN_CLICKED(IDC_RADIO9, OnNotPaint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NModePainterDlg message handlers

void NModePainterDlg::OnClickCurveType() 
{
	if (!m_CurveType) SetModified(); 
	SetValues(1);
}

void NModePainterDlg::OnClickToolN() 
{
	if (!m_ToolN) SetModified(); 
	SetValues(2);
}

void NModePainterDlg::OnClickCoorect() 
{
	if (!m_Correction) SetModified(); 
	SetValues(3);
}

void NModePainterDlg::OnClickCorrectLine() 
{
	if (!m_Correction || !m_CorrectLine) SetModified(); 
	SetValues(31);
}

void NModePainterDlg::OnClickCorrectCont() 
{
	if (!m_Correction || m_CorrectLine) SetModified(); 
	SetValues(32);
}

void NModePainterDlg::OnClickCoolant() 
{
	if (!m_Coolant) SetModified(); 
	SetValues(4);
}

void NModePainterDlg::OnClickFeed() 
{
	if (!m_Feed) SetModified();
	SetValues(5);
}

void NModePainterDlg::OnClickCycle() 
{
	if (!m_Cycle) SetModified();
	SetValues(6);
}

void NModePainterDlg::OnNotPaint() 
{
	if (!m_NotPaint) SetModified();
	SetValues(0);
}

BOOL NModePainterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CorrectLine = true;
	CheckRadioButton(IDC_RADIO1, IDC_RADIO8, ModeID);
	switch(ModeType)
	{
	case 1:
		m_CurveType = true;
		CheckDlgButton(IDC_RADIO1, 1);
		break;
	case 2:
		m_ToolN = true;
		CheckDlgButton(IDC_RADIO2, 1);
		break;
	case 3:
		m_Correction = true;
		CheckDlgButton(IDC_RADIO3, 1);
		SetValues(3);
		break;
	case 31:
		m_Correction = true;
		m_CorrectLine = true;
		CheckDlgButton(IDC_RADIO4, 1);
		SetValues(31);
		break;
	case 32:
		m_Correction = true;
		m_CorrectLine = false;
		CheckDlgButton(IDC_RADIO5, 1);
		SetValues(32);
		break;
	case 4:
		m_Coolant = true;
		CheckDlgButton(IDC_RADIO6, 1);
		break;
	case 5:
		m_Feed = true;
		CheckDlgButton(IDC_RADIO7, 1);
		break;
	case 6:
		m_Cycle = true;
		CheckDlgButton(IDC_RADIO8, 1);
		break;
	case 0:
		m_NotPaint = true;
		CheckDlgButton(IDC_RADIO9, 1);
		break;
	default: // если не выбран не один из типов
		m_CurveType = true;
		CheckDlgButton(IDC_RADIO1, 1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NModePainterDlg::SetValues(int ChosenMode)
{
	if (ChosenMode == 1 || ChosenMode == 2 || ChosenMode == 4 ||
		ChosenMode == 5 || ChosenMode == 6 || ChosenMode == 0)
		CheckDlgButton(( m_CorrectLine ) ? IDC_RADIO4 : IDC_RADIO5, 0);	
		
	switch (ChosenMode)
	{
	case 3://IDC_RADIO3:	//case Correction:
		CheckDlgButton(( m_CorrectLine ) ? IDC_RADIO4 : IDC_RADIO5, 1);
		break;
	case 31://IDC_RADIO4:	//case CorrectLine:
		ModeID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
		CheckDlgButton(ModeID, 0);
		ModeID = GetCheckedRadioButton(IDC_RADIO6, IDC_RADIO9);
		CheckDlgButton(ModeID, 0);
		CheckDlgButton(IDC_RADIO3, 1);
		m_CorrectLine = true;
		break;
	case 32://IDC_RADIO5:	//case CorrcetCont:
		ModeID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
		CheckDlgButton(ModeID, 0);
		ModeID = GetCheckedRadioButton(IDC_RADIO6, IDC_RADIO9);
		CheckDlgButton(ModeID, 0);
		CheckDlgButton(IDC_RADIO3, 1);
		m_CorrectLine = false;
		break;
	}
}

void NModePainterDlg::UpdateControlData()
{
	if(!UpdateData())
		return;
	m_CurveType = false;
	m_ToolN = false;
	m_Correction = false;
	m_Coolant = false;
	m_Feed = false;
	m_Cycle = false;
	m_NotPaint = false;

	ModeID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);
	switch(ModeID)
	{
	case IDC_RADIO1:	//case CurveType:
		m_CurveType = true;
		break;
	case IDC_RADIO2:	//case ToolN:
		m_ToolN = true;
		break;
	case IDC_RADIO3:	//case Correction:
		m_Correction = true;
		break;
	}
	ModeID = GetCheckedRadioButton(IDC_RADIO6, IDC_RADIO9);
	switch(ModeID)
	{
	case IDC_RADIO6:	//case Coolant:
		m_Coolant = true;
		break;
	case IDC_RADIO7:	//case Feed:
		m_Feed = true;
		break;
	case IDC_RADIO8:	//case Cycle:
		m_Cycle = true;
		break;
	case IDC_RADIO9:	//case NotPaint:
		m_NotPaint = true;
		break;
	}
}

int NModePainterDlg::GetMode()
{
	UpdateControlData();

	int ChosenMode = 0;
	
	if (m_CurveType) 	ChosenMode = 1;
	else	if (m_ToolN)	 	ChosenMode = 2;
	else	if (m_Correction && m_CorrectLine)	ChosenMode = 31;
	else	if (m_Correction && !m_CorrectLine)	ChosenMode = 32;
	else	if (m_Coolant)	 	ChosenMode = 4;
	else	if (m_Feed)	 		ChosenMode = 5;
	else	if (m_Cycle)	 	ChosenMode = 6;
	else	if (m_NotPaint)	 	ChosenMode = 0;

	return ChosenMode;
}
