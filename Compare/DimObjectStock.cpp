#include "StdAfx.h"
#include "DimObjectStock.h"
#include "NSurfSettings.h"
#include "BaseRender.h"
#include "DimObjectSegmentEdge.h"
#include "NCEnums.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectStock, DimObject, 0)

DimObjectStock::DimObjectStock(const BPoint &Min, const BPoint &Max, float Width) : PointMin(Min), PointMax(Max), DimObject(Width)
{
	for(int i = 0; i < 12; ++i)
		Edges[i] = 0;
}
void DimObjectStock::Change(const BPoint& iPoint, const BPoint& iViewDir)
{
	BPoint pt = BPoint::ProjectPointOnPlane(iPoint, _oldPt, iViewDir);
    BPoint vect = pt - _oldPt;
    _oldPt = _oldPt + vect;
	PointMax = _oldPt;
}

BBox DimObjectStock::GetGabar() const
{
	BBox Box;
	Box.Expand(PointMin);
	Box.Expand(PointMax);
	return Box;
}

void DimObjectStock::Serialize(CArchive& ar)
{
	DimObject::Serialize(ar);
	SerializeElements(ar, &PointMax, 1);
	SerializeElements(ar, &PointMin, 1);
}

void DimObjectStock::ChangeByEdge(const BPoint& iPoint, const BPoint& iViewDir, const class DimObjectSegmentEdge *pDOSEObj)
{
	double Min[4];
	PointMin.Get(Min[0], Min[1], Min[2], Min[3]);
	double Max[4];
	PointMax.Get(Max[0], Max[1], Max[2], Max[3]);
	for(int k = 0; k < 3; ++k)
	{
		double CurCoord =  pDOSEObj->Get_oldPt().GetArray()[k];
		switch(int(pDOSEObj->GetCoordMap().GetArray()[k]))
		{
		case 0:
			break;
		case 1:
			Min[k] = CurCoord;
			break;
		case 2:
			Max[k] = CurCoord;
			break;
		}
	}
	PointMin.Set(Min[0], Min[1], Min[2], Min[3]);
	PointMax.Set(Max[0], Max[1], Max[2], Max[3]);
}

void DimObjectStock::GenEdges(DimObjectSegmentEdge *oEdges[12])
{
	BPoint CoordMaps[12] = {
		BPoint(0, 1, 1, -1),
		BPoint(0, 1, 2, -1),
		BPoint(0, 2, 1, -1),
		BPoint(0, 2, 2, -1),
		BPoint(1, 0, 1, -1),
		BPoint(1, 0, 2, -1),
		BPoint(2, 0, 1, -1),
		BPoint(2, 0, 2, -1),
		BPoint(1, 1, 0, -1),
		BPoint(1, 2, 0, -1),
		BPoint(2, 1, 0, -1),
		BPoint(2, 2, 0, -1)
	}; // 0 - this coordinate don't change any point, 1 - this coordinate changes PointMin, 1 - this coordinate changes PointMax

	for(int i = 0; i < 12; ++i)
	{
		double B[3];
		double E[3];
		for(int k = 0; k < 3; ++k)
		{
			switch(int(CoordMaps[i].GetArray()[k]))
			{
			case 0:
				B[k] = PointMin.GetArray()[k];
				E[k] = PointMax.GetArray()[k];
				break;
			case 1:
				B[k] = PointMin.GetArray()[k];
				E[k] = PointMin.GetArray()[k];
				break;
			case 2:
				B[k] = PointMax.GetArray()[k];
				E[k] = PointMax.GetArray()[k];
				break;
			}
		}
		oEdges[i] = new DimObjectSegmentEdge(BPoint(B[0], B[1], B[2], 1.), BPoint(E[0], E[1], E[2], 1.), CoordMaps[i]);
	}
}
