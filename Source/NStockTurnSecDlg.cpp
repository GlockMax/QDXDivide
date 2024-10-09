// NStockTurnSecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "GQuadRenderRot.h"
#include "NStockTurnSecDlg.h"

// NStockTurnSecDlg dialog

IMPLEMENT_DYNAMIC(NStockTurnSecDlg, CDialog)
NStockTurnSecDlg::NStockTurnSecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NStockTurnSecDlg::IDD, pParent)
	, m_StartAngle(0)
	, m_AngleSize(0)
{
}

NStockTurnSecDlg::~NStockTurnSecDlg()
{
}

void NStockTurnSecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SECVIEW_START, m_StartAngle);
	DDX_Text(pDX, IDC_SECVIEW_DELTA, m_AngleSize);
}


BEGIN_MESSAGE_MAP(NStockTurnSecDlg, CDialog)
	ON_BN_CLICKED(IDC_SECVIEW_APPLY, OnBnClickedSecviewApply)
END_MESSAGE_MAP()


// NStockTurnSecDlg message handlers

void NStockTurnSecDlg::OnBnClickedSecviewApply()
{
	UpdateData();
	if(GQuadRenderRot::StoreCounter == 0)
	{
		GQuadRenderRot::StartAngle = m_StartAngle;
		GQuadRenderRot::Angle = m_AngleSize;
		GQuadRenderRot::AngleStepsNum = int(m_AngleSize / GQuadRenderRot::AngleStep);
	}
	CMainFrame *pMainFrame = (CMainFrame *)GetParent();
	CNCMDoc *pDoc = (CNCMDoc *)(pMainFrame->GetActiveDocument());
	pDoc->CreateStocksSectGeom();
	pDoc->CreatePartsSectGeom();
	pMainFrame->AnimateUpdate();
}

BOOL NStockTurnSecDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_StartAngle = GQuadRenderRot::StartAngle;
	m_AngleSize = GQuadRenderRot::Angle;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NStockTurnSecDlg::OnOK()
{
	OnBnClickedSecviewApply();
	CDialog::OnOK();
}
