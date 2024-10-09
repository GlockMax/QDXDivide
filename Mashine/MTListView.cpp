// MTListView.cpp : implementation file
//

#include "stdafx.h"
#include "MachineTool.h"
#include "MTListView.h"
#include "MachineToolDoc.h"
#include "UserMessages.h"
//#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MTListView

IMPLEMENT_DYNCREATE(MTListView, CListView)

MTListView::MTListView()
{
	Edit=NULL;
	RegisterWindowMessage("UM_ENDWORK");
}

MTListView::~MTListView()
{
}


BEGIN_MESSAGE_MAP(MTListView, CListView)
	//{{AFX_MSG_MAP(MTListView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(UM_ENDWORK,OnItemChenged)
//	ON_MESSAGE(UM_SELCTITEMINVIEW,OnItemSelectedInView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MTListView drawing

void MTListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// MTListView diagnostics

#ifdef _DEBUG
void MTListView::AssertValid() const
{
	CListView::AssertValid();
}

void MTListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MTListView message handlers

BOOL MTListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_dwDefaultStyle|=WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT |LVS_SHOWSELALWAYS | LVS_SINGLESEL;	
	return CListView::PreCreateWindow(cs);
}

int MTListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);	
	CListCtrl &m_List=GetListCtrl();
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.SetBkColor(RGB(0,0,0));
	m_List.SetTextBkColor(RGB(0,0,0));
	m_List.SetTextColor(RGB(255,255,255));
	m_List.InsertColumn(0,"Ось",LVCFMT_LEFT,50);
	m_List.InsertColumn(1,"Старт",LVCFMT_LEFT,80);
	m_List.InsertColumn(2,"Текущая",LVCFMT_LEFT,80);
	m_List.InsertColumn(3,"Финиш",LVCFMT_LEFT,80);

	return 0;
}



void MTListView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//AfxMessageBox("ok");
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	if(pDoc->Mooving)
	{
		CListView::OnLButtonDblClk(nFlags, point);
		return;
	}
	if(Edit)
	{
		delete Edit;
		Edit=NULL;
	}
	CListCtrl &m_List=GetListCtrl();
	int i=-1,j=-1,k,h;
	LVHITTESTINFO lvhti;
	lvhti.pt=point;
	m_List.SubItemHitTest(&lvhti);
	i=lvhti.iItem;
	j=lvhti.iSubItem;
	if(i==-1)
	{
		CListView::OnLButtonDblClk(nFlags, point);
		return;
	}
	if(j<=0)
	{
		CListView::OnLButtonDblClk(nFlags, point);
		return;
	}
	CPoint p,p2;
	m_List.GetItemPosition(i,&p);
	for(k=0;k<j;k++)
	{
		p.x+=m_List.GetColumnWidth(k);
	}
	p2.x=p.x+m_List.GetColumnWidth(j);
	CRect rect;
	m_List.GetItemRect(i,rect,LVIR_BOUNDS);
	h=rect.Height();
	p2.y=p.y+h;
	rect.SetRect(p.x,p.y,p2.x,p2.y);
	Edit=new ListEditBox;
	Edit->ParentWnd=m_hWnd;
	Edit->Create(ES_WANTRETURN   | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,rect,this,1);
	Edit->ParentWnd=m_hWnd;
	Edit->SetFocus();
	CFont *f=GetFont();
	Edit->SetFont(f);
	CString str;
	str=m_List.GetItemText(i,j);
	Edit->SetWindowText(str);
	Edit->Item=i;
	Edit->SubItem=j;
	SelectedItem=m_List.HitTest(point);
	//AfxMessageBox("ok1");
	CListView::OnLButtonDblClk(nFlags, point);
}

void MTListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	CListCtrl &m_List=GetListCtrl();
	int i;
	m_List.DeleteAllItems();
	CString str;
	int s1,s2,s3;
	s1=pDoc->MTMachineTool.Start.GetSize();
	s2=pDoc->MTMachineTool.Now.GetSize();
	s3=pDoc->MTMachineTool.End.GetSize();
	if(s1!=s2 ||s2!=s3||s1!=s3)
	{
		AfxMessageBox("ERROR");
	}
	for(i=0;i<pDoc->MTMachineTool.Start.GetSize();i++)
	{
		str="";
		m_List.InsertItem(i,pDoc->MTMachineTool.Start[i].AxisName);
		str.Format("%f",pDoc->MTMachineTool.Start[i].Value);
		m_List.SetItemText(i,1,str);
		str.Format("%f",pDoc->MTMachineTool.Now[i].Value);
		m_List.SetItemText(i,2,str);
		str.Format("%f",pDoc->MTMachineTool.End[i].Value);
		m_List.SetItemText(i,3,str);
	}
}
void MTListView::OnItemChenged()
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	if(pDoc->Mooving)
		return;
	if(!Edit)
		return;
	CListCtrl &m_List=GetListCtrl();
	m_List.SetItemText(Edit->Item,Edit->SubItem,Edit->Text);
	int i;
	i=0;
	CString a1,a2;
	pDoc->MTMachineTool.GoToNullPosition();
	int j;
	while(i<pDoc->MTMachineTool.Start.GetSize())
	{
		for(j=0;j<pDoc->MTMachineTool.Start.GetSize();j++)
		{
			a1=m_List.GetItemText(i,0);
			a2=pDoc->MTMachineTool.Start[j].AxisName;
			if(a1==a2)
			{
				double v;
				v=atof(m_List.GetItemText(i,1));
				pDoc->MTMachineTool.Start[j].Value=v;
				v=atof(m_List.GetItemText(i,2));
				pDoc->MTMachineTool.Now[j].Value=v;
				v=atof(m_List.GetItemText(i,3));
				pDoc->MTMachineTool.End[j].Value=v;
			}
		}
		i++;
	}
	for(i=0;i<pDoc->MTMachineTool.Now.GetSize();i++)
		pDoc->MTMachineTool.Axis.MoveAxis(pDoc->MTMachineTool.Now[i].AxisName,pDoc->MTMachineTool.Now[i].Value);
	pDoc->UpdateAllViews(NULL,UPDATEOGLVIEW);
}

void MTListView::OnItemSelectedInView()
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	CListCtrl &m_List=GetListCtrl();
	/*LVFINDINFO lf;
	lf.psz=pDoc->SelectedItem;*/
	int i;
//	i=m_List->FindItem(&lf);
	for(i=0;i<pDoc->MTMachineTool.Start.GetSize();i++)
	{
		if(pDoc->MTMachineTool.Start[i].AxisName==pDoc->SelectedItem)
		{
			m_List.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			break;
		}
	}
	
}

void MTListView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl &m_List=GetListCtrl();
	int ni=m_List.HitTest(point);
	if(ni==SelectedItem || ni<0)
	{
		CListView::OnLButtonDown(nFlags, point);
		return;
	}
	SelectedItem=ni;
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
//	ASSERT(m_List);
//	int i;
	pDoc->SelectedItem=pDoc->MTMachineTool.Start[SelectedItem].AxisName;
	::SendMessage(m_MTreeViewWnd,UM_SELCTITEMINVIEW,0,0);
	CListView::OnLButtonDown(nFlags, point);
}

LRESULT MTListView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case (UM_ENDWORK):
		{
			OnItemChenged();
			return 0;
		}
	case (UM_SELCTITEMINVIEW):
		{
			OnItemSelectedInView();
			return 0;
		}
	}
	return CListView::WindowProc(message, wParam, lParam);
}
