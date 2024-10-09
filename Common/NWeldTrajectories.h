#pragma once
#include "BPoint.h"
#include "NColor.h"
#include "NCadrGeom.h"
#include "BPointsBuf.h"
#include "RElemArray.h"
#include "BProgVisInterval.h"

class NWeldTrajectories
{
public:
	NWeldTrajectories(const class NProgramWeld &Prog);
	virtual ~NWeldTrajectories(void);
	INT_PTR AddFour(const BPoint &PFirst, const BPoint &PSecond, const BPoint &PDelta, const BPoint &PRoot);
	void SetColors(const NColor &F, const NColor &S, const NColor &R, const NColor &B) { FirstColor = F; SecondColor = S; RootColor = R; BeamVColor = B;};
	void GetColors(NColor &F, NColor &S, NColor &R, NColor &B) const { F = FirstColor; S = SecondColor; R = RootColor; B = BeamVColor;};
	void SetVis(bool F, bool S, bool R, bool B) { FirstVis = F; SecondVis = S; RootVis = R; BeamVVis = B;};
	void GetVis(bool &F, bool &S, bool &R, bool &B) const { F = FirstVis; S = SecondVis; R = RootVis; B = BeamVVis;};
	void SetThick(double F, double S, double R, double B) { FirstThick = F; SecondThick = S; RootThick = R; BeamVThick = B;};
	void GetThick(int &F, int &S, int &R, int &B) const { F = int(FirstThick - 1.); S = int(SecondThick - 1.); R = int(RootThick - 1.); B = int(BeamVThick - 1.);};
	bool DrawInt(bool WithNames, const std::vector<BProgVisInterval> & VisInt, int BoldCadr = -1, double t = -1.);
	bool AddElem(const BPointsBuf &Pts1, const BPointsBuf &Pts2, const BPointsBuf &Pts3, const BPointsBuf &Pts4, int FirstName, int SecondName, int k, const NColor &Col);
	bool Create(int ElemsNum, const NColor &CBold);
	bool Close(void);
	void SetTolerance(double Tol) { Tolerance = Tol;};
	int GetCadrLenghs(int AbsCadrNum, double &FirstL, double &SecondL, double &RootL, double &PenDepth) const;
	void Serialize(CArchive& ar);
	BBox GetGabar(void);
	void DrawPenetration(int Cadr, double t);
protected:
	void DrawWithNames(INT_PTR iFirst, INT_PTR iLast);
	void DrawWithoutNames(INT_PTR iFirst, INT_PTR iLast);
	void DrawPenCyl(INT_PTR iFirst, double t);
	void DrawPart(bool WithNames, INT_PTR iFirst, INT_PTR iLast);
	void DrawElemPart(INT_PTR iElem, double t);

protected:
	typedef CArray <BPoint, const BPoint &> PointsArray;
	PointsArray First;
	PointsArray Second;
	PointsArray Root;
	PointsArray Delta;
	NColor FirstColor;
	NColor SecondColor;
	NColor RootColor;
	NColor BeamVColor;
	bool FirstVis;
	bool SecondVis;
	bool RootVis;
	bool BeamVVis;
	double FirstThick;
	double SecondThick;
	double RootThick;
	double BeamVThick;

	RElemArray RElems;
	int VertInd;
	int ElemsInd;
	class NColor ColorBold;
	double Tolerance;
};
