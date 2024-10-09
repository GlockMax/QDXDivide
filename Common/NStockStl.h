// NStockStl.h: interface for the NStockStl class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NStock.h"
#include "BPoint.h"	
#include "BStl.h"	

class AFX_EXT_CLASS NStockStl : public NStock 
{
public:
//	CString GetType();
	const BPoint & GetBase();
	CString GetFileName() const override;
	void MoveBase(double x, double y, double z);
	virtual void CreateEx(const CString &name);
	void Serialize(CArchive &ar);
	NStockStl();
	virtual ~NStockStl();
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf) override;
	virtual void CreateCopyTo(class NStock* &stock) const;

protected:
	BStl Stl;
	virtual void Create();
	virtual bool CreateBody(void);
	virtual void Recalc(void);
	DECLARE_SERIAL(NStockStl)
public:
	const GLfloat * GetTriangles(void) const;
	void WriteSTL(CStdioFile& f);
	const BPoint &GetOrigZero(void) const { return Stl.GetOrigZero(); };
	void Transformm(const BMatr & M) override;
	virtual GLfloat *MakeOneSection(const BPoint &P, const BPoint &V, bool Trimed = false) const;
	virtual void DrawSelected(void) const;
	virtual const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
    bool FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr) override;
	BStl & GetSTL(void) { return Stl; }
	const BStl & GetSTL(void) const { return Stl; }
	virtual void SetDrawSmoothSurf(bool SmoothSurf);
	virtual void SetRMode(RenderMode Mode);
	const CTime &GetFileTime(void) { return Stl.GetFileTime();}
	void ResetMatr(void) { NMovableObj::ResetMatr(); Stl.MoveBase(0., 0., 0.);}
	StockCrType GetType() const override;
	void SetEdgesAttrs(const class BEdgesAttrs &Attr) override;
};
