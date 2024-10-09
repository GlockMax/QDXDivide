// NToolsCrFasetPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrFasetPage.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrFasetPage property page

IMPLEMENT_DYNCREATE(NToolsCrFasetPage, NToolsCrCommonPage)

NToolsCrFasetPage::NToolsCrFasetPage() : NToolsCrCommonPage(NToolsCrFasetPage::IDD)
{
	//{{AFX_DATA_INIT(NToolsCrFasetPage)
	m_diam = (MM_INCH == M_MM) ? 50.0 : 2.0;
	m_b = (MM_INCH == M_MM) ? 20.0 : 0.75;
	m_check1 = FALSE;
	m_check2 = FALSE;
	m_displacement = 0.0;
	m_name = (MM_INCH == M_MM) ? _T("D10B3A45F6Disk") : _T("D2B0.75A45F0.25Disk");
	m_alf1 = 45.0;
	m_alf2 = 0.0;
	m_faset1 = (MM_INCH == M_MM) ? 6.0 : 0.25;
	m_faset2 = 0.0;
	m_check3 = FALSE;
	m_h1 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	m_h2 = (MM_INCH == M_MM) ? 25.0 : 1.0;
	//}}AFX_DATA_INIT
	IsNameAuto = true;
}

NToolsCrFasetPage::~NToolsCrFasetPage()
{
}

void NToolsCrFasetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrFasetPage)
	DDX_Control(pDX, IDC_DISK_H2, m_h2_ctrl);
	DDX_Control(pDX, IDC_DISK_H1, m_h1_ctrl);
	DDX_Control(pDX, IDC_DISK_FASET_PIC2, m_Draw2);
	DDX_Control(pDX, IDC_DISK_FASET_PIC1, m_Draw1);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_FASET_FASET2, m_faset2_ctrl);
	DDX_Control(pDX, IDC_FASET_FASET1, m_faset1_ctrl);
	DDX_Control(pDX, IDC_FASET_ALF2, m_alf2_ctrl);
	DDX_Control(pDX, IDC_FASET_ALF1, m_alf1_ctrl);
	DDX_Control(pDX, IDC_DISK_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	DDX_Text(pDX, IDC_DISK_B, m_b);
	DDV_MinMaxDouble(pDX, m_b, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
	DDV_MinMaxDouble(pDX, m_diam, MINTOOLDIM, 500.);
	DDX_Check(pDX, IDC_FASET_CHK1, m_check1);
	DDX_Check(pDX, IDC_FASET_CHK2, m_check2);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Text(pDX, IDC_FASET_ALF1, m_alf1);
	DDX_Text(pDX, IDC_FASET_ALF2, m_alf2);
	DDX_Text(pDX, IDC_FASET_FASET1, m_faset1);
	DDX_Text(pDX, IDC_FASET_FASET2, m_faset2);
	DDX_Check(pDX, IDC_DISK_CHECK1, m_check3);
	DDX_Text(pDX, IDC_DISK_H1, m_h1);
	DDX_Text(pDX, IDC_DISK_H2, m_h2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrFasetPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrFasetPage)
	ON_EN_CHANGE(IDC_DISK_B, OnChangeDiskB)
	ON_BN_CLICKED(IDC_FASET_CHK1, OnDiskCheck1)
	ON_BN_CLICKED(IDC_FASET_CHK2, OnDiskCheck2)
	ON_EN_CHANGE(IDC_DISK_DIAM, OnChangeDiskDiam)
	ON_EN_CHANGE(IDC_FASET_ALF1, OnChangeFasetAlf1)
	ON_EN_CHANGE(IDC_FASET_ALF2, OnChangeFasetAlf2)
	ON_EN_CHANGE(IDC_FASET_FASET1, OnChangeFasetFaset1)
	ON_EN_CHANGE(IDC_FASET_FASET2, OnChangeFasetFaset2)
	ON_BN_CLICKED(IDC_DISK_CHECK1, OnDiskCheck3)
	ON_EN_KILLFOCUS(IDC_DISK_NAME, OnKillfocusDiskName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrFasetPage message handlers

BOOL NToolsCrFasetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (m_alf1 != 0. || m_faset1 != 0.)
		m_check1 = TRUE;
	else
		m_check1 = FALSE;
	if (m_alf2 != 0. || m_faset2 != 0.)
		m_check2 = TRUE;
	else
		m_check2 = FALSE;
	if (m_h1 == m_diam/2. && m_h2 == m_diam/2.)
		m_check3 = TRUE;
	else
		m_check3 = FALSE;

	m_alf1_ctrl.EnableWindow(m_check1);
	m_alf2_ctrl.EnableWindow(m_check2);
	m_faset1_ctrl.EnableWindow(m_check1);
	m_faset2_ctrl.EnableWindow(m_check2);
	m_h1_ctrl.EnableWindow(!m_check3);
	m_h2_ctrl.EnableWindow(!m_check3);

	CheckDlgButton(IDC_FASET_CHK1, m_check1);
	CheckDlgButton(IDC_FASET_CHK2, m_check2);
	CheckDlgButton(IDC_DISK_CHECK1, m_check3);
	
	m_Draw1.ShowWindow((m_check1 == FALSE && m_check2 == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((m_check1 == TRUE || m_check2 == TRUE) ? SW_SHOW : SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString& NToolsCrFasetPage::GetName()
{
	return m_name;
}

CString NToolsCrFasetPage::GenName()
{
	CString d, b, a, f, buf;
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

void NToolsCrFasetPage::OnKillfocusDiskName() 
{
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());
}

void NToolsCrFasetPage::OnDiskCheck1() 
{
	if (m_check1)
		m_check1 = FALSE;
	else
		m_check1 = TRUE;
	
	m_alf1_ctrl.EnableWindow(m_check1);
	m_alf2_ctrl.EnableWindow(m_check2);
	m_faset1_ctrl.EnableWindow(m_check1);
	m_faset2_ctrl.EnableWindow(m_check2);

// создание видимой картинки
	m_Draw1.ShowWindow((!m_check1 && !m_check2) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((m_check1 || m_check2) ? SW_SHOW : SW_HIDE);
// заполнение полей формы
	if (!m_check1)
	{
		m_alf1_ctrl.SetWindowText("0");
		m_faset1_ctrl.SetWindowText("0");
	}
}

void NToolsCrFasetPage::OnDiskCheck2() 
{
	if (m_check2)
		m_check2 = FALSE;
	else
		m_check2 = TRUE;

	m_alf1_ctrl.EnableWindow(m_check1);
	m_alf2_ctrl.EnableWindow(m_check2);
	m_faset1_ctrl.EnableWindow(m_check1);
	m_faset2_ctrl.EnableWindow(m_check2);
// создание видимой картинки
	m_Draw1.ShowWindow((m_check1 == FALSE && m_check2 == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((m_check1 == TRUE || m_check2 == TRUE) ? SW_SHOW : SW_HIDE);
// заполнение полей формы
	if (!m_check2)
	{
		m_alf2_ctrl.SetWindowText("0");
		m_faset2_ctrl.SetWindowText("0");
	}
}

void NToolsCrFasetPage::OnDiskCheck3() 
{
	if (m_check3)
		m_check3 = FALSE;
	else
		m_check3 = TRUE;

	m_h1_ctrl.EnableWindow(!m_check3);
	m_h2_ctrl.EnableWindow(!m_check3);

	if (m_diam_ctrl.GetWindowTextLength() == 0)
	{
		if (m_check3)
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
	if (m_check3)	
		m_h1 = m_h2 = m_diam/2.;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_DISK_H1);
	DDX_Text(pDX, IDC_DISK_H2, m_h1);
	pDX->PrepareEditCtrl(IDC_DISK_H2);
	DDX_Text(pDX, IDC_DISK_H2, m_h2);
	delete pDX;
}

void NToolsCrFasetPage::OnChangeDiskDiam() 
{
	if (m_diam_ctrl.GetWindowTextLength() != 0 && m_check3)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_DISK_DIAM);
		DDX_Text(pDX, IDC_DISK_DIAM, m_diam);
		m_h1 = m_h2 = m_diam/2.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_DISK_H1);
		DDX_Text(pDX, IDC_DISK_H1, m_h1);
		pDX->PrepareEditCtrl(IDC_DISK_H2);
		DDX_Text(pDX, IDC_DISK_H2, m_h2);
		delete pDX;
	}
	if(m_diam_ctrl.GetWindowTextLength() == 0 && m_check3)
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

void NToolsCrFasetPage::OnChangeDiskB() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrFasetPage::OnChangeFasetAlf1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrFasetPage::OnChangeFasetAlf2() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrFasetPage::OnChangeFasetFaset1() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrFasetPage::OnChangeFasetFaset2() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

bool NToolsCrFasetPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	if (m_diam < 0. || m_b < 0. || m_alf1 < 0. || m_alf2 < 0. 
		|| m_faset1 < 0. || m_faset2 < 0. || m_h1 < 0. || m_h2 < 0)
	{
		AfxMessageBox(IDS_MES_POSV);
		return false;
	}
	if (m_check1 && (m_faset1 == 0. || m_alf1 == 0.))
	{
		if (m_alf1 == 0.)
		{
			AfxMessageBox(IDS_MES_ANGLE);
			m_alf1_ctrl.SetFocus();		m_alf1_ctrl.SetSel(0, -1, FALSE);
		}
		else
		{
			AfxMessageBox(IDS_MES_CHAMF);
			m_faset1_ctrl.SetFocus();	m_faset1_ctrl.SetSel(0, -1, FALSE);
		}
		return false;
	}
	if (m_check2 && (m_faset2 == 0. || m_alf2 == 0.))
	{
		if (m_alf2 == 0.)
		{
			AfxMessageBox(IDS_MES_ANGLE);
			m_alf2_ctrl.SetFocus();		m_alf2_ctrl.SetSel(0, -1, FALSE);
		}
		else
		{
			AfxMessageBox(IDS_MES_CHAMF);
			m_faset2_ctrl.SetFocus();	m_faset2_ctrl.SetSel(0, -1, FALSE);
		}
		return false;
	}

	return true;
}

void NToolsCrFasetPage::IsError(int IDErr)
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
	double a;
	int  decimal1, decimal2, sign;
	switch (IDErr)
	{
	case 1:	// неверны фаски
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
	case 2: case 8: case 9:	// неверны размеры фасок
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
	case 10: // активная часть инструмента		
		a = m_faset1*tan(PI/2. - PI*m_alf1/180.);
		tmp1 = _fcvt(a, 3, &decimal1, &sign);
		tmp2 = _fcvt(m_diam/2., 3, &decimal2, &sign);
		str.Format(IDS_MES_INT,
			tmp1.Mid(0,decimal1),tmp1.Mid(decimal1),
			tmp2.Mid(0,decimal2),tmp2.Mid(decimal2));

		m_h1_ctrl.SetFocus();	
		m_h1_ctrl.SetSel(0, -1, FALSE);
		break;
	case 11: // активная часть инструмента
		a = m_faset2*tan(PI/2. - PI*m_alf2/180.);
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
BOOL NToolsCrFasetPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
		::WinHelp((HWND) pHelpInfo->hItemHandle,
		AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, (DWORD) aHelpIDs);

	return TRUE;
}
*/
/*
void NToolsCrFasetPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	::WinHelp((HWND) *pWnd,	AfxGetApp()->m_pszHelpFilePath, 
	HELP_CONTEXTMENU, (DWORD) aHelpIDs);	
}
*/