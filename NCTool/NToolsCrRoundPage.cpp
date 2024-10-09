// NToolsCrRoundPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrRoundPage.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// NToolsCrRoundPage property page

IMPLEMENT_DYNCREATE(NToolsCrRoundPage, NToolsCrCommonPage)

NToolsCrRoundPage::NToolsCrRoundPage() : NToolsCrCommonPage(NToolsCrRoundPage::IDD)
{
	//{{AFX_DATA_INIT(NToolsCrRoundPage)
	m_diam = (MM_INCH == M_MM) ? 30.0 :1.0;
	m_b = (MM_INCH == M_MM) ? 30.0 : 1.0;
	m_check1 = FALSE;
	m_displacement = 0.0;
	m_name = (MM_INCH == M_MM) ? _T("D30B30Disk"): _T("D1B1Disk") ;
	m_r1 = (MM_INCH == M_MM) ? 15.0 : 0.5;
	m_r2 = 0.0;
	m_r3 = 0.0;
	m_check2 = FALSE;
	m_h1 = (MM_INCH == M_MM) ? 15.0 : 0.5;
	m_h2 = (MM_INCH == M_MM) ? 15.0 : 0.5;
	//}}AFX_DATA_INIT
	IsNameAuto = true;
}

NToolsCrRoundPage::~NToolsCrRoundPage()
{
}

void NToolsCrRoundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrRoundPage)
	DDX_Control(pDX, IDC_ROUND_H2, m_h2_ctrl);
	DDX_Control(pDX, IDC_ROUND_H1, m_h1_ctrl);
	DDX_Control(pDX, IDC_DISK_ROUND_PIC2, m_Draw2);
	DDX_Control(pDX, IDC_DISK_ROUND_PIC1, m_Draw1);
	DDX_Control(pDX, IDC_ROUND_R1, m_r1_ctrl);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_ROUND_R3, m_r3_ctrl);
	DDX_Control(pDX, IDC_ROUND_R2, m_r2_ctrl);
	DDX_Control(pDX, IDC_DISK_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	DDV_MinMaxDouble(pDX, m_b, MINTOOLDIM, 500.);
	DDX_Check(pDX, IDC_ROUND_CHK, m_check1);
	DDX_Check(pDX, IDC_PROBE_TOOL, m_check_probe);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	DDV_MinMaxDouble(pDX, m_diam, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Text(pDX, IDC_ROUND_R2, m_r2);
	DDX_Text(pDX, IDC_ROUND_R3, m_r3);
	DDX_Text(pDX, IDC_ROUND_R1, m_r1);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check2);
	DDX_Text(pDX, IDC_ROUND_H1, m_h1);
	DDX_Text(pDX, IDC_ROUND_H2, m_h2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrRoundPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrRoundPage)
	ON_EN_CHANGE(IDC_DISK_B, OnChangeDiskB)
	ON_EN_CHANGE(IDC_DISK_DIAM, OnChangeDiskDiam)
	ON_BN_CLICKED(IDC_ROUND_CHK, OnDiskCheck1)
	ON_EN_CHANGE(IDC_ROUND_R2, OnChangeRoundR2)
	ON_EN_CHANGE(IDC_ROUND_R3, OnChangeRoundR3)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck2)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrRoundPage message handlers
BOOL NToolsCrRoundPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (m_r1 == m_b/2.)
		m_check1 = TRUE;
	else
		m_check1 = FALSE;
	if (m_h1 == m_diam/2. && m_h2 == m_diam/2.)
		m_check2 = TRUE;
	else
		m_check2 = FALSE;

	CheckDlgButton(IDC_ROUND_CHK, m_check1);
	CheckDlgButton(IDC_DISK_CHECK1, m_check2);

	m_r2_ctrl.EnableWindow(!m_check1);
	m_r3_ctrl.EnableWindow(!m_check1);
	m_h1_ctrl.EnableWindow(!m_check2);
	m_h2_ctrl.EnableWindow(!m_check2);
	
	m_Draw1.ShowWindow((m_check1 == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((m_check1 == FALSE) ? SW_SHOW : SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString& NToolsCrRoundPage::GetName()
{
	return m_name;
}

CString NToolsCrRoundPage::GenName()
{
	CString d, b, r, buf;
	double value;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	d.Format("D%s", buf);

	m_b_ctrl.GetWindowText(buf);
	if (buf == "")
		buf = "..";
	b.Format("B%s", buf);

	if (m_check1)
		m_r1_ctrl.GetWindowText(buf);
	if ((m_r2 > m_r3) && m_check1 != TRUE)
		m_r2_ctrl.GetWindowText(buf);
	else
		m_r3_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		r.Format("R%s", buf);

	return d+b+r+_T("Disk");
}

void NToolsCrRoundPage::OnKillfocusDiskName() 
{
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());	
}

void NToolsCrRoundPage::OnDiskCheck1() 
{
	if (m_check1)
		m_check1 = FALSE;
	else
		m_check1 = TRUE;
	
	m_r2_ctrl.EnableWindow(!m_check1);
	m_r3_ctrl.EnableWindow(!m_check1);

	m_Draw1.ShowWindow((m_check1 == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((m_check1 == FALSE) ? SW_SHOW : SW_HIDE);
	
	if (m_diam_ctrl.GetWindowTextLength() == 0 ||
		m_b_ctrl.GetWindowTextLength() == 0 )
	{
		if (m_check1)
			m_r1_ctrl.SetWindowText("0");
		else
			m_r1_ctrl.SetWindowText("0");
		return;
	}
// заполение полей формы
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_DISK_B);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	if(m_check1)
		m_r1 = m_b / 2.;
	else
		m_r1 = 0.;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_ROUND_R1);
	DDX_Text(pDX, IDC_ROUND_R1, m_r1);
	if (m_check1)
	{
		m_r2 = m_r3 = 0.;
		pDX->PrepareEditCtrl(IDC_ROUND_R2);
		DDX_Text(pDX, IDC_ROUND_R2, m_r2);
		pDX->PrepareEditCtrl(IDC_ROUND_R3);
		DDX_Text(pDX, IDC_ROUND_R3, m_r3);
	}
	delete pDX;
}

void NToolsCrRoundPage::OnDiskCheck2() 
{
	if (m_check2)
		m_check2 = FALSE;
	else
		m_check2 = TRUE;

	m_h1_ctrl.EnableWindow(!m_check2);
	m_h2_ctrl.EnableWindow(!m_check2);

	if (m_diam_ctrl.GetWindowTextLength() == 0)
	{
		if (m_check2)
		{
			m_h1_ctrl.SetWindowText("0");
			m_h2_ctrl.SetWindowText("0");
		}
		else
		{
			m_h1_ctrl.SetWindowText("0");
			m_h2_ctrl.SetWindowText("0");
		}
		return;
	}

	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_DISK_DIAM);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	if (m_check2)	
		m_h1 = m_h2 = m_diam/2.;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_ROUND_H1);
	DDX_Text(pDX, IDC_ROUND_H1, m_h1);
	pDX->PrepareEditCtrl(IDC_ROUND_H2);
	DDX_Text(pDX, IDC_ROUND_H2, m_h2);
	delete pDX;
}

void NToolsCrRoundPage::OnChangeDiskDiam() 
{
	if (m_diam_ctrl.GetWindowTextLength() != 0 && m_check2)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_DISK_DIAM);
		DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
		m_h1 = m_h2 = m_diam/2.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_ROUND_H1);
		DDX_Text(pDX, IDC_ROUND_H1, m_h1);
		pDX->PrepareEditCtrl(IDC_ROUND_H2);
		DDX_Text(pDX, IDC_ROUND_H2, m_h2);
		delete pDX;
	}
	if(m_diam_ctrl.GetWindowTextLength() == 0 && m_check2)
	{
		m_h1_ctrl.SetWindowText("0");
		m_h2_ctrl.SetWindowText("0");
	}
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrRoundPage::OnChangeDiskB() 
{
	if (m_b_ctrl.GetWindowTextLength() != 0 && m_check1)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_DISK_B);
		DDX_Text(pDX, IDC_DISK_B, m_b);
		m_r1 = m_b/2.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_ROUND_R1);
		DDX_Text(pDX, IDC_ROUND_R1, m_r1);
		delete pDX;
	}
	if(m_b_ctrl.GetWindowTextLength() == 0 && m_check1)
		m_r1_ctrl.SetWindowText("0");
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrRoundPage::OnChangeRoundR2() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrRoundPage::OnChangeRoundR3() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

bool NToolsCrRoundPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_b < 0. || m_r1 < 0. || m_r2 < 0. 
		|| m_r3 < 0. || m_h1 < 0. || m_h2 < 0)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
		
	return true;
}

void NToolsCrRoundPage::IsError(int IDErr)
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
	switch(IDErr)
	{
	case 5:	// неверны параметры R1
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r2_ctrl.SetFocus();
		m_r2_ctrl.SetSel(0, -1, FALSE);
		break;
	case 6: // неверны параметры R2
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r3_ctrl.SetFocus();
		m_r3_ctrl.SetSel(0, -1, FALSE);
		break;
	case 7:	// неверны параметры R1 или R2
		tmps.LoadString(IDS_MES_RAD1);
		str += tmps;
		m_r2_ctrl.SetFocus();
		m_r2_ctrl.SetSel(0, -1, FALSE);
		break;
	case 10: case 11: // активная часть инструмента
		double a, b;
		CString tmp;
		int  decimal1, decimal2, sign;
		a = m_r2;
		b = m_r3;
		(a > m_h1) ? tmp1 = _fcvt(a, 3, &decimal1, &sign) : tmp1 = _fcvt(b, 3, &decimal1, &sign);
		tmp2 = _fcvt(m_diam/2., 3, &decimal2, &sign);
		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal1),tmp1.Mid(decimal1),
			tmp2.Mid(0,decimal2),tmp2.Mid(decimal2));
		if (a > m_h1)
		{
			m_h1_ctrl.SetFocus();	m_h1_ctrl.SetSel(0, -1, FALSE);
		}
		else
		{
			m_h2_ctrl.SetFocus();	m_h2_ctrl.SetSel(0, -1, FALSE);
		}
		break;
	}
	AfxMessageBox(str);
}
