#include "stdafx.h"
#include <future>
#include "DXMaps.h"

DXMaps::DXMaps(int nX, int nY)
{
	n0 = nX;
	n1 = nY;
	Line0 = new DXMap* [n0];
	DXMap* Stor = new DXMap[n0 * n1];
	for (int i = 0; i < n0; ++i)
		Line0[i] = Stor + i * n1;
}

DXMaps::~DXMaps()
{
	delete[] Line0[0];
	delete[] Line0;
}

void DXMaps::MakeTrianglesPar() const
{
	if (BDebugState::Get().Async)
	{
		std::vector<std::future<void>> Futures;
		Futures.reserve(n0 * n1);
		for (int B0 = 0; B0 < n0; ++B0)
			for (int B1 = 0; B1 < n1; ++B1)
			{
				Futures.push_back(std::async(std::launch::async, [this](int I0, int I1) { (*this)[I0][I1].MakeTrianglesBigColumn(); }, B0, B1));
			}
		for (int i = 0; i < Futures.size(); ++i)
			Futures[i].get();
	}
	else
	{
		for (int B0 = 0; B0 < n0; ++B0)
			for (int B1 = 0; B1 < n1; ++B1)
				(*this)[B0][B1].MakeTrianglesBigColumn();
	}
}

