#pragma once
#include "NStock.h"
class AFX_EXT_CLASS NStockOrigR : public NStock
{
public:
	NStockOrigR();
	virtual ~NStockOrigR();
	DECLARE_SERIAL(NStockOrigR)

public:
	const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const override;
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSurf) override;
	CString GetFileName() const override;
	bool CreateBodyFrBSP(class MQuadTree *pTree);
	virtual void CreateCopyTo(class NStock* &stock) const override;
	virtual void SetColor(const NColor & Col) override;
	void DeleteOrigRenders();
protected:
	class GBodyRender *pOrigRender;
	class GQuadRenderRot *pOrigRenderRot;
	CString FileName;
};

