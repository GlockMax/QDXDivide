#include "stdafx.h"
#include "BStl.h"
#include "RDCEL.h"
#include "StlRend.h"

StlRend::StlRend()
{
	pStl = new BStl;
}

StlRend::~StlRend()
{
	delete pStl;
}

bool StlRend::create(const char* filePath)
{
	const bool res = pStl->CreateEx(CString(filePath));
	if(res)
		pStl->MakeRDCEL();
	return res;
}

bool StlRend::create(int facesCount, const float* vert)
{
	const bool res = pStl->CreateFrMem(facesCount, vert);
	if (res)
		pStl->MakeRDCEL();
	return res;
}

bool StlRend::getMesh(double minAngle, int& facesCount, int& stride, std::vector<float>& points, int*& sharpInd)
{
	auto pRDCEL = pStl->GetRDCEL();
	if (pRDCEL == nullptr)
		return false;
	if (pStl->GetTriangles() == nullptr)
		return false;
	pStl->GetDCEL().SetSmoothAngle(minAngle);
	facesCount = int(pStl->GetTriangles()[0]);
	stride = pRDCEL->Stride;
	points.resize(facesCount * 3 * stride);
	for (int i = 0; i < facesCount * 3 * stride; ++i)
		points[i] = pStl->GetTriangles()[1 + i];
	sharpInd = pStl->GetDCEL().FillEdges(true, false, false);
	return true;
}
