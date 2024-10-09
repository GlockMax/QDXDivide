// NGeomArray.h: interface for the NGeomArray class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "NColor.h"
#include "BGeomArray.h"

class NGeomArray : public BGeomArray  
{
public:
	void Draw(int FirstHCadr,int LastHCadr, int BoldCadr = -1, double t = -1.);
	void DrawBoldCadr(int BoldCadr, const NColor &Color);
	void NDrawCadr(int iName, int CycleName, NCadrGeom *pGeom, double t, bool IsBoldCadr, const NColor &Color, const NColor &FastMovColor);
	NGeomArray();
	virtual ~NGeomArray();
};
