#pragma once
#include "stdafx.h"
#include "NCMB_API.h"

class NCMB_API BImageRGB
{
	friend class CNCMHDoc;
public:
	BImageRGB();
	~BImageRGB();
	void Clear();
	void Set(int mem, int ix, int iy, BYTE* pData);
	void Serialize(CArchive& ar);
protected:
	const int PixelSize = 3;// in bytes (don't change)
	int xSize;
	int ySize;
	int memSize;
	BYTE* pImage;
};

