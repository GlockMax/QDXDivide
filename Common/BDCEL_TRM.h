#pragma once
#include "NCMB_API.h"
#include "BDCEL.h"
#include <vector>

class NCMB_API BDCEL_TRM :
	public BDCEL
{
public:
	BDCEL_TRM();
	virtual ~BDCEL_TRM();

	void Switch2TRMMode();
	int AddPoint(float x, float y, float z, float xn, float yn, float zn, float color);// xyz - point, (xyz)n - normal
	int DivideEdges(double MaxEdgeLength, int MaxDivNum);
protected:
	std::vector<float> Pointsv;
};

