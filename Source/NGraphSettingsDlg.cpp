// NGraphSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NCMProject.h"
#include "NGraphSettingsDlg.h"
#include "NCMDoc.h"
#include "NTool.h"
#include "GAnimateCadr.h"
#include "NSurfSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NGraphSettingsDlg property page

IMPLEMENT_DYNCREATE(NGraphSettingsDlg, CMFCPropertyPage)

NGraphSettingsDlg::NGraphSettingsDlg() : CMFCPropertyPage(NGraphSettingsDlg::IDD)
{
	//{{AFX_DATA_INIT(NGraphSettingsDlg)
	m_Axes1Size = 0.0;
	m_Axes2Size = 0.0;
	m_Axes3Size = 0.0;
	m_CircleTolerance = 0.0;
	m_SelectionRegion = 0.0;
	m_SnapMarker = 0.0;
	m_ToolMarker = 0.0;
	m_ToolMash = _T("");
	m_weld_beamr = 0.1;
	m_VChange_Time = 500;
	m_mod_tolerance = 0.1;
	m_contexp_tolerance = MIND;
	m_TolLineApp = 0.01;
	m_DevArcSpan = 0.0002;
	m_MinRadArc = 0.05;
	m_ProgToolAxesLength = 30;
	m_ProgNodesSize = 3;
	m_ProgToolAxesThickness = 1;
	m_ConverterAcc = 0.2;
	m_ConverterAng = 1.2;
	//}}AFX_DATA_INIT
}

NGraphSettingsDlg::~NGraphSettingsDlg()
{
}

void NGraphSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NGraphSettingsDlg)
	DDX_Control(pDX, IDC_TOOL_MASH, m_ToolMashCtrl);
	DDX_Text(pDX, IDC_AXES1_SIZE, m_Axes1Size);
	DDV_MinMaxDouble(pDX, m_Axes1Size, 0., 500.);
	DDX_Text(pDX, IDC_AXES2_SIZE, m_Axes2Size);
	DDV_MinMaxDouble(pDX, m_Axes2Size, 0., 500.);
	DDX_Text(pDX, IDC_AXES3_SIZE, m_Axes3Size);
	DDV_MinMaxDouble(pDX, m_Axes3Size, 0., 500.);
	DDX_Text(pDX, IDC_CIRCLE_TOLERANCE, m_CircleTolerance);
	DDV_MinMaxDouble(pDX, m_CircleTolerance, 0.1, 100.);
	DDX_Text(pDX, IDC_SELECTION_REGION, m_SelectionRegion);
	DDV_MinMaxDouble(pDX, m_SelectionRegion, 0., 500.);
	DDX_Text(pDX, IDC_SNAP_MARKER, m_SnapMarker);
	DDV_MinMaxDouble(pDX, m_SnapMarker, 0., 500.);
	DDX_Text(pDX, IDC_TOOL_MARKER, m_ToolMarker);
	DDV_MinMaxDouble(pDX, m_ToolMarker, 0., 500.);
	DDX_CBString(pDX, IDC_TOOL_MASH, m_ToolMash);
	DDX_Text(pDX, IDC_WELD_BEAMR, m_weld_beamr);
	DDV_MinMaxDouble(pDX, m_weld_beamr, 0., 1.);
	DDX_Text(pDX, IDC_VCHANGE_TIME, m_VChange_Time);
	DDV_MinMaxUInt(pDX, m_VChange_Time, 0, 10000);
	DDX_Text(pDX, IDC_MOD_TOLERANCE, m_mod_tolerance);
	DDV_MinMaxDouble(pDX, m_mod_tolerance, MIN_TOLERANCE, 10.);
	DDX_Text(pDX, IDC_CONTEXP_TOL, m_contexp_tolerance);
	DDV_MinMaxDouble(pDX, m_contexp_tolerance, 0., 0.01);
	DDX_Text(pDX, IDC_TOLLINEAPP, m_TolLineApp);
	DDX_Text(pDX, IDC_DEVARCSPAN, m_DevArcSpan);
	DDX_Text(pDX, IDC_MINRADARC, m_MinRadArc);
	DDX_Text(pDX, IDC_TOOLVECTOR_LENGTH, m_ProgToolAxesLength);
	DDV_MinMaxDouble(pDX, m_ProgToolAxesLength, 5., 100.);
	DDX_Text(pDX, IDC_PATHPOINT_THICKNESS, m_ProgNodesSize);
	DDV_MinMaxDouble(pDX, m_ProgNodesSize, 1., 100.);
	DDX_Text(pDX, IDC_TOOLVECTOR_THICKNESS, m_ProgToolAxesThickness);
	DDV_MinMaxDouble(pDX, m_ProgToolAxesThickness, 1., 10.);
	DDX_Text(pDX, IDC_CONVERTERSTL_ACCURACY, m_ConverterAcc);
	DDV_MinMaxDouble(pDX, m_ConverterAcc, 0., 100.);
	DDX_Text(pDX, IDC_CONVERTERSTL_MINANGLE, m_ConverterAng);
	DDV_MinMaxDouble(pDX, m_ConverterAng, 0., 100.);
	DDX_Control(pDX, IDC_CUT_HELIX, m_CutHelix);
}


BEGIN_MESSAGE_MAP(NGraphSettingsDlg, CMFCPropertyPage)
	//{{AFX_MSG_MAP(NGraphSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NGraphSettingsDlg message handlers
static CString ToolMashThick;
static CString ToolMashAverage;
static CString ToolMashRare;

BOOL NGraphSettingsDlg::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();
	
	ToolMashThick.LoadString(Thick);
	ToolMashAverage.LoadString(AVERAGE);
	ToolMashRare.LoadString(RARE);
	int *pInt;
	if(NCM_PROJECT.IntParams.Lookup("ToolMash", pInt))
	{
		switch(*pInt)
		{
		case Thick:
			m_ToolMash = ToolMashThick;
			break;
		case AVERAGE:
			m_ToolMash = ToolMashAverage;
			break;
		case RARE:
			m_ToolMash = ToolMashRare;
			break;
		}
	}
	m_ToolMashCtrl.AddString(ToolMashThick);
	m_ToolMashCtrl.AddString(ToolMashAverage);
	m_ToolMashCtrl.AddString(ToolMashRare);
	if(NCM_PROJECT.IntParams.Lookup("ViewChangeTime", pInt))
		m_VChange_Time = *pInt;
	if(NCM_PROJECT.IntParams.Lookup("CutHelix", pInt))
		m_CutHelix.SetCheck(*pInt != 0);

	double *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("CircleTolerance", pDouble))
		m_CircleTolerance = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("SelectionRegion", pDouble))
		m_SelectionRegion = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("ToolMarker", pDouble))
		m_ToolMarker = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("SnapMarker", pDouble))
		m_SnapMarker = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("Axes1Size", pDouble))
		m_Axes1Size = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("Axes2Size", pDouble))
		m_Axes2Size = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("Axes3Size", pDouble))
		m_Axes3Size = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("WeldBeamR", pDouble))
		m_weld_beamr = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("TolLineApp", pDouble))
		m_TolLineApp = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("MinRadArc", pDouble))
		m_MinRadArc = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("ProgToolAxesLength", pDouble))
		m_ProgToolAxesLength = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("ProgNodesSize", pDouble))
		m_ProgNodesSize = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("DevArcSpan", pDouble))
		m_DevArcSpan = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("ToolAxesThickness", pDouble))
		m_ProgToolAxesThickness = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("ConverterAcc", pDouble))
		m_ConverterAcc = *pDouble;
	if (NCM_PROJECT.DoubleParams.Lookup("ConverterAng", pDouble))
		m_ConverterAng = *pDouble;
	CString ModToleranceKey;
	switch(NCMComData::GetType())
	{
	case AT_MILL:
		ModToleranceKey = _T("ModTolerance");
		break;
	case AT_TURN:
		ModToleranceKey = _T("ModToleranceTurn");
		break;
	case AT_MILL_TURN:
		ModToleranceKey = _T("ModToleranceMillTurn");
		break;
	case AT_WELD:
		ModToleranceKey = _T("ModToleranceWeld");
		break;
	}
	if(NCM_PROJECT.DoubleParams.Lookup(ModToleranceKey, pDouble))
		m_mod_tolerance = *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("ContTolerance", pDouble))
		m_contexp_tolerance = *pDouble;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NGraphSettingsDlg::OnOK() 
{
	if(!UpdateData())
		return;
	int *pInt;
	if(NCM_PROJECT.IntParams.Lookup("ToolMash", pInt))
	{
		if(m_ToolMash == ToolMashThick)
			*pInt = Thick;
		if(m_ToolMash == ToolMashAverage)
			*pInt = AVERAGE;
		if(m_ToolMash == ToolMashRare)
			*pInt = RARE;
	}
	if(NCM_PROJECT.IntParams.Lookup("ViewChangeTime", pInt))
		*pInt = m_VChange_Time;
	if(NCM_PROJECT.IntParams.Lookup("CutHelix", pInt))
		*pInt = m_CutHelix.GetCheck();
	bool NeedRegenPrograms = false;
	double *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("CircleTolerance", pDouble))
		*pDouble = m_CircleTolerance;
	if(NCM_PROJECT.DoubleParams.Lookup("SelectionRegion", pDouble))
		*pDouble = m_SelectionRegion;
	if(NCM_PROJECT.DoubleParams.Lookup("ToolMarker", pDouble))
		*pDouble = m_ToolMarker;
	if(NCM_PROJECT.DoubleParams.Lookup("SnapMarker", pDouble))
		*pDouble = m_SnapMarker;
	if(NCM_PROJECT.DoubleParams.Lookup("Axes1Size", pDouble))
		*pDouble = m_Axes1Size;
	if(NCM_PROJECT.DoubleParams.Lookup("Axes2Size", pDouble))
		*pDouble = m_Axes2Size;
	if(NCM_PROJECT.DoubleParams.Lookup("Axes3Size", pDouble))
		*pDouble = m_Axes3Size;
	if(NCM_PROJECT.DoubleParams.Lookup("WeldBeamR", pDouble))
		*pDouble = m_weld_beamr;
	if(NCM_PROJECT.DoubleParams.Lookup("ProgToolAxesLength", pDouble))
		*pDouble = m_ProgToolAxesLength;
	if(NCM_PROJECT.DoubleParams.Lookup("ProgNodesSize", pDouble))
		*pDouble = m_ProgNodesSize;
	bool ResetConverter = false;
	if (NCM_PROJECT.DoubleParams.Lookup("ConverterAcc", pDouble))
	{
		if (*pDouble != m_ConverterAcc)
		{
			*pDouble = m_ConverterAcc;
			ResetConverter = true;
		}
	}
	if (NCM_PROJECT.DoubleParams.Lookup("ConverterAng", pDouble))
	{
		if (*pDouble != m_ConverterAng)
		{
			*pDouble = m_ConverterAng;
			ResetConverter = true;
		}
	}
	if (NCM_PROJECT.DoubleParams.Lookup("ProgToolAxesThickness", pDouble))
		*pDouble = m_ProgToolAxesThickness;
	if(NCM_PROJECT.DoubleParams.Lookup("TolLineApp", pDouble))
		if (*pDouble != m_TolLineApp)
		{
			*pDouble = m_TolLineApp;
			NeedRegenPrograms = true;
		}
	if(NCM_PROJECT.DoubleParams.Lookup("MinRadArc", pDouble))
		if (*pDouble != m_MinRadArc)
		{
			*pDouble = m_MinRadArc;
			NeedRegenPrograms = true;
		}
	if(NCM_PROJECT.DoubleParams.Lookup("DevArcSpan", pDouble))
		if (*pDouble != m_DevArcSpan)
		{
			*pDouble = m_DevArcSpan;
			NeedRegenPrograms = true;
		}
	CString ModToleranceKey;
	switch(NCMComData::GetType())
	{
	case AT_MILL:
		ModToleranceKey = _T("ModTolerance");
		break;
	case AT_TURN:
		ModToleranceKey = _T("ModToleranceTurn");
		break;
	case AT_MILL_TURN:
		ModToleranceKey = _T("ModToleranceMillTurn");
		break;
	case AT_WELD:
		ModToleranceKey = _T("ModToleranceWeld");
		break;
	}
	if(NCM_PROJECT.DoubleParams.Lookup(ModToleranceKey, pDouble))
	{
		*pDouble = m_mod_tolerance;
		NSurfSettings::SetTolerance(m_mod_tolerance);
		((CNCMDoc *)((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument())->RegenerateTools();
	}
	if(NCM_PROJECT.DoubleParams.Lookup("ContTolerance", pDouble))
		*pDouble = m_contexp_tolerance;
	if (NeedRegenPrograms)
		((CNCMDoc *)((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument())->RegeneratePrograms();

	if (ResetConverter)
		MTEdit::ResetSTLConverter();
}

