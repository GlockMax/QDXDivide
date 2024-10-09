#include "..\Common\NBreakpointDlg.h"
#include "..\Common\NBreakpointDlg.h"
#include "..\Common\NBreakpointDlg.h"
// NBrakpointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nbreakpointdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// NBrakpointDlg dialog

IMPLEMENT_DYNAMIC(NBreakpointDlg, CDialog)
NBreakpointDlg::NBreakpointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NBreakpointDlg::IDD, pParent)
{
	Reset();
	v_BREAKPOINT_ON_OFF = TRUE;
}

NBreakpointDlg::~NBreakpointDlg()
{
}

void NBreakpointDlg::Reset()
{
	v_MiToolChange = FALSE;
	v_MiProgStartEnd = FALSE;
	v_MiZOnly = FALSE;
	v_MiXOnly = FALSE;
	v_MiYOnly = FALSE;
	v_MiBookmark = FALSE;
	v_MiCollision = FALSE;
	v_MiProgOptStop = FALSE;
	v_MiSpindleSpeed = FALSE;
	v_MiSpindleForwardReverse = FALSE;
	v_MiFeed = FALSE;
	v_MiCoolant = FALSE;
	v_MiPositioning = FALSE;
	v_MiCircularInterpolation = FALSE;
	v_MiLinearInterpolation = FALSE;
	v_MiCycle = FALSE;
	v_MiSubprogramCall = FALSE;
	v_MiSubprogramStartEnd = FALSE;
	v_MiToolLengthCompensationActive = FALSE;
	v_MiToolLengthCompensationCancel = FALSE;
	v_MiToolDiameterCompensationActive = FALSE;
	v_MiToolDiameterCompensationCancel = FALSE;
	v_MiLifeTime = FALSE;
}

void NBreakpointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BP_PAR, m_St_par);
	DDX_Control(pDX, IDC_BP_G, m_St_G);
	DDX_Control(pDX, IDC_BP_MOTION, m_St_MOTION);
	DDX_Control(pDX, IDC_BP_SUBPROG, m_St_SUBPROG);
	DDX_Control(pDX, IDC_BP_LENGTHCOMP, m_St_LENGTHCOMP);
	DDX_Control(pDX, IDC_BP_DIAMETERCOMP, m_St_DIAMETERCOMP);

	DDX_Control(pDX, IDC_BREAKPOINTADD_ON_OFF, m_BREAKPOINT_ON_OFF);

	DDX_Check(pDX, IDC_MiToolChange, v_MiToolChange);
	DDX_Check(pDX, IDC_MiProgStartEnd, v_MiProgStartEnd);
	DDX_Check(pDX, IDC_MiZOnly, v_MiZOnly);
	DDX_Check(pDX, IDC_MiXOnly, v_MiXOnly);
	DDX_Check(pDX, IDC_MiYOnly, v_MiYOnly);
//	DDX_Check(pDX, IDC_MiBookmark, v_MiBookmark);
	DDX_Check(pDX, IDC_MiBookmark, v_MiCollision);
	DDX_Check(pDX, IDC_MiProgOptStop, v_MiProgOptStop);
	DDX_Check(pDX, IDC_MiSpindleSpeed, v_MiSpindleSpeed);
	DDX_Check(pDX, IDC_MiSpindleForwardReverse, v_MiSpindleForwardReverse);
	DDX_Check(pDX, IDC_MiFeed, v_MiFeed);
	DDX_Check(pDX, IDC_MiCoolant, v_MiCoolant);
	DDX_Check(pDX, IDC_MiPositioning, v_MiPositioning);
	DDX_Check(pDX, IDC_MiCircularInterpolation, v_MiCircularInterpolation);
	DDX_Check(pDX, IDC_MiLinearInterpolation, v_MiLinearInterpolation);
	DDX_Check(pDX, IDC_MiCycle, v_MiCycle);
	DDX_Check(pDX, IDC_MiSubprogramCall, v_MiSubprogramCall);
	DDX_Check(pDX, IDC_MiSubprogramStartEnd, v_MiSubprogramStartEnd);
	DDX_Check(pDX, IDC_MiToolLengthCompensationActive, v_MiToolLengthCompensationActive);
	DDX_Check(pDX, IDC_MiToolLengthCompensationCancel, v_MiToolLengthCompensationCancel);
	DDX_Check(pDX, IDC_MiToolDiameterCompensationActive, v_MiToolDiameterCompensationActive);
	DDX_Check(pDX, IDC_MiToolDiameterCompensationCancel, v_MiToolDiameterCompensationCancel);
	DDX_Check(pDX, IDC_MiLifeTime, v_MiLifeTime);
	DDX_Check(pDX, IDC_BREAKPOINTADD_ON_OFF, v_BREAKPOINT_ON_OFF);
}


BEGIN_MESSAGE_MAP(NBreakpointDlg, CDialog)
	ON_BN_CLICKED(IDC_BREAKPOINTADD_ON_OFF, OnBnClickedBreakpointaddOnOff)
	ON_STN_CLICKED(ID_MiProgStartEnd, OnStnClickedMiprogstartend)
	ON_STN_CLICKED(ID_MiToolChange, OnStnClickedMitoolchange)
	ON_STN_CLICKED(ID_MiProgOptStop, OnStnClickedMiprogoptstop)
	ON_STN_CLICKED(ID_MiBookmark, OnStnClickedMibookmark)
	ON_STN_CLICKED(ID_MiCollision, OnStnClickedMicollision)
	ON_STN_CLICKED(ID_MiLifeTime, OnStnClickedMiLifeTime)
	ON_STN_CLICKED(ID_BREAKPOINTADD_ON_OFF, OnStnClickedBreakpointaddOnOff)
END_MESSAGE_MAP()


// NBrakpointDlg message handlers

BOOL NBreakpointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Update();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NBreakpointDlg::OnBnClickedBreakpointaddOnOff()
{
	Update();
}

void NBreakpointDlg::Update(void)
{

	GetDlgItem(IDC_ADDIT)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());

	m_St_par.EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	m_St_G.EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	m_St_MOTION.EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	m_St_SUBPROG.EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	m_St_LENGTHCOMP.EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	m_St_DIAMETERCOMP.EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());

	GetDlgItem(IDC_MiZOnly)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiXOnly)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiYOnly)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiSpindleSpeed)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiSpindleForwardReverse)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiFeed)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiCoolant)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiPositioning)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiCircularInterpolation)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiLinearInterpolation)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiCycle)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiSubprogramCall)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiSubprogramStartEnd)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiToolLengthCompensationActive)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiToolLengthCompensationCancel)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiToolDiameterCompensationActive)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiToolDiameterCompensationCancel)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
	GetDlgItem(IDC_MiLifeTime)->EnableWindow(m_BREAKPOINT_ON_OFF.GetCheck());
}


void NBreakpointDlg::OnStnClickedMiprogstartend()
{
	((CButton *)GetDlgItem(IDC_MiProgStartEnd))->SetCheck(
		!((CButton *)GetDlgItem(IDC_MiProgStartEnd))->GetCheck());
}

void NBreakpointDlg::OnStnClickedMitoolchange()
{
	((CButton *)GetDlgItem(IDC_MiToolChange))->SetCheck(
		!((CButton *)GetDlgItem(IDC_MiToolChange))->GetCheck());
}

void NBreakpointDlg::OnStnClickedMiprogoptstop()
{
	((CButton *)GetDlgItem(IDC_MiProgOptStop))->SetCheck(
		!((CButton *)GetDlgItem(IDC_MiProgOptStop))->GetCheck());
}

void NBreakpointDlg::OnStnClickedMibookmark()
{
	((CButton *)GetDlgItem(IDC_MiBookmark))->SetCheck(
		!((CButton *)GetDlgItem(IDC_MiBookmark))->GetCheck());
}

void NBreakpointDlg::OnStnClickedMicollision()
{
	((CButton*)GetDlgItem(IDC_MiCollision))->SetCheck(
		!((CButton*)GetDlgItem(IDC_MiCollision))->GetCheck());
}

void NBreakpointDlg::OnStnClickedMiLifeTime()
{
	((CButton *)GetDlgItem(IDC_MiLifeTime))->SetCheck(
		!((CButton *)GetDlgItem(IDC_MiLifeTime))->GetCheck());
}

void NBreakpointDlg::OnStnClickedBreakpointaddOnOff()
{
	((CButton *)GetDlgItem(IDC_BREAKPOINTADD_ON_OFF))->SetCheck(
		!((CButton *)GetDlgItem(IDC_BREAKPOINTADD_ON_OFF))->GetCheck());
	Update();
}

void NBreakpointDlg::FillNames(CArray<enum StoneNames>& Names) const
{
	Names.RemoveAll();

	if (v_MiProgStartEnd) Names.Add(MiProgStartEnd);
	if (v_MiToolChange) Names.Add(MiToolChange);
	if (v_MiBookmark) Names.Add(MiBookmark);
	if (v_MiProgOptStop) Names.Add(MiProgOptStop);
	if (v_MiLifeTime) Names.Add(MiLifeTime);

	if (v_MiZOnly) Names.Add(MiZOnly);
	if (v_MiXOnly) Names.Add(MiXOnly);
	if (v_MiYOnly) Names.Add(MiYOnly);
	if (v_MiSpindleSpeed) Names.Add(MiSpindleSpeed);
	if (v_MiSpindleForwardReverse) Names.Add(MiSpindleForwardReverse);
	if (v_MiFeed) Names.Add(MiFeed);
	if (v_MiCoolant) Names.Add(MiCoolant);
	if (v_MiPositioning) Names.Add(MiPositioning);
	if (v_MiCircularInterpolation) Names.Add(MiCircularInterpolation);
	if (v_MiLinearInterpolation) Names.Add(MiLinearInterpolation);
	if (v_MiCycle) Names.Add(MiCycle);
	if (v_MiSubprogramCall) Names.Add(MiSubprogramCall);
	if (v_MiSubprogramStartEnd) Names.Add(MiSubprogramStartEnd);
	if (v_MiToolLengthCompensationActive) Names.Add(MiToolLengthCompensationActive);
	if (v_MiToolLengthCompensationCancel) Names.Add(MiToolLengthCompensationCancel);
	if (v_MiToolDiameterCompensationActive) Names.Add(MiToolDiameterCompensationActive);
	if (v_MiToolDiameterCompensationCancel) Names.Add(MiToolDiameterCompensationCancel);
}

void NBreakpointDlg::SetFromNames(const CArray<enum StoneNames>& Names)
{
	Reset();
	for (int i = 0; i < Names.GetSize(); ++i)
	{
		switch (Names.GetAt(i))
		{
			case MiProgStartEnd: v_MiProgStartEnd = TRUE; break;
			case MiToolChange: v_MiToolChange = TRUE; break;
			case MiBookmark: v_MiBookmark = TRUE; break;
			case MiCollision: v_MiCollision = TRUE; break;
			case MiProgOptStop: v_MiProgOptStop = TRUE; break;
			case MiLifeTime: v_MiLifeTime = TRUE; break;

			case MiZOnly: v_MiZOnly = TRUE; break;
			case MiXOnly: v_MiXOnly = TRUE; break;
			case MiYOnly: v_MiYOnly = TRUE; break;
			case MiSpindleSpeed: v_MiSpindleSpeed = TRUE; break;
			case MiSpindleForwardReverse: v_MiSpindleForwardReverse = TRUE; break;
			case MiFeed: v_MiFeed = TRUE; break;
			case MiCoolant: v_MiCoolant = TRUE; break;
			case MiPositioning: v_MiPositioning = TRUE; break;
			case MiCircularInterpolation: v_MiCircularInterpolation = TRUE; break;
			case MiLinearInterpolation: v_MiLinearInterpolation = TRUE; break;
			case MiCycle: v_MiCycle = TRUE; break;
			case MiSubprogramCall: v_MiSubprogramCall = TRUE; break;
			case MiSubprogramStartEnd: v_MiSubprogramStartEnd = TRUE; break;
			case MiToolLengthCompensationActive: v_MiToolLengthCompensationActive = TRUE; break;
			case MiToolLengthCompensationCancel: v_MiToolLengthCompensationCancel = TRUE; break;
			case MiToolDiameterCompensationActive: v_MiToolDiameterCompensationActive = TRUE; break;
			case MiToolDiameterCompensationCancel: v_MiToolDiameterCompensationCancel = TRUE; break;
		}
	}
}

void NBreakpointDlg::Serialize(CArchive& ar)
{
	SerializeElements(ar, &v_BREAKPOINT_ON_OFF, 1);
	CArray<enum StoneNames> Names;
	if (ar.IsStoring())
		FillNames(Names);
	Names.Serialize(ar);
	if (ar.IsLoading())
		SetFromNames(Names);
}
