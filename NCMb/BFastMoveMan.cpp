#include "StdAfx.h"
#include "bfastmoveman.h"

int BFastMoveMan::StepsNum = 0;
bool BFastMoveMan::CoordFlags[2][3][3];// UP-DOWN; step num; coord num

BFastMoveMan::BFastMoveMan()
{
	StepsNum = 0;
	CoordFlags[0][0][0] = true;
	CoordFlags[0][0][1] = true;
	CoordFlags[0][0][2] = true;
	CoordFlags[0][1][0] = true;
	CoordFlags[0][1][1] = true;
	CoordFlags[0][1][2] = true;
	CoordFlags[0][2][0] = true;
	CoordFlags[0][2][1] = true;
	CoordFlags[0][2][2] = true;
	CoordFlags[1][0][0] = true;
	CoordFlags[1][0][1] = true;
	CoordFlags[1][0][2] = true;
	CoordFlags[1][1][0] = true;
	CoordFlags[1][1][1] = true;
	CoordFlags[1][1][2] = true;
	CoordFlags[1][2][0] = true;
	CoordFlags[1][2][1] = true;
	CoordFlags[1][2][2] = true;
}
bool BFastMoveMan::PartInterp()
{
	return StepsNum != 0;
}
BFastMoveMan::~BFastMoveMan()
{
}
void BFastMoveMan::Fill(const CString &Val)
{
	StepsNum = 0;
	if(Val != "Yes")
		return;
	StepsNum = 2;
	CoordFlags[0][0][0] = false;
	CoordFlags[0][0][1] = false;
	CoordFlags[0][0][2] = true;
	CoordFlags[0][1][0] = true;
	CoordFlags[0][1][1] = true;
	CoordFlags[0][1][2] = true;
	CoordFlags[0][2][0] = false;
	CoordFlags[0][2][1] = false;
	CoordFlags[0][2][2] = false;
	CoordFlags[1][0][0] = true;
	CoordFlags[1][0][1] = true;
	CoordFlags[1][0][2] = false;
	CoordFlags[1][1][0] = true;
	CoordFlags[1][1][1] = true;
	CoordFlags[1][1][2] = true;
	CoordFlags[1][2][0] = false;
	CoordFlags[1][2][1] = false;
	CoordFlags[1][2][2] = false;
}

void BFastMoveMan::MakeCadrs(NCadrGeom &Geom, NCadrGeom &AddGeom)
{
	// Разделить, если необходимо, Geom на два кадра Geom и AddGeom
	double xb, yb, zb, xe, ye, ze;
	Geom.GetB(&xb, &yb, &zb);
	Geom.GetE(&xe, &ye, &ze);
	int UpDownInd = (zb > ze) ? 1 : 0;
	double xc = xb;
	double yc = yb;
	double zc = zb;
	int st = 0;
	for(; st < StepsNum ; ++st)
	{
		xc = CoordFlags[UpDownInd][st][0] ? xe : xb;
		yc = CoordFlags[UpDownInd][st][1] ? ye : yb;
		zc = CoordFlags[UpDownInd][st][2] ? ze : zb;
		if(xc != xb || yc != yb || zc != zb )
			break;
	}
	if(st == StepsNum)
	{
		AddGeom.Clear();
	}
	else
	{
		AddGeom = Geom;
		Geom.SetE(xc, yc, zc);
		AddGeom.SetB(xc, yc, zc);
	}
}
