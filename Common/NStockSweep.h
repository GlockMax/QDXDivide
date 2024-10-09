#pragma once
#include "NStockOrigR.h"

class MeshArr;
class TBladeCont;

class AFX_EXT_CLASS NStockSweep :
	public NStockOrigR
{
public:
	NStockSweep(void);
	~NStockSweep(void);
	DECLARE_SERIAL(NStockSweep)

public:
	void Serialize(CArchive &ar) override;
	void Create(const CString &iFileName, NCadrGeom *GeomArr, int Num, const BPoint &iBase, const BPoint &iVector, const BPoint &ABC);
	bool CreateBody(void) override;
	virtual void CreateCopyTo(class NStock* &stock) const override;

protected:
	BPoint BaseCenter;
	BPoint Vector;
	BPoint ABC;
	TBladeCont Cont;
protected:
	void Create();
	MQuadTree *MakeBSPTree(int Num, bool ForDX5X = false) const;

public:
	const BPoint & GetBase(void);
	void MoveBase(double x, double y, double z);
	const TBladeCont & GetCont() const { return Cont;}
	const BPoint & GetABC() const { return ABC;}
	const BPoint & GetVector() const { return Vector;}
	StockCrType GetType() const override;
};
