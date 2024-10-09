// NCadrGeom.h: interface for the NCadrGeom class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCMB_API.h"
#include "BCadrGeom.h"
#include "NCEnums.h"
#include "curve.h"
#include "BPoint.h"
#include "BTimeLine.h"

#include "CadrID.h"

class BBox;
class BMatr;
class NColor;
class NLine;
class BCurve;
class SMachStateFix1;

class NCMB_API NCadrGeom
{
public:
	class NCMB_API Curve
	{
	public:
		Curve(void) { type = undef; flags = CU_DEF; };
		Curve(enum curve icurve)
		{ 
			if(icurve == fast)
			{
				type = line;
				flags = CU_FAST;
			}
			else
			{
				type = icurve;
				flags = CU_DEF;
			}
		};
		enum curve type:16;
		_int32 flags;
	};
public:
	class CadrTimes
	{
	public:
		CadrTimes();
		~CadrTimes();
		void Clear();
	public:
		MainTime Start;
		MainTime WorkStart;
		MainTime WorkEnd;
		MainTime End;
	};
public:
	bool CalcCurTang(const BMatr & CorrMatr, double t, SMachStateFix1 & P);
	bool CalcCurCoordMS(const BMatr & CorrMatr, double t, SMachStateFix1 & P) const;
	void CalcCurCoord(double t, BPoint &XYZ, BPoint &ABC) const;
	void CalcCurCoordMCS(double t, const BMatr &MCS, BPoint &XYZ, BPoint &ABC) const;
	bool IsFast() const;
	bool IsPolar(void) const;
	bool IsCylindric(void) const;
	bool IsGeomValid(double eps) const;
	bool IsVertical(void) const;
	bool IsVerticalUp(void) const;
	bool IsHorizontal(void) const;
	bool IsLine(void) const;
	bool IsArc(void) const;
	bool IsFullArc(double eps) const;
	bool IsFull3DArc(double eps) const;
	bool Is3DArc(void) const;
	bool IsNotXYArc(void) const;
	bool IsSpline(void) const;
	bool IsMachConfCh(void) const;
	BPoint GetStartDir(void) const;
	BPoint GetStartDirP(void) const;
	BPoint GetEndDir(void) const;
	double GetBY() const { return yb; }
	BPoint GetB() const;
	BPoint GetE() const;
	BPoint GetBP() const;
	BPoint GetEP() const;
	BPoint GetN() const;
	BPoint GetI() const;
	BPoint GetC() const;
	void GetArcA(double *px,double *py,double *pz) const;
	void GetA(double *px,double *py,double *pz) const;
	double GetR() const;
	void GetC(double *px,double *py,double *pz) const;
	void Move(BPoint r);
	void GetN(double *px,double *py,double *pz) const;
	void GetI(double *pi,double *pj,double *pk) const;
	Curve GetType() const;
	const NCadrGeom & Tr(const BMatr &m);
	const NCadrGeom &SetX(double Val);
	const NCadrGeom &SetY(double Val);
	const NCadrGeom &SetZ(double Val);
	double Angle(const NCadrGeom &PrevCadr) const;
	enum Plane GetPlane() const;
	void SetAttr(unsigned char val);
	void SetPolar(void);
	void SetCylindric(void);
	unsigned short GetAttr() const noexcept;
	bool HaveGeom() const;
	bool HaveGeom5() const;
	void ExpandWorkBox(BBox *pBox) const;
	void ExpandBox(BBox *pBox) const;
	void Clear();
	void ShiftZ(double s);
	void Offset(enum LR,double d);
	bool SetABSIJ(enum Plane pl);
	double Length() const;
	double GetRotSpecLength() const;
	double Deflect() const;
	void GetB(double *px,double *py,double *pz) const;
	void GetE(double *px,double *py,double *pz) const;
	void Set(Curve type,
		double xb,double yb,double zb,
		double xe,double ye,double ze,
		double i,double j,double k,
		enum Plane pl);
	void Set(Curve type,
		double xb,double yb,double zb,
		double xe,double ye,double ze);
	void Set(Curve type, const BPoint &Pb, const BPoint &Pe);
	void SetE(double xei,double yei,double zei);
	void SetN(double xni,double yni,double zni);
	void SetI(double xii,double yii,double zii);
	void SetB(double xbi, double ybi, double zbi);
	void SetE(const BPoint &);
	void SetN(const BPoint &);
	void SetI(const BPoint &);
	void SetB(const BPoint &);
	void SetType(Curve type);
	void SetType(enum curve icurve);
	const NCadrGeom &TrEndPoint(const BMatr &m);
	static void FitCadrs(NCadrGeom *pPrev, NCadrGeom *pCur, bool BiArc);
	void ShiftTime(MainTime Shift);
	void ShiftStartTime2(MainTime Time);
	void ShiftWorkTime2(MainTime Time);
	void SetWorkTime(MainTime WorkTime);
	void SetWorkTimeS(double WorkTimeS);
	bool IsTimeDefined() const;
	double GetWorkTimeS() const;
	double GetWholeTimeS() const;
	double GetStartTimeS() const;
	MainTime GetWorkTime() const;
	MainTime GetWholeTime() const;
	MainTime GetWaitTime0() const;
	MainTime GetWaitTime1() const;
	MainTime GetEndTime() const;
	MainTime GetStartTime() const;
	MainTime GetMidTime(double t) const;// t is part of work time
	void SetEndTime(MainTime EndTime);
	void SetPl(enum Plane Pl);
	NCadrGeom();
	~NCadrGeom();

//protected:
	static void Line(double xb, double yb, double zb, double xe, double ye, double ze, bool fast);
	double xb;//начальная 
	double yb;
	double zb;
	double xe;//Конечная точка
	double ye;
	double ze;
	double i;//вектор из начала в центр
	double j;
	double k;
	double xn;//нормаль к плоскости
	double yn;
	double zn;
	enum  Plane pl;//плоскость, в которой лежит элемент
	BCurve * pCurve;
	Curve type;//тип линии
	float LengthComp;
	unsigned short MatrNum;
	unsigned short InterpNum;
	unsigned short MSHistNum;
	unsigned short StockPos;
	BMTLinkageDiff MTLink;
	unsigned int TimeInd;
	unsigned char Attr;// 0-default;1-cycle; 2-spindle off;3 - разметка; 4 - кернение; 5 - поворот;6 - без обработки(TURN only); 7 - резание только в конце(TURN only)
protected:
	CadrTimes Times;
	cadrID HistID;
public:
	bool CrArc(const BPoint & P0, const BPoint & P1, const BPoint & e0);
	int GetMatrNum(void) const;
	int GetMSHistNum(void) const;
	void SetMatrNum(int n);
	void SetMSHistNum(INT_PTR n);
	void SetInterpNum(int n);
	int GetInterpNum(void) const;
	const NCadrGeom & Reverse(void);
	bool MoveToXY(void);
	bool IsInXYPlane(void);
	const NCadrGeom & ProjXY(void);
	const NCadrGeom& FlipXY(void);
	const NCadrGeom & MakeHorizontal(void);
	const NCadrGeom & MakeHorizontal(double inZ);
	const NCadrGeom & MakeVertical(void);
	bool Draw(double Eps, double t) const;
	bool DrawCyl(double Eps, double t) const;
	bool DrawOrd(double Eps, double t) const;
	static void RestoreLineFont();
	static void SetLineFont(int mask = 0xf0f0);
	static void FastLine(double xb, double yb, double zb, double xe, double ye, double ze);
	static void LineSimple(double xb, double yb, double zb, double xe, double ye, double ze);
	static int compare_SPY(const void *a, const void *b);
	bool IsCycle(void) const noexcept;
	static enum Plane PlaneByNorm(const BPoint & N);
	static const BPoint NormByPlane(enum Plane Pl);
	static NColor FastMovCol;
	bool Draw5x(double Eps, double t) const;
	static double TotalAngle(const NCadrGeom* GeomArr, int Num);
	static int IsConvexGeom(const NCadrGeom* GeomArr, int Num);
	void SetCurve(BCurve *pCurve);
	BCurve * GetCurve(void) const;
	NCadrGeom(const NCadrGeom & In);
	const NCadrGeom & operator =(const NCadrGeom & In);
	const NCadrGeom & FullCopy(const NCadrGeom & In);
	bool GetStartABC(BPoint &ABC);
	bool GetEndABC(BPoint &ABC);
	void GetDeltaABC(BPoint &ABC);
	int GetNumAppr(double Eps) const;
	bool IsCWArc() const { return type.type == cwarc;}
	bool IsCCWArc() const { return type.type == ccwarc;}
	bool IsRotate() const { return type.type == ::rotate;}
	bool IsUndef() const { return type.type == undef; }
	bool IsRCP() const { return type.flags & (CU_RCP0 | CU_RCP1 | CU_RCP2); }
	int GetRCPMask() const { return ((type.flags & CU_RCP0) ? 1 : 0) + ((type.flags & CU_RCP1) ? 2 : 0) + ((type.flags & CU_RCP2) ? 4 : 0); }
	bool IsZeroTime() const { return type.type == undef && GetWholeTime() == 0; }
	void SetFast(bool fast);
	void SetSpline(bool spline);
	void SetGrooveFlag(bool IsGroove);

	void SetFlags(_int32 flags) { type.flags = flags; }
	bool IsFull5x(void) const;
	bool Is5x(void) const;
	double GetVolumeRot(void) const;
	int GetPolyLine(class BPointsBuf &Pts, double Eps, bool UseMatr) const;
	int GetPolyLineSize(double Eps);
	bool IsGroove(void) const;
	static int Divide(NCadrGeom & Source, double Angle, NCadrGeom & Dest);
	static int DivideX(NCadrGeom & Source, double XValue, NCadrGeom & Dest);
	static int DivideT(NCadrGeom& Source, double TValue, NCadrGeom& Dest);
	static double DivideTByTime(NCadrGeom& Source, double TValue, NCadrGeom& Dest);
	static void DivideWorkTime(NCadrGeom& Source, double XValue, NCadrGeom& Dest);
	int DivideQuad(NCadrGeom *& pGeomArr) const;
	int CalcQuadsNum(void) const;
	bool IsNormalStandard(void) const;
	void Serialize(CArchive &ar);
	void ApplyMatr(void);
	bool LiesOnSameCurve(const NCadrGeom &In);
	bool IsPlaneCorrect(void) const;
	BPoint RayCast(const BPoint& P, const BPoint& V, double Epsilon) const;
	BMTLinkage GetMTLink() const;
	BMTLinkageDiff GetMTLinkChange() const { return MTLink; }
	void SetMTLink(const BMTLinkageDiff& Link) { MTLink = Link; }
	int GetStockPos() const { return StockPos; }
	void SetStockPos(int SP) { StockPos = unsigned short(SP); }
	bool RepairArc();
	int CreateRArcXY(double x1, double y1, double z1,
		double x2, double y2, double z2, double R, curve CurveType, double MinArcDist);
	double RecalcT2Geom(double tTime) const;
	void SetTimeInd(unsigned int Ind) { TimeInd = Ind; }
	auto GetTimeInd() const noexcept { return TimeInd; }
	void SetHistID(cadrID ID) { HistID = ID; }
	cadrID GetHistID() const { return HistID; }
	static void FlipXY(NCadrGeom* GeomArray, int Size);
	void SetLCompFlags(bool Beg, bool End);
	bool HaveLComp() const;
	bool IsLCompBeg() const;
	bool IsLCompEnd() const;
	double GetCadrLength(const class SMachState* pStartSt, const class SMachState* pEndSt) const;
	void SetLengthComp(float Val) { LengthComp = Val; }
	double GetLengthComp() const { return LengthComp; }
	bool ApplyLCompSimple(const BPoint& ShiftVect);
	void Divide3DArc(BGeomArray& Array) const;
	static void GetCadrArray4Helix(const NCadrGeom* pGeom, BGeomArray& Cadrs);

private:
	BPoint LineRayCast(const BPoint& P, const BPoint& V, double Epsilon) const;
	BPoint CurveRayCast(const BPoint& P, const BPoint& V, double Epsilon) const;
	BPoint ArcRayCast(const BPoint& P, const BPoint& V, double Epsilon) const;
	BPoint ArcRayCastOnPlane(const BPoint& P, const BPoint& V, double Epsilon) const;
	bool IsPointOnArc(const BPoint& point) const;
public:
	static constexpr double Sec2Tick = 100000.;
	static constexpr double Tick2Sec = 0.00001;
};
