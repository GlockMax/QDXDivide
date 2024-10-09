// NPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCM.h"
#include "NCMDoc.h"
#include "MainFrm.h"
#include "NCUnit.h"
#include "ResLocal.h"
#include "CMapStringToNStock.h"
#include "NStock.h"
#include "BMatr.h"
#include "RXYZArrows.h"
#include "NPositionDlg.h"

extern CNCMApp theApp;
// NPositionDlg dialog

IMPLEMENT_DYNAMIC(NPositionDlg, CDialog)

NPositionDlg::NPositionDlg(CWnd* pParent, CNCMDoc *ipDoc)
	: CDialog(NPositionDlg::IDD, pParent)
	, m_move_x(0)
	, m_move_y(0)
	, m_move_z(0)
	, m_pos_x(0)
	, m_pos_y(0)
	, m_pos_z(0)
	, m_rel(0)
	, m_ang_x(0)
	, m_ang_y(0)
	, m_ang_z(0)
	, m_scale_x(1.)
	, m_scale_y(1.)
	, m_scale_z(1.)
	, m_MovePrg(FALSE)
	, m_move_copy(0)
	, m_abs_x(0.)
	, m_abs_y(0.)
	, m_abs_z(0.)
	, m_abs_sx(0.)
	, m_abs_sy(0.)
	, m_abs_sz(0.)
	, m_abs_a(0.)
	, m_abs_b(0.)
	, m_abs_c(0.)
	, m_abs_simm(FALSE)
	, m_abs_2sol(FALSE)
{
	pDoc = ipDoc;
}

NPositionDlg::~NPositionDlg()
{
}

void NPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_X, m_abs_x);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_Y, m_abs_y);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_Z, m_abs_z);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_A, m_abs_a);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_B, m_abs_b);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_C, m_abs_c);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_SX, m_abs_sx);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_SY, m_abs_sy);
	NExprEdit::DDX_Text_my(pDX, IDC_ABS_SZ, m_abs_sz);
	DDX_Check(pDX, IDC_ABS_SIMM, m_abs_simm);
	DDX_Check(pDX, IDC_ABS_2SOL, m_abs_2sol);
	DDX_Control(pDX, IDC_ABS_SX, m_abs_sx_c);
	DDX_Control(pDX, IDC_ABS_SY, m_abs_sy_c);
	DDX_Control(pDX, IDC_ABS_SZ, m_abs_sz_c);
	DDX_Control(pDX, IDC_ABS_SIMM, m_abs_simm_c);
	DDX_Control(pDX, IDC_ABS_2SOL, m_abs_2sol_c);
	DDX_Control(pDX, IDC_ABS_X, m_abs_x_c);
	DDX_Control(pDX, IDC_ABS_Y, m_abs_y_c);
	DDX_Control(pDX, IDC_ABS_Z, m_abs_z_c);
	DDX_Control(pDX, IDC_ABS_A, m_abs_a_c);
	DDX_Control(pDX, IDC_ABS_B, m_abs_b_c);
	DDX_Control(pDX, IDC_ABS_C, m_abs_c_c);
	NExprEdit::DDX_Text_my(pDX, IDC_MOVE_X, m_move_x);
	NExprEdit::DDX_Text_my(pDX, IDC_MOVE_Y, m_move_y);
	NExprEdit::DDX_Text_my(pDX, IDC_MOVE_Z, m_move_z);
	NExprEdit::DDX_Text_my(pDX, IDC_POS_X, m_pos_x);
	NExprEdit::DDX_Text_my(pDX, IDC_POS_Y, m_pos_y);
	NExprEdit::DDX_Text_my(pDX, IDC_POS_Z, m_pos_z);
	DDX_Text(pDX, IDC_OBJ_NAME, m_ObjName);
	DDX_Control(pDX, IDC_OBJ_NAME, m_ObjName_c);
	DDX_Radio(pDX, IDC_REL, m_rel);
	DDX_Control(pDX, IDC_MOVE_X, m_move_x_c);
	DDX_Control(pDX, IDC_MOVE_Y, m_move_y_c);
	DDX_Control(pDX, IDC_MOVE_Z, m_move_z_c);
	DDX_Control(pDX, IDC_POS_X, m_pos_x_c);
	DDX_Control(pDX, IDC_POS_Y, m_pos_y_c);
	DDX_Control(pDX, IDC_POS_Z, m_pos_z_c);
	DDX_Control(pDX, IDC_ROT_X, m_ang_x_c);
	DDX_Control(pDX, IDC_ROT_Y, m_ang_y_c);
	DDX_Control(pDX, IDC_ROT_Z, m_ang_z_c);
	NExprEdit::DDX_Text_my(pDX, IDC_ROT_X, m_ang_x);
	NExprEdit::DDX_Text_my(pDX, IDC_ROT_Y, m_ang_y);
	NExprEdit::DDX_Text_my(pDX, IDC_ROT_Z, m_ang_z);
	DDX_Control(pDX, IDC_SCALE_X, m_scale_x_c);
	DDX_Control(pDX, IDC_SCALE_Y, m_scale_y_c);
	DDX_Control(pDX, IDC_SCALE_Z, m_scale_z_c);
	DDX_Text(pDX, IDC_SCALE_X, m_scale_x);
	DDX_Text(pDX, IDC_SCALE_Y, m_scale_y);
	DDX_Text(pDX, IDC_SCALE_Z, m_scale_z);
	DDX_Control(pDX, IDC_REDO, m_redo_c);
	DDX_Control(pDX, IDC_UNDO, m_undo_c);
	DDX_Control(pDX, IDC_NOTE_DELTA, m_Title_Delta);
	DDX_Control(pDX, IDC_NOTE_ROTANGL, m_Title_Rotang);
	DDX_Control(pDX, IDC_NOTE_SCALE, m_Title_Scale);
	DDX_Check(pDX, IDC_MOVCNCPROG, m_MovePrg);
	DDX_Control(pDX, IDC_MOVCNCPROG, m_CheckBox);
	DDX_Radio(pDX, IDC_MOVE, m_move_copy);
	DDX_Control(pDX, IDC_APPLY, m_apply_c);
	DDX_Control(pDX, IDC_APPLY_ABS, m_apply_abs_c);
}


BEGIN_MESSAGE_MAP(NPositionDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &NPositionDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_APPLY_ABS, &NPositionDlg::OnBnClickedApplyAbs)
	ON_BN_CLICKED(IDC_REL, &NPositionDlg::OnBnClickedFields)
	ON_BN_CLICKED(IDC_ANG, &NPositionDlg::OnBnClickedFields)
	ON_BN_CLICKED(IDC_SCALE, &NPositionDlg::OnBnClickedFields)
	ON_BN_CLICKED(IDC_UNDO, &NPositionDlg::OnBnClickedUndo)
	ON_BN_CLICKED(IDC_REDO, &NPositionDlg::OnBnClickedRedo)
	ON_BN_CLICKED(IDC_ABS_2SOL, &NPositionDlg::OnBnClickedAbs2Sol)
	ON_EN_KILLFOCUS(IDC_POS_X, &NPositionDlg::OnKillfocusPos)
	ON_EN_KILLFOCUS(IDC_POS_Y, &NPositionDlg::OnKillfocusPos)
	ON_EN_KILLFOCUS(IDC_POS_Z, &NPositionDlg::OnKillfocusPos)
END_MESSAGE_MAP()


// NPositionDlg message handlers

void NPositionDlg::OnBnClickedApply()
{
	if(pDoc->NGetPosChecks()->empty() )
		return;
	Buffer.CopyFrom(pDoc->NGetPosChecks());
	UpdateData(TRUE);
	UpdateData(FALSE);
	StoreBasePos();
	BMatr Matr;
	switch(m_rel)
	{
	case 0:
		Matr.Trans(m_move_x, m_move_y, m_move_z);
		break;
	case 1:
		Matr = BMatr().RotX(m_pos_x, m_pos_y, m_pos_z, m_ang_x)
			* BMatr().RotY(m_pos_x, m_pos_y, m_pos_z, m_ang_y)
			* BMatr().RotZ(m_pos_x, m_pos_y, m_pos_z, m_ang_z);
		break;
	case 2:
		Matr = BMatr().Trans(-m_pos_x, -m_pos_y, -m_pos_z)
			* BMatr().Scale(m_scale_x, m_scale_y, m_scale_z)
			* BMatr().Trans(m_pos_x, m_pos_y, m_pos_z);
		break;
	}


	if (m_move_copy == 1)
	{
		bool StockChanged = Copy(Matr);
		pDoc->NGetPosChecks()->CopyFrom(&Buffer);
		LPARAM Hint = N_OGL | N_LEFT;
		if (StockChanged)// to prevent tree nodes collapsing when stocks are not changed (is not actual now)
			Hint |= N_STOCKSTATECHANGE;
		pDoc->UpdateAllViews(NULL, Hint, NULL);
	}
	else
		Transform(Matr);
	RecalcBasePos();

	m_undo_c.EnableWindow(TRUE);
	m_redo_c.EnableWindow(CanPosRedo() ? TRUE : FALSE);

	FillAbsFields();

	pDoc->UpdateAllViews(NULL, N_OGL);
}

void NPositionDlg::OnBnClickedApplyAbs()
{
	if(pDoc->NGetPosChecks()->empty() )
		return;
	Buffer.CopyFrom(pDoc->NGetPosChecks());
	UpdateData(TRUE);
	UpdateData(FALSE);

	const double Simm = m_abs_simm ? -1. : 1.;
	BMatr Matr;
	Matr = BMatr().Scale(Simm * m_abs_sx, Simm * m_abs_sy, Simm * m_abs_sz)
		* BMatr().RotX(0., 0., 0., m_abs_a)
		* BMatr().RotY(0., 0., 0., m_abs_b)
		* BMatr().RotZ(0., 0., 0., m_abs_c)
		* BMatr().Trans(m_abs_x, m_abs_y, m_abs_z);
	m_undo_c.EnableWindow(TRUE);
	m_redo_c.EnableWindow(FALSE);

	Transform(Matr, true);

	pDoc->UpdateAllViews(NULL, N_OGL);
}

void NPositionDlg::OnBnClickedAbs2Sol()
{
	FillAbsFields();
}

void NPositionDlg::OnKillfocusPos()
{
	UpdateData(TRUE);
	pDoc->UpdateAllViews(nullptr, N_OGL);
}

bool NPositionDlg::CanPosRedo() const
{
	if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
		return false;
	return pDoc->UHCanRedo();
}

bool NPositionDlg::CanPosUndo() const
{
	if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
		return false;
	return pDoc->UHCanUndo();
}

void NPositionDlg::OnCancel()
{
	SetMoving(false);
	if (pDoc->GetUndoHistory().IsStockChanged())
	{
		pDoc->GetUndoHistory().ResetStockChanged();
		pDoc->ResetStocks();
	}
	pDoc->NGetPosChecks()->RemoveAll();

	CDialog::OnCancel();
	Active = false;
	pDoc->UpdateAllViews(nullptr, N_POS_CHECK | N_OGL);
}

BOOL NPositionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillAbsFields();
	UpdateData(FALSE);

	RecalcBasePos();
	EnableFields();

	m_undo_c.EnableWindow(CanPosUndo() ? TRUE : FALSE);
	m_redo_c.EnableWindow(CanPosRedo() ? TRUE : FALSE);

	UpdateData(FALSE);
	GetParent()->RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	Active = true;
	pDoc->UpdateAllViews(nullptr, N_POS_CHECK | N_OGL);
	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls
		m_ToolTip.AddTool(&m_abs_x_c, IDS_TOOLTIP_POSITION_ABCXYZ);
		m_ToolTip.AddTool(&m_abs_y_c, IDS_TOOLTIP_POSITION_ABCXYZ);
		m_ToolTip.AddTool(&m_abs_z_c, IDS_TOOLTIP_POSITION_ABCXYZ);
		m_ToolTip.AddTool(&m_abs_a_c, IDS_TOOLTIP_POSITION_ANGLE_XYZ);
		m_ToolTip.AddTool(&m_abs_b_c, IDS_TOOLTIP_POSITION_ANGLE_XYZ);
		m_ToolTip.AddTool(&m_abs_c_c, IDS_TOOLTIP_POSITION_ANGLE_XYZ);
		m_ToolTip.AddTool(&m_move_x_c, IDS_TOOLTIP_POSITION_MOVE_XYZ);
		m_ToolTip.AddTool(&m_move_y_c, IDS_TOOLTIP_POSITION_MOVE_XYZ);
		m_ToolTip.AddTool(&m_move_z_c, IDS_TOOLTIP_POSITION_MOVE_XYZ);
		m_ToolTip.AddTool(&m_ang_x_c, IDS_TOOLTIP_POSITION_RELANGLE_XYZ);
		m_ToolTip.AddTool(&m_ang_y_c, IDS_TOOLTIP_POSITION_RELANGLE_XYZ);
		m_ToolTip.AddTool(&m_ang_z_c, IDS_TOOLTIP_POSITION_RELANGLE_XYZ);
		m_ToolTip.AddTool(&m_apply_c, IDS_TOOLTIP_POSITION_APPLY);
		m_ToolTip.AddTool(&m_apply_abs_c, IDS_TOOLTIP_POSITION_ABSAPPLY);
		m_ToolTip.AddTool(&m_ObjName_c, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_abs_sx_c, IDS_TOOLTIP_POSITION_SCALE);
		m_ToolTip.AddTool(&m_abs_sy_c, IDS_TOOLTIP_POSITION_SCALE);
		m_ToolTip.AddTool(&m_abs_sz_c, IDS_TOOLTIP_POSITION_SCALE);
		m_ToolTip.AddTool(&m_abs_simm_c, IDS_TOOLTIP_POSITION_SIMM);
		m_ToolTip.AddTool(&m_abs_2sol_c, IDS_TOOLTIP_POSITION_2SOL);

		m_ToolTip.Activate(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NPositionDlg::StartPosDlg()
{
	Buffer.CopyFrom(pDoc->NGetPosChecks());
	FillBasePos();
	SetMoving(true);
	UpdateData(FALSE);
}

void NPositionDlg::RecalcBasePos()
{
	Buffer.CopyFrom(pDoc->NGetPosChecks());
	if (Buffer.empty())
		return;
	FillBasePos();

	UpdateData(FALSE);

	GetParent()->RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

void NPositionDlg::OnBnClickedFields()
{
	EnableFields();
}

void NPositionDlg::EnableFields()
{
	UpdateData(TRUE);
	BOOL move = FALSE;
	BOOL pos = FALSE;
	BOOL ang = FALSE;
	BOOL scale = FALSE;
	switch(m_rel)
	{
	case 0: // Relative
		move = TRUE;
		break;
	case 1: // Angle
		pos = TRUE;
		ang = TRUE;
		break;
	case 2: // Scale
		pos = TRUE;
		scale = TRUE;
		break;
	}
	m_move_x_c.EnableWindow(move);
	m_move_y_c.EnableWindow(move);
	m_move_z_c.EnableWindow(move);

	m_move_x_c.ShowWindow(move ? SW_SHOW : SW_HIDE);
	m_move_y_c.ShowWindow(move ? SW_SHOW : SW_HIDE);
	m_move_z_c.ShowWindow(move ? SW_SHOW : SW_HIDE);
	m_Title_Delta.ShowWindow(move ? SW_SHOW : SW_HIDE);

	m_pos_x_c.EnableWindow(pos);
	m_pos_y_c.EnableWindow(pos);
	m_pos_z_c.EnableWindow(pos);

	m_ang_x_c.EnableWindow(ang);
	m_ang_y_c.EnableWindow(ang);
	m_ang_z_c.EnableWindow(ang);

	m_ang_x_c.ShowWindow(ang ? SW_SHOW : SW_HIDE);
	m_ang_y_c.ShowWindow(ang ? SW_SHOW : SW_HIDE);
	m_ang_z_c.ShowWindow(ang ? SW_SHOW : SW_HIDE);
	m_Title_Rotang.ShowWindow(ang ? SW_SHOW : SW_HIDE);

	m_scale_x_c.EnableWindow(scale);
	m_scale_y_c.EnableWindow(scale);
	m_scale_z_c.EnableWindow(scale);

	m_scale_x_c.ShowWindow(scale ? SW_SHOW : SW_HIDE);
	m_scale_y_c.ShowWindow(scale ? SW_SHOW : SW_HIDE);
	m_scale_z_c.ShowWindow(scale ? SW_SHOW : SW_HIDE);
	m_Title_Scale.ShowWindow(scale ? SW_SHOW : SW_HIDE);

	UpdateData(FALSE);
}

void NPositionDlg::OnBnClickedUndo()
{
	if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
		return;
	((CMainFrame*)AfxGetMainWnd())->Send2PropPane(nullptr);
	theApp.OnDontcutMode();
	pDoc->UHUndoGroup();

	m_undo_c.EnableWindow(CanPosUndo() ? TRUE : FALSE);
	m_redo_c.EnableWindow(TRUE);

	GetParent()->RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	LPARAM Hint = N_POS_CHECK | N_OGL | N_LEFT;

	pDoc->UpdateAllViews(NULL, Hint, NULL);
	FillAbsFields();
}

void NPositionDlg::OnBnClickedRedo()
{
	if (((CMainFrame*)AfxGetMainWnd())->IsCuttingInProgress())
		return;
	((CMainFrame*)AfxGetMainWnd())->Send2PropPane(nullptr);
	theApp.OnDontcutMode();
	pDoc->UHDoGroup();

	m_undo_c.EnableWindow(TRUE);
	m_redo_c.EnableWindow(CanPosRedo() ? TRUE : FALSE);

	GetParent()->RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	LPARAM Hint = N_OGL | N_LEFT;
	pDoc->UpdateAllViews(NULL, Hint, NULL);
	FillAbsFields();
}

void NPositionDlg::Transform(const BMatr &M, bool Abs /*= false*/)
{
	CString key;
	class NStock* pS = nullptr;
	bool StocksPresent = false;
	for(int i = 0; i < Buffer.GetSize() && !StocksPresent; ++i)
	{
		Buffer.GetAt(i, key, (CObject*&)pS);
		StocksPresent |= (key.Left(CMapStringToNStock::PrefixStock.GetLength()) == CMapStringToNStock::PrefixStock);
	}
	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	if (StocksPresent) // если позиционирум заготовки
		pDoc->NGetStockList()->ResetBodiesAndRendersLists(); // скидываем указатели в цепочках заготовок, тк Transform может вызывать деструтор
	for each(const auto& el in Buffer)
	{	
		pS = static_cast<NStock*>(el.second);
		pDoc->TransMovObjWU(pS, Abs ? pS->GetMatr().invr() * M : M);
	}
	if (StocksPresent) // если позиционирум заготовки
	{
		//pDoc->NGetStockList()->CreateBodiesAndRendersLists();
		//pDoc->ResetStocks();
		if (m_MovePrg)
		{
			pDoc->TransMovObjWU(nullptr, M);// move UCSs
		}
	}
	pDoc->GetUndoHistory().EndGroup(InternalGroup);
}

void NPositionDlg::SetMoving(bool val)
{
	CString key;
	for each (const auto & el in *pDoc->NGetStockList())
	{
		class NStock* pS = static_cast<NStock*>(el.second);
		if (pS != nullptr)
			pS->SetMoving(false);
	}
	for each (const auto & el in *pDoc->NGetPartList())
	{
		class NStock* pS = static_cast<NStock*>(el.second);
		if (pS != nullptr)
			pS->SetMoving(false);
	}
	for each (const auto & el in *pDoc->NGetExObjList())
	{
		class NStock* pS = static_cast<NStock*>(el.second);
		if (pS != nullptr)
			pS->SetMoving(false);
	}
	if (val)
	{
		for each (const auto & el in Buffer)
		{
			class NStock* pS = static_cast<NStock*>(el.second);
			if (pS != nullptr)
				pS->SetMoving(false);
		}
	}
}

const BBox NPositionDlg::GetGabar(void)
{
	BBox TmpBox;
	for each (const auto & el in Buffer)
	{
		class NStock* pS = static_cast<NStock*>(el.second);
		if (pS != nullptr)
			TmpBox.Expand(pS->GetGabar());
	}
	if(!TmpBox.IsDefined())
		TmpBox.Expand(0., 0., 0.);
	return TmpBox;
}

void NPositionDlg::FillAbsFields()
{
	UpdateData(TRUE);
	class NStock* pS = nullptr;
	if (GetLastObject(pS, m_ObjName))
		FillAbsFields(pS->GetMatr());
	else
		FillAbsFields(BMatr());
	UpdateData(FALSE);
}

void NPositionDlg::Draw()
{
	glPushMatrix();
	BPoint Shift(BPoint(m_pos_x, m_pos_y, m_pos_z, 1.));
	glTranslated(Shift.GetX(), Shift.GetY(), Shift.GetZ());
	RXYZArrows BaseXYZ;
	BaseXYZ.Draw(NColor(0.5, 0.5, 0.5, 1.));
	glPopMatrix();
}

void NPositionDlg::FillBasePos()
{
	CString Name;
	NStock* pStock;
	if (GetLastObject(pStock, Name))
	{
		m_pos_x = pStock->bas_X;
		m_pos_y = pStock->bas_Y;
		m_pos_z = pStock->bas_Z;
	}
}

void NPositionDlg::StoreBasePos()
{
	CString Name;
	NStock* pStock = nullptr;
	for (int i = 0; i < Buffer.GetSize(); ++i)
	{
		Buffer.GetAt(i, Name, (CObject*&)pStock);
		if (pStock != nullptr)
		{
			pStock->bas_X = m_pos_x;
			pStock->bas_Y = m_pos_y;
			pStock->bas_Z = m_pos_z;
		}
	}

}

bool NPositionDlg::GetLastObject(NStock*& pStock, CString& Name)
{
	Name.Empty();
	pStock = nullptr;
	for (int i = 0; i < Buffer.GetSize(); ++i)
		Buffer.GetAt(i, Name, (CObject*&)pStock);
	bool Found = pStock != nullptr;
	if(Found)
		Name.Delete(0, CMapStringToNStock::PrefixStock.GetLength());

	return Found;
}

void NPositionDlg::FillAbsFields(const BMatr& M)
{
	BPoint Ang[2];
	BPoint Scale;
	double Simm;
	M.CalcDecomp(Simm, Scale, Ang);
	const int SolInd = m_abs_2sol ? 1 : 0;
	m_abs_a = Ang[SolInd].GetX();
	m_abs_b = Ang[SolInd].GetY();
	m_abs_c = Ang[SolInd].GetZ();
	m_abs_x = M.GetRow(3).GetX();
	m_abs_y = M.GetRow(3).GetY();
	m_abs_z = M.GetRow(3).GetZ();
	m_abs_sx = Scale.GetX();
	m_abs_sy = Scale.GetY();
	m_abs_sz = Scale.GetZ();
	m_abs_simm = Simm < 0.;
}

void NPositionDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

bool NPositionDlg::Copy(const BMatr &M)
{
	bool Res = false;
	// Создаем копии элементов из буффера, к которым будет применена матрица сдвига
	CMapStringToNStock local;
	for each (const auto & el in Buffer)
	{
		class NStock* pS = static_cast<NStock*>(el.second);
		NStock *newStock = NULL;
		pS->CreateCopyTo(newStock);
		newStock->SetMoving(false);
		local.SetAt(el.first, newStock);
	}

	// Применение матрицы сдвига к копиям, лежащим в Buffer, и последующие добавление их в документ
	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	CString NewName;
//	Buffer.RemoveAll();
	for each (const auto & el in local)
	{
		class NStock* pS = static_cast<NStock*>(el.second);
		CString key = el.first;
		pS->Transformm(M);// don't need TransMovObjWU because object will be deleted on Undo
		if (key.Left(CMapStringToNStock::PrefixStock.GetLength()) == CMapStringToNStock::PrefixStock)
		{
			pDoc->AddStockWU(pS, BMatr().SetE());
			Res = true;
			const int index = pDoc->NGetStockList()->GetIndex(pS);
			NewName = CMapStringToNStock::PrefixStock + pDoc->NGetStockList()->GetKey(index);
		}
		else if (key.Left(CMapStringToNStock::PrefixPart.GetLength()) == CMapStringToNStock::PrefixPart)
		{
			pDoc->AddPartWU(pS);
			const int index = pDoc->NGetPartList()->GetIndex(pS);
			NewName = CMapStringToNStock::PrefixPart + pDoc->NGetPartList()->GetKey(index);
		}
		else if (key.Left(CMapStringToNStock::PrefixFixt.GetLength()) == CMapStringToNStock::PrefixFixt)
		{
			pDoc->AddExobjWU(pS);
			const int index = pDoc->NGetExObjList()->GetIndex(pS);
			NewName = CMapStringToNStock::PrefixFixt + pDoc->NGetExObjList()->GetKey(index);
		}
//		Buffer.SetAt(NewName, pS);
	}
	pDoc->GetUndoHistory().EndGroup(InternalGroup);
	return Res;
}

BOOL NPositionDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}
