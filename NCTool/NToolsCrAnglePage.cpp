// NToolsCrAnglePage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrAnglePage.h"
#include "math.h"
#include "float.h"
#include "ConstDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAnglePage property page


IMPLEMENT_DYNCREATE(NToolsCrAnglePage, NToolsCrCommonPage)

NToolsCrAnglePage::NToolsCrAnglePage() : NToolsCrCommonPage(NToolsCrAnglePage::IDD)
{
	//{{AFX_DATA_INIT(NToolsCrAnglePage)
	m_diam = (MM_INCH == M_MM) ? 50.0 : 2.0;
	m_b = (MM_INCH == M_MM) ? 6.0 : 0.5;
	m_alf = 45.0;
	m_r = 0.0;		
	m_displacement = 0.0;
	m_name = _T("");
	m_check = FALSE;
	m_h1 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	m_h2 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	m_check2 = FALSE;
	//}}AFX_DATA_INIT
	m_name = (MM_INCH == M_MM) ? "D50B6A45Disk" : "D2B0.5A45Disk";
	IsNameAuto = true;
}

NToolsCrAnglePage::~NToolsCrAnglePage()
{
}

void NToolsCrAnglePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrAnglePage)
	DDX_Control(pDX, IDC_DISK_ANGLE_PIC2, m_Draw2);
	DDX_Control(pDX, IDC_DISK_ANGLE_PIC1, m_Draw1);
	DDX_Control(pDX, IDC_ANGLE_H2, m_h2_ctrl);
	DDX_Control(pDX, IDC_ANGLE_H1, m_h1_ctrl);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_DISK_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	DDX_Control(pDX, IDC_ANGLE_R1, m_r_ctrl);
	DDX_Control(pDX, IDC_ANGLE_ALF, m_alf_ctrl);
	DDX_Text(pDX, IDC_ANGLE_ALF, m_alf);
	DDX_Text(pDX, IDC_ANGLE_R1, m_r);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	DDV_MinMaxDouble(pDX, m_b, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	DDV_MinMaxDouble(pDX, m_diam, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Check(pDX, IDC_ANGLE_CHK, m_check);
	DDX_Text(pDX, IDC_ANGLE_H1, m_h1);
	DDX_Text(pDX, IDC_ANGLE_H2, m_h2);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrAnglePage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrAnglePage)
	ON_EN_CHANGE(IDC_DISK_DIAM, OnChangeDiskDiam)
	ON_EN_CHANGE(IDC_DISK_B, OnChangeDiskB)
	ON_EN_CHANGE(IDC_ANGLE_ALF, OnChangeAngleAlf)
	ON_EN_CHANGE(IDC_ANGLE_R1, OnChangeAngleR1)
	ON_BN_CLICKED(IDC_ANGLE_CHK, OnDiskCheck)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck2)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrAnglePage message handlers

BOOL NToolsCrAnglePage::OnInitDialog() 
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

	CheckDlgButton(IDC_ANGLE_CHK, m_check);
	CheckDlgButton(IDC_DISK_CHECK1, m_check2);

	m_r_ctrl.EnableWindow(m_check);
	m_h1_ctrl.EnableWindow(!m_check2);
	m_h2_ctrl.EnableWindow(!m_check2);
	
	m_Draw1.ShowWindow(!m_check ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check ? SW_SHOW : SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString& NToolsCrAnglePage::GetName()
{
	return m_name;
}

CString NToolsCrAnglePage::GenName()
{
	CString d, b, a, r, buf;
	double value;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	d.Format("D%s", buf);

	m_b_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	b.Format("B%s", buf);

	m_alf_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	a.Format("A%s", buf);

	m_r_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		r.Format("R%s", buf);

	return d+b+a+r+_T("Disk");
}

void NToolsCrAnglePage::OnKillfocusDiskName() 
{
 	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());
}

void NToolsCrAnglePage::OnDiskCheck() 
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
	pDX->PrepareEditCtrl(IDC_ANGLE_R1);
	DDX_Text(pDX, IDC_ANGLE_R1, m_r);
	delete pDX;

}

void NToolsCrAnglePage::OnDiskCheck2() 
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
	pDX->PrepareEditCtrl(IDC_ANGLE_H1);
	DDX_Text(pDX, IDC_ANGLE_H1, m_h1);
	pDX->PrepareEditCtrl(IDC_ANGLE_H2);
	DDX_Text(pDX, IDC_ANGLE_H2, m_h2);
	delete pDX;
}

void NToolsCrAnglePage::OnChangeDiskDiam() 
{
	if (m_diam_ctrl.GetWindowTextLength() != 0 && m_check2)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_DISK_DIAM);
		DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
		m_h1 = m_h2 = m_diam/2.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_ANGLE_H1);
		DDX_Text(pDX, IDC_ANGLE_H1, m_h1);
		pDX->PrepareEditCtrl(IDC_ANGLE_H2);
		DDX_Text(pDX, IDC_ANGLE_H2, m_h2);
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

void NToolsCrAnglePage::OnChangeDiskB() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAnglePage::OnChangeAngleAlf() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrAnglePage::OnChangeAngleR1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

bool NToolsCrAnglePage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введЄнние всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_b < 0. || m_alf < 0.
		|| m_h1 < 0. || m_h2 < 0)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
	if (m_alf == 0.)
	{
		AfxMessageBox(IDS_MES_ANGLE);
		m_alf_ctrl.SetFocus();		
		m_alf_ctrl.SetSel(0, -1, FALSE);
		return false;
	}

	return true;
}

void NToolsCrAnglePage::IsError(int IDErr)
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	double a;
	CString tmp,tmps;
	CString tmp1,tmp2;
	int  decimal1, decimal2, sign;
	switch(IDErr)
	{
	case 1:	// Ќедопустимый угол
		tmps.LoadString(IDS_MES_ANG);
		str += tmps;
		m_alf_ctrl.SetFocus();
		m_alf_ctrl.SetSel(0, -1, FALSE);
		break;
	case 6: case 8: // неверны параметры R2 или R3
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r_ctrl.SetFocus();
		m_r_ctrl.SetSel(0, -1, FALSE);
		break;
	case 10: // активна€ часть инструмента
		double faset;
		
		faset = m_b-m_r*(1-tan((PI/2.-(m_alf*PI/180.))/2.));
		a = faset*tan(PI/2. - PI*m_alf/180.);
		tmp1 = _fcvt(a, 3, &decimal1, &sign);
		tmp2 = _fcvt(m_diam/2., 3, &decimal2, &sign);
		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal1),tmp1.Mid(decimal1),
			tmp2.Mid(0,decimal2),tmp2.Mid(decimal2));
		
		m_h1_ctrl.SetFocus();
		m_h1_ctrl.SetSel(0, -1, FALSE);
		break;
	case 11: // активна€ часть инструмента
		a = m_r;
		tmp1 = _fcvt(a, 3, &decimal1, &sign);
		tmp2 = _fcvt(m_diam/2., 3, &decimal2, &sign);
		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal1),tmp1.Mid(decimal1),
			tmp2.Mid(0,decimal2),tmp2.Mid(decimal2));
		
		m_h2_ctrl.SetFocus();
		m_h2_ctrl.SetSel(0, -1, FALSE);
		break;
	}
	AfxMessageBox(str);
}

/*
BOOL NToolsCrAnglePage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
		::WinHelp((HWND) pHelpInfo->hItemHandle,
		AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, (DWORD) aHelpIDs);

	return TRUE;
}
*/
/*
void NToolsCrAnglePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	::WinHelp((HWND) *pWnd,	AfxGetApp()->m_pszHelpFilePath, 
	HELP_CONTEXTMENU, (DWORD) aHelpIDs);	
}
*/