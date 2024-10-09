#include "stdafx.h"
#include "ResLocal.h"
#include "NCadrGeom.h"
#include "BMatr.h"
#include "N_dl_import.h"

N_dl_import::N_dl_import() 
	: PrevPoint(0., 0., 0., -1.)
{
	PolylineFinished = false;
	PolylinePresent = false;
	LineArcPresent = false;
	ErrorCode = 0;
}

void N_dl_import::addLine(const DL_LineData& data)
{
	if (PolylinePresent && !PolylineFinished)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return;
	}
	if (PolylineFinished)
		return;
	LineArcPresent = true;

	NCadrGeom Cadr;
	Cadr.Set(line, data.x1, data.y1, 0., data.x2, data.y2, 0.);
	ContCadrs.push_back(Cadr);
}

void N_dl_import::addArc(const DL_ArcData& data)
{
	if (PolylinePresent && !PolylineFinished)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return;
	}
	if (PolylineFinished)
		return;
	LineArcPresent = true;

	BPoint C(data.cx, data.cy, 0., 1.);
	BPoint Vs(data.radius, 0., 0., 0.);
	BPoint P0 = C + Vs * BMatr().RotZ(0., 0., 0., data.angle1);
	BPoint P1 = C + Vs * BMatr().RotZ(0., 0., 0., data.angle2);
	NCadrGeom::Curve CurveType = ccwarc;
	if (getExtrusion()->getDirection()[2] < 0.)
	{
		CurveType = cwarc;
		BMatr M;
		M.RotY(0., 0., 0., 180.);
		C = C * M;
		P0 = P0 * M;
		P1 = P1 * M;
	}
	BPoint I = C - P0;
	NCadrGeom Cadr;
	Cadr.Set(CurveType, P0.GetX(), P0.GetY(), P0.GetZ(), P1.GetX(), P1.GetY(), P1.GetZ(), I.GetX(), I.GetY(), I.GetZ(), XY);
	ContCadrs.push_back(Cadr);
}

void N_dl_import::addPolyline(const DL_PolylineData& data)
{
	//if (LineArcPresent)
	//{
	//	ErrorCode = IDS_STATE_NOTE_Electr;//INSERT ID
	//	return;
	//}
	if (PolylinePresent)
	{
//		ErrorCode = IDS_STATE_NOTE_Electr;//INSERT ID
		return;
	}
	PolylinePresent = true;
	ContCadrs.clear();
}

void N_dl_import::addVertex(const DL_VertexData& data)
{
	//if (LineArcPresent)
	//{
	//	ErrorCode = IDS_STATE_NOTE_Electr;//INSERT ID
	//	return;
	//}
	if (!PolylinePresent)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return;
	}
	if (PolylineFinished)
		return;
	if (!PrevPoint.IsPoint())
	{
		PrevPoint.Set(data.x, data.y, data.bulge, 1.);
		return;
	}
	NCadrGeom Cadr;
	if (PrevPoint.GetZ()/* bulge */ == 0.) 
	{// Line
		Cadr.Set(line, PrevPoint, BPoint(data.x, data.y, 0., 1.));
	}
	else
	{// Arc
		const double PI = 3.1415926535897932384626433832795;
		BPoint C;
		BPoint P0(PrevPoint.GetX(), PrevPoint.GetY(), 0., 1.);
		BPoint P1(data.x, data.y, 0., 1.);
		BPoint V0 = P1 - P0;
		double d = sqrt(V0.D2());
		V0 = V0 * (1. / d);
		double conerArad = 4. * tanh(PrevPoint.GetZ() /* bulge */);
		double conerBrad = (PI / 2.) - (fabs(conerArad) / 2.);
		double Radius = sqrt((d * d) / (2. - 2. * cos(fabs(conerArad))));
		BMatr matrix;
		double conerB = 180. * conerBrad / PI;
		if (PrevPoint.GetZ() /* bulge */ < 0.)
			conerB = -conerB;
		matrix.RotZ(P0, conerB);
		V0 = V0 * matrix;
		C = P0 + (V0 * Radius);
		if (getExtrusion()->getDirection()[2] < 0.)
		{
			BMatr M;
			M.RotY(0., 0., 0., 180.);
			C = C * M;
			P0 = P0 * M;
			P1 = P1 * M;
		}
		V0 = C - P0;
		NCadrGeom::Curve CurveType = ccwarc;
		if ((PrevPoint.GetZ() /* bulge */ < 0.) != (getExtrusion()->getDirection()[2]) < 0.)
			CurveType = cwarc;
		Cadr.Set(CurveType, P0.GetX(), P0.GetY(), 0., P1.GetX(), P1.GetY(), 0., V0.GetX(), V0.GetY(), 0., XY);
	}
	ContCadrs.push_back(Cadr);
	PrevPoint.Set(data.x, data.y, data.bulge, 1.);
}

void N_dl_import::endSequence()
{
	if (!PolylinePresent)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return;
	}
	PolylineFinished = true;
}
