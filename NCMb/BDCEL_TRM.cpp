#include "stdafx.h"
#include "BDCEL_TRM.h"


BDCEL_TRM::BDCEL_TRM() : BDCEL()
{
	TRMMode = false;
}


BDCEL_TRM::~BDCEL_TRM()
{
}

void BDCEL_TRM::Switch2TRMMode()
{
	Pointsv.resize(GetVertexCount() * SOURCE_VERTEX_SIZE);
	for (int i = 0; i < GetVertexCount(); ++i)
	{
		for (int l = 0; l < SOURCE_VERTEX_SIZE; ++l)
			Pointsv[SOURCE_VERTEX_SIZE * i + l] = (Get3fvByVn(i) - 3)[l];
		Vertexes[i][2] = i;// we can't do it before Get3fvByVn
	}
	TRMMode = true;// we can't do it before Get3fvByVn
	// we can not restore from TRMMode. 
	pC = Pointsv.data();
}

int BDCEL_TRM::AddPoint(float x, float y, float z, float xn, float yn, float zn, float color)
{
	if (!TRMMode)
		return -1;
	auto Size = Pointsv.size();
	Pointsv.resize(Size + SOURCE_VERTEX_SIZE);
	Pointsv[Size + 0] = xn;
	Pointsv[Size + 1] = yn;
	Pointsv[Size + 2] = zn;
	Pointsv[Size + 3] = x;
	Pointsv[Size + 4] = y;
	Pointsv[Size + 5] = z;
	if(SOURCE_VERTEX_SIZE > 6)
		Pointsv[Size + 6] = color;
	pC = Pointsv.data();
	return int(Pointsv.size()) / SOURCE_VERTEX_SIZE - 1;// the new point number
}

int BDCEL_TRM::DivideEdges(double MaxEdgeLength, int MaxDivNum)
{

	for (int LongEdgeInd = FindLongestEdgeLocal(MaxEdgeLength, 0), i = 0; LongEdgeInd >= 0 && i < MaxDivNum; ++i, LongEdgeInd = FindLongestEdgeLocal(MaxEdgeLength, LongEdgeInd + 2))
	{
		BPoint Mid = GetEdgeMidPoint(LongEdgeInd);
		int PointInd = AddPoint(float(Mid.GetX()), float(Mid.GetY()), float(Mid.GetZ()), 0., 0., 1., 0.);
		if (PointInd >= 0)
			Divide1Edge(LongEdgeInd, PointInd);
	}
	return 0;
}

