// NMendrelDial.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolCombinedMill.h"
#include "NMendrelDial.h"
#include "NMapStringToOb.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NMendrelDial dialog

NMendrelDial::NMendrelDial(CWnd* pParent /*=NULL*/)
	: CDialog(NMendrelDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(NMendrelDial)
	m_dprevious = (MM_INCH == M_MM) ? 40.0 : 1.5;
	m_dbot = (MM_INCH == M_MM) ? 40.0 : 1.5;
	m_h = (MM_INCH == M_MM) ? 80.0 : 3.5;
	m_hm = (MM_INCH == M_MM) ? 20.0 : 1.0;
	m_dtop = (MM_INCH == M_MM) ? 60.0 : 2.5;
	m_displacement = 0.0;
	m_name = (MM_INCH == M_MM) ? _T("HoldD12H6d40h20") : _T("HoldD2.5H3.5d1.5h1");
	//}}AFX_DATA_INIT
	IsNameAuto = true;
	pTool = NULL;
}

NMendrelDial::~NMendrelDial()
{
}

void NMendrelDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NMendrelDial)
	DDX_Control(pDX, IDC_MENDREL_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_MENDREL_H, m_h_ctrl);
	DDX_Control(pDX, IDC_MENDREL_D, m_diam_ctrl);
	DDX_Control(pDX, IDC_MENDREL_D2, m_d2_ctrl);
	DDX_Control(pDX, IDC_MENDREL_HM, m_hm_ctrl);
	DDX_Text(pDX, IDC_MENDREL_D, m_dbot);
	DDV_MinMaxDouble(pDX, m_dbot, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_MENDREL_H, m_h);
	DDV_MinMaxDouble(pDX, m_h, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_MENDREL_HM, m_hm);
	DDV_MinMaxDouble(pDX, m_hm, 0., 500.);
	DDX_Text(pDX, IDC_MENDREL_D2, m_dtop);
	DDV_MinMaxDouble(pDX, m_dtop, MINTOOLDIM, 500.);
	DDX_Text(pDX, IDC_MENDREL_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Control(pDX, IDC_MENDREL_DISPLACEMENT, m_displacement_ctrl);
	DDX_Text(pDX, IDC_MENDREL_NAME, m_name);
	DDX_Check(pDX, IDC_SHANK, m_check_shank);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NMendrelDial, CDialog)
	//{{AFX_MSG_MAP(NMendrelDial)
	ON_EN_KILLFOCUS(IDC_MENDREL_NAME, OnKillfocusMendrelName)
	ON_EN_CHANGE(IDC_MENDREL_D, OnChangeMendrelDTop)
	ON_EN_CHANGE(IDC_MENDREL_H, OnChangeMendrelH)
	ON_EN_CHANGE(IDC_MENDREL_D2, OnChangeMendrelDBot)
	ON_EN_CHANGE(IDC_MENDREL_HM, OnChangeMendrelHm)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_MENDRELHELP, OnMendrelHelp)
	ON_BN_CLICKED(IDC_SHANK, OnShankCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NMendrelDial message handlers

// генерирование имени инструмента
CString NMendrelDial::GenName()
{
	CString d, h, r, a, buf;
	double value;
	
	m_d2_ctrl.GetWindowText(buf);
	if (buf == "")
		buf = "..";
	d.Format("D%s", buf);

	m_h_ctrl.GetWindowText(buf);
	if (buf == "")
		buf = "..";
	h.Format("H%s", buf);

	m_diam_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		r.Format("d%s", buf);

	m_hm_ctrl.GetWindowText(buf);
	value = atof(buf);
	if(-DBL_MIN>value||value>DBL_MIN)
		a.Format("h%s", buf);

	buf = (m_check_shank ? _T("Shank") : _T("Hold")) + d + h;
	if (d != r)
		buf = buf + r;
	if (h != a)
		buf = buf + a;
	
	return buf;
}

// определение созданного инструмента
NToolStd* NMendrelDial::GetNewTool()
{
	return pTool;
}

// определение имени инструмента
CString& NMendrelDial::GetName()
{
	return m_name;
}

void NMendrelDial::OnCancel() 
{
	if(pTool) delete pTool;	

	CDialog::OnCancel();
}

void NMendrelDial::OnOK() 
{
	if(!UpdateData())
		return;
// проверки на введённие всех небходимых данных
	if(m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return;
	}
	
	if(pTool) delete pTool;
	double alf, faset;

	if (m_hm == 0.) // инструмент без фаски
	{
		alf = atan((m_dtop-m_dbot)/(2.*m_h));
		alf = alf *180./PI;
		pTool = new NToolStd(m_name,m_dbot,m_h,alf,0.,0.,0.,0.,m_h);
		if (m_dbot == m_dtop)
			pTool->SetType(41);
		else
			pTool->SetType(42);
	}
	else // инструмент с фаской
	{
		alf = atan((m_dtop-m_dbot)/(2.*m_hm));
		alf = alf *180./PI;
		alf = 90 - alf;
		faset = (m_dtop-m_dbot)/2.;
		pTool = new NToolStd(m_name,m_dtop,m_h,0.,faset,alf,0.,0.,m_h);
		pTool->SetType(43);
	}
	pTool->IsNameAuto = IsNameAuto;
	pTool->SetColor(pTool->GetColorPassive());

	int IsErr = pTool->GenInit();
	pTool->SetZdisp(m_displacement);

	if(IsErr != 0)
	{
		m_d2_ctrl.SetFocus();
		m_d2_ctrl.SetSel(0, -1, FALSE);
		AfxMessageBox(IDS_MES_PAR);
		delete pTool;
		pTool = NULL;
		return;
	}
	pTool->SetShank(m_check_shank == TRUE);
	CDialog::OnOK();
}

void NMendrelDial::OnKillfocusMendrelName() 
{
 	CString NewName;
 	m_name_ctrl.GetWindowText(NewName);
 	IsNameAuto = (bool) (NewName == GenName());	
}

void NMendrelDial::OnChangeMendrelDTop() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NMendrelDial::OnChangeMendrelH() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NMendrelDial::OnChangeMendrelDBot() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

void NMendrelDial::OnChangeMendrelHm() 
{
	if(IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

/*
BOOL NMendrelDial::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
		::WinHelp((HWND) pHelpInfo->hItemHandle,
		AfxGetApp()->m_pszHelpFilePath, HELP_WM_HELP, (DWORD) aHelpIDs);
	
	return TRUE;
}
*/
/*
void NMendrelDial::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	::WinHelp((HWND) *pWnd,	AfxGetApp()->m_pszHelpFilePath, 
	HELP_CONTEXTMENU, (DWORD) aHelpIDs);		
}
*/
void NMendrelDial::OnMendrelHelp() 
{
	AfxGetApp()->WinHelp(0x20000 + IDD_MENDREL);
}

BOOL NMendrelDial::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_name = GenName();
	UpdateData(FALSE);
	return 0;
}

void NMendrelDial::OnShankCheck()
{
	UpdateData();
	if (m_check_shank) 
	{
		m_dbot = m_dprevious;
		m_dtop = m_dbot;
		m_hm = 0.;
	}
	else
	{
		m_dbot = (MM_INCH == M_MM) ? 40.0 : 1.5;
		m_hm = fmin((MM_INCH == M_MM) ? 20.0 : 1.0, m_h);
		m_dtop = (MM_INCH == M_MM) ? 60.0 : 2.5;
	}
	UpdateData(FALSE);
	if (IsNameAuto)
	{
		m_name_ctrl.SetSel(0, -1);
		m_name_ctrl.ReplaceSel(GenName());
	}
}

