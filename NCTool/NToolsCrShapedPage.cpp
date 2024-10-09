// NToolsCrShapedPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrShapedPage.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// NToolsCrShapedPage property page

IMPLEMENT_DYNCREATE(NToolsCrShapedPage, NToolsCrCommonPage)

NToolsCrShapedPage::NToolsCrShapedPage() : NToolsCrCommonPage(NToolsCrShapedPage::IDD)
{
	//{{AFX_DATA_INIT(NToolsCrShapedPage)
	m_diam = (MM_INCH == M_MM) ? 50.0 : 2.0;
	m_b = (MM_INCH == M_MM) ? 12.0 : 0.5;
	m_displacement = 0.0;
	m_name = (MM_INCH == M_MM) ? _T("D50B6A15F5Disk") : _T("D2B0.5A15F0.2");
	m_alf1 = 15.0;
	m_alf2 = 15.0;
	m_faset1 = (MM_INCH == M_MM) ? 5.0 : 0.2;
	m_faset2 = (MM_INCH == M_MM) ? 5.0 : 0.2;
	m_r1 = 0.0;
	m_r2 = 0.0;
	m_r3 = 0.0;
	m_r4 = 0.0;
	m_check = FALSE;
	m_h1 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	m_h2 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	//}}AFX_DATA_INIT
	IsNameAuto = true;
}

NToolsCrShapedPage::~NToolsCrShapedPage()
{
}

void NToolsCrShapedPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrShapedPage)
	DDX_Control(pDX, IDC_SHAPED_H2, m_h2_ctrl);
	DDX_Control(pDX, IDC_SHAPED_H1, m_h1_ctrl);
	DDX_Control(pDX, IDC_SHAPED_R4, m_r4_ctrl);
	DDX_Control(pDX, IDC_SHAPED_R3, m_r3_ctrl);
	DDX_Control(pDX, IDC_SHAPED_R2, m_r2_ctrl);
	DDX_Control(pDX, IDC_SHAPED_R1, m_r1_ctrl);
	DDX_Control(pDX, IDC_SHAPED_FASET2, m_faset2_ctrl);
	DDX_Control(pDX, IDC_SHAPED_FASET1, m_faset1_ctrl);
	DDX_Control(pDX, IDC_SHAPED_ALF2, m_alf2_ctrl);
	DDX_Control(pDX, IDC_SHAPED_ALF1, m_alf1_ctrl);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_DISK_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Text(pDX, IDC_SHAPED_ALF1, m_alf1);
	DDX_Text(pDX, IDC_SHAPED_ALF2, m_alf2);
	DDX_Text(pDX, IDC_SHAPED_FASET1, m_faset1);
	DDX_Text(pDX, IDC_SHAPED_FASET2, m_faset2);
	DDX_Text(pDX, IDC_SHAPED_R1, m_r1);
	DDX_Text(pDX, IDC_SHAPED_R2, m_r2);
	DDX_Text(pDX, IDC_SHAPED_R3, m_r3);
	DDX_Text(pDX, IDC_SHAPED_R4, m_r4);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check);
	DDX_Text(pDX, IDC_SHAPED_H1, m_h1);
	DDX_Text(pDX, IDC_SHAPED_H2, m_h2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrShapedPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrShapedPage)
	ON_EN_CHANGE(IDC_DISK_DIAM, OnChangeDiskDiam)
	ON_EN_CHANGE(IDC_DISK_B, OnChangeDiskB)
	ON_EN_CHANGE(IDC_SHAPED_R4, OnChangeShapedR4)
	ON_EN_CHANGE(IDC_SHAPED_R3, OnChangeShapedR3)
	ON_EN_CHANGE(IDC_SHAPED_R2, OnChangeShapedR2)
	ON_EN_CHANGE(IDC_SHAPED_R1, OnChangeShapedR1)
	ON_EN_CHANGE(IDC_SHAPED_FASET2, OnChangeShapedFaset2)
	ON_EN_CHANGE(IDC_SHAPED_FASET1, OnChangeShapedFaset1)
	ON_EN_CHANGE(IDC_SHAPED_ALF2, OnChangeShapedAlf2)
	ON_EN_CHANGE(IDC_SHAPED_ALF1, OnChangeShapedAlf1)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck4)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrShapedPage message handlers
BOOL NToolsCrShapedPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	bool flag;

	if (m_h1 == m_diam/2. && m_h2 == m_diam/2.)
		m_check = TRUE;
	else
		m_check = FALSE;

	if ((m_alf1 != 0. && m_faset1 != 0.) || m_r3 != 0.)
		flag = true;
	else
		flag = false;
	m_r3_ctrl.EnableWindow(flag);

	if ((m_alf2 != 0. && m_faset2 != 0.) || m_r4 != 0.)
		flag = true;
	else
		flag = false;
	m_r4_ctrl.EnableWindow(flag);

	m_h1_ctrl.EnableWindow(!m_check);
	m_h2_ctrl.EnableWindow(!m_check);

	CheckDlgButton(IDC_DISK_CHECK1, m_check);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString NToolsCrShapedPage::GenName()
{
	CString d, b, a, f, r, buf;
	double value;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	d.Format("D%s", buf);

	m_b_ctrl.GetWindowText(buf);
	if (buf == "")
	buf = "..";
	b.Format("B%s", buf);

	if (m_alf1 > m_alf2)
		m_alf1_ctrl.GetWindowText(buf);
	else
		m_alf2_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		a.Format("A%s", buf);

	if (m_faset1 > m_faset2)
		m_faset1_ctrl.GetWindowText(buf);
	else
		m_faset2_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		f.Format("F%s", buf);

	return d+b+a+f+_T("Disk");
}

void NToolsCrShapedPage::OnKillfocusDiskName() 
{
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());		
}

CString& NToolsCrShapedPage::GetName()
{	
	return m_name;
}

void NToolsCrShapedPage::OnDiskCheck4() 
{
	if (m_check)
		m_check = FALSE;
	else
		m_check = TRUE;

	m_h1_ctrl.EnableWindow(!m_check);
	m_h2_ctrl.EnableWindow(!m_check);

	if (m_diam_ctrl.GetWindowTextLength() == 0)
	{
		if (m_check)
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
	if (m_check)	
		m_h1 = m_h2 = m_diam/2.;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_SHAPED_H1);
	DDX_Text(pDX, IDC_SHAPED_H1, m_h1);
	pDX->PrepareEditCtrl(IDC_SHAPED_H2);
	DDX_Text(pDX, IDC_SHAPED_H1, m_h2);
	delete pDX;
}

void NToolsCrShapedPage::OnChangeDiskDiam() 
{
	if (m_diam_ctrl.GetWindowTextLength() != 0 && m_check)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_DISK_DIAM);
		DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
		m_h1 = m_h2 = m_diam/2.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_SHAPED_H1);
		DDX_Text(pDX, IDC_SHAPED_H1, m_h1);
		pDX->PrepareEditCtrl(IDC_SHAPED_H2);
		DDX_Text(pDX, IDC_SHAPED_H2, m_h2);
		delete pDX;
	}
	if(m_diam_ctrl.GetWindowTextLength() == 0 && m_check)
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

void NToolsCrShapedPage::OnChangeDiskB() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedAlf1() 
{
	bool flag;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_SHAPED_ALF1);
	DDX_Text(pDX, IDC_SHAPED_ALF1, m_alf1);
	DDX_Text(pDX, IDC_SHAPED_FASET1, m_faset1);
	if (m_alf1 != 0. && m_faset1 != 0.)
		flag = true;
	else
		flag = false;
	delete pDX;
	
	m_r3_ctrl.EnableWindow(flag);

	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}


void NToolsCrShapedPage::OnChangeShapedAlf2() 
{
	bool flag;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_SHAPED_ALF1);
	DDX_Text(pDX, IDC_SHAPED_ALF2, m_alf2);
	DDX_Text(pDX, IDC_SHAPED_FASET2, m_faset2);
	if (m_alf2 != 0. && m_faset2 != 0.)
		flag = true;
	else
		flag = false;
	delete pDX;
	
	m_r4_ctrl.EnableWindow(flag);

	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedFaset1() 
{
	bool flag;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_SHAPED_FASET1);
	DDX_Text(pDX, IDC_SHAPED_FASET1, m_faset1);
	DDX_Text(pDX, IDC_SHAPED_ALF1, m_alf1);
	if (m_alf1 != 0. && m_faset1 != 0.)
		flag = true;
	else
		flag = false;
	delete pDX;
	
	m_r3_ctrl.EnableWindow(flag);

	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedFaset2() 
{
	bool flag;
	
	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_SHAPED_FASET1);
	DDX_Text(pDX, IDC_SHAPED_FASET2, m_faset2);
	DDX_Text(pDX, IDC_SHAPED_ALF2, m_alf2);
	if (m_alf2 != 0. && m_faset2 != 0.)
		flag = true;
	else
		flag = false;
	delete pDX;
	
	m_r4_ctrl.EnableWindow(flag);

	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedR1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedR2() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedR3() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrShapedPage::OnChangeShapedR4() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

bool NToolsCrShapedPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_b < 0. 
		|| m_r1 < 0. || m_r2 < 0. || m_r3 < 0. || m_r4 < 0.
		|| m_faset1 < 0.|| m_faset2 < 0. 
		|| m_alf1 < 0.|| m_alf2 < 0. || m_h1 < 0. || m_h2 < 0)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
	if ((m_faset1 != 0. && m_alf1 == 0.) ||
		(m_faset1 == 0. && m_alf1 != 0.) ||
		(m_faset2 != 0. && m_alf2 == 0.) ||
		(m_faset2 == 0. && m_alf2 != 0.))
	{
		AfxMessageBox(IDS_MES_CHAMFAV);
		return false;
	}

	return true;
}

void NToolsCrShapedPage::IsError(int IDErr)
{
	double a;
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
	int  decimal1, decimal2, sign;
	switch (IDErr)
	{
	case 1:	// Недопустимый угол
		tmps.LoadString(IDS_MES_ANG);
		str += tmps;
		if (PI-m_alf1*PI/180. < MINAR)
		{
			m_alf1_ctrl.SetFocus();		m_alf1_ctrl.SetSel(0, -1, FALSE);
		}
		else
		{
			m_alf2_ctrl.SetFocus();		m_alf2_ctrl.SetSel(0, -1, FALSE);
		}
		break;
	case 2: // неверны размеры фасок
		tmps.LoadString(IDS_MES_CHAMFP);
		str += tmps;
		if (m_faset1 > m_b)
		{
			tmps.LoadString(IDS_MES_CHAMFB);
			str += tmps;
			m_faset1_ctrl.SetFocus();	m_faset1_ctrl.SetSel(0, -1, FALSE);
			break;
		}
		if (m_faset2 > m_b)
		{
			tmps.LoadString(IDS_MES_CHAMFB);
			str += tmps;
			m_faset2_ctrl.SetFocus();	m_faset2_ctrl.SetSel(0, -1, FALSE);
			break;
		}
		if (m_faset1 + m_faset2 > m_b)
		{
			tmps.LoadString(IDS_MES_CHAMFS);
			str += tmps;
			m_faset1_ctrl.SetFocus();	m_faset1_ctrl.SetSel(0, -1, FALSE);
		}
		break;
	case 3:	// неверны параметры фаски
		tmps.LoadString(IDS_MES_CHAMFA);
		str += tmps;
		m_alf1_ctrl.SetFocus();
		m_alf1_ctrl.SetSel(0, -1, FALSE);
		break;
	case 4: // неверны параметры фаски
		tmps.LoadString(IDS_MES_CHAMFA);
		str += tmps;
		m_alf2_ctrl.SetFocus();
		m_alf2_ctrl.SetSel(0, -1, FALSE);
		break;
	case 5: // неверны параметры R1
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r1_ctrl.SetFocus();
		m_r1_ctrl.SetSel(0, -1, FALSE);
		break;
	case 6: // неверны параметры R2 
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r2_ctrl.SetFocus();
		m_r2_ctrl.SetSel(0, -1, FALSE);
		break;
	case 7:	// неверны параметры R1 или R2
		tmps.LoadString(IDS_MES_RAD1);
		str += tmps;
		m_r1_ctrl.SetFocus();
		m_r1_ctrl.SetSel(0, -1, FALSE);
		break;
	case 8: // неверны параметры R3
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r3_ctrl.SetFocus();
		m_r3_ctrl.SetSel(0, -1, FALSE);
		break;
	case 9: // неверны параметры R4
		tmps.LoadString(IDS_MES_RAD);
		str += tmps;
		m_r4_ctrl.SetFocus();
		m_r4_ctrl.SetSel(0, -1, FALSE);
		break;
	case 10: // активная часть инструмента		
		if (m_faset1 != 0.)
			a = m_faset1*tan(PI/2. - m_alf1*PI/180.) + m_r1*tan(m_alf1*PI/180.);
		else
			a = m_r1;
		
		tmp1 = _fcvt(a, 3, &decimal1, &sign);
		tmp2 = _fcvt(m_diam/2., 3, &decimal2, &sign);
		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal1),tmp1.Mid(decimal1),
			tmp2.Mid(0,decimal2),tmp2.Mid(decimal2));
		
		m_h1_ctrl.SetFocus();
		m_h1_ctrl.SetSel(0, -1, FALSE);
		break;
	case 11: // активная часть инструмента
		if (m_faset2 != 0.)
			a = m_faset2*tan(PI/2. - m_alf2*PI/180.) + m_r2*tan(m_alf2*PI/180.);
		else
			a = m_r2;
	
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
BOOL NToolsCrShapedPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
		::WinHelp((HWND) pHelpInfo->hItemHandle,
		AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, (DWORD) aHelpIDs);

	return TRUE;
}
*/
/*
void NToolsCrShapedPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	::WinHelp((HWND) *pWnd,	AfxGetApp()->m_pszHelpFilePath, 
	HELP_CONTEXTMENU, (DWORD) aHelpIDs);	
}
*/