// NToolsConCrPage.cpp : implementation file
//
#include "stdafx.h"
#include "NCMVersion.h"
#include "NToolsConCrPage.h"
#include "NToolsCrWeldSheet.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTurnHoldSheet.h"
#include "NToolsCrTurnHoldSTLDlg.h"
#include "NToolsCrTBOut.h"
#include "NToolsCrTBCirc.h"
#include "NToolsCrTHBar.h"

#include "NToolTurnBladeMill.h"
#include "NToolsCrDial.h"
#include "NToolsCrDiskSheet.h"
#include "NMendrelDial.h"
#include "NToolID.h"
#include "ResLocal.h"

#include "NToolsCrWeldPage.h"
#include "NToolCombined.h"
#include "ntoolsconcrpage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

// NToolsConCrPage dialog

IMPLEMENT_DYNAMIC(NToolsConCrPage, NToolsCrCommonPage)

NToolsConCrPage::~NToolsConCrPage()
{
	delete m_pToolProduced;
}

NToolsConCrPage::NToolsConCrPage(UINT nIDTemplate)
	: NToolsCrCommonPage(nIDTemplate)
{
	//{{AFX_DATA_INIT(NToolsConCrPage)
	m_name = _T("Tool ");
	m_pos = 1;
	//}}AFX_DATA_INIT
	m_PointSize = 5;
	double *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("ToolMarker", pDouble))
		m_PointSize = int(*pDouble);
	m_IsNameAuto = true;
	m_bMoreFunction = false;
	m_pToolProduced = NULL;
	m_num_rem = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lComToolID = -1;
	m_aSimToolID.RemoveAll();
	m_aSimToolType.RemoveAll();
	m_LifeTime = 0.;
}

void NToolsConCrPage::DoDataExchange(CDataExchange* pDX)
{
	NToolsCrCommonPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NToolsConCrPage)
	DDX_Control(pDX, IDC_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_POS, m_pos_ctrl);
	DDX_Control(pDX, IDC_POS_TURRET, m_pos_turret_ctrl);
	DDX_Control(pDX, IDC_TOOL_LIST, m_ToolList);
	DDX_Control(pDX, IDC_EDIT_TOOL, m_editool_ctrl);
	DDX_Control(pDX, IDC_DELETE_TOOL, m_cltool_ctrl);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_POINT_SIZE, m_PointSize);
	DDV_MinMaxInt(pDX, m_PointSize, 0, 99);
	DDX_Text(pDX, IDC_POS, m_pos);
	DDX_Text(pDX, IDC_POS_TURRET, m_pos_turret);
	DDV_MinMaxUInt(pDX, m_pos, 1, 10000);
	DDV_MinMaxUInt(pDX, m_pos_turret, 0, 500);
	DDX_Text(pDX, IDC_COMBO1, m_pos_attach);
	DDV_MinMaxUInt(pDX, m_pos_attach, 0, 100);
	DDX_Check(pDX, IDC_AUTOLENGTH, m_pos_cshift_auto);
	DDX_Check(pDX, IDC_IGNOREUPTOOLSTOCKCOLLISION, m_IgnoreUpToolStockColl);
	DDX_Text(pDX, IDC_TOOLLENGTH_X, m_pos_cshift_x);
	DDX_Text(pDX, IDC_TOOLLENGTH_Y, m_pos_cshift_y);
	DDX_Text(pDX, IDC_TOOLLENGTH_Z, m_pos_cshift_z);
	DDX_Text(pDX, IDC_LIFETIME, m_LifeTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(NToolsConCrPage, NToolsCrCommonPage)
	//{{AFX_MSG_MAP(NToolsConCrPage)
	ON_BN_CLICKED(IDC_DELETE_TOOL, OnDeleteTool)
	ON_NOTIFY(NM_DBLCLK, IDC_TOOL_LIST, OnDblclkToolList)
	ON_BN_CLICKED(IDC_EDIT_TOOL, OnEditTool)
	ON_EN_KILLFOCUS(IDC_NAME, OnKillfocusName)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
//	ON_BN_CLICKED(IDC_MORE_FUNCTION, OnMoreFunction)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_BN_CLICKED(IDC_AUTOLENGTH, &NToolsConCrPage::OnBnClickedAutolength)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NToolsConCrPage message handlers

BOOL NToolsConCrPage::OnSetActive()
{
	if (!m_pToolProduced)
		m_pToolProduced = CreateNewToolCom(m_name);

	CPropertySheet* pParent = static_cast<CPropertySheet*>(GetParent());
	NToolsConCrPage* pPrevPage = static_cast<NToolsConCrPage*>(pParent->GetActivePage());
	if(pPrevPage != nullptr && pPrevPage != this)
		pPrevPage->ClearNewTool();

	return NToolsCrCommonPage::OnSetActive();
}

BOOL NToolsConCrPage::OnInitDialog()
{
	NToolsCrCommonPage::OnInitDialog();

	if (!m_pToolProduced)
		m_pToolProduced = CreateNewToolCom(m_name);

	CString key;
	CString skey,sval;

	CString tmpstr;
	tmpstr.LoadString(IDS_TNAME);
	m_ToolList.InsertColumn(0,tmpstr,LVCFMT_LEFT,130,1);
	tmpstr.LoadString(IDS_TTIPE);
	m_ToolList.InsertColumn(1,tmpstr,LVCFMT_LEFT,130,0);
	tmpstr.LoadString(IDS_TDISP);
	m_ToolList.InsertColumn(2,tmpstr,LVCFMT_LEFT,70,1);

	// Create the Image List
	m_ImageListBmp.LoadBitmap(IDB_IMAGELIST);
	if(HIMAGELIST(m_ImageList) == NULL)
		m_ImageList.Create(LIST_VIEW_ICON_WIDTH, LIST_VIEW_ICON_HEIGHT, ILC_COLORDDB | ILC_MASK, 16, 4);
	m_ImageList.Add(&m_ImageListBmp, RGB(255,255,255));
	/// Attach image list to Tree
	m_ToolList.SetImageList(&m_ImageList, LVSIL_SMALL);
	///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	// для выделения всей строки
	DWORD ExStyle = m_ToolList.GetExtendedStyle();
	ExStyle |= LVS_EX_FULLROWSELECT;
	m_ToolList.SetExtendedStyle(ExStyle);

	// задание номера столбца, который можно изменять
	m_ToolList.AddChangingColumn(2);
	m_ToolList.SetRectStyle(TRUE);
	
	UpdateData(FALSE);

	NUpdateControls();
	NUpdate();
	SetModified(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// задание списка инструментов
void NToolsConCrPage::SetToolsList(NMapStringToOb *plist)
{
	m_ptool_list = plist;
}

// задание составного инструмента
void NToolsConCrPage::SetToolCombined(const NToolCombined *pTool)
{
	m_pToolProduced = pTool->CreateCopyTool();
	m_pToolProduced->SetChildrenDispl();
}

// редактирование созданного инструмента
void NToolsConCrPage::EditTool()
{
	int SelCount = m_ToolList.GetSelectedCount();

	if( SelCount == LB_ERR || SelCount == 0)
		return;

	POSITION pos = m_ToolList.GetFirstSelectedItemPosition();
	int pFirstSelected = m_ToolList.GetNextSelectedItem(pos);
 	int index = int(m_ToolList.GetItemData(pFirstSelected));
	int ToolType = m_pToolProduced->GetTool(index)->GetType();
	int BToolType = ToolType;
 	NTool *pTool = NULL;

	if( START_TURNBLADEMILL + 1 <= ToolType && ToolType <= END_TURNBLADEMILL)// TurnMill tool
	{
		ToolType -= START_TURNBLADEMILL;
		NTool *pTurnMillTool = m_pToolProduced->GetTool(index);
		NTool *pMillTool = ((NToolTurnBladeMill *)pTurnMillTool)->GetMillTool()->CreateCopyTool();
		m_pToolProduced->RemoveTool(index);
		index = m_pToolProduced->AddTool(pMillTool) - 1;
	}
	if( 11 <= ToolType && ToolType <= 14)
	{
		NToolsCrDial CreateDial;
		m_pToolProduced->GetTool(index)->FillDialog(&CreateDial);
 		CreateDial.SetOtherTool(true);
		if( CreateDial.DoModal() != IDOK ) 
			return;
		pTool = CreateDial.GetNewTool();
 		if ( pTool == NULL )
 			return;
	 	pTool->SetColor(m_pToolProduced->GetTool(index)->GetColor());
		pTool->SetColorPassive(m_pToolProduced->GetTool(index)->GetColorPassive());
 		pTool->Visible = m_pToolProduced->GetTool(index)->Visible;
	}
	if( ((20 <= ToolType && ToolType <= 24) || ToolType == 10))
	{
		NToolsCrDiskSheet DiskSheet(IDS_DISK_TOOL);
		m_pToolProduced->GetTool(index)->FillSheet(&DiskSheet);
 		if( DiskSheet.DoModal() != IDOK ) 
			return;
 		pTool=DiskSheet.GetNewTool();
 		if ( pTool == NULL )
 			return;
 		pTool->SetColor(m_pToolProduced->GetTool(index)->GetColor());
		pTool->SetColorPassive(m_pToolProduced->GetTool(index)->GetColorPassive());
 		pTool->Visible = m_pToolProduced->GetTool(index)->Visible;
	}
	if(40 <= ToolType && ToolType <= 43)
	{
		NMendrelDial CreateDial;
		m_pToolProduced->GetTool(index)->FillDialog(&CreateDial);
		if( CreateDial.DoModal() != IDOK ) 
			return;
 	 	pTool=CreateDial.GetNewTool();
 		if ( pTool == NULL )
 			return;
	 	pTool->SetColor(m_pToolProduced->GetTool(index)->GetColor());
		pTool->SetColorPassive(m_pToolProduced->GetTool(index)->GetColorPassive());
 		pTool->Visible = m_pToolProduced->GetTool(index)->Visible;
	}
	if( 50 <= ToolType && ToolType < 70)
	{// TurnTool
		NToolsCrTurn *pPage = NULL;
		switch(ToolType)
		{
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
//		case 56:
			{
			NToolsCrTurnSheet EditSheet(this);
			pPage = (NToolsCrTurn *)EditSheet.GetPage(ToolType - 50);
//			ASSERT(pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBOut)));
			m_pToolProduced->GetTool(index)->FillPage(pPage);
			EditSheet.SetActivePage(pPage);
 			if( EditSheet.DoModal() == IDOK ) 
				pTool = EditSheet.GetNewTool();
			}
			break;
		case 60:
		case 61:
		case 63:
			{
			NToolsCrTurnHoldSheet EditSheet(this);
			pPage = (NToolsCrTurn *)EditSheet.GetPage(ToolType - 60);
//			ASSERT(pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTHBar)));
			m_pToolProduced->GetTool(index)->FillPage(pPage);
			EditSheet.SetActivePage(pPage);
 			if( EditSheet.DoModal() == IDOK ) 
				pTool = EditSheet.GetNewTool();
			}
			break;
		case 62:
			{
			NToolsCrTurnHoldSTLDlg EditSheet(this);
			m_pToolProduced->GetTool(index)->FillPage(&EditSheet);
 			if( EditSheet.DoModal() == IDOK ) 
				pTool = EditSheet.GetNewTool();
			}
			break;
		default:
			return;
		}
 		if ( pTool == NULL )
 			return;
 		pTool->SetColor(m_pToolProduced->GetTool(index)->GetColor());
		pTool->SetColorPassive(m_pToolProduced->GetTool(index)->GetColorPassive());
 		pTool->Visible = m_pToolProduced->GetTool(index)->Visible;
 		pTool->SetZdisp(m_pToolProduced->GetTool(index)->GetZdisp());
 	}
	if( START_TURNBLADEMILL + 1 <= BToolType && BToolType <= END_TURNBLADEMILL)// TurnMill tool
	{
		NToolTurnBladeMill *pBTool = new NToolTurnBladeMill;
		pBTool->SetMillTool((NToolMill *)pTool->CreateCopyTool());
		pTool = pBTool;
	}
	if( 70 <= BToolType && BToolType < 80)// Concave tool
	{
		NToolsCrDiskSheet DiskSheet(IDS_BROUND);
		m_pToolProduced->GetTool(index)->FillSheet(&DiskSheet);
 		if( DiskSheet.DoModal() != IDOK ) 
			return;
 		pTool = DiskSheet.GetNewTool();
 		if ( pTool == NULL )
 			return;
 		pTool->SetColor(m_pToolProduced->GetTool(index)->GetColor());
		pTool->SetColorPassive(m_pToolProduced->GetTool(index)->GetColorPassive());
 		pTool->Visible = m_pToolProduced->GetTool(index)->Visible;
	}
	if( 80 <= BToolType && BToolType < 90)// Weld tool
	{
		NToolsCrWeldSheet EditSheet(this);
		NToolsCrWeldPage *pPage = (NToolsCrWeldPage *)EditSheet.GetPage(ToolType - 80);
		ASSERT(pPage->IsKindOf(RUNTIME_CLASS(NToolsCrWeldPage)));
		m_pToolProduced->GetTool(index)->FillPage(pPage);
		EditSheet.SetActivePage(pPage);
		if( EditSheet.DoModal() == IDOK ) 
			pTool = EditSheet.GetNewTool();
		if(!pTool)
			return;
	}
	m_pToolProduced->AddTool(pTool);
	m_pToolProduced->RemoveTool(index);

	NUpdate();
}

// изменение оправки или инструмента
void NToolsConCrPage::OnEditTool() 
{
	EditTool();
}

// двойное нажатие эквивалентно изменению инструмента
void NToolsConCrPage::OnDblclkToolList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EditTool();
	
	*pResult = 0;
}

// удаление инструмента или оправки
void NToolsConCrPage::OnDeleteTool() 
{
	int SelCount = m_ToolList.GetSelectedCount();

	if( SelCount == LB_ERR || SelCount == 0)
		return;

	int * pIndexArray = new int [SelCount];
	POSITION pos = m_ToolList.GetFirstSelectedItemPosition();
	for(int i = 0; pos != NULL; i++)
		pIndexArray[i] = int(m_ToolList.GetItemData(m_ToolList.GetNextSelectedItem(pos)));
	for(int i = SelCount - 1; i >= 0; i--)
		m_pToolProduced->RemoveTool(pIndexArray[i]);

	delete[] pIndexArray;

	NUpdate();
}

// обновление списка инструментов
void NToolsConCrPage::NUpdate()
{
	CString str;
// Update values
	m_ToolList.DeleteAllItems();
	
	int num_tools = m_pToolProduced->num_tools;
	double *Zdisp_array = new double[num_tools];
	double *Zdisp_vspom = new double[num_tools];

	for (int i=0; i<num_tools; i++)
		Zdisp_array[i] = Zdisp_vspom[i] = m_pToolProduced->GetTool(i)->GetZdisp();

	qsort((void *)Zdisp_array, num_tools, sizeof(double), compare_double);

	for (int i = 0; i < num_tools; i++)
	{
		int index = 0;
		for (int j = 0; j < num_tools; j++)
			if (Zdisp_array[i] == Zdisp_vspom[j] &&
				Zdisp_vspom[j] != -10000.)
			{
				index = j;
				Zdisp_vspom[j] = -10000.;
				j = num_tools;
			}
		int item_index = 0;
		str.Format("%i",index);
		int tool_type = m_pToolProduced->GetTool(index)->GetType();
		if (tool_type == 10 || tool_type == 11)
			item_index = 3;
		else if(tool_type == 12)
			item_index = 4;
		else if(tool_type == 13)
			item_index = 5;
		else if(tool_type == 14)
			item_index = 6;
		else if(tool_type == 21)
			item_index = 7;
		else if(tool_type == 22)
			item_index = 8;
		else if(tool_type == 23)
			item_index = 9;
		else if(tool_type == 20 || tool_type == 24)
			item_index = 10;
		else if(tool_type == 40 || tool_type == 41)
			item_index = 11;
		else if(tool_type == 42)
			item_index = 12;
		else if(tool_type == 43)
			item_index = 13;
		else if(tool_type == 50 || tool_type == 51 || tool_type == 52 || tool_type == 53 || tool_type == 54 ||  tool_type == 55)
			item_index = 14;
		else if(tool_type == 60 || tool_type == 61 || tool_type == 62 || tool_type == 63)
			item_index = 15;
		else if (tool_type == 71)
			item_index = 4;
		else if (tool_type == 72)
			item_index = 16; // vert tool
		else if(tool_type == 80)
			item_index = 4;

		m_ToolList.InsertItem(i, str, item_index);
		m_ToolList.SetItemData(i, index);

		m_ToolList.SetItemText(i, 0, m_pToolProduced->GetTool(index)->GetName());
		if(m_IsNameAuto)
		{
			m_name_ctrl.SetSel(0, -1);
			str = m_pToolProduced->GetTool(index)->GetName();
			m_name_ctrl.ReplaceSel(str);
			m_pToolProduced->SetName(str);
		}
		switch(tool_type)
		{
		case 10: case 11: case 12: case 13: case 14:
		case 110: case 111: case 112: case 113: case 114:
			str.LoadString(IDS_TSIMP);
			break;
		case 20: case 21: case 22: case 23: case 24:
			str.LoadString(IDS_TDISK);
			break;
		case 40: case 41: case 42: case 43:
			str.LoadString(IDS_THOLD);
			break;
		case 50: case 51: case 52: case 53: case 54: case 55: case 56:
			str.LoadString(IDS_TBLADE);
			break;
		case 60: case 61: case 62: case 63:
			str.LoadString(IDS_TLHOLD);
			break;
		case 71:
			str.LoadString(IDS_BROUND);
			break;
		case 70:
			str.LoadString(IDS_BROUND);
			break;
		case 72:
			str.LoadString(IDS_TBORE);// vert tool
			break;
		case 80:
			str.LoadString(IDS_BEAM);
			break;
		default:
			str.Empty();
		}
		m_ToolList.SetItemText(i, 1, str);
		str.Format("%lf",Zdisp_array[i] - m_pToolProduced->Zdisp);
		m_ToolList.SetItemText(i, 2, str);
	}

	delete []Zdisp_array;
	delete []Zdisp_vspom;



	m_editool_ctrl.EnableWindow((num_tools > 0) ? TRUE : FALSE);
	m_cltool_ctrl.EnableWindow((num_tools > 0) ? TRUE : FALSE);

	//CWnd * pButton;
	//pButton = GetParent()->GetDlgItem(IDOK);
	//pButton->EnableWindow(num_tools > 0);

//	m_pos = 1;
	m_pos_ctrl.EnableWindow(TRUE);

	// поля для редактирования
	m_ToolList.InsertEditLAbels();

	//
	if(m_pos_cshift_auto)
	{
		m_pos_cshift_x = 0.;
		m_pos_cshift_y = 0.;
		m_pos_cshift_z = -m_pToolProduced->GetGabar().dZ();
		UpdateData(FALSE); 
	}
	if(m_IsNameAuto)
	{
		NTool *pTool = NULL;
		if(m_pToolProduced->num_tools > 0)
		{
			pTool = m_pToolProduced->GetTool(0);
			for(int i = 1; i < m_pToolProduced->num_tools; ++i)
			{
				NTool *pTTool = m_pToolProduced->GetTool(i);
				if(pTTool->GetZdisp() < pTool->GetZdisp())
					pTool = pTTool;
			}
		}
		if (pTool)
		{
			m_name = pTool->GetName();
			UpdateData(FALSE);
		}
	}
}

// определение созданного инструмента
NToolCombined * NToolsConCrPage::GetNewTool()
{
	return m_pToolProduced;
}

void NToolsConCrPage::OnKillfocusName() 
{
	CString NewName;
	m_name_ctrl.GetWindowText(NewName);
	if(NewName.IsEmpty())
		m_IsNameAuto = true;	
	else
		m_IsNameAuto = false;
}

void NToolsConCrPage::OnOK()
{
	m_name_ctrl.GetWindowText(m_name);
	m_pToolProduced->IsNameAuto = m_IsNameAuto;
	m_pToolProduced->SetName(m_name);
	m_pToolProduced->PointSize = m_PointSize;
	m_pToolProduced->Position = m_pos;
	m_pToolProduced->Turret = m_pos_turret;
	m_pToolProduced->Attach = m_pos_attach;
	m_pToolProduced->CompShift.Set(m_pos_cshift_x, m_pos_cshift_y, m_pos_cshift_z, 0.);
	m_pToolProduced->CompShiftAuto = (m_pos_cshift_auto == TRUE);
	m_pToolProduced->LifeTime = m_LifeTime;
	m_pToolProduced->IgnoreUpToolStockColl = (m_IgnoreUpToolStockColl == TRUE);

	NToolsCrCommonPage::OnOK();

	AfxGetMainWnd()->SendMessage(WM_APP_MAKE_TOOL);
}

void NToolsConCrPage::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// определение размеров диалога и видимых контролов
void NToolsConCrPage::NUpdateControls()
{
	CRect WndRect, NewRect;
// размеры формы
	GetWindowRect(&WndRect);
	NewRect.left = WndRect.left;
	NewRect.top = WndRect.top;
	NewRect.right = WndRect.right;
		
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_TOOLLENGTH_X);
	pWnd->EnableWindow(!m_pos_cshift_auto);
	pWnd = GetDlgItem(IDC_TOOLLENGTH_Y);
	pWnd->EnableWindow(!m_pos_cshift_auto);
	pWnd = GetDlgItem(IDC_TOOLLENGTH_Z);
	pWnd->EnableWindow(!m_pos_cshift_auto);
}

void NToolsConCrPage::PrepNew(NMapStringToOb * pTList)
{
	SetToolsList(pTList);
	// Find new position
	m_pos = 0;
	m_pos_turret = 0;
	m_pos_attach = 0;
	m_pos_cshift_x = 0.;
	m_pos_cshift_y = 0.;
	m_pos_cshift_z = 0.;
	m_pos_cshift_auto = FALSE;
	m_IgnoreUpToolStockColl = FALSE;
	if(pTList != NULL)
	{
		for each (auto el in *pTList)
		{
			NToolCombined* pToolCon = static_cast<NToolCombined*>(el.second);
			NToolID ToolID = el.first;
			int tool_pos = ToolID.GetToolNum();
			if(int(m_pos) < tool_pos)
				m_pos = tool_pos;
		}
	}
	++m_pos;
}

bool NToolsConCrPage::CheckActive(void)
{
	if(((CPropertySheet *)GetParent())->GetActivePage() != this)
	{
		//delete m_pToolProduced;
		//m_pToolProduced = NULL;
		return false;
	}

	if(!UpdateData())
		return false;
	return true;
}

void NToolsConCrPage::ClearNewTool()
{
	delete m_pToolProduced;
	m_pToolProduced = nullptr;
}

void NToolsConCrPage::OnBnClickedAutolength()
{
	UpdateData(TRUE);
	if(m_pos_cshift_auto)
	{
		m_pos_cshift_x = 0.;
		m_pos_cshift_y = 0.;
		m_pos_cshift_z = -m_pToolProduced->GetGabar().dZ();
	}
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_TOOLLENGTH_X);
	pWnd->EnableWindow(!m_pos_cshift_auto);
	pWnd = GetDlgItem(IDC_TOOLLENGTH_Y);
	pWnd->EnableWindow(!m_pos_cshift_auto);
	pWnd = GetDlgItem(IDC_TOOLLENGTH_Z);
	pWnd->EnableWindow(!m_pos_cshift_auto);
	UpdateData(FALSE);
}


void NToolsConCrPage::OnCancel()
{
	ClearNewTool();
	NToolsCrCommonPage::OnCancel();
}

BOOL NToolsConCrPage::OnApply()
{
	auto Res = NToolsCrCommonPage::OnApply();
	CheckActive();
	UpdateData(FALSE);// MainAngle can be changed
	return Res;
}

