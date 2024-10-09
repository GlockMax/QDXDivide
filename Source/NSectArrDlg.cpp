// NSectArrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSectArrParams.h"
#include "NSectArrDlg.h"
#include "ResLocal.h"

// NSectArrDlg dialog

IMPLEMENT_DYNAMIC(NSectArrDlg, CMFCPropertyPage )

NSectArrDlg::NSectArrDlg() : CMFCPropertyPage()
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_ix(0)
	, m_iy(0)
	, m_iz(0)
	, m_thick(0)
	, m_colorbypart(FALSE)
{

}
NSectArrDlg::NSectArrDlg(UINT ResourceID) : CMFCPropertyPage(ResourceID)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_ix(0)
	, m_iy(0)
	, m_iz(0)
	, m_thick(0)
	, m_colorbypart(FALSE)
{

}

NSectArrDlg::~NSectArrDlg()
{
}

void NSectArrDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WLS_X, m_x);
	DDX_Text(pDX, IDC_WLS_Y, m_y);
	DDX_Text(pDX, IDC_WLS_Z, m_z);
	DDX_Text(pDX, IDC_WLS_IX, m_ix);
	DDX_Text(pDX, IDC_WLS_IY, m_iy);
	DDX_Text(pDX, IDC_WLS_IZ, m_iz);
	DDX_CBIndex(pDX, IDC_WS_THICK, m_thick);
	DDX_Control(pDX, IDC_WS_THICK, m_widthctrl);
	DDX_Control(pDX, IDC_COLOR, m_color);
	DDX_Check(pDX, IDC_COLORBYPART, m_colorbypart);
}


BEGIN_MESSAGE_MAP(NSectArrDlg, CMFCPropertyPage )
END_MESSAGE_MAP()


// NSectArrDlg message handlers
void NSectArrDlg::SetParams(const NSectArrParams &iSectParams)
{
	SectParams = iSectParams;
}
const NSectArrParams NSectArrDlg::GetParams(void)
{
	UpdateData();
	NSectArrParams SectParams;
	SectParams.Type = SAT_LINE;
	SectParams.P.Set(m_x, m_y, m_z, 1.);
	SectParams.V.Set(m_ix, m_iy, m_iz, 0.);
	SectParams.LineThickness = m_thick + 1;
	SectParams.ColorByPart = (m_colorbypart == TRUE);
	SectParams.Color.SetRGB(m_color.GetColor());
	return SectParams;
}

BOOL NSectArrDlg::OnInitDialog()
{
//	UpdateData(FALSE);
	CMFCPropertyPage::OnInitDialog();
	m_widthctrl.Init();
	double b;
	SectParams.P.Get(m_x, m_y, m_z, b);
	SectParams.V.Get(m_ix, m_iy, m_iz, b);
	m_thick = int(SectParams.LineThickness - 1.);
	m_colorbypart = BOOL(SectParams.ColorByPart);
	if(m_colorbypart)
	{
		m_color.SetColor(SectParams.PartColor);
		m_color.Enable(false);
	}
	else
	{
		m_color.SetColor(SectParams.Color);
		m_color.Enable(true);
	}
	m_color.Update();
	//OnBnClickedColorbypart();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NSectArrDlg::OnBnClickedColorbypart()
{
	UpdateData(TRUE);
	if(m_colorbypart)
	{
		m_color.SetColor(SectParams.PartColor);
		m_color.Update();
		m_color.Enable(false);
	}
	else
		m_color.Enable(true);

	UpdateData(FALSE);
}
