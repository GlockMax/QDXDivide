// NCollSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "NCollSettingsDlg.h"


// NCollSettingsDlg dialog

IMPLEMENT_DYNAMIC(NCollSettingsDlg, CDialog)

NCollSettingsDlg::NCollSettingsDlg(bool CutDisabled, CWnd* pParent /*=NULL*/)
	: CDialog(NCollSettingsDlg::IDD, pParent)
	, m_MargMach(0)
	, m_MargTool(0)
	, m_MargPart(0)
	, m_MargStock(0)
	, m_MargFix(0)
	, m_CheckStep(0)
	, m_TurnOffset(0)
	, m_XYOffset(0)
	, m_ZOffset(0)
	, m_XYOffsetSh(0)
	, m_ZOffsetSh(0)
	, m_MaskMach(FALSE)
	, m_MaskTool(FALSE)
	, m_MaskStock(FALSE)
	, m_MaskPart(FALSE)
	, m_MaskFix(FALSE)
	, m_NotIgnoreLimits(FALSE)
	, m_IgnoreUp(FALSE)
	, m_MaskCut(TRUE)
	, m_MaskHold(FALSE)
	, m_CuttingDisabled(CutDisabled)
{

}

NCollSettingsDlg::~NCollSettingsDlg()
{
}

void NCollSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MARG_MACH, m_MargMach);
	DDV_MinMaxDouble(pDX, m_MargMach, -1, 100);
	DDX_Text(pDX, IDC_MARG_TOOL, m_MargTool);
	DDV_MinMaxDouble(pDX, m_MargTool, 0, 100);
	DDX_Text(pDX, IDC_MARG_STOCK, m_MargStock);
	DDV_MinMaxDouble(pDX, m_MargStock, 0, 100);
	DDX_Text(pDX, IDC_MARG_PART, m_MargPart);
	DDV_MinMaxDouble(pDX, m_MargPart, 0, 100);
	DDX_Text(pDX, IDC_MARG_FIX, m_MargFix);
	DDV_MinMaxDouble(pDX, m_MargFix, 0, 100);
	DDX_Text(pDX, IDC_CHECK_STEP, m_CheckStep);
	DDV_MinMaxDouble(pDX, m_CheckStep, 0.1, 10.);
	DDX_Text(pDX, IDC_XYOFFSET, m_XYOffset);
	DDX_Text(pDX, IDC_ZOFFSET, m_ZOffset);
	DDX_Text(pDX, IDC_TURNHOLDER_OFFSET, m_TurnOffset);
	DDX_Text(pDX, IDC_SHANK_OFFSET, m_XYOffsetSh);
	DDX_Check(pDX, IDC_MASK_MACH, m_MaskMach);
	DDX_Check(pDX, IDC_MASK_TOOL, m_MaskTool);
	DDX_Check(pDX, IDC_MASK_STOCK, m_MaskStock);
	DDX_Check(pDX, IDC_MASK_PART, m_MaskPart);
	DDX_Check(pDX, IDC_MASK_FIX, m_MaskFix);
	DDX_Check(pDX, IDC_MASK_TOOLCUT, m_MaskCut);
	DDX_Check(pDX, IDC_MASK_TOOLHOLDER, m_MaskHold);
	DDX_Control(pDX, IDC_MASK_FIX, m_MaskFixCtrl);
	DDX_Control(pDX, IDC_MASK_PART, m_MaskPartCtrl);
	DDX_Control(pDX, IDC_MASK_TOOL, m_MaskToolCtrl);
	DDX_Control(pDX, IDC_MASK_MACH, m_MaskMachCtrl);
	DDX_Control(pDX, IDC_MASK_STOCK, m_MaskStockCtrl);
	DDX_Control(pDX, IDC_MASK_TOOLCUT, m_MaskCutCtrl);
	DDX_Control(pDX, IDC_MASK_TOOLHOLDER, m_MaskHoldCtrl);
	DDX_Check(pDX, IDC_TRAVELLIMITCHECK, m_NotIgnoreLimits);
	DDX_Check(pDX, IDC_IGNOREUPTOOLSTOCKCOLL, m_IgnoreUp);
}


BEGIN_MESSAGE_MAP(NCollSettingsDlg, CDialog)
END_MESSAGE_MAP()


// NCollSettingsDlg message handlers

BOOL NCollSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	m_MaskFixCtrl.SetImage(IDB_CHECKNO32);
	m_MaskFixCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskFixCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	m_MaskPartCtrl.SetImage(IDB_CHECKNO32);
	m_MaskPartCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskPartCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	m_MaskMachCtrl.SetImage(IDB_CHECKNO32);
	m_MaskMachCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskMachCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	m_MaskToolCtrl.SetImage(IDB_CHECKNO32);
	m_MaskToolCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskToolCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	m_MaskStockCtrl.SetImage(IDB_CHECKNO32);
	m_MaskStockCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskStockCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_MaskStockCtrl.EnableWindow(!m_CuttingDisabled);

	m_MaskCutCtrl.SetImage(IDB_CHECKNO32);
	m_MaskCutCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskCutCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	m_MaskHoldCtrl.SetImage(IDB_CHECKNO32);
	m_MaskHoldCtrl.SetCheckedImage(IDB_CHECK32);
	m_MaskHoldCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
