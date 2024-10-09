// NSectArrLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSectArrParams.h"
#include "NSectArrLDlg.h"


// NSectArrLDlg dialog

IMPLEMENT_DYNAMIC(NSectArrLDlg, NSectArrDlg )

NSectArrLDlg::NSectArrLDlg() : NSectArrDlg(NSectArrLDlg::IDD)
	, m_left(FALSE)
	, m_right(FALSE)
	, m_counter(0)
	, m_step(0)
{

}

NSectArrLDlg::~NSectArrLDlg()
{
}

void NSectArrLDlg::DoDataExchange(CDataExchange* pDX)
{
	SetModified();
	NSectArrDlg::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_left);
	DDX_Check(pDX, IDC_CHECK6, m_right);
	DDX_Text(pDX, IDC_SLW_NUMB, m_counter);
	DDX_Text(pDX, IDC_WLS_SDIST, m_step);
}


BEGIN_MESSAGE_MAP(NSectArrLDlg, CMFCPropertyPage )
	ON_BN_CLICKED(IDC_COLORBYPART, &NSectArrLDlg::OnBnClickedColorbypart)
END_MESSAGE_MAP()


// NSectArrLDlg message handlers
void NSectArrLDlg::SetParams(const NSectArrParams &SectParams)
{
	NSectArrDlg::SetParams(SectParams);
	m_left = (SectParams.StartVal < 0.);
	m_right = (SectParams.EndVal > 0.);
	m_counter = SectParams.IntervalsNum;
	if(m_left && m_right)
		m_counter /= 2;
	if(SectParams.IntervalsNum == 0)
		m_step = 1;
	else
		m_step = (SectParams.EndVal - SectParams.StartVal) / SectParams.IntervalsNum;
}
const NSectArrParams NSectArrLDlg::GetParams(void)
{
	UpdateData();
	NSectArrParams SectParams = NSectArrDlg::GetParams();
	SectParams.Type = SAT_LINE;
	double Diap = m_counter * m_step;
	if(m_left && m_right)
		m_counter *= 2;
	SectParams.IntervalsNum = m_counter;
	SectParams.StartVal = m_left ? -Diap : 0.;
	SectParams.EndVal = m_right ? Diap : 0.;
	return SectParams;
}

void NSectArrLDlg::OnBnClickedColorbypart()
{
	NSectArrDlg::OnBnClickedColorbypart();
}

