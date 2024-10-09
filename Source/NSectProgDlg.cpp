// NSectProgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCMDoc.h"
#include "NProgramWeld.h"
#include "NSectProgDlg.h"


// NSectProgDlg dialog

IMPLEMENT_DYNAMIC(NSectProgDlg, CDialog)

NSectProgDlg::NSectProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NSectProgDlg::IDD, pParent)
	, pDoc(NULL)
	, pProg(NULL)
	, m_vdsect(FALSE)
	, m_step(0)
	, m_vstep(FALSE)
	, m_thick(0)
	, m_dthick(0)
	, m_endline(0)
	, m_startline(0)
	, m_list(_T(""))
{

}
NSectProgDlg::NSectProgDlg(CNCMDoc *pD, NProgramWeld *pP)
	: CDialog(NSectProgDlg::IDD, NULL)
	, pDoc(pD)
	, pProg(pP)
	, m_vdsect(FALSE)
	, m_step(0)
	, m_vstep(FALSE)
	, m_thick(0)
	, m_dthick(0)
	, m_endline(0)
	, m_startline(0)
	, m_list(_T(""))
{

}

NSectProgDlg::~NSectProgDlg()
{
}

void NSectProgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_WS_VDSECT, m_vdsect);
	DDX_Text(pDX, IDC_WS_STEP, m_step);
	DDV_MinMaxInt(pDX, m_step, 1, 100000);
	DDX_Check(pDX, IDC_WS_VSTEP, m_vstep);
	DDX_Control(pDX, IDC_WS_THICK, m_widthctrl);
	DDX_CBIndex(pDX, IDC_WS_THICK, m_thick);
	DDX_Control(pDX, IDC_WS_DTHICK, m_dwidthctrl);
	DDX_CBIndex(pDX, IDC_WS_DTHICK, m_dthick);
	DDX_Text(pDX, IDC_WS_ENDLINE, m_endline);
	DDX_Text(pDX, IDC_WS_STARTLINE, m_startline);
	DDX_Text(pDX, IDC_WS_LIST, m_list);
}


BEGIN_MESSAGE_MAP(NSectProgDlg, CDialog)
	ON_BN_CLICKED(ID_WS_APPLY, &NSectProgDlg::OnBnClickedWsApply)
END_MESSAGE_MAP()


// NSectProgDlg message handlers
BOOL NSectProgDlg::OnInitDialog()
{
	UpdateData(FALSE);
	m_widthctrl.Init();
	m_dwidthctrl.Init();

	m_vdsect = pDoc->GetToolSection().GetVisible();
	m_dthick = int(pDoc->GetToolSection().GetThickness() - 1.);

	m_step = pProg->IntSections;
	m_vstep = pProg->VisSections;
	m_thick = int(pProg->ProgSections.GetThickness() - 1.);
	int Start, End; 
	if(pProg->SectNums.GetFirst(&Start, &End))
	{
		m_startline = Start;
		m_endline = End;
	}
	else
	{
		m_startline = 1;
		m_endline = 0;
	}
	m_list = pProg->SectNums.GetString();

	return CDialog::OnInitDialog();
}

void NSectProgDlg::OnOK()
{
	ApplyChanges();
	CDialog::OnOK();
}

void NSectProgDlg::OnBnClickedWsApply()
{
	ApplyChanges();
}

void NSectProgDlg::ApplyChanges()
{
	UpdateData();
	pDoc->GetToolSection().SetVisible(m_vdsect == TRUE);
	pDoc->GetToolSection().SetThickness(GLfloat(m_dthick + 1));

	pProg->SectNums = m_list;
	pProg->SectNums.AddPair(m_startline, m_endline);
	pProg->IntSections = m_step;
	pProg->VisSections = (m_vstep == TRUE);
	pProg->ProgSections.SetThickness(GLfloat(m_thick + 1));

	pDoc->UpdateProgWeldSections();

	pDoc->UpdateAllViews(NULL, N_OGL);
}


void NSectProgDlg::PostNcDestroy()
{
	pProg->SetDialog(NULL);
	CDialog::PostNcDestroy();
	delete this;
}
