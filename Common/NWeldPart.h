#pragma once
#include "NSectArrParams.h"
#include "NSectionSet.h"
#include "NStockStl.h"

class AFX_EXT_CLASS NWeldPart :
	public NStockStl
{
public:
	NWeldPart(void);
	virtual ~NWeldPart(void);
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSurf) override;
	virtual void Create();
	virtual void SetColor(const NColor & Col);
	void Serialize(CArchive &ar);
	NSectArrParams &GetSectParams();
	void MakeStockSections();
	bool GetVisibleSect(void) { return VisibleSect;}
	void SetVisibleSect(bool Vis) { VisibleSect = Vis;}
	void Transformm(const BMatr & M) override;
	virtual void CreateCopyTo(class NStock* &stock) const;
	StockCrType GetType() const override;
protected:
	NSectionSet SectionSet;
	bool VisibleSect;
	NSectArrParams SectArrParams;
	DECLARE_SERIAL(NWeldPart)
};
