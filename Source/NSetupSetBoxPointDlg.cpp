// ..\source\NSetupSetBoxPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCMDoc.h"
#include "NSetupSetBoxPointDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// NSetupSetBoxPointDlg dialog

IMPLEMENT_DYNAMIC(NSetupSetBoxPointDlg, CDialogEx)

NSetupSetBoxPointDlg::NSetupSetBoxPointDlg(UINT nIDTemplate, CWnd* pParentWnd, CNCMDoc *ipDoc) : CDialogEx(nIDTemplate, pParentWnd)
{
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_heig = 0.0;
	m_len = 0.0;
	m_wid = 0.0;
	but = 0;
	Auto = FALSE;
	FillG54 = TRUE;
	pDoc = ipDoc;
	m_Pos = 0;
}

NSetupSetBoxPointDlg::NSetupSetBoxPointDlg(CWnd* pParent /*=NULL*/, CNCMDoc *ipDoc)	: CDialogEx(NSetupSetBoxPointDlg::IDD, pParent)
{
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_heig = 0.0;
	m_len = 0.0;
	m_wid = 0.0;
	but = 0;
	Auto = FALSE;
	FillG54 = TRUE;
	pDoc = ipDoc;
	m_Pos = 0;
}

NSetupSetBoxPointDlg::~NSetupSetBoxPointDlg()
{
}

void NSetupSetBoxPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOX_SMALL, m_Pict_s);
	DDX_Control(pDX, IDC_TITLE3, m_t3);
	DDX_Control(pDX, IDC_TITLE2, m_t2);
	DDX_Control(pDX, IDC_TITLE1, m_t1);
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_X, m_x);
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_Y, m_y);
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_Z, m_z);
	DDX_Control(pDX, IDC_STOCK_X, m_x_ctrl);
	DDX_Control(pDX, IDC_STOCK_Y, m_y_ctrl);
	DDX_Control(pDX, IDC_STOCK_Z, m_z_ctrl);
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_HEIG, m_heig);
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_LEN, m_len);
	NExprEdit::DDX_Text_my(pDX, IDC_STOCK_WID, m_wid);
	DDX_Control(pDX, IDC_STOCK_HEIG, m_heig_ctrl);
	DDX_Control(pDX, IDC_STOCK_LEN, m_len_ctrl);
	DDX_Control(pDX, IDC_STOCK_WID, m_wid_ctrl);
	DDV_MinMaxDouble(pDX, m_heig, 0., 10000.);
	DDV_MinMaxDouble(pDX, m_len, 0., 10000.);
	DDV_MinMaxDouble(pDX, m_wid, 0., 10000.);
	DDX_Radio(pDX, IDC_R0, but);
	DDX_Check(pDX, IDC_STOCK_AUTO, Auto);
	DDX_Check(pDX, IDC_STOCK_WRITEPROG0, FillG54);
	DDX_Text(pDX, IDC_STOCK_POS, m_Pos);
}


BEGIN_MESSAGE_MAP(NSetupSetBoxPointDlg, CDialogEx)
ON_COMMAND(IDC_TOOL_APPLY, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R0, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R1, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R2, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R3, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R4, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R5, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R6, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R7, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R8, &NSetupSetBoxPointDlg::OnToolApply)
ON_COMMAND(IDC_R9, &NSetupSetBoxPointDlg::OnToolApply)
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// NSetupSetBoxPointDlg message handlers


BOOL NSetupSetBoxPointDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	COLORMAP Map;
	Map.from = RGB(255, 255, 255);
	Map.to = GetSysColor(COLOR_3DFACE);
	UpdateData(FALSE);
// Set visible bitmap according to system font size
//	int dpi = GetDC()->GetDeviceCaps(LOGPIXELSX);
//	if(dpi == 96) // "SmallFonts"
	{
		
		CBitmap *pBMP,t;
		pBMP = t.FromHandle(m_Pict_s.GetBitmap());
		pBMP->Detach();
		pBMP->LoadMappedBitmap(IDB_BOX_SMALL, 0, &Map, 1);
		m_Pict_s.SetBitmap(*pBMP);
		pBMP->Detach();
	}
	//else
	//{
	//	m_Pict_s.ShowWindow(SW_HIDE);

	//	CBitmap *pBMP,t;
	//	pBMP = t.FromHandle(m_Pict_l.GetBitmap());
	//	pBMP->Detach();
	//	pBMP->LoadMappedBitmap(IDB_BOX_LARGE, 0, &Map, 1);
	//	m_Pict_l.SetBitmap(*pBMP);
	//	pBMP->Detach();
	//}
	std::vector<int> IDs;
	std::vector<CPoint> Positions;
	IDs.push_back(IDC_R0);
	IDs.push_back(IDC_R1);
	IDs.push_back(IDC_R2);
	IDs.push_back(IDC_R3);
	IDs.push_back(IDC_R4);
	IDs.push_back(IDC_R5);
	IDs.push_back(IDC_R6);
	IDs.push_back(IDC_R7);
	IDs.push_back(IDC_R8);
	IDs.push_back(IDC_R9);
	Positions.push_back(CPoint(78, 140));
	Positions.push_back(CPoint(176, 99));
	Positions.push_back(CPoint(22, 117));
	Positions.push_back(CPoint(78, 78));
	Positions.push_back(CPoint(120, 76));
	Positions.push_back(CPoint(176, 38));
	Positions.push_back(CPoint(22, 56));
	Positions.push_back(CPoint(120, 16));
	Positions.push_back(CPoint(99, 108));
	Positions.push_back(CPoint(99, 46));
	PlaceControlsOnPict(m_Pict_s, IDs, Positions);

	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NSetupSetBoxPointDlg::GetBaseShift(int &x, int &y, int &z) const
{

	x=0;y=0;z=0;
	// 0 - 0,0,0
	// 1 - 2,0,0
	// 2 - 0,2,0
	// 3 - 0,0,2
	// 4 - 2,2,0
	// 5 - 2,0,2
	// 6 - 0,2,2
	// 7 - 2,2,2
	// 8 - 1,1,0
	// 9 - 1,1,2
	switch(but)
	{
	case 0:
		return;
	case 1:
		x=2;
		return;
	case 2:
		y=2;
		return;
	case 3:
		z=2;
		return;
	case 4:
		x=2;
		y=2;
		return;
	case 5:
		x=2;
		z=2;
		return;
	case 6:
		y=2;
		z=2;
		return;
	case 7:
		x=2;
		y=2;
		z=2;
		return;
	case 8:
		x=1;
		y=1;
		return;
	case 9:
		x=1;
		y=1;
		z=2;
		return;
	}
}

void NSetupSetBoxPointDlg::OnOK()
{
	UpdateData();
//	but = GetCheckedRadioButton(IDC_R0,IDC_R9) - IDC_R0;	
	if(pDoc)
		pDoc->SetBoxPointEnd();
	CDialogEx::OnOK();
}

void NSetupSetBoxPointDlg::OnCancel()
{
	if(pDoc)
		pDoc->SetBoxPointEnd();
	CDialogEx::OnCancel();
}

void NSetupSetBoxPointDlg::CalcToolPos(BPoint &P, BPoint &N)
{
	int x, y, z;
	GetBaseShift(x, y, z);
	P.Set(m_x + x * StockGabar.dX() * 0.5, m_y + y * StockGabar.dY() * 0.5, m_z + z * StockGabar.dZ() * 0.5, 0.);
	N.Set(0., 0., 1., 0.);
	P += StockGabar.GetMinPoint();
}


void NSetupSetBoxPointDlg::UpdateStockGabar(void)
{
	m_len  = 0.;
	m_wid  = 0.;
	m_heig = 0.;
	class NStock *ppStock = ((CMainFrame *)AfxGetMainWnd())->GetActiveStock();
	if (!ppStock)
	{
		ppStock = pDoc->GetStock(0);
	}
	if(ppStock)
	{
		if(ppStock->GetGabar().IsDefined())
		{
			StockGabar = ppStock->GetGabar();
			m_len  = StockGabar.dX();
			m_wid  = StockGabar.dY();
			m_heig = StockGabar.dZ();
		}
	}
	UpdateData(FALSE);
}

void NSetupSetBoxPointDlg::CalcStockToolGabar(void)
{
	if(!pDoc)
		return;
	pDoc->GetToolMatr(ToolMatr);
	UpdateStockGabar();
	StockGabar.Transform(ToolMatr.invr());
	pDoc->PosTool.SetStockGabar(StockGabar, ToolMatr);
}

void NSetupSetBoxPointDlg::OnToolApply()
{
	if(!pDoc)
		return;
	pDoc->JumpToProjStart();
	UpdateData();
//	but = GetCheckedRadioButton(IDC_R0,IDC_R9) - IDC_R0;
	CalcStockToolGabar();
	int x, y, z;
	GetBaseShift(x, y, z);
	BPoint P(m_x + x * StockGabar.dX() * 0.5, m_y + y * StockGabar.dY() * 0.5, m_z + z * StockGabar.dZ() * 0.5, 0.);
	BPoint N(0., 0., 1., 0.);
	P += StockGabar.GetMinPoint();
	P = P * ToolMatr;
	N = N * ToolMatr;
	if(!pDoc->SetBoxPointApply(P, N))
		AfxMessageBox(IDS_MES_ERR_UCSSETONSTOCK);
}

void NSetupSetBoxPointDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(!pDoc)
		return;
	if(bShow)
		OnToolApply();
}

void NSetupSetBoxPointDlg::PlaceControlsOnPict(const CWnd &Pict, const std::vector<int> IDs, const std::vector<CPoint> Positions)
{
	if (IDs.size() != Positions.size())
		return;
	CRect RectPict;
	Pict.GetWindowRect(RectPict);
	CWnd *pParent = Pict.GetParent();
	if (!pParent)
		return;
	pParent->ScreenToClient(RectPict);
	for (unsigned int i = 0; i < IDs.size(); ++i)
	{
		CWnd *pCtrl = GetDlgItem(IDs[i]);
		if (!pCtrl)
			return;
		CRect RectCtrl;
		pCtrl->GetWindowRect(RectCtrl);
		pCtrl->MoveWindow(RectPict.left - RectCtrl.Width() / 2 + Positions[i].x, RectPict.top - RectCtrl.Height() / 2 + Positions[i].y, RectCtrl.Width(), RectCtrl.Height());

	}
}
