// NStateGeomCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "constdef.h"
#include "NColor.h"
#include "NFont.h"
#include "NCMProject.h"
#include "NCMComData.h"
#include "NStateGeomCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStateGeomCtrl

IMPLEMENT_DYNAMIC(NStateGeomCtrl, MTreeCtrl)
NStateGeomCtrl::NStateGeomCtrl()
{
}

NStateGeomCtrl::~NStateGeomCtrl()
{
}


BEGIN_MESSAGE_MAP(NStateGeomCtrl, MTreeCtrl)
	//{{AFX_MSG_MAP(NStateGeomCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStateGeomCtrl message handlers

int NStateGeomCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateOtr();
	NCadrGeom PrevGeom;
	PrevGeom.Set(cwarc, 0.,0.,0.,0.,0.,0.,0.,0.,0.,XY);
	Update(PrevGeom, NCadrGeom(), BPoint(0., 0., 1., 0.));

	return 0;
}



void NStateGeomCtrl::CreateOtr()
{
	DeleteAllItems();
	CString tmp;
	HTREEITEM Item;
	tmp.LoadString(IDS_V_START);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	BXItem = InsertItem("",Item);
	BYItem = InsertItem("",Item);
	BZItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND  );
	tmp.LoadString(IDS_V_END);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	EXItem = InsertItem("",Item);
	EYItem = InsertItem("",Item);
	EZItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND  );
	tmp.LoadString(IDS_V_ANGLES);
	Item = InsertItem(tmp);
	SetItemState(Item,TVIS_BOLD,TVIS_BOLD);
	AXItem = InsertItem("",Item);
	AYItem = InsertItem("",Item);
	AZItem = InsertItem("",Item);
	Expand(Item,TVE_EXPAND  );
	tmp.LoadString(IDS_V_CENTER);
	CenterItem = InsertItem(tmp);
	SetItemState(CenterItem,TVIS_BOLD,TVIS_BOLD);
	CXItem = InsertItem("", CenterItem);
	CYItem = InsertItem("", CenterItem);
	CZItem = InsertItem("", CenterItem);
	CRItem = InsertItem("", CenterItem);
	Expand(CenterItem,TVE_EXPAND  );
	tmp.LoadString(IDS_TOOLVECTOR);// NEW AXIS
	Item = InsertItem(tmp);
	SetItemState(Item, TVIS_BOLD, TVIS_BOLD);
	TXItem = InsertItem("", Item);
	TYItem = InsertItem("", Item);
	TZItem = InsertItem("", Item);
	Expand(Item, TVE_EXPAND);
	tmp.LoadString(IDS_V_LENGTH);
	LenItem = InsertItem(tmp);
	SetItemState(LenItem,TVIS_BOLD,TVIS_BOLD);
}

void NStateGeomCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
// Does nothing and returns 1 to prevent standard behaveur	
	*pResult = 1;
}

void NStateGeomCtrl::Update(const NCadrGeom &PrevGeom, const NCadrGeom &NextGeom, const BPoint &ToolAxis)
{
	double x = 0., y = 0., z = 0., xe = 0., ye = 0., ze = 0.;
	CString FormatStr(""),EmptyStr;
	EmptyStr.LoadString(IDS_V_EMPTY);
	if(NextGeom.HaveGeom())
	{
// Point selected
		PrevGeom.GetE(&x,&y,&z);
		if (NCMComData::GetType() == AT_MILL)
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
		else
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
		SetItemText(BXItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH,y);
		SetItemText(BYItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH,z);
		SetItemText(BZItem,FormatStr);
		if (NCMComData::GetType() == AT_MILL)
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
		else
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
		SetItemText(EXItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH,y);
		SetItemText(EYItem,FormatStr);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH,z);
		SetItemText(EZItem,FormatStr);

		x = NextGeom.Angle(PrevGeom);
		FormatStr.Format(IDS_ANGFORMAT,x);
		SetItemText(AXItem,FormatStr);
		SetItemText(AYItem,EmptyStr);
		SetItemText(AZItem,EmptyStr);

		FormatStr.LoadString(IDS_V_CENTER);
		SetItemText(CenterItem, FormatStr);
		SetItemText(CXItem,EmptyStr);
		SetItemText(CYItem,EmptyStr);
		SetItemText(CZItem,EmptyStr);
		SetItemText(CRItem,EmptyStr);

		SetItemText(LenItem,EmptyStr);
		
	}
	else if (PrevGeom.HaveGeom5())
	{
		if (PrevGeom.IsLine())
		{
			PrevGeom.GetB(&x, &y, &z);
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(BXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(BYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(BZItem, FormatStr);

			PrevGeom.GetE(&x, &y, &z);
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(EXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(EYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(EZItem, FormatStr);

			PrevGeom.GetA(&x, &y, &z);
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(AXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(AYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(AZItem, FormatStr);

			FormatStr.LoadString(IDS_V_CENTER);
			SetItemText(CenterItem, FormatStr);
			SetItemText(CXItem, EmptyStr);
			SetItemText(CYItem, EmptyStr);
			SetItemText(CZItem, EmptyStr);
			SetItemText(CRItem, EmptyStr);


			FormatStr.Format((MM_INCH == M_MM) ? IDS_LENFORMAT : IDS_LENFORMAT_INCH, PrevGeom.Length());
			SetItemText(LenItem, FormatStr);

		}
		else if (PrevGeom.IsArc())
		{
			PrevGeom.GetB(&x, &y, &z);
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(BXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(BYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(BZItem, FormatStr);

			PrevGeom.GetE(&x, &y, &z);
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(EXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(EYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(EZItem, FormatStr);

			FormatStr.LoadString(IDS_V_CENTER);
			SetItemText(CenterItem, FormatStr);
			PrevGeom.GetC(&x, &y, &z);
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(CXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(CYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(CZItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_CRFORMAT : IDS_CRFORMAT_INCH, PrevGeom.GetR());
			SetItemText(CRItem, FormatStr);

			PrevGeom.GetArcA(&x, &y, &z);
			FormatStr.Format(IDS_BANGFORMAT, x);
			SetItemText(AXItem, FormatStr);
			FormatStr.Format(IDS_EANGFORMAT, y);
			SetItemText(AYItem, FormatStr);
			FormatStr.Format(IDS_CANGFORMAT, z);
			SetItemText(AZItem, FormatStr);

			FormatStr.Format((MM_INCH == M_MM) ? IDS_LENFORMAT : IDS_LENFORMAT_INCH, PrevGeom.Length());
			SetItemText(LenItem, FormatStr);

		}
		else if (PrevGeom.IsRotate())
		{
			{ BPoint P = PrevGeom.GetBP(); x = P.GetX(); y = P.GetY(); z = P.GetZ(); }
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(BXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(BYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(BZItem, FormatStr);

			{ BPoint P = PrevGeom.GetEP(); x = P.GetX(); y = P.GetY(); z = P.GetZ(); }
			if (NCMComData::GetType() == AT_MILL)
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
			else
				FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
			SetItemText(EXItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
			SetItemText(EYItem, FormatStr);
			FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
			SetItemText(EZItem, FormatStr);

			PrevGeom.GetI(&x, &y, &z);
			PrevGeom.GetN(&xe, &ye, &ze);
			FormatStr.Format(IDS_ABCXFORMAT, x, xe);
			SetItemText(AXItem, FormatStr);
			FormatStr.Format(IDS_ABCYFORMAT, y, ye);
			SetItemText(AYItem, FormatStr);
			FormatStr.Format(IDS_ABCZFORMAT, z, ze);
			SetItemText(AZItem, FormatStr);

			FormatStr.LoadString(IDS_V_CENTER);
			SetItemText(CenterItem, FormatStr);
			SetItemText(CXItem, EmptyStr);
			SetItemText(CYItem, EmptyStr);
			SetItemText(CZItem, EmptyStr);
			SetItemText(CRItem, EmptyStr);


			FormatStr.Format((MM_INCH == M_MM) ? IDS_LENFORMAT : IDS_LENFORMAT_INCH, PrevGeom.Length());
			SetItemText(LenItem, FormatStr);

		}
	}
	else
	{
		NCadrGeom PrevGeom;
		PrevGeom.Set(cwarc, 0., 0., 0., 0., 0., 0., 0., 0., 0., XY);
		Update(PrevGeom, NCadrGeom(), BPoint(0., 0., 1., 0.));
		return;
	}
	FormatStr.Format(IDS_TOOLVECANGX, ToolAxis.Angle0_180(BPoint(1., 0., 0., 0.)));// NEW AXIS
	SetItemText(TXItem, FormatStr);
	FormatStr.Format(IDS_TOOLVECANGY, ToolAxis.Angle0_180(BPoint(0., 1., 0., 0.)));// NEW AXIS
	SetItemText(TYItem, FormatStr);
	FormatStr.Format(IDS_TOOLVECANGZ, ToolAxis.Angle0_180(BPoint(0., 0., 1., 0.)));// NEW AXIS
	SetItemText(TZItem, FormatStr);
	
}

void NStateGeomCtrl::UpdateDist(const BPoint& p0, const BPoint& p1, const BPoint& ToolAxis)
{
	double x = 0., y = 0., z = 0., h;
	CString FormatStr(""), EmptyStr;
	EmptyStr.LoadString(IDS_V_EMPTY);
	if (p0.IsPoint())
		p0.Get(x, y, z, h);
	if (NCMComData::GetType() == AT_MILL)
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
	else
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
	SetItemText(BXItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
	SetItemText(BYItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
	SetItemText(BZItem, FormatStr);
	x = 0.;
	y = 0.;
	z = 0.;
	if (p0.IsPoint() && p1.IsPoint())
		p1.Get(x, y, z, h);
	if (NCMComData::GetType() == AT_MILL)
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
	else
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT2 : IDS_BXFORMAT2_INCH, x, 2. * x);
	SetItemText(EXItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
	SetItemText(EYItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
	SetItemText(EZItem, FormatStr);
	BPoint V(0., 0., 0., 0.);
	if (p0.IsPoint() && p1.IsPoint())
	{
		V = p1 - p0;
		x = V.Angle0_180(BPoint(1., 0., 0., 0.));
		y = V.Angle0_180(BPoint(0., 1., 0., 0.));
		z = V.Angle0_180(BPoint(0., 0., 1., 0.));
	}
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, x);
	SetItemText(AXItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, y);
	SetItemText(AYItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, z);
	SetItemText(AZItem, FormatStr);

	FormatStr.LoadString(IDS_V_DISTANCE);
	SetItemText(CenterItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BXFORMAT : IDS_BXFORMAT_INCH, V.GetX());
	SetItemText(CXItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BYFORMAT : IDS_BYFORMAT_INCH, V.GetY());
	SetItemText(CYItem, FormatStr);
	FormatStr.Format((MM_INCH == M_MM) ? IDS_BZFORMAT : IDS_BZFORMAT_INCH, V.GetZ());
	SetItemText(CZItem, FormatStr);
	double Length = sqrt(V.D2());
	if (NCMComData::GetType() == AT_MILL)
	{
		V.SetZ(0.);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXYFORMAT : IDS_BXYFORMAT_INCH, sqrt(V.D2()));
	}
	else
	{
		V.SetY(0.);
		FormatStr.Format((MM_INCH == M_MM) ? IDS_BXZFORMAT : IDS_BXZFORMAT_INCH, sqrt(V.D2()));
	}
	SetItemText(CRItem, FormatStr);

	FormatStr.Format((MM_INCH == M_MM) ? IDS_LENFORMAT : IDS_LENFORMAT_INCH, Length);
	SetItemText(LenItem, FormatStr);
	FormatStr.Format(IDS_TOOLVECANGX, ToolAxis.Angle0_180(BPoint(1., 0., 0., 0.)));// NEW AXIS
	SetItemText(TXItem, FormatStr);
	FormatStr.Format(IDS_TOOLVECANGY, ToolAxis.Angle0_180(BPoint(0., 1., 0., 0.)));// NEW AXIS
	SetItemText(TYItem, FormatStr);
	FormatStr.Format(IDS_TOOLVECANGZ, ToolAxis.Angle0_180(BPoint(0., 0., 1., 0.)));// NEW AXIS
	SetItemText(TZItem, FormatStr);
}

void NStateGeomCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	*pResult = 1;// Do not draw selection
}

void NStateGeomCtrl::PreSubclassWindow()
{
	SetSettings();
	CreateOtr();
	NCadrGeom PrevGeom;
	PrevGeom.Set(cwarc, 0.,0.,0.,0.,0.,0.,0.,0.,0.,XY);
	Update(PrevGeom, NCadrGeom(), BPoint(0., 0., 1., 0.));
	
	MTreeCtrl::PreSubclassWindow();
}

void NStateGeomCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_LBUTTONDOWN,WPARAM(nFlags),LPARAM(MAKELONG (point.x, point.y)));
	//CTreeCtrl::OnLButtonDown(nFlags, point);
}
