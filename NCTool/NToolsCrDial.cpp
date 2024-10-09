// NToolsCrDial.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolsCrDial.h"
#include "NToolStd.h"
#include "NToolCombinedMill.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOL_0 0
#define TOOL_1 1
#define TOOL_2 2
#define TOOL_3 3

/////////////////////////////////////////////////////////////////////////////
// NToolsCrDial dialog

NToolsCrDial::NToolsCrDial(CWnd* pParent /*=NULL*/)
	: CDialog(NToolsCrDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(NToolsCrDial)
	m_alf = 0.0;
	m_diam = (MM_INCH == M_MM) ? 10.0 : 0.75;
	m_h = (MM_INCH == M_MM) ? 50.0 : 2.0;
	m_r = 0.0;
	m_displacement = 0.0;
	m_beta = 0.0;
	m_faset = 0.;
	m_hm = m_h;
	m_check_flag = FALSE;
	m_check_flange = FALSE;
	//}}AFX_DATA_INIT
	IsNameAuto = true;
	m_name = (MM_INCH == M_MM) ? _T("D10H50") : _T("D0.75H2");
	pTool = NULL;
	ToolPics.Create(IDB_TOOL_PICS,30,10,RGB(255,255,255));
	m_OtherTool = false;
}


void NToolsCrDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrDial)
	DDX_Control(pDX, IDC_TOOL_OTHER, m_tool_other);
	DDX_Control(pDX, IDC_TOOL_DRAW6, m_Draw6);
	DDX_Control(pDX, IDC_TOOL_DRAW5, m_Draw5);
	DDX_Control(pDX, IDC_TOOL_DRAW4, m_Draw4);
	DDX_Control(pDX, IDC_TOOL_DRAW3, m_Draw3);
	DDX_Control(pDX, IDC_TOOL_DRAW2, m_Draw2);
	DDX_Control(pDX, IDC_TOOL_DRAW1, m_Draw1);
	DDX_Control(pDX, IDC_MILL_HM, m_hm_ctrl);
	DDX_Control(pDX, IDC_MILL_FACET, m_faset_ctrl);	
	DDX_Control(pDX, IDC_MILL_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_MILL_R, m_r_ctrl);
	DDX_Control(pDX, IDC_MILL_H, m_h_ctrl);
	DDX_Control(pDX, IDC_MILL_DIAM, m_diam_ctrl);
	DDX_Control(pDX, IDC_MILL_ALF, m_alf_ctrl);
	DDX_Control(pDX, IDC_TOOL_CHECK, m_check_flag_ctrl);
	DDX_Control(pDX, IDC_LABEL3, m_label3_ctrl);
	DDX_Control(pDX, IDC_LABEL2, m_label2_ctrl);
	DDX_Control(pDX, IDC_LABEL1, m_label1_ctrl);
	DDX_Text(pDX, IDC_MILL_ALF, m_alf);
	DDX_Text(pDX, IDC_MILL_DIAM, m_diam);
	DDV_MinMaxDouble(pDX, m_diam, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_MILL_H, m_h);
	DDV_MinMaxDouble(pDX, m_h, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_MILL_NAME, m_name);
	DDX_Text(pDX, IDC_MILL_R, m_r);
	DDV_MinMaxDouble(pDX, m_r, 0., 500.);
	DDX_Text(pDX, IDC_DISPLACEMENT, m_displacement);
	DDX_Control(pDX, IDC_DISPLACEMENT, m_displacement_ctrl);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Text(pDX, IDC_MILL_FACET, m_faset);
	DDX_Text(pDX, IDC_MILL_HM, m_hm);
	DDX_Check(pDX, IDC_TOOL_CHECK, m_check_flag);
	DDX_Check(pDX, IDC_CHECK_ALL, m_check_flange);
	DDX_Check(pDX, IDC_VERT_TOOL, m_vert_tool);
	DDX_Check(pDX, IDC_PROBE_TOOL, m_check_probe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NToolsCrDial, CDialog)
	//{{AFX_MSG_MAP(NToolsCrDial)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TOOLS_TAB, OnSelchangeToolsTab)
	ON_EN_CHANGE(IDC_MILL_DIAM, OnChangeMillDiam)
	ON_EN_CHANGE(IDC_MILL_H, OnChangeMillH)
	ON_EN_KILLFOCUS(IDC_MILL_R, OnChangeMillR)
	ON_EN_CHANGE(IDC_MILL_ALF, OnChangeMillAlf)
	ON_EN_CHANGE(IDC_MILL_FACET, OnChangeMillFacet)
	ON_BN_CLICKED(IDC_TOOL_CHECK, OnToolCheck)
	ON_BN_CLICKED(IDC_TOOL_OTHER, OnToolOther)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_EN_KILLFOCUS(IDC_MILL_NAME, OnKillfocusMillName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrDial message handlers

BOOL NToolsCrDial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Tab = (CTabCtrl * )GetDlgItem(IDC_TOOLS_TAB);
	ASSERT(Tab != NULL);
	Tab->SetImageList(&ToolPics);
	CString s0, s1, s2, s3;

	Tab->InsertItem(0,s0,0);
	Tab->InsertItem(1,s1,1);
	Tab->InsertItem(2,s2,2);
	Tab->InsertItem(3,s3,3);
	int tab_pos;
	if (m_r != 0. && m_faset == 0.)
	{
		if (m_alf != 0.)
		{
			tab_pos = TOOL_2;
			CString tmp;
			tmp.LoadString(IDS_SPH_END);
			SetDlgItemText(IDC_TOOL_CHECK,tmp);
		}
		else
		{
			tab_pos = TOOL_1;
			CString tmp;
			tmp.LoadString(IDS_SPH_END);
			SetDlgItemText(IDC_TOOL_CHECK,tmp);
		}
	}
	else
	{
		if (m_alf != 0. && m_beta == 0.)
		{
			tab_pos = TOOL_2;
			CString tmp;
			tmp.LoadString(IDS_SPH_END);
			SetDlgItemText(IDC_TOOL_CHECK,tmp);
		}
		else
		{
			if (m_beta != 0. && m_faset == 0)
			{
				tab_pos = TOOL_3;
				CString tmp;
				tmp.LoadString(IDS_NOT_USED);
				SetDlgItemText(IDC_TOOL_CHECK,tmp);
			}
			else
			{
				tab_pos = TOOL_0;
				CString tmp;
				tmp.LoadString(IDS_CHAMF_END);
				SetDlgItemText(IDC_TOOL_CHECK,tmp);
			}
			m_beta = 0.;
		}
	}

	Tab->SetCurSel(tab_pos);
	prev_tab_pos = tab_pos;
	if((tab_pos == TOOL_0 && m_faset != 0.)
		|| (tab_pos == TOOL_1 && fabs(m_r - m_diam/2.) < MRD)
		|| (tab_pos == TOOL_2 && fabs(m_r - m_diam/2./tan((PI/2.-m_alf*PI/180)/2.)) < MRD))
		m_check_flag = TRUE;
	else
		m_check_flag = FALSE;
	(m_h == m_hm) ? m_check_flange = TRUE : m_check_flange = FALSE;

	CheckDlgButton(IDC_TOOL_CHECK, m_check_flag);
	CheckDlgButton(IDC_CHECK_ALL, m_check_flange);
	m_r_ctrl.EnableWindow(	(m_check_flag == FALSE  && tab_pos == TOOL_2) ||
							(m_check_flag == FALSE  && tab_pos == TOOL_1));
	m_alf_ctrl.EnableWindow( tab_pos != TOOL_1 
		&& (m_check_flag == TRUE && tab_pos == TOOL_0) 
		|| tab_pos == TOOL_2 || tab_pos == TOOL_3);
	m_faset_ctrl.EnableWindow( tab_pos != TOOL_1 
		&& tab_pos != TOOL_2 
		&& tab_pos != TOOL_3
		&& (m_check_flag == TRUE && tab_pos == TOOL_0));
	m_hm_ctrl.EnableWindow( !m_check_flange );
	
	m_check_flag_ctrl.EnableWindow(tab_pos != TOOL_3);
	m_label1_ctrl.EnableWindow(tab_pos != TOOL_0 && tab_pos != TOOL_3);
	m_label2_ctrl.EnableWindow(tab_pos != TOOL_1);
	m_label3_ctrl.EnableWindow(tab_pos != TOOL_1 && tab_pos != TOOL_2 && tab_pos != TOOL_3);

	m_tool_other.EnableWindow(!m_OtherTool);
// visible pictures
	m_Draw1.ShowWindow((tab_pos == TOOL_0 && m_check_flag == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((tab_pos == TOOL_0 && m_check_flag == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw3.ShowWindow((tab_pos == TOOL_1) ? SW_SHOW : SW_HIDE);
	m_Draw4.ShowWindow((tab_pos == TOOL_2 && m_check_flag == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw5.ShowWindow((tab_pos == TOOL_2 && m_check_flag == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw6.ShowWindow((tab_pos == TOOL_3) ? SW_SHOW : SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// определение созданного простого инструмента
NToolStd * NToolsCrDial::GetNewTool()
{
	return pTool;
}

// определение инмени инструмента
CString& NToolsCrDial::GetName()
{
	return m_name;
}

void NToolsCrDial::OnCancel() 
{
 	if(pTool) delete pTool;
 
 	CDialog::OnCancel();
}

void NToolsCrDial::OnToolOther() 
{
	
	m_OtherTool = true;
	EndDialog(IDCANCEL);
}

void NToolsCrDial::SetOtherTool(bool flag)
{
	m_OtherTool = flag;
}

bool NToolsCrDial::GetOtherTool()
{
	return m_OtherTool;
}

void NToolsCrDial::OnOK() 
{
	if(!UpdateData())
		return;
// проверки на введение всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return;
	}
	if (m_r < 0. || m_faset <0. || m_hm <0.)
	{
		AfxMessageBox(IDS_MES_POSV);
		return;
	}
	int tab_pos = Tab->GetCurSel();
	if (tab_pos == TOOL_0 && m_check_flag && (m_faset == 0. || m_alf == 0.))
	{
		AfxMessageBox(IDS_MES_TFA);
		return;
	}
	
	switch( tab_pos )
	{
	case TOOL_0:
		m_diam -= TOOL_OFFSET;//!To prevent equality between diam/2 and trajectory arc radius
		if (m_check_flag == TRUE)
		{
			m_r = m_faset; // искусcтвенный приём
			m_beta = 90-m_alf;
			m_alf = 0.;
		}
		else
			m_r = m_alf = 0.;
		break;
	case TOOL_1:
		m_diam -= TOOL_OFFSET;//!To prevent equality between diam/2 and trajectory arc radius
		m_alf = 0.;
		if (m_check_flag == TRUE)
			m_r = m_diam/2.;
		break;
	case TOOL_2:
		if (m_check_flag == TRUE)
			m_r = NToolStd::CalcEndRad(m_diam, m_alf  * PI / 180.);
		break;
	case TOOL_3:
		m_beta = (180 - m_alf)/2.;
		m_r = m_alf = 0.;
		break;
	}
	// простой инструмент
	if(pTool) delete pTool;
	pTool = new NToolStd(m_name,m_diam,m_h,m_alf,m_r,m_beta,0.,0.,m_hm);
	pTool->IsNameAuto = IsNameAuto;
	pTool->SetType(11+tab_pos);
	pTool->SetVertTool(m_vert_tool == TRUE);
	
	int IsErr = pTool->GenInit();
	pTool->SetProbe(m_check_probe == TRUE);
	if (IsErr != 0)
	{
		CString str;
		str.LoadString(IDS_MES_PAR);
		CString tmp, tmps;
		CString tmp1, tmp2;
		switch (IsErr)
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
			m_alf_ctrl.SetFocus();
			m_alf_ctrl.SetSel(0, -1, FALSE);
			break;
		case 3:
			tmps.LoadString(IDS_MES_CHAMFD);
			str += tmps;
			m_faset_ctrl.SetFocus();
			m_faset_ctrl.SetSel(0, -1, FALSE);
			break;
		case 4:
			tmps.LoadString(IDS_MES_CHAMFA);
			str += tmps;
			m_alf_ctrl.SetFocus();
			m_alf_ctrl.SetSel(0, -1, FALSE);
			break;
		case 5:
			tmps.LoadString(IDS_MES_DRA);
			str += tmps;
			m_alf_ctrl.SetFocus();
			m_alf_ctrl.SetSel(0, -1, FALSE);
			break;
		case 6:
			double a;
			int  decimal1, decimal2, sign;

			if (tab_pos == TOOL_0)
				(m_beta != 0.) ? a = m_faset * tan(PI / 2. - m_beta*PI / 180) : a = 0.;
			if (tab_pos == TOOL_1)
				a = m_r;
			if (tab_pos == TOOL_2)
				a = m_r*tan((PI / 2. - m_alf*PI / 180.) / 2.) * cos(m_alf * PI / 180.);
			if (tab_pos == TOOL_3)
				(m_beta != 0.) ? a = m_diam / 2. * tan(m_beta * PI / 180.) : a = 0;

			tmp1 = _fcvt(a, 3, &decimal1, &sign);
			tmp2 = _fcvt(m_h, 3, &decimal2, &sign);
			str.Format(IDS_MES_INT,
				tmp1.Mid(0, decimal1), tmp1.Mid(decimal1),
				tmp2.Mid(0, decimal2), tmp2.Mid(decimal2));
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
		delete pTool;
		pTool = NULL;
		return;
	}
	pTool->SetZdisp(m_displacement);
	CDialog::OnOK();
}

void NToolsCrDial::OnSelchangeToolsTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	CString str;
	if( ! UpdateData() )
	{
		Tab->SetCurSel(prev_tab_pos);
		return;
	}
	int tab_pos = Tab->GetCurSel();
	m_vert_tool = FALSE;
	switch( tab_pos )
	{
	case TOOL_0:
		m_r = m_alf = m_faset = m_beta = 0.;
		m_check_flag = FALSE;
		str.LoadString(IDS_CHAMF_END);
		break;
	case TOOL_1:
		m_r = m_diam/2.;
		m_alf = m_faset = m_beta = 0.;
		m_check_flag = TRUE;
//		m_check_flange = FALSE;
		str.LoadString(IDS_SPH_END);
		break;
	case TOOL_2:
		m_r = m_alf = m_faset = m_beta = 0.;
		m_check_flag = FALSE;
		str.LoadString(IDS_SPH_END);
		break;
	case TOOL_3:
		m_r = m_faset = m_beta = 0.;
		m_alf = 118.;
		m_check_flag = FALSE;
		str.LoadString(IDS_NOT_USED);
		break;
	}
	//if(!m_check_flange)
	//	m_hm = m_r;
	UpdateData(FALSE);
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
	SetDlgItemText(IDC_TOOL_CHECK, str);

	CheckDlgButton(IDC_TOOL_CHECK, m_check_flag);

	m_r_ctrl.EnableWindow(	(m_check_flag == FALSE  && tab_pos == TOOL_2) ||
							(m_check_flag == FALSE  && tab_pos == TOOL_1));
	m_alf_ctrl.EnableWindow( tab_pos != TOOL_1 
		&& (m_check_flag == TRUE && tab_pos == TOOL_0) 
		|| tab_pos == TOOL_2 || tab_pos == TOOL_3);
	m_faset_ctrl.EnableWindow( tab_pos != TOOL_1 
		&& tab_pos != TOOL_2 
		&& tab_pos != TOOL_3
		&& (m_check_flag == TRUE && tab_pos == TOOL_0));

	m_check_flag_ctrl.EnableWindow(tab_pos != TOOL_3);
	m_label1_ctrl.EnableWindow(tab_pos != TOOL_0 && tab_pos != TOOL_3);
	m_label2_ctrl.EnableWindow(tab_pos != TOOL_1);
	m_label3_ctrl.EnableWindow(tab_pos != TOOL_1 && tab_pos != TOOL_2 && tab_pos != TOOL_3);

	m_Draw1.ShowWindow((tab_pos == TOOL_0 && m_check_flag == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((tab_pos == TOOL_0 && m_check_flag == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw3.ShowWindow((tab_pos == TOOL_1) ? SW_SHOW : SW_HIDE);
	m_Draw4.ShowWindow((tab_pos == TOOL_2 && m_check_flag == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw5.ShowWindow((tab_pos == TOOL_2 && m_check_flag == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw6.ShowWindow((tab_pos == TOOL_3) ? SW_SHOW : SW_HIDE);

	prev_tab_pos = tab_pos;
	*pResult = 0;
}

CString NToolsCrDial::GenName()
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

	m_faset_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		r.Format("F%s", buf);

//	UpdateData();
	return d+h+r+a;

}

void NToolsCrDial::OnChangeMillDiam() 
{
	
	int tab_pos = Tab->GetCurSel();
	if(m_diam_ctrl.GetWindowTextLength() != 0 && m_check_flag && 
		(tab_pos == TOOL_1 || tab_pos == TOOL_2))
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_MILL_DIAM);
		DDX_Text(pDX, IDC_MILL_DIAM, m_diam);
		if (tab_pos == TOOL_1)
			m_r = m_diam / 2.;
		else
			m_r = NToolStd::CalcEndRad(m_diam, m_alf  * PI / 180.);
		m_r = ceil(m_r*1000.) / 1000.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_MILL_R);
		DDX_Text(pDX, IDC_MILL_R, m_r);
		delete pDX;
		OnChangeMillR();
	}
	if(m_diam_ctrl.GetWindowTextLength() == 0 && m_check_flag && 
		(tab_pos == TOOL_1 || tab_pos == TOOL_2))
	{
		m_r_ctrl.SetWindowText("0");
	}

	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrDial::OnChangeMillH() 
{
	
	if (m_h_ctrl.GetWindowTextLength() != 0 && m_check_flange)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_MILL_H);
		DDX_Text(pDX, IDC_MILL_H, m_h);
		m_hm = m_h;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_MILL_HM);
		DDX_Text(pDX, IDC_MILL_HM, m_hm);
		delete pDX;
	}
	else
	{
		if(m_check_flange)
			m_hm_ctrl.SetWindowText("0");
	}
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrDial::OnChangeMillR() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
	UpdateData(TRUE);
}

void NToolsCrDial::OnChangeMillAlf() 
{
	
	int tab_pos = Tab->GetCurSel();
	if(m_alf_ctrl.GetWindowTextLength() != 0 && 
		m_check_flag && tab_pos == TOOL_2)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_MILL_ALF);
		DDX_Text(pDX, IDC_MILL_ALF, m_alf);
		m_r = NToolStd::CalcEndRad(m_diam, m_alf  * PI / 180.);
		m_r = ceil(m_r*1000.) / 1000.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_MILL_R);
		DDX_Text(pDX, IDC_MILL_R, m_r);
		delete pDX;
		OnChangeMillR();
	}

	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrDial::OnChangeMillFacet() 
{
	
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrDial::OnKillfocusMillName() 
{
	
 	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());
}

void NToolsCrDial::OnToolCheck() 
{
	
	if (m_check_flag)
		m_check_flag = FALSE;
	else
		m_check_flag = TRUE;
	
	int tab_pos = Tab->GetCurSel();
// саздание активных и не активных полей на закладке
	m_r_ctrl.EnableWindow(	(m_check_flag == FALSE  && tab_pos == TOOL_2) ||
							(m_check_flag == FALSE  && tab_pos == TOOL_1));
	m_alf_ctrl.EnableWindow( tab_pos != TOOL_1 
		&& (m_check_flag == TRUE && tab_pos == TOOL_0) 
		|| tab_pos == TOOL_2 || tab_pos == TOOL_3);
	m_faset_ctrl.EnableWindow( tab_pos != TOOL_1 
		&& tab_pos != TOOL_2 
		&& tab_pos != TOOL_3
		&& (m_check_flag == TRUE && tab_pos == TOOL_0));
// создание видимой картинки
	m_Draw1.ShowWindow((tab_pos == TOOL_0 && m_check_flag == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow((tab_pos == TOOL_0 && m_check_flag == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw3.ShowWindow((tab_pos == TOOL_1) ? SW_SHOW : SW_HIDE);
	m_Draw4.ShowWindow((tab_pos == TOOL_2 && m_check_flag == FALSE) ? SW_SHOW : SW_HIDE);
	m_Draw5.ShowWindow((tab_pos == TOOL_2 && m_check_flag == TRUE) ? SW_SHOW : SW_HIDE);
	m_Draw6.ShowWindow((tab_pos == TOOL_3) ? SW_SHOW : SW_HIDE);
// проверка на пустое поле
	if ((m_diam_ctrl.GetWindowTextLength() == 0 ||
		m_alf_ctrl.GetWindowTextLength() == 0) &&
		(tab_pos==TOOL_1 || tab_pos==TOOL_2))
	{
		if (m_check_flag)
			m_r_ctrl.SetWindowText("0");
		else
			m_r_ctrl.SetWindowText("0");
		return;
	}
// заполнение данными полей
	if (tab_pos==TOOL_1 || tab_pos==TOOL_2)
	{
		CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
		pDX->PrepareEditCtrl(IDC_MILL_DIAM);
		DDX_Text(pDX, IDC_MILL_DIAM, m_diam);
		pDX->PrepareEditCtrl(IDC_MILL_ALF);
		DDX_Text(pDX, IDC_MILL_ALF, m_alf);
		if ( m_check_flag == TRUE )
		{
			if (tab_pos == TOOL_1)
				m_r = m_diam/2.;
			else
				m_r = m_diam/2.*tan(PI/4.+m_alf*PI/360.);
		}
		else
			m_r = 0.;
	
		m_r = int(m_r*1000.)/1000.;
		pDX->m_bSaveAndValidate = FALSE;
		pDX->PrepareEditCtrl(IDC_MILL_R);
		DDX_Text(pDX, IDC_MILL_R, m_r);
		delete pDX;
		OnChangeMillR();
	}
}

void NToolsCrDial::OnCheckAll() 
{
	
	if (m_check_flange)
		m_check_flange = FALSE;
	else
		m_check_flange = TRUE;
	
	m_hm_ctrl.EnableWindow( !m_check_flange );

	if (m_h_ctrl.GetWindowTextLength() == 0)
	{
		if (m_check_flange)
			m_hm_ctrl.SetWindowText("0");
		else
			m_hm_ctrl.SetWindowText("0");
		return;
	}

	CDataExchange* pDX = new CDataExchange((CWnd *)this,TRUE);
	pDX->PrepareEditCtrl(IDC_MILL_H);
	DDX_Text(pDX, IDC_MILL_H, m_h);
	m_hm = m_h;
	pDX->m_bSaveAndValidate = FALSE;
	pDX->PrepareEditCtrl(IDC_MILL_HM);
	DDX_Text(pDX, IDC_MILL_HM, m_hm);
	delete pDX;
}
