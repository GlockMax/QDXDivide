// NToolsCrWeldPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolWeld.h"
#include "NToolsCrWeldPage.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// NToolsCrWeldPage property page

IMPLEMENT_DYNCREATE(NToolsCrWeldPage, CPropertyPage)

NToolsCrWeldPage::NToolsCrWeldPage() : CPropertyPage(NToolsCrWeldPage::IDD)
, m_focus(0)
{
	//{{AFX_DATA_INIT(NToolsCrWeldPage)
	m_displacement = 0.0;
	m_diam = (MM_INCH == M_MM) ? 50.0 : 2.0;
	m_b = (MM_INCH == M_MM) ? 12.0 : 0.5;
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

NToolsCrWeldPage::~NToolsCrWeldPage()
{
}

void NToolsCrWeldPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsCrWeldPage)
	DDX_Text(pDX, IDC_DISPLACEMENT, m_displacement);
	DDX_Control(pDX, IDC_DISPLACEMENT, m_displacement_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_H2, m_h2_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_H1, m_h1_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_R4, m_r4_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_R3, m_r3_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_R2, m_r2_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_R1, m_r1_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_FASET2, m_faset2_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_FASET1, m_faset1_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_ALF2, m_alf2_ctrl);
	//DDX_Control(pDX, IDC_SHAPED_ALF1, m_alf1_ctrl);
	DDX_Control(pDX, IDC_WELD_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_WELD_DIAM, m_diam_ctrl);
	//DDX_Control(pDX, IDC_DISK_B, m_b_ctrl);
	//DDX_Text(pDX, IDC_DISK_B, m_b);
	DDX_Text(pDX, IDC_WELD_DIAM, m_diam);
	//DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	//DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	//DDX_Control(pDX, IDC_DISK_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_WELD_NAME, m_name);
	//DDX_Text(pDX, IDC_SHAPED_ALF1, m_alf1);
	//DDX_Text(pDX, IDC_SHAPED_ALF2, m_alf2);
	//DDX_Text(pDX, IDC_SHAPED_FASET1, m_faset1);
	//DDX_Text(pDX, IDC_SHAPED_FASET2, m_faset2);
	//DDX_Text(pDX, IDC_SHAPED_R1, m_r1);
	//DDX_Text(pDX, IDC_SHAPED_R2, m_r2);
	//DDX_Text(pDX, IDC_SHAPED_R3, m_r3);
	//DDX_Text(pDX, IDC_SHAPED_R4, m_r4);
	//DDX_Check(pDX, IDC_DISK_CHECK1, m_check);
	//DDX_Text(pDX, IDC_SHAPED_H1, m_h1);
	//DDX_Text(pDX, IDC_SHAPED_H2, m_h2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WELD_FOCUS, m_focus_ctrl);
	DDX_Text(pDX, IDC_WELD_FOCUS, m_focus);
}


BEGIN_MESSAGE_MAP(NToolsCrWeldPage, CPropertyPage)
	//{{AFX_MSG_MAP(NToolsCrWeldPage)
	ON_EN_KILLFOCUS(IDC_WELD_NAME, OnKillfocusName)
	ON_EN_KILLFOCUS(IDC_WELD_FOCUS, &NToolsCrWeldPage::OnEnKillfocusWeldFocus)
	ON_EN_KILLFOCUS(IDC_WELD_DIAM, &NToolsCrWeldPage::OnEnKillfocusWeldDiam)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsCrWeldPage message handlers
BOOL NToolsCrWeldPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_diam = 10.;
	m_focus = 200.;

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString NToolsCrWeldPage::GenName()
{
	CString d, b, a, f, r, buf;

	m_diam_ctrl.GetWindowText(buf);
	if (buf == "")
		buf = "..";
	d.Format("D%s", buf);

	m_focus_ctrl.GetWindowText(buf);
	if (buf == "")
		buf = "..";
	b.Format("B%s", buf);


	return d + b + _T("Beam");
}

void NToolsCrWeldPage::OnKillfocusName() 
{
	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());		
}

CString& NToolsCrWeldPage::GetName()
{	
	return m_name;
}


bool NToolsCrWeldPage::IsOkInput()
{
	if(!UpdateData())
		return false;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	return true;
}

void NToolsCrWeldPage::IsError(int IDErr)
{
//	double a;
	CString str;
	str.LoadString(IDS_MES_PAR);
	CString tmp,tmps;
	CString tmp1,tmp2;
	//int  decimal, sign;
	//switch(IDErr)
	//{
	//case 1:	// Недопустимый угол
	//	tmps.LoadString(IDS_MES_ANG);
	//	str += tmps;
	//	if (PI-m_alf1*PI/180. < MINAR)
	//	{
	//		m_alf1_ctrl.SetFocus();		m_alf1_ctrl.SetSel(0, -1, FALSE);
	//	}
	//	else
	//	{
	//		m_alf2_ctrl.SetFocus();		m_alf2_ctrl.SetSel(0, -1, FALSE);
	//	}
	//	break;
	//case 2: // неверны размеры фасок
	//	tmps.LoadString(IDS_MES_CHAMFP);
	//	str += tmps;
	//	if (m_faset1 > m_b)
	//	{
	//		tmps.LoadString(IDS_MES_CHAMFB);
	//		str += tmps;
	//		m_faset1_ctrl.SetFocus();	m_faset1_ctrl.SetSel(0, -1, FALSE);
	//		break;
	//	}
	//	if (m_faset2 > m_b)
	//	{
	//		tmps.LoadString(IDS_MES_CHAMFB);
	//		str += tmps;
	//		m_faset2_ctrl.SetFocus();	m_faset2_ctrl.SetSel(0, -1, FALSE);
	//		break;
	//	}
	//	if (m_faset1 + m_faset2 > m_b)
	//	{
	//		tmps.LoadString(IDS_MES_CHAMFS);
	//		str += tmps;
	//		m_faset1_ctrl.SetFocus();	m_faset1_ctrl.SetSel(0, -1, FALSE);
	//	}
	//	break;
	//case 3:	// неверны параметры фаски
	//	tmps.LoadString(IDS_MES_CHAMFA);
	//	str += tmps;
	//	m_alf1_ctrl.SetFocus();
	//	m_alf1_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 4: // неверны параметры фаски
	//	tmps.LoadString(IDS_MES_CHAMFA);
	//	str += tmps;
	//	m_alf2_ctrl.SetFocus();
	//	m_alf2_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 5: // неверны параметры R1
	//	tmps.LoadString(IDS_MES_RAD);
	//	str += tmps;
	//	m_r1_ctrl.SetFocus();
	//	m_r1_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 6: // неверны параметры R2 
	//	tmps.LoadString(IDS_MES_RAD);
	//	str += tmps;
	//	m_r2_ctrl.SetFocus();
	//	m_r2_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 7:	// неверны параметры R1 или R2
	//	tmps.LoadString(IDS_MES_RAD1);
	//	str += tmps;
	//	m_r1_ctrl.SetFocus();
	//	m_r1_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 8: // неверны параметры R3
	//	tmps.LoadString(IDS_MES_RAD);
	//	str += tmps;
	//	m_r3_ctrl.SetFocus();
	//	m_r3_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 9: // неверны параметры R4
	//	tmps.LoadString(IDS_MES_RAD);
	//	str += tmps;
	//	m_r4_ctrl.SetFocus();
	//	m_r4_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 10: // активная часть инструмента		
	//	if (m_faset1 != 0.)
	//		a = m_faset1*tan(PI/2. - m_alf1*PI/180.) + m_r1*tan(m_alf1*PI/180.);
	//	else
	//		a = m_r1;
	//	
	//	tmp1 = _fcvt(a, 3, &decimal, &sign);
	//	tmp2 = _fcvt(m_diam/2., 3, &decimal, &sign);
	//	str.Format(IDS_MES_INT,
	//		tmp1.Mid(0,decimal),tmp1.Mid(decimal),
	//		tmp2.Mid(0,decimal),tmp2.Mid(decimal));
	//	
	//	m_h1_ctrl.SetFocus();
	//	m_h1_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 11: // активная часть инструмента
	//	if (m_faset2 != 0.)
	//		a = m_faset2*tan(PI/2. - m_alf2*PI/180.) + m_r2*tan(m_alf2*PI/180.);
	//	else
	//		a = m_r2;
	//
	//	tmp1 = _fcvt(a, 3, &decimal, &sign);
	//	tmp2 = _fcvt(m_diam/2., 3, &decimal, &sign);
	//	str.Format(IDS_MES_INT,
	//		tmp1.Mid(0,decimal),tmp1.Mid(decimal),
	//		tmp2.Mid(0,decimal),tmp2.Mid(decimal));
	//	
	//	m_h2_ctrl.SetFocus();
	//	m_h2_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//}
	AfxMessageBox(str);

}
NToolWeld *NToolsCrWeldPage::CreateTool()
{
	if(!UpdateData(TRUE))
		return NULL;

	NToolWeld *pTool = new NToolWeld;

	pTool->FillFromPage(this);

	if(pTool->GenInit() == 0)
	{
		return pTool;
	}
	else
	{
		delete pTool;
		AfxMessageBox(IDS_MES_PAR);
		return NULL;
	}
}

void NToolsCrWeldPage::OnEnKillfocusWeldFocus()
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NToolsCrWeldPage::OnEnKillfocusWeldDiam()
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

