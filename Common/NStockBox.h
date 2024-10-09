// NStockBox.h: interface for the NStockBox class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NStock.h"
#include "BBox.h"

class AFX_EXT_CLASS NStockBox : public NStock  
{
public:
//	CString GetType();
	void Serialize(CArchive &ar);
	NStockBox();
	bool Create(BBox gabar);
	virtual ~NStockBox();
	DECLARE_SERIAL(NStockBox)
	virtual void CreateCopyTo(class NStock* &stock) const;

protected:
	void Create();
	BBox Box;
protected:
	class MQuadTree *MakeBSPTree(int Num, bool ForDX5X = false) const;

public:
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf) override;
	virtual bool CreateBody(void);
	virtual const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
	void Transformm(const BMatr & M) override;
	bool MakeTurnFromMillBox(void);
	StockCrType GetType() const override;
};
