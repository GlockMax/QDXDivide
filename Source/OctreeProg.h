#pragma once
#include "Octree.h"
class OctreeProg :
	public Octree<cadrID>
{
public:
	OctreeProg();
	virtual ~OctreeProg();
	void Initialize(class CNCMDoc *pDoc, int StPos);
protected:
	const NCadrGeom *GetCadrGeom(cadrID ID, int StPos) override;
};

