#pragma once
#include "stdafx.h"
#include "NCMB_API.h"
#include "NCadrGeom.h"

class NCMB_API BFastMoveMan
{
public:
	BFastMoveMan();
	~BFastMoveMan();
	static bool PartInterp();
	static void Fill(const CString &Val);
	static int StepsNum;
	static bool CoordFlags[2][3][3];// UP-DOWN; step num; coord num
	static void MakeCadrs(NCadrGeom &Geom, NCadrGeom &AddGeom);
};
