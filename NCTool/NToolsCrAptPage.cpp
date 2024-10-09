// NToolsCrAptPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrAptPage.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAptPage property page

IMPLEMENT_DYNCREATE(NToolsCrAptPage, NToolsCrCommonPage)

NToolsCrAptPage::NToolsCrAptPage() : NToolsCrCommonPage(NToolsCrAptPage::IDD)
{
	//{{AFX_DATA_INIT(NToolsCrAptPage)
	m_diam = (MM_INCH == M_MM) ? 10.0 : 0.75;
	m_h = (MM_INCH == M_MM) ? 40.0 : 2.0;
	m_r = 0.0;
	m_alf = 0.0;
	m_beta = 0.0;
	m_e = 0.;
	m_f = 0.;
	m_displacement = 0.0;
	m_hm = (MM_INCH == M_MM) ? 40 : 2.00;
	m_check1 = FALSE;
	m_check2 = FALSE;
	//}}AFX_DATA_INIT
	IsNameAuto = true;
	m_name = (MM_INCH == M_MM) ? _T("D10H40") : _T("D0.75H2");
}

NToolsCrAptPage::~NToolsCrAptPage()
{
}

void NToolsCrAptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrAptPage)
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_APT_H, m_hm_ctrl);
	DDX_Control(pDX, IDC_MILL_H, m_h_ctrl);
	DDX_Control(pDX, IDC_MILL_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_APT_R, m_r_ctrl);
	DDX_Control(pDX, IDC_APT_F, m_f_ctrl);
	DDX_Control(pDX, IDC_APT_E, m_e_ctrl);
	DDX_Control(pDX, IDC_APT_ALF2, m_beta_ctrl);
	DDX_Control(pDX, IDC_APT_ALF1, m_alf_ctrl);
	DDX_Control(pDX, IDC_APT_RADIO1, m_radio1_ctrl);
	DDX_Control(pDX, IDC_APT_RADIO2, m_radio2_ctrl);
	DDX_Text(pDX, IDC_APT_ALF1, m_alf);
	DDV_MinMaxDouble(pDX, m_alf, -89., 89.);
	DDX_Text(pDX, IDC_APT_ALF2, m_beta);
	DDX_Text(pDX, IDC_APT_E, m_e);
	DDX_Text(pDX, IDC_APT_F, m_f);
	DDX_Text(pDX, IDC_APT_R, m_r);
	DDX_Check(pDX, IDC_APT_CHK, m_check1);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check2);
	DDX_Check(pDX, IDC_APT_RADIO1, m_radio1);
	DDX_Check(pDX, IDC_APT_RADIO2, m_radio2);
	DDX_Text(pDX, IDC_MILL_DIAM, m_diam);
	DDX_Text(pDX, IDC_MILL_H, m_h);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Text(pDX, IDC_APT_H, m_hm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrAptPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrAptPage)
	ON_EN_CHANGE(IDC_MILL_DIAM, OnChangeMillDiam)
	ON_EN_CHANGE(IDC_MILL_H, OnChangeMillH)
	ON_EN_CHANGE(IDC_APT_ALF1, OnChangeAptAlf1)
	ON_EN_CHANGE(IDC_APT_ALF2, OnChangeAptAlf2)
	ON_EN_CHANGE(IDC_APT_R, OnChangeAptR)
	ON_BN_CLICKED(IDC_APT_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_APT_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_APT_CHK, OnDiskCheck1)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck2)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAptPage message handlers

BOOL NToolsCrAptPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (m_r != 0. || m_e != 0. || m_f != 0.)
		m_check1 = TRUE;
	else
		m_check1 = FALSE;

	if (m_hm == m_h)
		m_check2 = TRUE;
	else
		m_check2 = FALSE;

	if (m_e == 0. && m_f == 0.)
	{
		m_radio1 = TRUE;
		m_radio2 = FALSE;
	}
	else
	{
		m_radio1 = FALSE;
		m_radio2 = TRUE;
	}

	CheckDlgButton(IDC_APT_CHK, m_check1);
	CheckDlgButton(IDC_DISK_CHECK1, m_check2);

	m_radio1_ctrl.SetCheck(m_radio1);
	m_radio2_ctrl.SetCheck(m_radio2);

	m_radio1_ctrl.EnableWindow(m_check1);
	m_radio2_ctrl.EnableWindow(m_check1);
	
	m_r_ctrl.EnableWindow(m_check1 && m_radio1);
	m_e_ctrl.EnableWindow(m_check1 && m_radio2);
	m_f_ctrl.EnableWindow(m_check1 && m_radio2);
	m_hm_ctrl.EnableWindow(!m_check2);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString NToolsCrAptPage::GenName()
{
	CString d, h, r, a, buf;
	double value;
	int decimal, sign;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	d.Format("D%s", buf);

	m_h_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	h.Format("H%s", buf);

	m_r_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
	{
		buf = _fcvt(value, 1, &decimal, &sign);
		r.Format("R%s", buf.Mid(0,decimal)+"."+buf.Mid(decimal));
	}

	m_alf_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		a.Format("A%s", buf);

	m_beta_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		r.Format("G%s", buf);

	return d+h+r+a;
}

void NToolsCrAptPage::OnKillfocusDiskName() 
{
	
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());
}

CString& NToolsCrAptPage::GetName()
{
	return m_name;
}

void NToolsCrAptPage::OnDiskCheck1() 
{
	
	if (m_check1)
		m_check1 = FALSE;
	else
		m_check1 = TRUE;

	m_radio1_ctrl.EnableWindow(m_check1);
	m_radio2_ctrl.EnableWindow(m_check1);

	m_r_ctrl.EnableWindow(m_check1 && m_radio1);
	m_e_ctrl.EnableWindow(m_check1 && m_radio2);
	m_f_ctrl.EnableWindow(m_check1 && m_radio2);

	if (!m_check1)
		m_r = m_e = m_f = 0.;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_APT_R);
	DDX_Text(pDX, IDC_APT_R, m_r);
	pDX->PrepareEditCtrl(IDC_APT_E);
	DDX_Text(pDX, IDC_APT_E, m_e);
	pDX->PrepareEditCtrl(IDC_APT_F);
	DDX_Text(pDX, IDC_APT_F, m_f);
	delete pDX;
}

void NToolsCrAptPage::OnRadio1() 
{
	
	m_radio1 = TRUE;
	m_radio2 = FALSE;
	
	m_r_ctrl.EnableWindow(m_check1 && m_radio1);
	m_e_ctrl.EnableWindow(m_check1 && m_radio2);
	m_f_ctrl.EnableWindow(m_check1 && m_radio2);

	m_e = m_f = 0.;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_APT_E);
	DDX_Text(pDX, IDC_APT_E, m_e);
	pDX->PrepareEditCtrl(IDC_APT_F);
	DDX_Text(pDX, IDC_APT_F, m_f);
	delete pDX;
}

void NToolsCrAptPage::OnRadio2() 
{
	
	m_radio1 = FALSE;
	m_radio2 = TRUE;	

	m_r_ctrl.EnableWindow(m_check1 && m_radio1);
	m_e_ctrl.EnableWindow(m_check1 && m_radio2);
	m_f_ctrl.EnableWindow(m_check1 && m_radio2);

	m_r = 0.;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_APT_R);
	DDX_Text(pDX, IDC_APT_R, m_r);
	delete pDX;
}

void NToolsCrAptPage::OnDiskCheck2() 
{
	
	if (m_check2)
		m_check2 = FALSE;
	else
		m_check2 = TRUE;

	m_hm_ctrl.EnableWindow(!m_check2);
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_MILL_H);
	DDX_Text(pDX, IDC_MILL_H, m_h);
	if (m_check2)	
		m_hm = m_h;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_APT_H);
	DDX_Text(pDX, IDC_APT_H, m_hm);
	delete pDX;
}

void NToolsCrAptPage::OnChangeMillDiam() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAptPage::OnChangeMillH() 
{
	if (m_h_ctrl.GetWindowTextLength() != 0 && m_check2)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_MILL_H);
		DDX_Text(pDX, IDC_MILL_H, m_h);
		m_hm = m_h;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_APT_H);
		DDX_Text(pDX, IDC_APT_H, m_hm);
		delete pDX;
	}
	else
	{
		m_hm_ctrl.SetWindowText("0");
	}
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAptPage::OnChangeAptAlf1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAptPage::OnChangeAptAlf2() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}


void NToolsCrAptPage::OnChangeAptR() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

bool NToolsCrAptPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_h < 0. || m_beta < 0. 
		|| m_e < 0. || m_f < 0. || m_r < 0. || m_hm < 0)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
	return true;
}

void NToolsCrAptPage::IsError(int IDErr)
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
	switch(IDErr)
	{
	case 1:
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r_ctrl.SetFocus();
		m_r_ctrl.SetSel(0, -1, FALSE);
		break;
	case 2: 
		tmps.LoadString(IDS_MES_ANG);// CHANGE MESSAGE
		str += tmps;
		if (PI-m_alf*PI/180. < MINAR)
		{
			m_alf_ctrl.SetFocus();		m_alf_ctrl.SetSel(0, -1, FALSE);
		}
		else
		{
			m_beta_ctrl.SetFocus();		m_beta_ctrl.SetSel(0, -1, FALSE);
		}

		break;
	case 3: // невыпуклый инструмент
		tmps.LoadString(IDS_MES_ANG);
		str += tmps;
		m_alf_ctrl.SetFocus();
		m_alf_ctrl.SetSel(0, -1, FALSE);
		break;
	case 4: // невыпуклый инструмент
		tmps.LoadString(IDS_MES_ANG);
		str += tmps;
		m_beta_ctrl.SetFocus();
		m_beta_ctrl.SetSel(0, -1, FALSE);
		break;
	case 6: // активная часть инструмента
		double a;
		int  decimal1, decimal2, sign;

		a = m_r*tan((PI/2.-m_alf*PI/180.)/2);
		a = m_diam/2.*tan(m_beta*PI/180.)+a*cos(m_alf*PI/180.);
		tmp1 = _fcvt(a, 3, &decimal1, &sign);
		tmp2 = _fcvt(m_h, 3, &decimal2, &sign);

		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal1),tmp1.Mid(decimal1),
			tmp2.Mid(0,decimal2),tmp2.Mid(decimal2));
		m_hm_ctrl.SetFocus();
		m_hm_ctrl.SetSel(0, -1, FALSE);
		break;
	case 7:
		//tmps.LoadString(IDS_MES_DRA);// NEW MESSAGE
		str += tmps;
		m_alf_ctrl.SetFocus();
		m_alf_ctrl.SetSel(0, -1, FALSE);
		break;
	}
	AfxMessageBox(str);
}
/*
BOOL NToolsCrAptPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
		::WinHelp((HWND) pHelpInfo->hItemHandle,
		AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, (DWORD) aHelpIDs);

	return TRUE;
}
*/
/*
void NToolsCrAptPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	::WinHelp((HWND) *pWnd,	AfxGetApp()->m_pszHelpFilePath, 
	HELP_CONTEXTMENU, (DWORD) aHelpIDs);	
}
*/