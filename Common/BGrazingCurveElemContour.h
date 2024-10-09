#pragma once
#include "ntrimesh.h"
#include "grazingcurveelemcontour.h"

class BMatr;

class AFX_EXT_CLASS BGrazingCurveElemContour
{

public:
	class BSegmentGrazingCurve : public CUIntArray
	{
	public:
		BSegmentGrazingCurve() : CUIntArray(), SurfNum(-1) {}
		void SetColor(bool Col) { color = Col; }
		bool GetColor(void) const { return color; }
	private:
		bool color;//false-зеленый сегмент, true-красный сегмент
		int SurfNum;
	};
	BGrazingCurveElemContour(void);
	~BGrazingCurveElemContour(void);
	void StoreInForest(const GrazingCurveElemContour &Elem, int &LPNColorFalse, int &LPNColorTrue, bool First, const BMatr &M);
	void Copy(BGrazingCurveElemContour & bgc, bool f = false);
	BSegmentGrazingCurve *GetCont(bool Left, bool Top, bool Side);
	void MakeCopy(BGrazingCurveElemContour &Dest, const BMatr &M);
	void Clear(void);

protected:
	static int StoreSegmInForest(const SegmentGrazingCurve &Segm, BSegmentGrazingCurve *&PtsNums, int FirstPointNum, bool &First, const BMatr &M);
	static void MakeCopy(const BSegmentGrazingCurve *pSource, BSegmentGrazingCurve *&pDest, const BMatr &M);
	static void Connect(const BSegmentGrazingCurve *pSource, BSegmentGrazingCurve *pDest, bool Invert, NTriMesh *pMesh);
	static const SegmentGrazingCurve &GetByColor(bool Color, const SegmentGrazingCurve &Left, const SegmentGrazingCurve &Right);
	static bool IsColorRight(const SegmentGrazingCurve &Left, const SegmentGrazingCurve &Right);
protected:
	BSegmentGrazingCurve *Isgcleft,
		*Isgcright,
		*Isgcleft1,
		*Isgcright1,
		*Isgcldown,
		*Isgcrdown,
		*Isgclup,
		*Isgcrup;
public:
	static void RemoveDegraded(GrazingCurveElemContour & Segm);
	static void RemDegraded(SegmentGrazingCurve & Segm);

	void ConnectElems(const BGrazingCurveElemContour & In, NTriMesh *pMesh) const;
//	double CallLong(bool Left, bool Top, bool Side);//!!!
//	double CallLongi(bool Left, bool Top, bool Side, int i);//!!!
	static void ApproxQuadrangle(int iP0, int iP1, int iP2, int iP3, NTriMesh *pMesh, double Eps);
	void CalcSurf(const BMatrPair &Stock2Tool, const class BScrewMotion &Motion);
};
