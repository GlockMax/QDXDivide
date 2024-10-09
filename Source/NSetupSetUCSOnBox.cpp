// ..\source\NSetupSetUCSOnBox.cpp : implementation file
//

#include "stdafx.h"
#include "NCMDoc.h"
#include "NSetupSetUCSOnBox.h"


// NSetupSetUCSOnBox dialog

IMPLEMENT_DYNAMIC(NSetupSetUCSOnBox, NSetupSetBoxPointDlg)

NSetupSetUCSOnBox::NSetupSetUCSOnBox(CWnd* pParent, class CNCMDoc *pDoc)
	: NSetupSetBoxPointDlg(NSetupSetUCSOnBox::IDD, pParent, pDoc)
	, m_Stock_Vx(0)
	, m_Stock_Vy(0)
	, m_Stock_Vz(1)
	, m_Stock_VAngl(0)
{

}

NSetupSetUCSOnBox::~NSetupSetUCSOnBox()
{
}

void NSetupSetUCSOnBox::DoDataExchange(CDataExchange* pDX)
{
	NSetupSetBoxPointDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STOCK_VX, m_Stock_Vx);
	DDX_Text(pDX, IDC_STOCK_VY, m_Stock_Vy);
	DDX_Text(pDX, IDC_STOCK_VZ, m_Stock_Vz);
	DDX_Text(pDX, IDC_STOCK_VANGL, m_Stock_VAngl);
	DDX_Check(pDX, IDC_SETUCSOTHER, m_SecondSolution);
	DDX_Control(pDX, IDC_SETUCSOTHER, m_SecondSolutionCtrl);
}


BEGIN_MESSAGE_MAP(NSetupSetUCSOnBox, NSetupSetBoxPointDlg)
ON_COMMAND(IDC_TOOL_APPLY, &NSetupSetUCSOnBox::OnToolApply)
ON_COMMAND(IDC_SETUCSOTHER, &NSetupSetUCSOnBox::OnToolApply)
END_MESSAGE_MAP()


// NSetupSetUCSOnBox message handlers
void NSetupSetUCSOnBox::OnToolApply()
{
	if(!pDoc)
		return;
	pDoc->JumpToProjStart();
	UpdateData();
//	but = GetCheckedRadioButton(IDC_R0,IDC_R9) - IDC_R0;
	UpdateStockGabar();
	CalcStockToolGabar();
	int x, y, z;
	GetBaseShift(x, y, z);
	BPoint P(m_x + x * StockGabar.dX() * 0.5, m_y + y * StockGabar.dY() * 0.5, m_z + z * StockGabar.dZ() * 0.5, 0.);
	P += StockGabar.GetMinPoint();
	BPoint N(m_Stock_Vx, m_Stock_Vy, m_Stock_Vz, 0.);
	N = N * (1. / sqrt(N.D2()));
	P = P * ToolMatr;
	N = N * ToolMatr;
	BPoint X(0., 0., 1., 0.);
	if((fabs(N.GetZ()) > fabs(N.GetY())) && (fabs(N.GetZ()) > fabs(N.GetX())))
		X = BPoint(0., 1., 0., 0.) % N;
	else if(fabs(N.GetY()) > fabs(N.GetX()))
		X =  BPoint(1., 0., 0., 0.) % N;
	else
		X =  BPoint(0., 0., 1., 0.) % N;
	X = X * (1. / sqrt(X.D2()));
	X = X * BMatr().rotg(m_Stock_VAngl, P, P + N);
	BMatr CS; // Relative to stock
	CS.SetRow(0, X);
	CS.SetRow(1, N % X);
	CS.SetRow(2, N);
	CS.SetRow(3, P);
//	CS = CS * SMotion.GetMatr(1.).invr();
	int Res = pDoc->SetUCSOnBoxApply(CS, m_SecondSolution ? 1 : 0);
	m_SecondSolutionCtrl.EnableWindow(Res > 1);
	if(Res < 2)
		m_SecondSolution = FALSE;
	UpdateData(FALSE);
	if(Res <= 0)
		AfxMessageBox(IDS_MES_ERR_UCSSETONSTOCK);
}
