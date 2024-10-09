#include "stdafx.h"
#include "math.h"
#include "BSpacePos.h"
#include "BProgram.h"
#include "grerendercadr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GReRenderCadr::GReRenderCadr(void)
//{
//}

GReRenderCadr::GReRenderCadr(GBodyRender *piStart)
	: GDynamicCadr(piStart)
{
}

GReRenderCadr::~GReRenderCadr(void)
{
}

bool GReRenderCadr::Set(const NCadrGeom & InGeom, const BMatr &RMatr, GBodyRender &End, double tPart)
{
	SetGeom(InGeom)
		;// Geom in initial position
					// It is used for current position determination only (GetCurABC, GetCurPoint)
	TrGeom = Geom;// Geom in current position
	TrGeom.Tr(RMatr);
	if(InGeom.IsUndef())
		return false;
	pEnd = &End;
	return true;
}
bool GReRenderCadr::NextStep()
{
	SetMT2CurPos();
	return true;
}

bool GReRenderCadr::CalcCurCoordDC(BPoint &XYZ, BPoint &ABC)// not used
{
	Geom.CalcCurCoord(1., XYZ, ABC);
	return true;
}
bool GReRenderCadr::CalcCurCoordMCS(BPoint & XYZ, BPoint & ABC)
{
	Geom.CalcCurCoordMCS(1., MCS, XYZ, ABC);
	return true;
}
void GReRenderCadr::CalcCurAddCoord(SMachState &AddCoord, double t)
{
	pProg->GetAddCoord(Geom.GetMSHistNum(), AddCoord);
}
