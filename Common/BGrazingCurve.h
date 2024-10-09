#pragma once
#include <vector>
#include "GrazingCurve.h"
#include "BGrazingCurveElemContour.h"

class BMatr;

class AFX_EXT_CLASS BGrazingCurve :
	public GrazingCurve
{
public:
	BGrazingCurve(int k);//kol - количество элементов контура в простом инструменте
	BGrazingCurve(void);
	virtual ~BGrazingCurve(void);
	void StoreInForest(const BMatr &M);
	void RemoveDegraded(void);
	BGrazingCurveElemContour::BSegmentGrazingCurve *GetCont(int iElem, bool Left, bool Top, bool Side) const;
	GrazingCurveElemContour &GetElemCurve(int iElem) const;

	void ConnectCurves(const BGrazingCurve & In, class MeshArr *pMeshArr, INT_PTR ConCompNum) const;
	BGrazingCurveElemContour * GetGCurve(int i) const;//!!!¬озвращает граничную кривую i-го элемента контура
	void CalcSurf(const class NContourOfTool &Contour, const std::vector<int> C2Contour, const BMatrPair &Stock2Tool, const class BScrewMotion &Motion);
	void BMakeCopy(BGrazingCurve &Dest, const BMatr &TrM) const;
protected:
	BGrazingCurveElemContour *BGCurve;
};
