#pragma once
#include "Octree.h"

class OctreeSkel : public Octree<cadrID>
{
public:
	OctreeSkel();
	virtual ~OctreeSkel();
	void Initialize(class CNCMDoc *pDoc, int StPos);
protected:
	const NCadrGeom *GetCadrGeom(cadrID ID, int StPos) override;
protected:
	unsigned int SkelUuid;
};

