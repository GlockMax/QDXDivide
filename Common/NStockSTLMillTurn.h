#pragma once
#include "TBladeCont.h"
#include "NStockSTL.h"

class MeshArr;
class TBladeCont;

class AFX_EXT_CLASS NStockSTLMillTurn :
	public NStockStl
{
public:
	NStockSTLMillTurn(void);
	~NStockSTLMillTurn(void);
	DECLARE_SERIAL(NStockSTLMillTurn)

public:
	void Serialize(CArchive &ar);
	bool CreateBody(void);
	void CreateCopyTo(class NStock* &stock) const override;

protected:
	BPoint BaseCenter;
	bool Invert;
	GQuadRenderRot *pOrigRender;
	CString Name;
protected:
	void Create();
	virtual void Recalc(void) override;

public:
	bool CreateRenderRot(void);
	virtual void SetColor(const NColor & Col);
	const CString &GetName(void) const { return Name;}
	bool GetInvert() const { return Invert;}
	StockCrType GetType() const override;
};
