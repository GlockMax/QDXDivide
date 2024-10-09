// NToolsCrAnglesPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrAnglesPage.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAnglesPage property page

IMPLEMENT_DYNCREATE(NToolsCrAnglesPage, NToolsCrCommonPage)

NToolsCrAnglesPage::NToolsCrAnglesPage() : NToolsCrCommonPage(NToolsCrAnglesPage::IDD)
{
	//{{AFX_DATA_INIT(NToolsCrAnglesPage)
	m_diam = (MM_INCH == M_MM) ? 50.0 : 2.0;
	m_b = (MM_INCH == M_MM) ? 6.0 : 0.5;
	m_r = 0.0;
	m_alf1 = 30.0;
	m_alf2 = 30.0;
	m_faset = (MM_INCH == M_MM) ? 3.0 : 0.25;
	m_check = FALSE;
	m_check2 = FALSE;
	m_h1 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	m_h2 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	m_displacement = 0.0;
	m_name = _T("");
	//}}AFX_DATA_INIT
	m_name = (MM_INCH == M_MM) ? "D50B6A10G10Disk" : "D2B0.5A30G30Disk";
	IsNameAuto = true;
}

NToolsCrAnglesPage::~NToolsCrAnglesPage()
{
}

void NToolsCrAnglesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrAnglesPage)
	DDX_Control(pDX, IDC_DISK_ANGLE_PIC2, m_Draw2);
	DDX_Control(pDX, IDC_DISK_ANGLE_PIC1, m_Draw1);
	DDX_Control(pDX, IDC_ANGLES_H2, m_h2_ctrl);
	DDX_Control(pDX, IDC_ANGLES_H1, m_h1_ctrl);
	DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	DDX_Control(pDX, IDC_ANGLES_ALF2, m_alf2_ctrl);
	DDX_Control(pDX, IDC_DISK_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_ANGLES_R1, m_r_ctrl);
	DDX_Control(pDX, IDC_ANGLES_FASET, m_faset_ctrl);
	DDX_Control(pDX, IDC_ANGLES_ALF1, m_alf1_ctrl);
	DDX_Text(pDX, IDC_ANGLES_ALF1, m_alf1);
	DDX_Text(pDX, IDC_ANGLES_ALF2, m_alf2);
	DDX_Text(pDX, IDC_ANGLES_FASET, m_faset);
	DDX_Text(pDX, IDC_ANGLES_R1, m_r);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	DDV_MinMaxDouble(pDX, m_b, MINTOOLDIM, 500.);
	DDX_Check(pDX, IDC_ANGLES_CHK, m_check);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	DDV_MinMaxDouble(pDX, m_diam, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Text(pDX, IDC_ANGLES_H1, m_h1);
	DDX_Text(pDX, IDC_ANGLES_H2, m_h2);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrAnglesPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrAnglesPage)
	ON_EN_CHANGE(IDC_ANGLES_ALF1, OnChangeAnglesAlf1)
	ON_EN_CHANGE(IDC_ANGLES_ALF2, OnChangeAnglesAlf2)
	ON_EN_CHANGE(IDC_ANGLES_FASET, OnChangeAnglesFaset)
	ON_EN_CHANGE(IDC_ANGLES_R1, OnChangeAnglesR1)
	ON_EN_CHANGE(IDC_DISK_B, OnChangeDiskB)
	ON_EN_CHANGE(IDC_DISK_DIAM, OnChangeDiskDiam)
	ON_BN_CLICKED(IDC_ANGLES_CHK, OnDiskCheck)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck2)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAnglesPage message handlers

BOOL NToolsCrAnglesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (m_r != 0.)
		m_check = TRUE;
	else
		m_check = FALSE;

	if (m_h1 == m_diam/2. && m_h2 == m_diam/2.)
		m_check2 = TRUE;
	else
		m_check2 = FALSE;

	CheckDlgButton(IDC_ANGLES_CHK, m_check);
	CheckDlgButton(IDC_DISK_CHECK1, m_check2);

	m_r_ctrl.EnableWindow(m_check);
	m_h1_ctrl.EnableWindow(!m_check2);
	m_h2_ctrl.EnableWindow(!m_check2);
	
	m_Draw1.ShowWindow(!m_check ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check ? SW_SHOW : SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString& NToolsCrAnglesPage::GetName()
{
	return m_name;
}

CString NToolsCrAnglesPage::GenName()
{
	CString d, b, a1, a2, r, buf;
	double value;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	d.Format("D%s", buf);

	m_b_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";	
	b.Format("B%s", buf);

	m_alf1_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";	
	a1.Format("A%s", buf);

	m_alf2_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";	
	a2.Format("G%s", buf);

	m_r_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		r.Format("R%s", buf);

	return d+b+a1+a2+r+_T("Disk");
}

void NToolsCrAnglesPage::OnKillfocusDiskName() 
{
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());
}

void NToolsCrAnglesPage::OnDiskCheck() 
{
	if (m_check)
		m_check = FALSE;
	else
		m_check = TRUE;
	
	m_r_ctrl.EnableWindow(m_check == TRUE);

	m_Draw1.ShowWindow(!m_check ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check ? SW_SHOW : SW_HIDE);

	if (m_b_ctrl.GetWindowTextLength() == 0 )
	{
		if (m_check)
			m_r_ctrl.SetWindowText("0");
		else
			m_r_ctrl.SetWindowText("0");
		return;
	}
// заполнение полей формы
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_DISK_B);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	if(m_check)
		m_r = m_b / 4.; // значение по умолчанию
	else
		m_r = 0.;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_ANGLES_R1);
	DDX_Text(pDX, IDC_ANGLES_R1, m_r);
	delete pDX;
}

void NToolsCrAnglesPage::OnDiskCheck2() 
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
	pDX->PrepareEditCtrl(IDC_ANGLES_H1);
	DDX_Text(pDX, IDC_ANGLES_H1, m_h1);
	pDX->PrepareEditCtrl(IDC_ANGLES_H2);
	DDX_Text(pDX, IDC_ANGLES_H2, m_h2);
	delete pDX;
}

void NToolsCrAnglesPage::OnChangeDiskDiam() 
{
	if (m_diam_ctrl.GetWindowTextLength() != 0 && m_check2)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_DISK_DIAM);
		DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
		m_h1 = m_h2 = m_diam/2.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_ANGLES_H1);
		DDX_Text(pDX, IDC_ANGLES_H1, m_h1);
		pDX->PrepareEditCtrl(IDC_ANGLES_H2);
		DDX_Text(pDX, IDC_ANGLES_H2, m_h2);
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

void NToolsCrAnglesPage::OnChangeDiskB() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAnglesPage::OnChangeAnglesAlf1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAnglesPage::OnChangeAnglesAlf2() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAnglesPage::OnChangeAnglesFaset() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAnglesPage::OnChangeAnglesR1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

bool NToolsCrAnglesPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введЄнние всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_b < 0. || m_alf1 < 0. || m_alf2 < 0.
		|| m_faset < 0. || m_h1 < 0. || m_h2 < 0)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
	if (m_alf1 == 0.)
	{
		AfxMessageBox(IDS_MES_ANGLE);
		m_alf1_ctrl.SetFocus();		m_alf1_ctrl.SetSel(0, -1, FALSE);
		return false;
	}
	if (m_alf2 == 0.)
	{
		AfxMessageBox(IDS_MES_ANGLE);
		m_alf2_ctrl.SetFocus();		m_alf2_ctrl.SetSel(0, -1, FALSE);
		return false;
	}
	if (m_faset == 0.)
	{
		AfxMessageBox(IDS_MES_INTP);
		m_faset_ctrl.SetFocus();	m_faset_ctrl.SetSel(0, -1, FALSE);
		return false;
	}

	return true;
}

void NToolsCrAnglesPage::IsError(int IDErr)
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
	switch(IDErr)
	{
	case 1:	// Ќедопустимый угол
		tmps.LoadString(IDS_MES_ANG);
		str += tmps;
		if (m_alf1*PI/180 > PI)
		{
			m_alf1_ctrl.SetFocus();		m_alf1_ctrl.SetSel(0, -1, FALSE);
		}
		else
		{
			m_alf2_ctrl.SetFocus();		m_alf2_ctrl.SetSel(0, -1, FALSE);
		}
		break;
	case 8: case 9: // неверны параметры R3 или R4
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r_ctrl.SetFocus();
		m_r_ctrl.SetSel(0, -1, FALSE);
		break;
	case 10: case 11: // активна€ часть инструмента
		double a, b, f;
		CString tmp;
		int  decimal1, decimal2, sign;
		f = m_faset - m_r*tan((PI / 2. - (m_alf1*PI / 180.)) / 2.);
		a = f*tan(PI/2. - PI*m_alf1/180.);
		f = m_b - m_faset;
		f = f - m_r*tan((PI/2.-(m_alf2*PI/180.))/2.);
		b = f*tan(PI/2. - PI*m_alf2/180.);
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
