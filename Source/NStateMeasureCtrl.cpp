// NStateMeasureCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "constdef.h"
#include "NCMProject.h"
#include "NColor.h"
#include "NFont.h"
#include "NStateMeasureCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// NStateMeasureCtrl

IMPLEMENT_DYNAMIC(NStateMeasureCtrl, MTreeCtrl)
NStateMeasureCtrl::NStateMeasureCtrl()
{
	pMeasure = NULL;
}

NStateMeasureCtrl::~NStateMeasureCtrl()
{
}

BEGIN_MESSAGE_MAP(NStateMeasureCtrl, MTreeCtrl)
	//{{AFX_MSG_MAP(NStateGeomCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// NStateMeasureCtrl message handlers

int NStateMeasureCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateOtr();
	Update();
	return 0;
}

void NStateMeasureCtrl::CreateOtr(void)
{
	DeleteAllItems();
	CString tmp;
	HTREEITEM Item;
	CString LineTitle;
	LineTitle.LoadString(IDS_M_TOOLPATH_LINE_STEP);
	Item = InsertItem(LineTitle);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	LineIDItem = InsertItem("", Item);
	Expand(Item,TVE_EXPAND);

	tmp.LoadString(IDS_M_SURFTYPE);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	SurfTypeItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND);

	tmp.LoadString(IDS_M_CENTER_START);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	BXItem = InsertItem("",Item);
	BYItem = InsertItem("",Item);
	BZItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND);

	tmp.LoadString(IDS_M_NORMAL_DIRECTION);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	NXItem = InsertItem("",Item);
	NYItem = InsertItem("",Item);
	NZItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND);

	tmp.LoadString(IDS_M_ANGLES);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	Angle = InsertItem("",Item);
	Expand(Item,TVE_EXPAND);

	tmp.LoadString(IDS_M_RADIUS);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	RItem = InsertItem("",Item);
	R1Item = InsertItem("",Item);
	Expand(Item,TVE_EXPAND);

	tmp.LoadString(IDS_M_AXI_N_CYL);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	ANXItem = InsertItem("",Item);
	ANYItem = InsertItem("",Item);
	ANZItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND);

}

void NStateMeasureCtrl::Update(void)
{
	double x,y,z,r;
	double ax,ay,az;
	CString FormatStr(""),EmptyStr;
	EmptyStr.LoadString(IDS_V_EMPTY);
	if(pMeasure == NULL)
		return;
	
	if(!pMeasure->IsActivated())
	{
		x = 0.; ax =0.;
		y = 0.; ay =0.;
		z = 0.; az =0.;
		r = 0.;
		SetItemText(LineIDItem, EmptyStr);
		SetItemText(SurfTypeItem, EmptyStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH,x);
		SetItemText(BXItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH,y);
		SetItemText(BYItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH,z);
		SetItemText(BZItem,FormatStr);
		
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VXFORMAT : IDS_VXFORMAT_INCH,x,ax);
		SetItemText(NXItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VYFORMAT : IDS_VYFORMAT_INCH,y,ay);
		SetItemText(NYItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VZFORMAT : IDS_VZFORMAT_INCH,z,az);
		SetItemText(NZItem,FormatStr);
		
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VXFORMAT : IDS_VXFORMAT_INCH,x,ax);	
			SetItemText(ANXItem,FormatStr);			
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VYFORMAT : IDS_VYFORMAT_INCH,y,ay);	
			SetItemText(ANYItem,FormatStr);			
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VZFORMAT : IDS_VZFORMAT_INCH,z,az);	
			SetItemText(ANZItem,FormatStr);

		FormatStr.Format(IDS_ANGFORMAT,y);
		SetItemText(Angle,FormatStr);
		
		FormatStr.Format((MM_INCH == M_MM) ? IDS_M_RD_FORMAT : IDS_M_RD_FORMAT_INCH,r);
		SetItemText(RItem,FormatStr);
		SetItemText(R1Item,FormatStr);
		
		return;
	}
	else
	{
		SetItemText(LineIDItem, pMeasure->GetSufrInfo().LineID);

		SetItemText(SurfTypeItem, pMeasure->GetSufrInfo().Type);

		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH,pMeasure->GetSufrInfo().BeginPoint.GetX());
		SetItemText(BXItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH,pMeasure->GetSufrInfo().BeginPoint.GetY());
		SetItemText(BYItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH,pMeasure->GetSufrInfo().BeginPoint.GetZ());
		SetItemText(BZItem,FormatStr);
		
		ax = fabs(pMeasure->GetSufrInfo().Normal.Angle(BPoint(1.,0.,0.,0.)));
		ay = fabs(pMeasure->GetSufrInfo().Normal.Angle(BPoint(0.,1.,0.,0.)));
		az = fabs(pMeasure->GetSufrInfo().Normal.Angle(BPoint(0.,0.,1.,0.)));

		FormatStr.Format((MM_INCH == M_MM) ? IDS_VXFORMAT : IDS_VXFORMAT_INCH,pMeasure->GetSufrInfo().Normal.GetX(),ax);
		SetItemText(NXItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VYFORMAT : IDS_VYFORMAT_INCH,pMeasure->GetSufrInfo().Normal.GetY(),ay);
		SetItemText(NYItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VZFORMAT : IDS_VZFORMAT_INCH,pMeasure->GetSufrInfo().Normal.GetZ(),az);
		SetItemText(NZItem,FormatStr);

		FormatStr.Format(IDS_ANGFORMAT,pMeasure->GetSufrInfo().Angle);
		SetItemText(Angle,FormatStr);

		double R = pMeasure->GetSufrInfo().ExtR;
		FormatStr.Format((MM_INCH == M_MM) ? IDS_M_RD_FORMAT : IDS_M_RD_FORMAT_INCH,fabs(R),2*fabs(R));			
		SetItemText(RItem,FormatStr);
					
		double R1 = pMeasure->GetSufrInfo().IntR;
		if (R1 != 0.)
			FormatStr.Format((MM_INCH == M_MM) ? IDS_M_RD_FORMAT : IDS_M_RD_FORMAT_INCH,fabs(R1),2*fabs(R1));		
		else FormatStr.Format(" ");
		SetItemText(R1Item,FormatStr);
		
		
		x = pMeasure->GetSufrInfo().Axis.GetX();
		y = pMeasure->GetSufrInfo().Axis.GetY();
		z = pMeasure->GetSufrInfo().Axis.GetZ();
		ax = fabs(pMeasure->GetSufrInfo().Axis.Angle(BPoint(1.,0.,0.,0.)));
		ay = fabs(pMeasure->GetSufrInfo().Axis.Angle(BPoint(0.,1.,0.,0.)));
		az = fabs(pMeasure->GetSufrInfo().Axis.Angle(BPoint(0.,0.,1.,0.)));
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VXFORMAT : IDS_VXFORMAT_INCH,x,ax);			SetItemText(ANXItem,FormatStr);			
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VYFORMAT : IDS_VYFORMAT_INCH,y,ay);			SetItemText(ANYItem,FormatStr);			
		FormatStr.Format((MM_INCH == M_MM) ? IDS_VZFORMAT : IDS_VZFORMAT_INCH,z,az);			SetItemText(ANZItem,FormatStr);

		return;
	}


}
void NStateMeasureCtrl::SetMeasure(QMeasure * pM)
{
	pMeasure = pM;
}

void NStateMeasureCtrl::PreSubclassWindow()
{
	SetSettings();
	CreateOtr();
	Update();

	MTreeCtrl::PreSubclassWindow();
}

void NStateMeasureCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 1;// Do not draw selection
}
