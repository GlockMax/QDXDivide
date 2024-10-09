#pragma once
#include "NStock.h"

class MeshArr;

class AFX_EXT_CLASS NStockBar :
	public NStock
{
public:
	NStockBar(void);
	~NStockBar(void);
	DECLARE_SERIAL(NStockBar)

public:
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSurf) override;
	void Serialize(CArchive &ar);
	void Create( int iPolyNum, double iExtRad0, double iExtRad1
		, double iIntRad0, double iIntRad1
		, double iLength
		, double iRotAngle
		, const BPoint &iBaseCenter
		, int axis);
	bool CreateBody(void);
	virtual void CreateCopyTo(class NStock* &stock) const;

	int PolyNum;// A number of polygon edges (circle if 0)
	double ExtRad0, ExtRad1;//circumcircle rad for external contour
	double IntRad0, IntRad1;//circumcircle rad for internal contour
	double Length;// Along Y axis
	double RotAngle;// Rotation angle around Y axis
	BPoint BaseCenter;
	int Axis;

protected:
	void DrawConePipe(enum RenderMode GlobRMode);
	void DrawPolyPipe(enum RenderMode GlobRMode);
	void Create();
	bool MakePolyPipe(void);
	bool MakePolyPipeMesh(MeshArr &MeshArray);
	const BMatr GetTransfMatr(void) const;

	void Transformm(const BMatr & M) override;

public:
	bool FillPN(int * pPointsNum, BPoint * Points, BPoint * Normals);
	bool IsCone(void);
	virtual const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
	StockCrType GetType() const override;
};
