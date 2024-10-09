#include "stdafx.h"
#include "zlib.h"
#include "BImageRGB.h"

BImageRGB::BImageRGB()
{
	memSize = 0;
	xSize = 0;
	ySize = 0;
	pImage = nullptr;
}

BImageRGB::~BImageRGB()
{
	delete[] pImage;
}

void BImageRGB::Clear()
{
	delete[] pImage;
	pImage = nullptr;
	memSize = 0;
	xSize = 0;
	ySize = 0;
}

void BImageRGB::Set(int mem, int ix, int iy, BYTE* pData)
{
	memSize = mem;
	xSize = ix;
	ySize = iy;
	pImage = pData;
}

void BImageRGB::Serialize(CArchive& ar)
{
	SerializeElements(ar, &memSize, 1);
	SerializeElements(ar, &xSize, 1);
	SerializeElements(ar, &ySize, 1);
	if (ar.IsStoring())
	{
		ULONG DestLen = compressBound(memSize);
		BYTE* pDest = new BYTE[DestLen];
		compress2(pDest, &DestLen, pImage, memSize, Z_DEFAULT_COMPRESSION);
		SerializeElements(ar, &DestLen, 1);
		ar.Write(pDest, DestLen);
		delete[] pDest;
	}
	else
	{
		delete[] pImage;
		pImage = new BYTE[memSize];
		ULONG DestLen = 0;
		SerializeElements(ar, &DestLen, 1);
		BYTE* pDest = new BYTE[DestLen];
		ar.Read(pDest, DestLen);
		ULONG Size = memSize;
		uncompress(pImage, &Size, pDest, DestLen);
		delete[] pDest;
	}
}
