#include "StdAfx.h"
#include "ConstDef.h"
#include "GLee.h"
#include "GL\GL.h"
#include "BaseRender.h"
#include "DimObjectPoint.h"

IMPLEMENT_SERIAL(DimObjectPoint, DimObject, 0)

void DimObjectPoint::DrawSpecific() const
{
    BaseRender::DrawBall(P, _widthMain * DimObjects::GetScaleKoef(), false);
}

BBox DimObjectPoint::GetGabar() const
{
    BBox box;
    box.Expand(P);
    return box;
}

void DimObjectPoint::Serialize(CArchive& ar)
{
    DimObject::Serialize(ar);
    SerializeElements(ar, &P, 1);
}

bool DimObjectPoint::Contains(const BPoint& iPoint) const 
{
	return (iPoint - P).D2() < MIND * MIND; 
}
