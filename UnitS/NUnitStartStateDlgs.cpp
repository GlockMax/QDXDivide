// NUnitStartStateDlgs.cpp : implementation file
//

#include "stdafx.h"
#include "NUnitStartStateDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NUnitPositionDlg property page
static DWORD aHelpIDs[]=
{
	IDC_START_X,					HIDC_START_X,
	IDC_START_Y,					HIDC_START_Y,
	IDC_START_Z,					HIDC_START_Z,
	IDC_TOOL_POS,					HIDC_TOOL_POS,
	IDC_DEBUG,						HIDC_DEBUG,
	IDC_FEED_MEAS_MMpMIN,			HIDC_FEED_MEAS_MMpMIN,
	IDC_FEED_MEAS_MMpREV,			HIDC_FEED_MEAS_MMpREV,
	IDC_FEED,						HIDC_FEED,
	IDC_FEED_SCALE,					HIDC_FEED_SCALE,
	IDC_ABSOLUTE_COORD_PROGR,		HIDC_ABSOLUTE_COORD_PROGR,
	IDC_RELATIVE_COORD_PROGR,		HIDC_RELATIVE_COORD_PROGR,
	IDC_MEAS_MM,					HIDC_MEAS_MM,
	IDC_MEAS_INCH,					HIDC_MEAS_INCH,
	0,0
};

IMPLEMENT_DYNCREATE(NUnitPositionDlg, CPropertyPage)

NUnitPositionDlg::NUnitPositionDlg() : CPropertyPage(NUnitPositionDlg::IDD)
{
	//{{AFX_DATA_INIT(NUnitPositionDlg)
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_tool_pos = _T("");
	m_Debug = FALSE;
	//}}AFX_DATA_INIT
}

NUnitPositionDlg::~NUnitPositionDlg()
{
}

void NUnitPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitPositionDlg)
	DDX_Text(pDX, IDC_START_X, m_x);
	DDX_Text(pDX, IDC_START_Y, m_y);
	DDX_Text(pDX, IDC_START_Z, m_z);
	DDX_Text(pDX, IDC_START_A, m_a);
	DDX_Text(pDX, IDC_START_B, m_b);
	DDX_Text(pDX, IDC_START_C, m_c);
	DDX_Control(pDX, IDC_START_X, m_x_ctrl);
	DDX_Control(pDX, IDC_START_Y, m_y_ctrl);
	DDX_Control(pDX, IDC_START_Z, m_z_ctrl);
	DDX_Control(pDX, IDC_START_A, m_a_ctrl);
	DDX_Control(pDX, IDC_START_B, m_b_ctrl);
	DDX_Control(pDX, IDC_START_C, m_c_ctrl);
	DDX_Text(pDX, IDC_TOOL_POS, m_tool_pos);
	DDX_Check(pDX, IDC_DEBUG, m_Debug);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NUnitPositionDlg, CPropertyPage)
	//{{AFX_MSG_MAP(NUnitPositionDlg)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NUnitPositionDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// NUnitSpeedDlg property page

IMPLEMENT_DYNCREATE(NUnitSpeedDlg, CPropertyPage)

NUnitSpeedDlg::NUnitSpeedDlg() : CPropertyPage(NUnitSpeedDlg::IDD)
{
	//{{AFX_DATA_INIT(NUnitSpeedDlg)
	m_feed = _T("");
	m_feed_scale = _T("");
	//}}AFX_DATA_INIT
}

NUnitSpeedDlg::~NUnitSpeedDlg()
{
}

void NUnitSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitSpeedDlg)
	DDX_Control(pDX, IDC_FEED_SCALE, m_feed_scale_ctrl);
	DDX_Control(pDX, IDC_FEED, m_feed_ctrl);
	DDX_CBString(pDX, IDC_FEED, m_feed);
	DDX_CBString(pDX, IDC_FEED_SCALE, m_feed_scale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NUnitSpeedDlg, CPropertyPage)
	//{{AFX_MSG_MAP(NUnitSpeedDlg)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NUnitSpeedDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// NUnitModeDlg property page

IMPLEMENT_DYNCREATE(NUnitModeDlg, CPropertyPage)

NUnitModeDlg::NUnitModeDlg() : CPropertyPage(NUnitModeDlg::IDD)
{
	//{{AFX_DATA_INIT(NUnitModeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

NUnitModeDlg::~NUnitModeDlg()
{
}

void NUnitModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NUnitModeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NUnitModeDlg, CPropertyPage)
	//{{AFX_MSG_MAP(NUnitModeDlg)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NUnitModeDlg message handlers

BOOL NUnitSpeedDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CheckDlgButton(( is_feed_mm_per_min ) ? IDC_FEED_MEAS_MMpMIN : IDC_FEED_MEAS_MMpREV, 1);
	m_feed_ctrl.AddString(_T("0"));
	m_feed_ctrl.AddString(_T("100"));
	m_feed_ctrl.AddString(_T("200"));
	m_feed_ctrl.SelectString(-1, m_feed);
	CString str;
	for(int i=10; i<=120; i+=10)
	{
		str.Format(_T("%d"),i);
		m_feed_scale_ctrl.AddString(str);
	}
	m_feed_scale_ctrl.SelectString(-1, m_feed_scale);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NUnitSpeedDlg::OnOK() 
{
	is_feed_mm_per_min = (IsDlgButtonChecked(IDC_FEED_MEAS_MMpMIN) != 0);
	CPropertyPage::OnOK();
}

BOOL NUnitModeDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CheckDlgButton(( absolute_coord ) ? IDC_ABSOLUTE_COORD_PROGR : IDC_RELATIVE_COORD_PROGR, 1);
	CheckDlgButton(( metric_coord ) ? IDC_MEAS_MM : IDC_MEAS_INCH, 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NUnitModeDlg::OnOK() 
{
	absolute_coord = IsDlgButtonChecked(IDC_ABSOLUTE_COORD_PROGR);
	metric_coord = IsDlgButtonChecked(IDC_MEAS_MM);
	
	CPropertyPage::OnOK();
}
