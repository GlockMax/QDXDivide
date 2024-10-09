#pragma once
#include "NCMB_API.h"	
#include "ClrPoint.h"	
#include "NCadrGeomArray.h"	
#include "BDCEL.h"	
#include <memory>
#include <vector>
#include <list>
#include <deque>
#include <string>

typedef std::list<ClrPoint> BClrContour;

class NCMB_API BRotSolid
{
protected:
	class NVLine
	{
	public:
		NVLine(double xS, double yS, double xE, double yE, PackedColor Col);
		NVLine(double xS, double xE);
		void SetStart(double xS, double yS);
		void SetEnd(double xE, double yE);
		double GetY(double X) const;
		inline static const double Y_DUMMY = 1.e12;
	public:
		double x0;
		double y0;
		double x1;
		double y1;
		PackedColor Color;
		bool Dummy;
	};
	class Envelope
	{
	public:
		std::list<std::pair<NVLine*, NVLine*>> List;
	public:
		static bool Merge(Envelope& In0, Envelope& In1, Envelope& Out);
		void Expand(double Xstart, double Xend);
		void Add(NVLine* pLine);
		void FreeMem();
	};
public:
	BRotSolid();
	~BRotSolid();
public:
	const double epsilonf = 1e-7;//погрешность на фильтор
	const double epsilon = 1e-4;//kontor

	std::vector<std::vector<ClrPoint>> out;

	std::vector<std::vector<int>> sled;
	std::vector<ClrPoint> AllPoints;
	struct lene
	{
		int start;
		int end;
	};
	std::vector<lene> Lines;

	bool GetFaceOrient(const BDCEL& DCEL, int FaceInd);
	static BPoint GetVel(const BPoint& P);
	void Filter(const BDCEL& DCEL, bool IgnoreVoids);
	void FilterSoup(const BDCEL& DCEL);
	bool CrKontour(BClrContour& Contour);
	bool CrRotKontour(const BDCEL& DCEL, bool IgnoreVoids, BClrContour& Contour);
	static void Contour2GeomArray(const BClrContour& Contour, NCadrGeomArray& GeomArray);
	static bool CrPartEnvelope(const std::vector<ClrPoint>& AllPoints, const std::vector<lene>& Lines, BClrContour& Contour, double Xmin, double Xmax, double Ymin);
	static bool CrEnvelope(const std::vector<ClrPoint>& AllPoints, const std::vector<lene>& Lines, BClrContour& Contour);
	static bool MarkGaps(BClrContour& Contour);
	static bool RotateCont(BClrContour& Kontour, double Angle);
	static bool RotatePoints(std::vector<ClrPoint>& AllPoints, double Angle);


	ClrPoint intersection3d(const BPoint& vec1, const BPoint& vec2, const ClrPoint& SPoint, const ClrPoint& EPoint);
	BPoint crossroad(const BPoint& vec1, const BPoint& vec2, const BPoint& SPoint, const BPoint& EPoint);
	void Normalize(BPoint& N);
	void Sled();
	void IndexPointSled();
	bool CompareZ(int i0, int i1);
	void ZAxisLines(bool IgnoreVoids);
	static double LineArea(const BClrContour& Cont);
	static void PlaceXY(BClrContour& Cont);
	static void Reverse(BClrContour& Cont);
	static bool Make1EnvCont(const BDCEL& DCEL, bool IgnoreVoids, std::list<BClrContour>& Contours);
	static bool MakeEnvConts(BDCEL& DCEL, std::list<BClrContour>& Contours, enum TurnGenMode TGMode);
};
