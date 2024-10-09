#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectCircle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectCircle, DimObjectArc, 0)

bool DimObjectCircle::CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oCircle, double iCenterRadius, float iCenterWidth, float iWidth)
{
    // построить дугу
    DimObject* circle;
    bool success = DimObjectArc::CreateOnThreePoints(iPoint0, iPoint1, iPoint2, circle, iCenterRadius, iCenterWidth, iWidth);
    if (!success)
        return false;
    
    // сделать дугу замкнутой
    static_cast<DimObjectArc*>(circle)->MakeClosed();

    // взять её данные
    oCircle = new DimObjectCircle(static_cast<DimObjectArc*>(circle));

    delete circle;

    return true;
}

bool DimObjectCircle::IsEqualToInternal(class DimObjectArc* iArc) const 
{
	return fabs(iArc->GetRadius() - GetRadius()) < MIND; 
}
