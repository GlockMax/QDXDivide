// NSectArrCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSectArrParams.h"
#include "NSectArrCDlg.h"


// NSectArrCDlg dialog

IMPLEMENT_DYNAMIC(NSectArrCDlg, NSectArrDlg )

NSectArrCDlg::NSectArrCDlg() : NSectArrDlg(NSectArrCDlg::IDD)
	, m_sangl(0)
	, m_eangl(0)
	, m_counter(0)
	, m_ix2(0)
	, m_iy2(0)
	, m_iz2(0)
{

}

NSectArrCDlg::~NSectArrCDlg()
{
}

void NSectArrCDlg::DoDataExchange(CDataExchange* pDX)
{
	SetModified();
	NSectArrDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SLW_SANGL, m_sangl);
	DDX_Text(pDX, IDC_WLS_EANGL, m_eangl);
	DDX_Text(pDX, IDC_WLS_SNUM, m_counter);
	DDX_Text(pDX, IDC_WLS_IX2, m_ix2);
	DDX_Text(pDX, IDC_WLS_IY2, m_iy2);
	DDX_Text(pDX, IDC_WLS_IZ2, m_iz2);
}


BEGIN_MESSAGE_MAP(NSectArrCDlg, CMFCPropertyPage )
	ON_BN_CLICKED(IDC_COLORBYPART, &NSectArrCDlg::OnBnClickedColorbypart)
END_MESSAGE_MAP()


// NSectArrCDlg message handlers
void NSectArrCDlg::SetParams(const NSectArrParams &SectParams)
{
	NSectArrDlg::SetParams(SectParams);
	double b;
	SectParams.StartDir.Get(m_ix2, m_iy2, m_iz2, b);
	m_sangl = SectParams.StartVal;
	m_eangl = SectParams.EndVal;
	m_counter = SectParams.IntervalsNum;
}
const NSectArrParams NSectArrCDlg::GetParams(void)
{
	UpdateData();
	NSectArrParams SectParams = NSectArrDlg::GetParams();
	SectParams.Type = SAT_CIRCLE;
	SectParams.StartDir.Set(m_ix2, m_iy2, m_iz2, 0.);
	SectParams.StartVal = m_sangl;
	SectParams.EndVal = m_eangl;
	SectParams.IntervalsNum = m_counter;
	return SectParams;
}

void NSectArrCDlg::OnBnClickedColorbypart()
{
	NSectArrDlg::OnBnClickedColorbypart();
}

