#pragma once
#include "NCMB_API.h"
#include <vector>

class BPoint;
class BBox;

class NCMB_API BCurve
{
	class NCMB_API CurveStorage
	{
	public:
		CurveStorage();
		~CurveStorage();
		void AddCurve(const BCurve *pCurve);
		void ClearAll();
		void ClearExt();
		void ClearLastCurve();
		void Close();
	protected:
		std::vector<const BCurve *> MainCurves; // For deletion only
		std::vector<const BCurve *> ExtraCurves; // For deletion only
		bool Closed;
	};
public:
	static BCurve *NewBCurve(const BCurve &In);
	static BCurve *NewBCurve(int SegmNum);
public:
	~BCurve(void);
	bool SetPoint(int i, const BPoint &P);
	void Draw(double t);
	const BPoint & GetLastP() const;
	const BPoint & GetFirstP() const;
	const BPoint GetFirstDir() const;
	const BPoint GetLastDir() const;
	double GetLength(void) const;
	static CurveStorage Storage;
protected:
	int Size;
	BPoint *p;
	BPoint ToolAxisVec;
protected:
	BCurve();
	BCurve(const BCurve &In);
	BCurve(int SegmNum);
public:
	BBox GetGabar(void);
	void Tr(const BMatr & m);
	int GetSize(void) const;
	BPoint * GetSegm(int PointIndex) const;
	const BPoint & GetPoint(int PointIndex) const;
	int GetNumAppr(double Eps) const;
	bool GetLine(int i, class NLine &Line) const;
	int MakeAppr(double Eps, class BPointsBuf &Pts) const;
	void ZeroCurv(bool Start);
	const BPoint &GetToolAxisVec() const { return ToolAxisVec; }
	void SetToolAxisVec(const BPoint &Vec) { ToolAxisVec = Vec; }
	bool RayCast(const BPoint& P, const BPoint& V, double Epsilon, BPoint &Pres) const;
};
