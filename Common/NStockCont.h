#pragma once
#include "TBladeCont.h"
#include "NStock.h"

class MeshArr;

class AFX_EXT_CLASS NStockCont :
	public NStock
{
public:
	NStockCont();
	~NStockCont(void);
	DECLARE_SERIAL(NStockCont)

public:
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSurf) override;
	void Serialize(CArchive &ar);
	void Create(NCadrGeom *GeomArr, int Num, const BPoint &iBase, const CString &FileName, bool iFlipXY);
	bool CreateBody(void);
	virtual void CreateCopyTo(class NStock* &stock) const override;

protected:
	BPoint BaseCenter;
	bool Invert;
	bool FlipXY;
	TBladeCont Cont;
	GQuadRenderRot *pOrigRender;
	CString FileName;
protected:
	void Create();

public:
	const BPoint & GetBase(void);
	void MoveBase(double x, double y, double z, bool Invert);
	bool CreateRenderRot(void) override;
	const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const override;
	void SetColor(const NColor & Col) override;
	CString GetFileName(void) const override { return FileName;}
	bool GetInvert() const { return Invert;}
	StockCrType GetType() const override;
	void SetFlipXY(bool Val) { FlipXY = Val; }
	bool GetFlipXY() const { return FlipXY; }
};
