#pragma once
#include "nsectionset.h"

class AFX_EXT_CLASS NSectionSetDyn :
	public NSectionSet
{
public:
	NSectionSetDyn(void);
	~NSectionSetDyn(void);
	void Draw(bool EntrPoints = true, bool ExitPoints = true) const;
	void DrawPoints(const BPoint Points[]) const;
	virtual int MakeAllSections(void);
	const BPoint & GetN(void) const;
	const BPoint & GetP(void) const;
	void SetPosition(const class BPoint &iP, const class BPoint &iN, const class BPoint &iV);
	const BBox GetGabar(void) const;


protected:
	BPoint Entr[MAX_SECTIONS];
	BPoint Exit[MAX_SECTIONS];
	BPoint RayDir;
};
