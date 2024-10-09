// MTTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "MachineTool.h"
#include "MTTreeView.h"
#include "MachineToolDoc.h"
#include "DlgAddAxis.h"
#include "MTForwardAxis.h"
#include "MTRotaryAxis.h"
#include "DlgAddPart.h"
#include "windowsx.h"
#include "DlgAxisEditor.h"
#include "UserEnum.h"
#include "UserMessages.h"
#include "DlgEditPart.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MTTreeView

#define TREE_ICON_HEIGHT 18
#define TREE_ICON_WIDTH 21


IMPLEMENT_DYNCREATE(MTTreeView, CTreeView)

MTTreeView::MTTreeView()
{
//	m_dwDefaultStyle= TVS_HASLINES |TVS_LINESATROOT;
}

MTTreeView::~MTTreeView()
{
}


BEGIN_MESSAGE_MAP(MTTreeView, CTreeView)
	//{{AFX_MSG_MAP(MTTreeView)
	ON_WM_CREATE()
	ON_COMMAND(ID_MODEL_ADDAXIS, OnModelAddaxis)
	ON_COMMAND(ID_MODEL_REMOVEOBJECT, OnModelRemoveobject)
	ON_COMMAND(ID_MODEL_ADDBLOCK, OnModelAddblock)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MODEL_EDITAXIS, OnModelEditaxis)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
//	ON_MESSAGE(UM_SELCTITEMINVIEW,OnSelectItemInView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MTTreeView drawing

void MTTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDC->SetBkColor(RGB(0,0,0));
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// MTTreeView diagnostics

#ifdef _DEBUG
void MTTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void MTTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MTTreeView message handlers

void MTTreeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTreeView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void MTTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if(lHint!=UPDATEALLVIEW && lHint!=UPDATETREEVIEW)
		return;
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();	
	ASSERT(pDoc);
	int i,k;
	Node TNode;
	HTree.RemoveAll();
	Tree->DeleteAllItems();
	Tree->SetBkColor(pDoc->TreeViewBkColor);
	Tree->SetFont(&pDoc->TreeViewFont);
	for(i=0;i<pDoc->MTMachineTool.Axis.GetSize();i++)
	{
		k=GetHindex(pDoc->MTMachineTool.Axis[i]->GetParent());
		if(k==-1 && pDoc->MTMachineTool.Axis[i]->GetParent()!="")
		{
			AfxMessageBox("Ошибка!Произошла потеря данных!Корректная работа невозможна!");
			return;
		}
		TNode.Name=pDoc->MTMachineTool.Axis[i]->GetName();
		if(k==-1)
			TNode.hItem=Tree->InsertItem(pDoc->MTMachineTool.Axis[i]->GetName(),NULL);
		else
			TNode.hItem=Tree->InsertItem(pDoc->MTMachineTool.Axis[i]->GetName(),HTree[k].hItem);
		if(pDoc->MTMachineTool.Axis[i]->GetType()==1)
		{
			Tree->SetItemImage(TNode.hItem,0,0);
		}
		else if(pDoc->MTMachineTool.Axis[i]->GetType()==2)
		{
			Tree->SetItemImage(TNode.hItem,1,1);
		}
		else
			Tree->SetItemImage(TNode.hItem,2,2);
		HTree.Add(TNode);
	}
	for(i=0;i<HTree.GetSize();i++)
	{
		Tree->Expand(HTree[i].hItem,TVE_EXPAND);
	}
}

int MTTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();	
	ASSERT(pDoc);
	Tree=&GetTreeCtrl();
	
	Tree->SetBkColor(pDoc->TreeViewBkColor);
	Tree->SetTextColor(RGB (255,255,255));
	CBitmap a,b,c;
	a.LoadBitmap(IDB_BITMAP_BIGARROW);
	b.LoadBitmap(IDB_BITMAP_BIGROTARY);
	c.LoadBitmap(IDB_BITMAP_BIGPART);
	TreeImageList.Create(TREE_ICON_WIDTH, TREE_ICON_HEIGHT, ILC_COLOR16 | ILC_MASK, 1, 10);
	TreeImageList.Add(&a,RGB(0,0,0));
	TreeImageList.Add(&b,RGB(0,0,0));
	TreeImageList.Add(&c,RGB(0,0,0));
	Tree->SetImageList(&TreeImageList,TVSIL_NORMAL);
	return 0;
}

int MTTreeView::GetHindex(CString n)
{
	int i;
	for(i=0;i<HTree.GetSize();i++)
	{
		if(n==HTree[i].Name)
			return i;
	}
	return -1;
}

BOOL MTTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	//m_dwDefaultStyle= TVS_HASLINES |TVS_LINESATROOT;	
	cs.style|=TVS_HASLINES |TVS_LINESATROOT|TVS_SHOWSELALWAYS|TVS_HASBUTTONS;
	return CTreeView::PreCreateWindow(cs);
}



void MTTreeView::OnModelAddaxis() 
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	DlgAddAxis dlg;
	int i;
	for(i=0;i<pDoc->MTMachineTool.Axis.GetSize();i++)
	{
		if(pDoc->MTMachineTool.Axis[i]->GetType()!=3)
			dlg.Parents.Add(pDoc->MTMachineTool.Axis[i]->GetName());
	}
	HTREEITEM hi;
	hi=Tree->GetSelectedItem();
	if(hi!=NULL)
	{
		for(i=0;i<HTree.GetSize();i++)
		{
			if(HTree[i].hItem==hi)
			{
				dlg.m_Parent=HTree[i].Name;
				break;
			}
		}
	}
	Tree->SelectItem(hi);
	if(dlg.DoModal()==IDCANCEL)
		return;
	pDoc->MTMachineTool.GoToNullPosition();
	MTAxis *Ax,*NAx;
	if(dlg.Forward)
	{
		Ax=new MTForwardAxis;
		NAx=new MTForwardAxis;
		Ax->SetName(dlg.m_Name);
		Ax->SetParent(dlg.m_Parent);
		Ax->SetColor(dlg.Color);
		Ax->SetVector(dlg.m_VX,dlg.m_VY,dlg.m_VZ);
		Ax->SetBasePoint(dlg.m_BX,dlg.m_BY,dlg.m_BZ);
		(Ax->Length)=(dlg.m_Length);
		NAx->SetName(dlg.m_Name);
		NAx->SetParent(dlg.m_Parent);
		NAx->SetColor(dlg.Color);
		NAx->SetVector(dlg.m_VX,dlg.m_VY,dlg.m_VZ);
		NAx->SetBasePoint(dlg.m_BX,dlg.m_BY,dlg.m_BZ);
		(NAx->Length)=(dlg.m_Length);
		pDoc->MTMachineTool.Axis.AddAxis(Ax);
		pDoc->MTMachineTool.NullAxis.AddAxis(NAx);
	}
	else
	{
		Ax=new MTRotaryAxis;
		NAx=new MTRotaryAxis;
		Ax->SetName(dlg.m_Name);
		Ax->SetParent(dlg.m_Parent);
		Ax->SetColor(dlg.Color);
		Ax->SetVector(dlg.m_VX,dlg.m_VY,dlg.m_VZ);
		Ax->SetBasePoint(dlg.m_BX,dlg.m_BY,dlg.m_BZ);
		(Ax->Length)=(dlg.m_Length);
		NAx->SetName(dlg.m_Name);
		NAx->SetParent(dlg.m_Parent);
		NAx->SetColor(dlg.Color);
		NAx->SetVector(dlg.m_VX,dlg.m_VY,dlg.m_VZ);
		NAx->SetBasePoint(dlg.m_BX,dlg.m_BY,dlg.m_BZ);
		(NAx->Length)=(dlg.m_Length);
		pDoc->MTMachineTool.Axis.AddAxis(Ax);
		pDoc->MTMachineTool.NullAxis.AddAxis(NAx);
	}

	int m,k=0;
	m=pDoc->MTMachineTool.Axis.GetAxisIndex(Ax->GetName());
	if(m!=-1)
	{
		CooArray Temp;
		for(i=0;i<m;i++)
		{
			if(pDoc->MTMachineTool.Axis[i]->GetType()!=3)
			{
				Temp.Add(pDoc->MTMachineTool.Start[k]);
				k++;
			}
		}
		MTCoordinate n;
		n.AxisName=Ax->GetName();
		Temp.Add(n);
		for(i=k;i<pDoc->MTMachineTool.Start.GetSize();i++)
			Temp.Add(pDoc->MTMachineTool.Start[i]);
		pDoc->MTMachineTool.Start.RemoveAll();
		for(i=0;i<Temp.GetSize();i++)
			pDoc->MTMachineTool.Start.Add(Temp[i]);
		Temp.RemoveAll();
		for(i=0;i<k;i++)
		{
			Temp.Add(pDoc->MTMachineTool.Now[i]);
		}
		Temp.Add(n);
		for(i=k;i<pDoc->MTMachineTool.Now.GetSize();i++)
		{
			Temp.Add(pDoc->MTMachineTool.Now[i]);
		}
		pDoc->MTMachineTool.Now.RemoveAll();
		for(i=0;i<Temp.GetSize();i++)
			pDoc->MTMachineTool.Now.Add(Temp[i]);
		Temp.RemoveAll();
		for(i=0;i<k;i++)
		{
			Temp.Add(pDoc->MTMachineTool.End[i]);
		}
		Temp.Add(n);
		for(i=k;i<pDoc->MTMachineTool.End.GetSize();i++)
		{
			Temp.Add(pDoc->MTMachineTool.End[i]);
		}
		pDoc->MTMachineTool.End.RemoveAll();
		for(i=0;i<Temp.GetSize();i++)
			pDoc->MTMachineTool.End.Add(Temp[i]);
		Temp.RemoveAll();
		for(i=0;i<k;i++)
		{
			Temp.Add(pDoc->MTMachineTool.MoveParam[i]);
		}
		Temp.Add(n);
		for(i=k;i<pDoc->MTMachineTool.MoveParam.GetSize();i++)
		{
			Temp.Add(pDoc->MTMachineTool.MoveParam[i]);
		}
		pDoc->MTMachineTool.MoveParam.RemoveAll();
		for(i=0;i<Temp.GetSize();i++)
			pDoc->MTMachineTool.MoveParam.Add(Temp[i]);

	}
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,UPDATEALLVIEW);
	Tree->SelectItem(hi);
	

}







void MTTreeView::OnModelRemoveobject() 
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	HTREEITEM hi;
	hi=Tree->GetSelectedItem();
	if(hi==NULL)
	{
		AfxMessageBox("Объект не выбран!");
		return;
	}
	int i;
	CString str;
	for(i=0;i<HTree.GetSize();i++)
	{
		if(HTree[i].hItem==hi)
			str=HTree[i].Name;
	}
	pDoc->MTMachineTool.Axis.RemoveAxis(str);
	pDoc->MTMachineTool.NullAxis.RemoveAxis(str);
	pDoc->MTMachineTool.Check();
	int flag,j;
	i=0;
	while(i<pDoc->MTMachineTool.Start.GetSize())
	{
		flag=0;
		for(j=0;j<pDoc->MTMachineTool.Axis.GetSize();j++)
		{
			if(pDoc->MTMachineTool.Start[i].AxisName==pDoc->MTMachineTool.Axis[j]->GetName())
			{
				flag=1;
				break;
			}
		}
		if(flag==1)
			i++;
		else
		{
			pDoc->MTMachineTool.Start.RemoveAt(i);
			pDoc->MTMachineTool.End.RemoveAt(i);
			pDoc->MTMachineTool.Now.RemoveAt(i);
		}
	}
/*	for(i=0;i<pDoc->MTMachineTool.Start.GetSize();i++)
	{
		if(pDoc->MTMachineTool.Start[i].AxisName==str)
		{
			pDoc->MTMachineTool.Start.RemoveAt(i);
			break;
		}
	}
	for(i=0;i<pDoc->MTMachineTool.End.GetSize();i++)
	{
		if(pDoc->MTMachineTool.End[i].AxisName==str)
		{
			pDoc->MTMachineTool.End.RemoveAt(i);
			break;
		}
	}
	for(i=0;i<pDoc->MTMachineTool.Now.GetSize();i++)
	{
		if(pDoc->MTMachineTool.Now[i].AxisName==str)
		{
			pDoc->MTMachineTool.Now.RemoveAt(i);
			break;
		}
	}*/
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,UPDATEALLVIEW);
	
}

void MTTreeView::OnModelAddblock() 
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	int i;
	DlgAddPart dlg;
	HTREEITEM hi;
	hi=Tree->GetSelectedItem();
	if(hi!=NULL)
	{
		for(i=0;i<HTree.GetSize();i++)
		{
			if(HTree[i].hItem==hi)
			{
				dlg.m_Parent=HTree[i].Name;
				break;
			}
		}
	}
	dlg.Part=new MTPart;
	dlg.Axis=pDoc->MTMachineTool.NullAxis;
	if(dlg.DoModal()!=IDOK)
	{
		Tree->SelectItem(hi);
		delete dlg.Part;
		return;
	}

	pDoc->MTMachineTool.AddPart(dlg.Part,dlg.PAxis);
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,UPDATEALLVIEW);
	Tree->SelectItem(hi);
	
}



void MTTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	if(pDoc->Mooving)
		return;
	HTREEITEM hi;
	int i;
	hi=Tree->HitTest(point);
	if(hi!=NULL)
	{
		for(i=0;i<HTree.GetSize();i++)
			if(HTree[i].hItem==hi)
			{
				Tree->SelectItem(hi);
//				::PostMessage(m_MTListViewWnd,UM_SELCTITEMINVIEW,0,0);
				break;
			}
	}
	CMenu menu;
	ClientToScreen(&point);
	menu.LoadMenu(IDR_MENU_MODEL);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_CENTERALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
	menu.DestroyMenu();
	
//	CTreeView::OnRButtonDown(nFlags, point);
}

void MTTreeView::OnModelEditaxis() 
{
		CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
//	DlgAxisEditor dlg;
	int k=-1;
	int i;
	HTREEITEM hi;
	hi=Tree->GetSelectedItem();
	//dlg.EditedNumber=-1;
	for(i=0;i<HTree.GetSize();i++)	
	{
		if(hi==HTree[i].hItem)
		{
			k=pDoc->MTMachineTool.Axis.GetAxisIndex(HTree[i].Name);
			break;
		}
	}
	if(k==-1)
	{
		AfxMessageBox("Объект не выбран!");
		return;
	}
	if(pDoc->MTMachineTool.Axis[k]->GetType()==3)
	{
		int ind;
		ind=-1;
		CString str;
		str=pDoc->MTMachineTool.Axis[k]->GetName();
		for(i=0;i<pDoc->MTMachineTool.Part.GetSize();i++)
		{
			if(str==pDoc->MTMachineTool.Part[i]->GetAxisName())
			{
				ind=i;
				break;
			}
		}
		if(ind==-1)
			return;
		DlgEditPart dlg;
		MTPart *p;
		p=new MTPart;
		p->CreateEx(pDoc->MTMachineTool.Part[ind]->GetFileName());
		dlg.Part=p;
		dlg.m_Parent=pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetParent();
		dlg.m_Name=str;
		dlg.Color=pDoc->MTMachineTool.Part[ind]->Color.GetRGB();
		//p->RealGabar.Expand(pDoc->MTMachineTool.Part[ind]->RealGabar);
		p->RealGabar=pDoc->MTMachineTool.Part[ind]->RealGabar;
		dlg.m_GX=(float)pDoc->MTMachineTool.Part[ind]->RealGabar.dX();
		dlg.m_GY=(float)pDoc->MTMachineTool.Part[ind]->RealGabar.dY();
		dlg.m_GZ=(float)pDoc->MTMachineTool.Part[ind]->RealGabar.dZ();
		dlg.m_IX=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetBasePoint().GetX();
		dlg.m_IY=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetBasePoint().GetY();
		dlg.m_IZ=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetBasePoint().GetZ();
		dlg.m_XX=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetVector().GetX();
		dlg.m_XY=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetVector().GetY();
		dlg.m_XZ=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetVector().GetZ();
		dlg.m_YX=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetVectorY().GetX();
		dlg.m_YY=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetVectorY().GetY();
		dlg.m_YZ=(int)pDoc->MTMachineTool.Part[ind]->GetPartAxis()->GetVectorY().GetZ();
		dlg.Axis=pDoc->MTMachineTool.Axis;
		if(dlg.DoModal()!=IDOK)
		{
			delete p;
			return;
		}
		pDoc->MTMachineTool.RemovePart(ind);
		pDoc->MTMachineTool.AddPart(dlg.Part,dlg.PAxis);
		pDoc->MTMachineTool.Axis.Sorting();
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL,UPDATEALLVIEW);
				
	}
	else
	{
		DlgAxisEditor dlg;
		dlg.EditedNumber=k;
		dlg.Axis=pDoc->MTMachineTool.Axis;
		if(dlg.DoModal()==IDCANCEL)
			return;
		pDoc->MTMachineTool.Axis[k]->SetBasePoint(dlg.Axis[k]->GetBasePoint());
		pDoc->MTMachineTool.Axis[k]->SetColor(dlg.Axis[k]->GetColor());
		pDoc->MTMachineTool.Axis[k]->SetVector(dlg.Axis[k]->GetVector());
		pDoc->MTMachineTool.Axis[k]->SetParent(dlg.Axis[k]->GetParent());
		pDoc->MTMachineTool.Axis[k]->Length=dlg.Axis[k]->Length;
		pDoc->MTMachineTool.Check();
		pDoc->UpdateAllViews(NULL,UPDATEALLVIEW);
	}
	pDoc->SetModifiedFlag();
	
}







void MTTreeView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	HTREEITEM hi;
	hi=Tree->GetSelectedItem();
	int i;
	CString str;
	for(i=0;i<HTree.GetSize();i++)
	{
		if(HTree[i].hItem==hi)
		{
			str=HTree[i].Name;
		}
	}
	pDoc->SelectedItem=str;
	::PostMessage(m_MTListViewWnd,UM_SELCTITEMINVIEW,0,0);*/
	*pResult = 0;
}



void MTTreeView::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CMachineToolDoc *pDoc=(CMachineToolDoc *)GetDocument();
	ASSERT(pDoc);
	HTREEITEM hi;
	hi=pNMTreeView->itemNew.hItem;
	int i;
	CString str;
	for(i=0;i<HTree.GetSize();i++)
	{
		if(HTree[i].hItem==hi)
		{
			str=HTree[i].Name;
		}
	}
	pDoc->SelectedItem=str;
	::PostMessage(m_MTListViewWnd,UM_SELCTITEMINVIEW,0,0);
	*pResult = 0;
}
LRESULT MTTreeView::OnSelectItemInView(WPARAM w, LPARAM l)
{
	CMachineToolDoc* pDoc =(CMachineToolDoc*) GetDocument();
	ASSERT_VALID(pDoc);
	int i;
	CString str;
	str=pDoc->SelectedItem;
	for(i=0;i<HTree.GetSize();i++)
	{
		if(HTree[i].Name==str)
		{
			Tree->SelectItem(HTree[i].hItem);
		}
	}
	return 0;
}
LRESULT MTTreeView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case (UM_SELCTITEMINVIEW):
		{
			OnSelectItemInView(0,0);
			return 0;
		}
	}
	return CTreeView::WindowProc(message, wParam, lParam);
}