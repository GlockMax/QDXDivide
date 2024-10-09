// NToolsCrPunchDial.cpp : implementation file
//

#include "stdafx.h"
#include "ncm.h"
#include "NToolsCrPunchDial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrPunchDial dialog


NToolsCrPunchDial::NToolsCrPunchDial(CWnd* pParent /*=NULL*/)
	: CDialog(NToolsCrPunchDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(NToolsCrPunchDial)
	m_name = _T("");
	m_pos = _T("");
	m_PointSize = 0;
	m_displacement = 0.0;
	m_H = 0.0;
	m_R = 0.0;
	m_X = 0.0;
	m_Y = 0.0;
	//}}AFX_DATA_INIT
}


void NToolsCrPunchDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrPunchDial)
	DDX_Text(pDX, IDC_MILL_NAME, m_name);
	DDX_Text(pDX, IDC_MILL_POS, m_pos);
	DDX_Text(pDX, IDC_POINT_SIZE, m_PointSize);
	DDV_MinMaxUInt(pDX, m_PointSize, 0, 99);
	DDX_Text(pDX, IDC_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, 0., 100.);
	DDX_Text(pDX, IDC_PUNCH_H, m_H);
	DDV_MinMaxDouble(pDX, m_H, 1.e-002, 500.);
	DDX_Text(pDX, IDC_PUNCH_R, m_R);
	DDV_MinMaxDouble(pDX, m_R, 0., 500.);
	DDX_Text(pDX, IDC_PUNCH_X, m_X);
	DDV_MinMaxDouble(pDX, m_X, 1.e-002, 500.);
	DDX_Text(pDX, IDC_PUNCH_Y, m_Y);
	DDV_MinMaxDouble(pDX, m_Y, 1.e-002, 500.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrPunchDial, CDialog)
	//{{AFX_MSG_MAP(NToolsCrPunchDial)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrPunchDial message handlers
