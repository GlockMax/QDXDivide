// NStateRegsCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "ResLocal.h"
#include "NFont.h"
#include "NColor.h"
#include "NVarSet.h"
#include "NStateRegsCtrl.h"
#include "..\source\GridCtrl\GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
const COLORREF ChangedItemCol = RGB(255,0,0);
/////////////////////////////////////////////////////////////////////////////
// NStateGeomCtrl

IMPLEMENT_DYNAMIC(NStateRegsCtrl, CGridCtrl)
NStateRegsCtrl::NStateRegsCtrl()
{
	Editable = false;
}

NStateRegsCtrl::~NStateRegsCtrl()
{
}


BEGIN_MESSAGE_MAP(NStateRegsCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(NStateGeomCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStateGeomCtrl message handlers

int NStateRegsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetDefaultCellType(RUNTIME_CLASS( CGridCell ));
	SetEditable(Editable == true);
	EnableSelection(FALSE);
	SetColumnResize(FALSE);
	SetRowResize(FALSE);
	SetFrameFocusCell(FALSE);
	SetTrackFocusCell(FALSE);
	SetDefCellMargin(2);
	SetColumnCount(2);
	SetRowCount(1);
	SetFixedRowCount(1);
	SetAutoSizeStyle(GVS_BOTH);
	SetCellType(0,0, RUNTIME_CLASS( CGridCell ));
	SetCellType(0,1, RUNTIME_CLASS( CGridCell ));
	CString str;
	str.LoadString(IDS_COLUMN_N);
	SetItemText(0,0, str);
	str.LoadString(IDS_COLUMN_V);
	SetItemText(0,1, str);

	SetCompareFunction(CGridCtrl::pfnCellTextCompare);
	return 0;
}
bool NStateRegsCtrl::IsRegVisible(const vector<pair<int, int>>& VisibleRegisters, int Ind)
{
	for each(auto Diap in VisibleRegisters)
		if (Ind >= Diap.first && Ind < Diap.second)
			return true;
	return false;
}
void NStateRegsCtrl::SetSettings()
{
	CGridCellBase *pDefault = GetDefaultCell(FALSE, FALSE);
	NColor *pCol;
	if(NCM_PROJECT.Colors.Lookup("ТекстПанелиКонтроля",pCol))
	{
		pDefault->SetTextClr(pCol->GetRGB());
	}
	if(NCM_PROJECT.Colors.Lookup("ФонПанелиКонтроля",pCol))
	{
		pDefault->SetBackClr(pCol->GetRGB());
	}

	LOGFONT lf;
	lf = NCM_PROJECT.Defaults.GetFont(_T("Defaults/PaneParameters/Font"), lf);
	::DeleteObject(pDefault->GetFontObject()->Detach());
	pDefault->SetFont(&lf);

	SetBkColor(GetSysColor(COLOR_3DFACE));	
}

void NStateRegsCtrl::Update(NMapIntToDouble & Regs, const NVarSet& VarSet, const vector<pair<int, int>> &VisibleRegisters)
{
	auto Pos = GetScrollPos32(SB_VERT);
	DeleteNonFixedRows();
	POSITION pos;
	int i = 0, k;
	double val;
	int RegsNumber = 0;
	for(pos = Regs.GetStartPosition(), k = 1; pos != NULL;)
	{
		Regs.GetNextAssoc(pos, i, val);
		if (val != 0)
		{
			if (IsRegVisible(VisibleRegisters, i))
				++RegsNumber;
			else if (VarSet.HasIndex(i))
				++RegsNumber;
		}
	}
	SetRowCount(RegsNumber + 1);

	for(pos = Regs.GetStartPosition(), k = 1; pos != NULL;)
	{
		Regs.GetNextAssoc(pos, i, val);
		bool Variable = VarSet.HasIndex(i);
		if((IsRegVisible(VisibleRegisters, i) || Variable) && val != 0.)
		{
			int delta;
			CString sval = VarSet.GetName(i, delta);
			CString format = sval;
			if (delta >= 0 || !Variable)
			{
				format += " %d";
				sval.Format(format, Variable ? delta : i);
			}
			SetItemText(k, 0, sval);
			if (Editable)
				SetItemState(k, 0, GetItemState(k, 0) | GVIS_READONLY);
			if( PrevRegs[i] == 0)
				SetItemFgColour(k, 0, ChangedItemCol);
			else
				SetItemFgColour(k, 0, GetDefaultCell(FALSE, FALSE)->GetTextClr());

			sval.Format("%lg", Regs[i]);
			SetItemText(k, 1, sval);
			if( Regs[i] != PrevRegs[i])
			{
				SetItemFgColour(k, 1, ChangedItemCol);
			}
			else
				SetItemFgColour(k, 1, GetDefaultCell(FALSE, FALSE)->GetTextClr());
			++k;
		}
	}
	PrevRegs = Regs;
	SortItems(0, true);
	SetScrollPos32(SB_VERT, Pos);
	RedrawWindow();
}

BOOL NStateRegsCtrl::Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle)
{
	return CGridCtrl::Create( rect, parent, nID, dwStyle);
}

void NStateRegsCtrl::OnSize(UINT nType, int cx, int cy)
{
	m_arColWidths[0] = cx / 2;
	m_arColWidths[1] = cx / 2;
	CGridCtrl::OnSize(nType, cx, cy);
}
