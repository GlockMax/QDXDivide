// NStlConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "NCMProject.h"
#include "ResLocal.h"
#include "NStlConverterDlg.h"

// NStlConverterDlg dialog

IMPLEMENT_DYNAMIC(NStlConverterDlg, CDialogEx)

std::map<enum DocElements, std::pair<double /*Accuracy*/, double/*MinAngle*/>> NStlConverterDlg::AccValues;

NStlConverterDlg::NStlConverterDlg(enum DocElements DEl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STL_CONVERTER, pParent)
{
	LastDEl = DEl;
	const auto it = AccValues.find(DEl);
	if (it == AccValues.end())
	{
		double* pDouble = nullptr;
		if (NCM_PROJECT.DoubleParams.Lookup(_T("ConverterAcc"), pDouble))
			Accuracy = *pDouble;
		else
			Accuracy = 0.2;
		pDouble = nullptr;
		if (NCM_PROJECT.DoubleParams.Lookup(_T("ConverterAng"), pDouble))
			MinAngle = *pDouble;
		else
			MinAngle = 1.2;
	}
	else
	{
		Accuracy = it->second.first;
		MinAngle = it->second.second;
	}
}

NStlConverterDlg::~NStlConverterDlg()
{
}

void NStlConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ACCURACY, Accuracy);
	DDX_Text(pDX, IDC_MINANGLE, MinAngle);
	DDX_Text(pDX, IDC_FILE_NAME, SrcFileName);
	DDX_Text(pDX, IDC_FILE_STL_NAME, DestFileName);
	DDX_Control(pDX, IDC_STL_FORMAT, FormatCtrl);
}


BEGIN_MESSAGE_MAP(NStlConverterDlg, CDialogEx)
END_MESSAGE_MAP()


// NStlConverterDlg message handlers


BOOL NStlConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	FormatCtrl.SetCurSel(Format);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void NStlConverterDlg::OnOK()
{
	UpdateData();
	Format = FormatCtrl.GetCurSel();
	AccValues[LastDEl] = std::pair<double, double>(Accuracy, MinAngle);
	CDialogEx::OnOK();
}

void NStlConverterDlg::ResetAccValues()
{
	AccValues.clear();
}
