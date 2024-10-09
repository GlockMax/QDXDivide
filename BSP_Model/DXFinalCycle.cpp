#pragma once
#include "StdAfx.h"
#include "DXFinalCycle.h"

// ����� ������� ��������� ����� � ������
int DXFinalCycle::FirstTrue() const
{
	for (int i = 0; i < Size; ++i)
	{
		if (Cycle[i].BoundOrEdge)
			return i;
	}
	return -1;
}