#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "BPoint.h"
#include "NCadrGeom.h"
#include "NCUnitState.h"
#include "NChamfer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString NChamfer::Patterns[NPat];

NChamfer::NChamfer(void)
{
	Clear();
}

NChamfer::~NChamfer(void)
{
}
void NChamfer::Clear(void)
{
	Type = OFF;
	Size = 0.;
}
bool NChamfer::HavePattern(const CString& text)
{
	for(int i = 0; i < NPat; ++i)
	{
		if(!Patterns[i].IsEmpty())
			if(text.Find(Patterns[i]) >= 0)
				return true;
	}
	return  false;
}
bool NChamfer::MakeGeomParams(const NCadrGeom &iGeom0, const NCadrGeom &iGeom1, const BPoint &P0, BPoint Vp, BPoint Vn, NCadrGeom &Geom, enum Plane Pl) const
{
	if (iGeom0.IsArc() || iGeom1.IsArc())
	{
		if (GetType() != ROUND)
			return false;// Round chamfer only can be processed with arcs 

		Plane TmpPlane = iGeom0.GetPlane();
		if (TmpPlane != iGeom1.GetPlane())
			return false;
		NCadrGeom Geom0 = iGeom0;
		NCadrGeom Geom1 = iGeom1;
		NCadrGeom G0, G1;
		if (TmpPlane != XY)
		{
			BMatr Tr = NCUnitState::ChangePlaneMatr(TmpPlane, XY);
			Geom0.Tr(Tr);
			Geom1.Tr(Tr);
			Vp = Vp * Tr;
			Vn = Vn * Tr;
		}
		// XY plane only
		double Sign = ((Geom0.GetEndDir() % Geom1.GetStartDir()).GetZ() > 0.) ? 1. : -1.;
		if (Geom0.IsArc() && Geom1.IsArc())
		{
			BPoint C;
			if (!NCUnitStatePrevGeom::DIntArcArc(Geom0, Geom1, Sign * Size, C))
				return false;
			BPoint V0 = Geom0.GetC() - C;
			double D02 = V0.D2();
			BPoint V1 = Geom1.GetC() - C;
			double D12 = V1.D2();
			if (D02 < MIND * MIND || D12 < MIND * MIND)
				return false;
			Geom.SetType((Sign > 0.) ? ccwarc : cwarc);
			V0 = V0 * (Size * Sign * (Geom0.IsCWArc() ? 1. : -1.) / sqrt(D02));
			V1 = V1 * (Size * Sign * (Geom1.IsCWArc() ? 1. : -1.) / sqrt(D12));
			Geom.SetB(C + V0);
			Geom.SetE(C + V1);
			Geom.SetI(V0 * (-1.));
			Geom.SetPl(XY);
		}
		else
		{
			NCadrGeom &SGeom0 = Geom1.IsArc() ? Geom0 : Geom1;
			NCadrGeom &SGeom1 = Geom1.IsArc() ? Geom1 : Geom0;
			BPoint C;
			if (!NCUnitStatePrevGeom::DIntOtrArc(SGeom0, SGeom1, Sign * Size, Geom1.IsArc(), C))
				return false;
			BPoint Va = SGeom1.GetC() - C;
			double Da2 = Va.D2();
			BPoint Vo = SGeom0.GetStartDir() * BMatr().RotZ(0., 0., 0., 90.);
			double Do2 = Vo.D2();
			if (Do2 < MIND * MIND || Da2 < MIND * MIND)
				return false;
			Geom.SetType((Sign > 0.) ? ccwarc : cwarc);
			Vo = Vo * (-Size * Sign / sqrt(Do2));
			Va = Va * (Size * Sign * (SGeom1.IsCWArc() ? 1. : -1.) / sqrt(Da2));
			Geom.SetB(C + (Geom1.IsArc() ? Vo : Va));
			Geom.SetE(C + (Geom1.IsArc() ? Va : Vo));
			Geom.SetI((Geom1.IsArc() ? Vo : Va) * (-1.));
			Geom.SetPl(XY);
		}

		// END:XY plane only
		if (TmpPlane != XY)
		{
			BMatr Tr = NCUnitState::ChangePlaneMatr(XY, TmpPlane);
			Geom.Tr(Tr);
		}
	}
	else
	{
		if (!Vp.Make1(MIND) || !Vn.Make1(MIND))
			return false;
		if (Size < 0.)
			return false;
		if (GetType() == CHAMFER)
		{
			Geom.SetType(line);
			Geom.SetB(P0 + Vp * Size);
			Geom.SetE(P0 + Vn * Size);
		}
		else if (GetType() == ROUND)
		{
			double c = Vn * Vp;
			double z = 1. - c * c;
			if (z < MIND)
				return false;
			double t = Size / sqrt(z);

			Geom.SetType(((Vp % Vn) * NCadrGeom::NormByPlane(Pl) > 0.) ? cwarc : ccwarc);
			Geom.SetB(P0 + Vp * ((1. + c) * t));
			Geom.SetE(P0 + Vn * ((1. + c) * t));
			Geom.SetI(P0 + (Vn + Vp) * t - Geom.GetB());
			Geom.SetPl(Pl);
		}
	}
	return true;
}