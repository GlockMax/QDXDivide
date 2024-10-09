// NUnitWeldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NUnitWeldDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// NUnitWeldDlg dialog

IMPLEMENT_DYNAMIC(NUnitWeldDlg, CPropertyPage)

NUnitWeldDlg::NUnitWeldDlg()
	: CPropertyPage(NUnitWeldDlg::IDD)
	, m_ElectrA(0)
	, m_ElectrB(0)
	, m_ElectrR(1.)
{

}

NUnitWeldDlg::~NUnitWeldDlg()
{
}

void NUnitWeldDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UNIT_FOCUS, m_ElectrT1);
	DDX_Control(pDX, IDC_UNIT_FOCUS, m_ElectrT1_ctrl);
	DDX_Control(pDX, IDC_UNIT_WELD_A, m_ElectrA_ctrl);
	DDX_Text(pDX, IDC_UNIT_WELD_A, m_ElectrA);
	DDX_Control(pDX, IDC_UNIT_WELD_B, m_ElectrB_ctrl);
	DDX_Text(pDX, IDC_UNIT_WELD_B, m_ElectrB);
	DDX_Control(pDX, IDC_UNIT_WELD_DROOT, m_ElectrR_ctrl);
	DDX_Text(pDX, IDC_UNIT_WELD_DROOT, m_ElectrR);
}


BEGIN_MESSAGE_MAP(NUnitWeldDlg, CPropertyPage)
END_MESSAGE_MAP()


// NUnitWeldDlg message handlers
