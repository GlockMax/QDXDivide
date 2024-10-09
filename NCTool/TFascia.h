#pragma once

class TBladeCont;
class TBladeEdge;
class BMatr;
class BPoint;
class NColor;

class TFascia
{
public:
	TFascia(void);
	~TFascia(void);
protected:
	double Length;
	double Angle;// Relative to Z axis (-180 : +180) 0 - up; +-180 - down
				//  > 0 inside; < 0 outside
	const TBladeCont *pSrc;// This class dosn't delete contours
	TBladeCont *pDest;
	double XYshift;
	double Zshift;
public:
	TBladeCont * Create(const TBladeCont * pS, double L, double A);
	bool Draw(const NColor &Act, const NColor &Pas);
	void MakeMesh(class MeshArr& RMeshArr);
	bool IsCreated(void);
	void Clear(void);
	TBladeCont * Create(void);
protected:
	const BPoint Normal(TBladeEdge * pEdge);
	const BPoint NormalArc(const BPoint &P, const BPoint &C);
public:
	int NewEdge(const TBladeEdge * pCur, TBladeEdge * pCurD);
	bool DrawLines(void);
};
