#pragma once
#include "BBox.h"
class MBodyParams
{
public:
	MBodyParams(void);
	~MBodyParams(void);
	void Init();
	const MBodyParams &operator += (const MBodyParams &BP);

	double Volume;
	double SurfArea;
	BBox Bounds;
};
