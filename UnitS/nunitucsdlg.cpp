// NUnitUCSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NUnitUCSDlg.h"
#include "..\source\GridCtrl\GridCellNumeric.h"
#include "..\source\GridCtrl\GridCellCheck.h"
#include "NGridCellExp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NUnitUCSDlg property page

IMPLEMENT_DYNCREATE(NUnitUCSDlg, CPropertyPage)

NUnitUCSDlg::NUnitUCSDlg(NUCSs& In) : CPropertyPage(NUnitUCSDlg::IDD), UCSs(In)
{
	//{{AFX_DATA_INIT(NUnitUCSDlg)
	//}}AFX_DATA_INIT
	m_DontDrawIfEmpty = TRUE;
	m_DrawName = FALSE;
	UCSChangeDone = false;
}

NUnitUCSDlg::~NUnitUCSDlg()
{
}

void NUnitUCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_VIEW_AXES_AUTO, m_DontDrawIfEmpty);
	DDX_Check(pDX, IDC_VIEW_NAME_UCS, m_DrawName);
	DDX_Check(pDX, IDC_VIEW_AXIS, m_AXIS);
	DDX_Check(pDX, IDC_VIEW_FSYS, m_FSYS);
	DDX_Check(pDX, IDC_VIEW_SSYS, m_SSYS);
	//{{AFX_DATA_MAP(NUnitUCSDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NUnitUCSDlg, CPropertyPage)
	//{{AFX_MSG_MAP(NUnitUCSDlg)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NUnitUCSDlg message handlers

BOOL NUnitUCSDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CRect rect;
	GetDlgItem(IDC_BBOX_UCS)->GetClientRect(&rect);
//	GetClientRect(&rect);
	m_GridCtrl.Create(rect, this, IDD_UNIT_UCS);
	m_GridCtrl.SetDefaultCellType(RUNTIME_CLASS( NGridCellExp ));
	m_GridCtrl.SetDefCellMargin(2);
	m_GridCtrl.SetColumnCount(9);
	int RowsCount = 1 + int(UCSs.GetUsed().size());
	m_GridCtrl.SetRowCount(RowsCount);
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetFixedColumnCount(1);
	m_GridCtrl.SetAutoSizeStyle(GVS_BOTH);
	for (int i = 0; i < RowsCount; ++i)
	{
		m_GridCtrl.SetCellType(i, 0, RUNTIME_CLASS(CGridCell));
	}


	m_GridCtrl.SetCellType(0, 1, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 2, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 3, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 4, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 5, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 6, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 7, RUNTIME_CLASS(CGridCell));
	m_GridCtrl.SetCellType(0, 8, RUNTIME_CLASS(CGridCell));


	for (int i = 1; i < RowsCount; ++i)
	{
		m_GridCtrl.SetCellType(i, 1, RUNTIME_CLASS(CGridCellCheck));
		m_GridCtrl.SetCellType(i, 2, RUNTIME_CLASS(CGridCell));
	}

	CString tmpstr;
	tmpstr.LoadString(IDS_X_DISP);
	m_GridCtrl.SetItemText(0,3,tmpstr);
	tmpstr.LoadString(IDS_Y_DISP);
	m_GridCtrl.SetItemText(0,4,tmpstr);
	tmpstr.LoadString(IDS_Z_DISP);
	m_GridCtrl.SetItemText(0,5,tmpstr);
	tmpstr.LoadString(IDS_A_DISP);
	m_GridCtrl.SetItemText(0,6,tmpstr);
	tmpstr.LoadString(IDS_B_DISP);
	m_GridCtrl.SetItemText(0,7,tmpstr);
	tmpstr.LoadString(IDS_C_DISP);
	m_GridCtrl.SetItemText(0,8,tmpstr);
	tmpstr.LoadString(IDS_VISIBLE_DISP);
	m_GridCtrl.SetItemText(0, 1, tmpstr);
	tmpstr.LoadString(IDS_COLUMN_N);
	m_GridCtrl.SetItemText(0, 2, tmpstr);
	m_GridCtrl.AutoSize();
	m_GridCtrl.SetColumnWidth(1, 21);
	m_GridCtrl.SetColumnWidth(2,60);
	m_GridCtrl.SetColumnWidth(3,60);
	m_GridCtrl.SetColumnWidth(4,60);
	m_GridCtrl.SetColumnWidth(5,60);
	m_GridCtrl.SetColumnWidth(6,60);
	m_GridCtrl.SetColumnWidth(7, 60);
	m_GridCtrl.SetColumnWidth(8, 60);
	for( int c = 1; c < 9; ++c)
		m_GridCtrl.SetItemFgColour(0, c, RGB(0,0,255));
	for( int c = 1; c < RowsCount; ++c)
		m_GridCtrl.SetItemFgColour(c, 0, RGB(0,0,255));
//	m_GridCtrl.EnableSelection(FALSE);
	m_DrawName = UCSs.GetDrawName();
	int i = 0;
	for (auto it = UCSs.GetUsed().begin(); i < RowsCount - 1 && it != UCSs.GetUsed().end(); i++, it++)
	{
		CString sval;
		sval.Format("N %d", *it + 1);
		m_GridCtrl.SetItemText(i + 1, 0, sval);
		((CGridCellCheck *)m_GridCtrl.GetCell(i + 1, 1))->SetCheck(UCSs[*it].Visible);
		sval.Format(IDS_UCS_FORMAT, UCSs[*it].x0);
		m_GridCtrl.SetItemText(i + 1, 3, sval);
		sval.Format(IDS_UCS_FORMAT, UCSs[*it].y0);
		m_GridCtrl.SetItemText(i + 1, 4, sval);
		sval.Format(IDS_UCS_FORMAT, UCSs[*it].z0);
		m_GridCtrl.SetItemText(i + 1, 5, sval);
		sval.Format(IDS_UCS_FORMAT, UCSs[*it].a0);
		m_GridCtrl.SetItemText(i + 1, 6, sval);
		sval.Format(IDS_UCS_FORMAT, UCSs[*it].b0);
		m_GridCtrl.SetItemText(i + 1, 7, sval);
		sval.Format(IDS_UCS_FORMAT, UCSs[*it].c0);
		m_GridCtrl.SetItemText(i + 1, 8, sval);
		m_GridCtrl.SetItemText(i + 1, 2, UCSs[*it].GetName());
	}
	m_GridCtrl.AutoSizeColumn(0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void NUnitUCSDlg::OnOK() 
{
	UCSChangeDone = false;
	int i = 0;
	for (auto it = UCSs.GetUsed().begin(); i < m_GridCtrl.GetRowCount() - 1 && it != UCSs.GetUsed().end(); i++, it++)
	{
		UCSs[*it].Visible = (((CGridCellCheck *)m_GridCtrl.GetCell(i + 1, 1))->GetCheck() == TRUE);
		double b;
		double *pV;
		b = atof(m_GridCtrl.GetItemText(i + 1, 0 + 3));
		pV = &UCSs[*it].x0;
		UCSChangeDone |= (b != *pV);
		*pV = b;

		b = atof(m_GridCtrl.GetItemText(i + 1, 1 + 3));
		pV = &UCSs[*it].y0;
		UCSChangeDone |= (b != *pV);
		*pV = b;

		b = atof(m_GridCtrl.GetItemText(i + 1, 2 + 3));
		pV = &UCSs[*it].z0;
		UCSChangeDone |= (b != *pV);
		*pV = b;

		b = atof(m_GridCtrl.GetItemText(i + 1, 3 + 3));
		pV = &UCSs[*it].a0;
		UCSChangeDone |= (b != *pV);
		*pV = b;

		b = atof(m_GridCtrl.GetItemText(i + 1, 4 + 3));
		pV = &UCSs[*it].b0;
		UCSChangeDone |= (b != *pV);
		*pV = b;

		b = atof(m_GridCtrl.GetItemText(i + 1, 5 + 3));
		pV = &UCSs[*it].c0;
		UCSChangeDone |= (b != *pV);
		*pV = b;
		CString bstr = m_GridCtrl.GetItemText(i + 1, 2);
		if (bstr != UCSs[*it].GetName())
		{
			UCSs[*it].SetName(bstr, false);
			UCSChangeDone = true;
		}
	}
	if (UCSs.GetDrawName() != (m_DrawName == TRUE))
	{
		UCSs.SetDrawName(m_DrawName == TRUE);
		UCSChangeDone = true;
	}

	CPropertyPage::OnOK();
}

