// NUnitTurnCompDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\source\GridCtrl\GridCellNumeric.h"
#include "NUnitTurnCompDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// NUnitTurnCompDlg dialog

IMPLEMENT_DYNAMIC(NUnitTurnCompDlg, CPropertyPage)
NUnitTurnCompDlg::NUnitTurnCompDlg()
	: CPropertyPage(NUnitTurnCompDlg::IDD)
{
}

NUnitTurnCompDlg::~NUnitTurnCompDlg()
{
}

BEGIN_MESSAGE_MAP(NUnitTurnCompDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_TURN_COR_ADD, OnBnClickedTurnCorAdd)
	ON_BN_CLICKED(IDC_TURNCOMP_DEL, OnBnClickedClearAll)
END_MESSAGE_MAP()


// NUnitTurnCompDlg message handlers

BOOL NUnitTurnCompDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CRect rect, rect1;
	
	GetDlgItem(IDC_BBOX)->GetClientRect(&rect);
	GetClientRect(&rect1);
	rect.bottom = rect1.bottom;
	m_GridCtrl.Create(rect, this, IDD_UNIT_UCS);
	m_GridCtrl.SetDefaultCellType(RUNTIME_CLASS( CGridCellNumeric ));
	m_GridCtrl.SetDefCellMargin(2);
	m_GridCtrl.SetColumnCount(5);
	m_GridCtrl.SetRowCount(int(Elems.GetSize() + 1));
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetAutoSizeStyle(GVS_BOTH);
	for(int k = 0; k < 5; ++k)
		m_GridCtrl.SetCellType(0,k, RUNTIME_CLASS( CGridCell ));
	CString tmpstr;
	tmpstr.LoadString(IDS_TURN_CORR);
	m_GridCtrl.SetItemText(0,0,tmpstr);
	tmpstr.LoadString(IDS_TURN_X);
	m_GridCtrl.SetItemText(0,1,tmpstr);
	tmpstr.LoadString(IDS_TURN_Z);
	m_GridCtrl.SetItemText(0,2,tmpstr);
	tmpstr.LoadString(IDS_TURN_R);
	m_GridCtrl.SetItemText(0,3,tmpstr);
	tmpstr.LoadString(IDS_TURN_DIRECT);
	m_GridCtrl.SetItemText(0,4,tmpstr);
	for( int c = 0; c < 5; ++c)
	{
		m_GridCtrl.SetItemFgColour(0, c, RGB(0,0,255));
	}
	for( int c = 1; c < Elems.GetSize() + 1; ++c)
	{
		m_GridCtrl.SetItemFgColour(c, 0, RGB(0,0,255));
		CGridCellNumeric *pCell = (CGridCellNumeric *)m_GridCtrl.GetCell(c, 0);
		pCell->SetFlags(CGridCellNumeric::Integer);
		pCell = (CGridCellNumeric *)m_GridCtrl.GetCell(c, 4);
		pCell->SetFlags(CGridCellNumeric::Integer);
		pCell = (CGridCellNumeric *)m_GridCtrl.GetCell(c, 1);
		pCell->SetFlags(CGridCellNumeric::Math | CGridCellNumeric::Negative);
		pCell = (CGridCellNumeric *)m_GridCtrl.GetCell(c, 2);
		pCell->SetFlags(CGridCellNumeric::Math | CGridCellNumeric::Negative);
		pCell = (CGridCellNumeric *)m_GridCtrl.GetCell(c, 3);
		pCell->SetFlags(CGridCellNumeric::Math | CGridCellNumeric::Negative);
	}
	CString sval;
	for(int k = 0, l = 0; k < 1000; ++k)
	{
		NTurnCompElem elem;
		if(!Elems.Lookup(k, elem))
			continue;
		++l;
		sval.Format(_T("%d"), k);
		m_GridCtrl.SetItemText(l, 0, sval);
		sval.Format(_T("%lf"), elem.X);
		m_GridCtrl.SetItemText(l, 1, sval);
		sval.Format(_T("%lf"), elem.Z);
		m_GridCtrl.SetItemText(l, 2, sval);
		sval.Format(_T("%lf"), elem.R);
		m_GridCtrl.SetItemText(l, 3, sval);
		sval.Format(_T("%d"), elem.K);
		m_GridCtrl.SetItemText(l, 4, sval);
	}

	m_GridCtrl.AutoSize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NUnitTurnCompDlg::OnOK()
{
	Elems.RemoveAll();
	for(int i = 1; i < m_GridCtrl.GetRowCount(); ++i)
	{
		int n = atoi(m_GridCtrl.GetItemText(i, 0));
		Elems[n].X = atof(m_GridCtrl.GetItemText(i, 1));
		Elems[n].Z = atof(m_GridCtrl.GetItemText(i, 2));
		Elems[n].R = atof(m_GridCtrl.GetItemText(i, 3));
		Elems[n].K = atoi(m_GridCtrl.GetItemText(i, 4));
	}

	CPropertyPage::OnOK();
}

void NUnitTurnCompDlg::OnBnClickedTurnCorAdd()
{
	const int Ind = m_GridCtrl.GetRowCount();
	m_GridCtrl.SetRowCount(Ind + 1);
	m_GridCtrl.SetItemFgColour(m_GridCtrl.GetRowCount() - 1, 0, RGB(0,0,255));
	CGridCellNumeric* pCell = (CGridCellNumeric*)m_GridCtrl.GetCell(Ind, 0);
	pCell->SetFlags(CGridCellNumeric::Integer);
	pCell = (CGridCellNumeric*)m_GridCtrl.GetCell(Ind, 4);
	pCell->SetFlags(CGridCellNumeric::Integer);
	pCell = (CGridCellNumeric*)m_GridCtrl.GetCell(Ind, 1);
	pCell->SetFlags(CGridCellNumeric::Math | CGridCellNumeric::Negative);
	pCell = (CGridCellNumeric*)m_GridCtrl.GetCell(Ind, 2);
	pCell->SetFlags(CGridCellNumeric::Math | CGridCellNumeric::Negative);
	pCell = (CGridCellNumeric*)m_GridCtrl.GetCell(Ind, 3);
	pCell->SetFlags(CGridCellNumeric::Math | CGridCellNumeric::Negative);
	CString sval;
	sval.Format(_T("%lf"), 0.);
	m_GridCtrl.SetItemText(Ind, 1, sval);
	sval.Format(_T("%lf"), 0.);
	m_GridCtrl.SetItemText(Ind, 2, sval);
	sval.Format(_T("%lf"), 0.);
	m_GridCtrl.SetItemText(Ind, 3, sval);
	sval.Format(_T("%d"), 0);
	m_GridCtrl.SetItemText(Ind, 4, sval);
}

void NUnitTurnCompDlg::OnBnClickedClearAll()
{
	UpdateData();
	m_GridCtrl.SetRowCount(1);
	UpdateData(FALSE);
}
