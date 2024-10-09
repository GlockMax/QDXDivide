// NToolsCrBRoundPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "math.h"
#include "float.h"
#include "NToolsCrBRoundPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrBRoundPage property page

IMPLEMENT_DYNCREATE(NToolsCrBRoundPage, NToolsCrCommonPage)

NToolsCrBRoundPage::NToolsCrBRoundPage() : NToolsCrCommonPage(NToolsCrBRoundPage::IDD)
, m_l(0)
{
	//{{AFX_DATA_INIT(NToolsCrBRoundPage)
	m_diam = (MM_INCH == M_MM) ? 10.0 : 0.25;
	m_b = (MM_INCH == M_MM) ? 40.0 : 1.5;
	m_h1 = m_b;
	m_r = (MM_INCH == M_MM) ? 3.0 : 0.01;
	m_check = FALSE;
	m_check2 = TRUE;
	m_displacement = 0.0;
	//}}AFX_DATA_INIT
	m_name = (MM_INCH == M_MM) ? "D10H40R-3" : "D0.25H1.5R-0.01";
	IsNameAuto = true;
}

NToolsCrBRoundPage::~NToolsCrBRoundPage()
{
}

void NToolsCrBRoundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrBRoundPage)
	DDX_Control(pDX, IDC_DISK_BANGLE_PIC2, m_Draw2);
	DDX_Control(pDX, IDC_DISK_BANGLE_PIC1, m_Draw1);
	DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	DDX_Control(pDX, IDC_BROUND_R, m_r_ctrl);
	DDX_Control(pDX, IDC_DISK_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_BROUND_L, m_l_ctrl);
	DDX_Control(pDX, IDC_BROUND_H1, m_h1_ctrl);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Text(pDX, IDC_BROUND_R, m_r);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	DDX_Text(pDX, IDC_BROUND_H1, m_h1);
	DDV_MinMaxDouble(pDX, m_b, MINTOOLDIM, 500.);
	DDX_Check(pDX, IDC_BROUND_CHK, m_check);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	DDV_MinMaxDouble(pDX, m_diam, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check2);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_BROUND_L, m_l);
}


BEGIN_MESSAGE_MAP(NToolsCrBRoundPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrBRoundPage)
	ON_EN_CHANGE(IDC_DISK_B, OnChangeDiskB)
	ON_EN_CHANGE(IDC_DISK_DIAM, OnChangeDiskDiam)
	ON_BN_CLICKED(IDC_BROUND_CHK, OnDiskCheck)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck2)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_BROUND_R, OnEnChangeBroundR)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrBRoundPage message handlers

BOOL NToolsCrBRoundPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_check = (m_l != 0.);

	m_check2 = (m_h1 == m_b);


	m_l_ctrl.EnableWindow(m_check);
	m_h1_ctrl.EnableWindow(!m_check2);
	
	m_Draw1.ShowWindow(!m_check ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check ? SW_SHOW : SW_HIDE);
	UpdateData(FALSE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString& NToolsCrBRoundPage::GetName()
{
	return m_name;
}

CString NToolsCrBRoundPage::GenName()
{
	CString d, b, a1, a2, r, buf;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
		buf = "..";
	d.Format("D%s", buf);

	m_b_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";	
	b.Format("H%s", buf);

	m_r_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";	
	r.Format("R-%s", buf);

	m_name_ctrl.SetWindowText(d + b + r);
	return d + b + r;
}

void NToolsCrBRoundPage::OnKillfocusDiskName() 
{
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());
}

void NToolsCrBRoundPage::OnDiskCheck() 
{
	UpdateData();

	m_l_ctrl.EnableWindow(m_check == TRUE);

	if(!m_check)
		m_l = 0.;

	m_Draw1.ShowWindow(!m_check ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check ? SW_SHOW : SW_HIDE);

	UpdateData(FALSE);
}

void NToolsCrBRoundPage::OnDiskCheck2() 
{
	UpdateData();

	if(m_check2)
	{
		m_h1 = m_b;
	}
	m_h1_ctrl.EnableWindow(!m_check2);

	UpdateData(FALSE);
}

void NToolsCrBRoundPage::OnChangeDiskDiam() 
{
	UpdateData(TRUE);
	if(IsNameAuto)
		m_name = GenName();
//	UpdateData(FALSE);
}

void NToolsCrBRoundPage::OnChangeDiskB() 
{
	UpdateData(TRUE);
	if(m_check2)
		m_h1 = m_b;
	if(IsNameAuto)
		m_name = GenName();
//	UpdateData(FALSE);
}


bool NToolsCrBRoundPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_b < 0. || m_r <= 0. || m_l < 0.)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
	if (m_r * 2. > m_diam)
	{
		AfxMessageBox(IDS_MES_RAD);
		m_r_ctrl.SetFocus();		m_r_ctrl.SetSel(0, -1, FALSE);
		return false;
	}
	if (m_l != 0 && m_l <= m_r)
	{
		AfxMessageBox(IDS_MES_BRNDL);
		m_l_ctrl.SetFocus();		m_l_ctrl.SetSel(0, -1, FALSE);
		return false;
	}
	if (m_l >= m_b - m_r)
	{
		AfxMessageBox(IDS_MES_BRNDLH);
		m_l_ctrl.SetFocus();		m_l_ctrl.SetSel(0, -1, FALSE);
		return false;
	}

	return true;
}

void NToolsCrBRoundPage::IsError(int IDErr)
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
/*	switch(IDErr)
	{
	case 1:	// Недопустимый угол
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
	case 10: case 11: // активная часть инструмента
		double a, b, f;
		CString tmp;
		int  decimal, sign;
		f = m_faset - m_r*tan((PI/2.-(m_alf1*PI/180.))/2.);
		a = f*tan(PI/2. - PI*m_alf1/180.);
		f = m_b - m_faset;
		f = f - m_r*tan((PI/2.-(m_alf2*PI/180.))/2.);
		b = f*tan(PI/2. - PI*m_alf2/180.);
		(a > m_h1) ? tmp1 = _fcvt(a, 3, &decimal, &sign) : tmp1 = _fcvt(b, 3, &decimal, &sign);
		tmp2 = _fcvt(m_diam/2., 3, &decimal, &sign);

		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal),tmp1.Mid(decimal),
			tmp2.Mid(0,decimal),tmp2.Mid(decimal));
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
	*/
	AfxMessageBox(str);
}

void NToolsCrBRoundPage::OnEnChangeBroundR()
{
	UpdateData(TRUE);
	if(IsNameAuto)
		m_name = GenName();
//	UpdateData(FALSE);
}
