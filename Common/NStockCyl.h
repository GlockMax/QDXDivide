// NStockCyl.h: interface for the NStockCyl class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NStock.h"

class AFX_EXT_CLASS NStockCyl : public NStock  
{
public:
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf) override;
//	CString GetType();
	void Create(double X,double Y,double Z,double D,double H,int Axis);
	void Serialize(CArchive &ar);
	NStockCyl();
	virtual ~NStockCyl();
	DECLARE_SERIAL(NStockCyl)
	virtual void CreateCopyTo(class NStock* &stock) const;

protected:
	void Create();
	virtual bool CreateBody(void);
	double x,y,z,d,h;
	// 0 - X; 1 - Y ; 2 - Z
	int axis;
	const BMatr GetTransfMatr(void) const;
	virtual const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
	void Transformm(const BMatr & M) override;
	StockCrType GetType() const override;
};
