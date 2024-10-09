// NWeldProgPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCUnit.h"
#include "NCMDoc.h"
#include "WM_APP_messages.h"
#include "NWeldProgPropDlg.h"


// NWeldProgPropDlg dialog

IMPLEMENT_DYNAMIC(NWeldProgPropDlg, CDialog)

NWeldProgPropDlg::NWeldProgPropDlg(NProgramWeld &iProgram, CNCMDoc *ipDoc, CWnd* pParent /*=NULL*/)
	: CDialog(NWeldProgPropDlg::IDD, pParent)
	, m_comment(_T(""))
	, Program(iProgram)
	, m_th1(0)
	, pDoc(ipDoc)
	, m_Vis1(FALSE)
	, m_Vis2(FALSE)
	, m_Vis3(FALSE)
	, m_Vis4(FALSE)
	, m_Vis5(FALSE)
{

}

NWeldProgPropDlg::~NWeldProgPropDlg()
{
}

void NWeldProgPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WP_1, m_thick1);
	DDX_Control(pDX, IDC_WP_2, m_thick2);
	DDX_Control(pDX, IDC_WP_3, m_thick3);
	DDX_Control(pDX, IDC_WP_4, m_thick4);
	DDX_Control(pDX, IDC_WP_5, m_thick5);
	DDX_Text(pDX, IDC_PCOMMENT, m_comment);
	DDV_MaxChars(pDX, m_comment, 64);
	DDX_Control(pDX, IDC_WPCOLOR1, m_color1);
	DDX_Control(pDX, IDC_WPCOLOR2, m_color2);
	DDX_Control(pDX, IDC_WPCOLOR3, m_color3);
	DDX_Control(pDX, IDC_WPCOLOR4, m_color4);
	DDX_Control(pDX, IDC_WPCOLOR5, m_color5);
	DDX_CBIndex(pDX, IDC_WP_1, m_th1);
	DDX_CBIndex(pDX, IDC_WP_2, m_th2);
	DDX_CBIndex(pDX, IDC_WP_3, m_th3);
	DDX_CBIndex(pDX, IDC_WP_4, m_th4);
	DDX_CBIndex(pDX, IDC_WP_5, m_th5);
	DDX_Check(pDX, IDC_CH_PROGR_VISIBLE, m_Vis1);
	DDX_Check(pDX, IDC_CH_PROGR_VISIBLE_WELDING, m_Vis2);
	DDX_Check(pDX, IDC_CH_PROGR_VISIBLE_WELDEDJOINT, m_Vis3);
	DDX_Check(pDX, IDC_CH_PROGR_VISIBLE_WELDEDROOT, m_Vis4);
	DDX_Check(pDX, IDC_CH_PROG_BEAMVECTOR, m_Vis5);
}


BEGIN_MESSAGE_MAP(NWeldProgPropDlg, CDialog)
	ON_BN_CLICKED(ID_WS_APPLY, &NWeldProgPropDlg::OnBnClickedWsApply)
	ON_BN_CLICKED(IDCANCEL, &NWeldProgPropDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &NWeldProgPropDlg::OnBnClickedOk)
END_MESSAGE_MAP()



void NWeldProgPropDlg::OnBnClickedWsApply()
{
	FillProgram();
	AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL | N_CADRCHANGED);
}

void NWeldProgPropDlg::OnBnClickedCancel()
{
	OnCancel();
}

void NWeldProgPropDlg::OnBnClickedOk()
{
	FillProgram();
	OnOK();
}
void NWeldProgPropDlg::FillProgram()
{
	UpdateData(TRUE);
	Program.SetComment(m_comment);
	bool ProgChanged = false;
	if(Program.GetThickness() != m_th1 + 1)
	{
		ProgChanged = true;
		Program.SetThickness(double(m_th1 + 1));
	}
	if(m_color1.GetColor() != Program.GetColor().GetRGB())
	{
		ProgChanged = true;
		NColor Col;
		Col.SetRGB(m_color1.GetColor());
		Program.SetColor(Col);
	}
	NColor F0, S0, R0, B0;
	Program.Trajectories.GetColors(F0, S0, R0, B0);
	int t20, t30, t40, t50;
	Program.Trajectories.GetThick(t20, t30, t40, t50);
	NColor F, S, R, B;
	F.SetRGB(m_color2.GetColor());
	S.SetRGB(m_color3.GetColor());
	R.SetRGB(m_color4.GetColor());
	B.SetRGB(m_color5.GetColor());
	bool TrajChanged = false;
	if( 
		   F0.GetRGB() != F.GetRGB() 
		|| S0.GetRGB() != S.GetRGB()
		|| R0.GetRGB() != R.GetRGB()
		|| B0.GetRGB() != B.GetRGB()
		|| t20 != m_th2
		|| t30 != m_th3
		|| t40 != m_th4
		|| t50 != m_th5
		)
	{
		TrajChanged = true;
		Program.Trajectories.SetColors(F, S, R, B);
		Program.Trajectories.SetThick(double(m_th2 + 1), double(m_th3 + 1), double(m_th4 + 1), double(m_th5 + 1));
	}
	if(Program.IsVisible() != m_Vis1)
		Program.InvertVisible();
	Program.Trajectories.SetVis(m_Vis2 == TRUE, m_Vis3 == TRUE, m_Vis4 == TRUE, m_Vis5 == TRUE); 
	if(ProgChanged)
		Program.PrepRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));
	//if(TrajChanged)
	//	pDoc->UpdateProgWeldTraject();
}
BOOL NWeldProgPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_th1 = int(Program.GetThickness() - 1.);
	m_color1.SetColor(Program.GetColor());
	m_color1.Update();
	NColor F, S, R, B;
	Program.Trajectories.GetColors(F, S, R, B);
	m_color2.SetColor(F);
	m_color2.Update();
	m_color3.SetColor(S);
	m_color3.Update();
	m_color4.SetColor(R);
	m_color4.Update();
	m_color5.SetColor(B);
	m_color5.Update();
	Program.Trajectories.GetThick(m_th2, m_th3, m_th4, m_th5);
	m_thick1.Init();
	m_thick2.Init();
	m_thick3.Init();
	m_thick4.Init();
	m_thick5.Init();
	m_Vis1 = Program.IsVisible();
	bool Vis2, Vis3, Vis4, Vis5;
	Program.Trajectories.GetVis(Vis2, Vis3, Vis4, Vis5); 
	m_Vis2 = Vis2 ? TRUE : FALSE;
	m_Vis3 = Vis3 ? TRUE : FALSE;
	m_Vis4 = Vis4 ? TRUE : FALSE;
	m_Vis5 = Vis5 ? TRUE : FALSE;
	m_comment = Program.GetComment();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

