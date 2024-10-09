#include "stdafx.h"
#include "NCMProject.h"
#include "NCenums.h"
#include "PropList.h"
#include "ResLocal.h"
#include "ResLocal.h"
#include "WM_APP_messages.h"
#include "ResLocal.h"
#include "NColor.h"
#include "PropPane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

PropPane::PropPane() : Editor(this)
{
	L1 = new PropList();
	isChanged = 0;
	idChanged = 0;
	ChGroup = 0;
}

PropPane::~PropPane()
{
	L1->Clear();
	delete L1;
}

BEGIN_MESSAGE_MAP(PropPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()

void PropPane::Paste(PropList *pList,PropSingle * Prop, int id)
{
	isChanged = id;
	L1->Clear();
	for (int i = 0; i < pList->GetSize(); ++i)
	{
		L1->Add(pList->Copy(pList->GetAt(i)));
	}
	if (!pList->IsEmpty())
	{
		m_wndPropList.RemoveAll();
		InitPropList();
	}
	else
		m_wndPropList.RemoveAll();
}

void PropPane::Clear()
{
	AdjustLayout();
	m_wndPropList.RemoveAll();
	Editor.Clear();
	m_wndPropList.RedrawWindow();
	UpdateWindow();
}

void PropPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint & N_DELETECONT)
	{
		Editor.pabstractObject = NULL;
	}
	if (lHint & N_SETTINGS)
	{
		SetSettings();
		RecalcLayout();
	}
	if(lHint & N_CADRCHANGED)
	{
		if(IsPaneVisible())
		{
			if(Editor.pabstractObject)
			{
				PAbstractObject *pPObj = GetAbsObject();
				if(pPObj != Editor.pabstractObject)
					Editor.pabstractObject = pPObj;
				Editor.Put(Editor.pabstractObject);
			}
		}
	}
}

void PropPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);


	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
}

int PropPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDS_PROP_EDIT_ID))
	{
		TRACE0("Failed to create Properties Grid \n");    // fail to create
	}
	AdjustLayout();

	m_wndPropList.PrEdit(&Editor);
	return 0;
}

void PropPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void PropPane::InitPropList()
{
	m_wndPropList.SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.RemoveAll();
	ChGroup = 0;
	idChanged = 0;
	int numProp = 0;
	CString GROUP = _T("");
	CString GROUP2= _T("");
	LoadStringID(GROUP, L1->GetAt(0)->Group);
	int group = 0;
	PropGridProp * pGroup1 = new PropGridProp(GROUP);
	for(int h = 0; h<L1->GetSize(); ++h)
	{
		LoadStringID(GROUP2, L1->GetAt(h)->Group);
		if(GROUP != GROUP2)//проверка группы свойств
		{
			m_wndPropList.AddProperty(pGroup1);
			pGroup1 = new PropGridProp(GROUP2);
			GROUP = GROUP2;
			++group;
			numProp = 0;
		}
		if(L1->GetAt(h)->Type == 1)
		{//Combobox
			CString Value;
			LoadStringID(Value, L1->GetAt(h)->ST_Value);
			Value = Value.SpanExcluding(_T("\n"));

			CString COMM = _T("");
			LoadStringID(COMM, L1->GetAt(h)->Comment);
			CString NAME = _T("");
			LoadStringID(NAME, L1->GetAt(h)->Name);
			PropGridProp * pProp = new PropGridProp(NAME, (_variant_t)Value, COMM, L1->GetAt(h)->ID);
			for (int l=0;l!=L1->GetAt(h)->For_Enum.GetSize();l++)
			{
				CString k;
				LoadStringID(k, L1->GetAt(h)->For_Enum.GetAt(l));
				k = k.SpanExcluding(_T("\n"));
				pProp->AddOption(k);
				pProp->AllowEdit(FALSE);
			}
			if (L1->GetAt(h)->Color == 0)
				pProp->Enable(FALSE);

			pGroup1->AddSubItem(pProp);
			if (L1->GetAt(h)->ID == isChanged)
			{
				ChGroup=group;
				if (group!=0)
					idChanged = numProp;
				else 
					idChanged = h;
			}
			numProp = numProp + 1;
		}
		else if (L1->GetAt(h)->Type == 2)
		{//строка
			CString COMM = _T("");
			LoadStringID(COMM, L1->GetAt(h)->Comment);
			CString NAME = _T("");
			LoadStringID(NAME, L1->GetAt(h)->Name);
			CString VALUE=L1->GetAt(h)->Value;
			PropGridProp * pProp = new PropGridProp(NAME, (_variant_t)VALUE , COMM,L1->GetAt(h)->ID);
			if (L1->GetAt(h)->Color == 0)
				pProp->Enable(FALSE);
			pGroup1->AddSubItem(pProp);
			if (L1->GetAt(h)->ID == isChanged)
			{
				ChGroup = group;
				if (group != 0)
					idChanged = numProp;
				else 
					idChanged = h;
			}
			numProp = numProp + 1;
		}
		else if(L1->GetAt(h)->Type == 3)
		{//Color
			CString COMM = _T("");
			LoadStringID(COMM, L1->GetAt(h)->Comment);
			CString NAME = _T("");
			LoadStringID(NAME, L1->GetAt(h)->Name);
			int color = int(L1->GetAt(h)->V_Value);
			CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(NAME, color, NULL, COMM, L1->GetAt(h)->ID);
				pColorProp->EnableOtherButton(_T("Other..."));
				pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
				if (L1->GetAt(h)->Color == 0)
					pColorProp->Enable(FALSE);
				pGroup1->AddSubItem(pColorProp);
			if(L1->GetAt(h)->ID == isChanged)
			{
				ChGroup = group;
				if (group != 0)
					idChanged = numProp;
				else 
					idChanged = h;
			}
			numProp = numProp + 1;
		}
		else if(L1->GetAt(h)->Type == 4)
		{//вещесвенные
			CString COMM = _T("");
			LoadStringID(COMM, L1->GetAt(h)->Comment);
			CString NAME = _T("");
			LoadStringID(NAME, L1->GetAt(h)->Name);
			double MIN = L1->GetAt(h)->Min;
			double MAX = L1->GetAt(h)->Max;
				double VALUE = L1->GetAt(h)->V_Value;
				PropGridProp * pProp = new PropGridProp(NAME, (_variant_t)VALUE, COMM,L1->GetAt(h)->ID);
				if (L1->GetAt(h)->Color == 0)
					pProp->Enable(FALSE);
				pGroup1->AddSubItem(pProp);	
			if (L1->GetAt(h)->ID == isChanged)
			{
				ChGroup=group;
				if (group != 0)
					idChanged = numProp;
				else 
					idChanged = h;
			}
			numProp = numProp + 1;
		}
		else if (L1->GetAt(h)->Type == 5)
		{//целые 
			CString COMM = _T("");
			LoadStringID(COMM, L1->GetAt(h)->Comment);
			CString NAME = _T("");
			LoadStringID(NAME, L1->GetAt(h)->Name);
			double MIN = L1->GetAt(h)->Min;
			double MAX = L1->GetAt(h)->Max;
			int VALUE = (int)L1->GetAt(h)->V_Value;
			CString u;
			u.Format("%d",VALUE);
			PropGridProp* pProp = new PropGridProp(NAME, (_variant_t)u, COMM,L1->GetAt(h)->ID);
			if (L1->GetAt(h)->Color == 0)
					pProp->Enable(FALSE);
			pGroup1->AddSubItem(pProp);	
			int gg = L1->GetAt(h)->ID;
			if (L1->GetAt(h)->ID == isChanged)
			{
				ChGroup = group;
				if(group != 0)
					idChanged = numProp;
				else 
					idChanged = h;
			}
			numProp = numProp + 1;
		}
		else if(L1->GetAt(h)->Type == 6)
		{//¬рем€
			CString COMM = _T("");
			LoadStringID(COMM, L1->GetAt(h)->Comment);
			CString NAME = _T("");
			LoadStringID(NAME, L1->GetAt(h)->Name);
			CString t = GetTimeFromDouble(L1->GetAt(h)->Time);		
			PropGridProp* pProp = new PropGridProp(NAME, (_variant_t)t, COMM,L1->GetAt(h)->ID);
			if (L1->GetAt(h)->Color == 0)
				pProp->Enable(FALSE);
			pGroup1->AddSubItem(pProp);
			if(L1->GetAt(h)->ID == isChanged)
			{
				ChGroup = group;
				if(group !=0)
					idChanged = numProp;
				else 
					idChanged = h;
			}
			numProp = numProp + 1;
		}			
	}
	m_wndPropList.AddProperty(pGroup1);
	if(isChanged != 0)
	{
		m_wndPropList.SetCurSel(m_wndPropList.GetProperty(ChGroup)->GetSubItem(idChanged), TRUE);
//		((PropGridProp *)(m_wndPropList.GetProperty(ChGroup)->GetSubItem(idChanged)))->SetModifiedFlag();
	}
}

CString PropPane::GetTimeFromDouble(double Val)
{
	CTimeSpan t((unsigned long)Val);
	CString s(t.Format(":%M:%S"));
	CString h;
	h.Format("%d",t.GetTotalHours());
	return h + s;
}
void PropPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}

void PropPane::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	m_wndPropList.SetPropListFont();
}

void PropPane::SetSettings()
{
	NColor* pColBk = nullptr;
	NColor* pCol = nullptr;
	if (NCM_PROJECT.Colors.Lookup("‘онѕанели—войств", pColBk) && NCM_PROJECT.Colors.Lookup("“екстѕанели—войств", pCol))
	{
		COLORREF Buf[7];
		m_wndPropList.GetCustomColors(Buf[0], Buf[1], Buf[2], Buf[3], Buf[4], Buf[5], Buf[6]);
		m_wndPropList.SetCustomColors(pColBk->GetRGB(), pCol->GetRGB(), pColBk->GetRGB(), pCol->GetRGB(), pColBk->GetRGB(), pCol->GetRGB(), Buf[6]);
	}

	m_wndPropList.SetPropListFont();
}

void PropPane::LoadStringID(CString &Str, int ID)
{
	Str.LoadString(ID);
}

PAbstractObject * PropPane::GetAbsObject(void)
{
	CWnd *pMainWnd = AfxGetMainWnd();
	if(pMainWnd)
		return (PAbstractObject *)pMainWnd->SendMessage(WM_APP_GET_ABS_OBJ);
	else
		return NULL;
}

void PropPane::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if(bShow)
		Editor.Put(GetAbsObject());
}


int PropPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	Editor.Put(GetAbsObject());
	return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
