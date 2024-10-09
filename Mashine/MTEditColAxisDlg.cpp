// MTEditColAxisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MTConfig.h"
#include "MTEditColAxisDlg.h"


// MTEditColAxisDlg dialog

IMPLEMENT_DYNAMIC(MTEditColAxisDlg, CDialog)

BEGIN_MESSAGE_MAP(MTEditColAxisDlg, CDialog)
	ON_LBN_DBLCLK(IDC_AXIS_NAME, OnDblclkAxisName)
END_MESSAGE_MAP()

MTEditColAxisDlg::MTEditColAxisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MTEditColAxisDlg::IDD, pParent)
{

}

MTEditColAxisDlg::~MTEditColAxisDlg()
{
}

INT_PTR MTEditColAxisDlg::DoModal(MTMachineTool *MT, CString Axis)
{
	pMT = MT;
	AxisName = Axis;
	return CDialog::DoModal();
}//int MTEditColAxisDlg::DoModal(MTMachineTool *MT, CString Axis)

BOOL MTEditColAxisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i = 0; i < pMT->GetCurConfig().GetSize(); i++)
	{
		CString Axis = pMT->GetCurConfig()[i]->GetName();
		if(Axis != AxisName)
			m_ListBox.AddString(Axis);
	}//for
	GetDlgItem(IDOK)->EnableWindow(m_ListBox.GetCount() > 0);
	m_ListBox.SetCurSel(0);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MTEditColAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AXIS_NAME, m_ListBox);
	DDX_LBString(pDX, IDC_AXIS_NAME, m_name);
}


// MTEditColAxisDlg message handlers

void MTEditColAxisDlg::OnDblclkAxisName()
{
	CDialog::OnOK();
}//void MTEditColAxisDlg::OnDblclkAxisName()
void MTEditColAxisDlg::OnOK()
{
	CDialog::OnOK();
}
