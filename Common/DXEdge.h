#pragma once
#include "StdAfx.h"
#include "DXMemory.h"
#include "BSP_MODEL_API.h"


class BSP_MODEL_API DXEdge
{
protected:
	ElemID SurfID;
	union
	{
		unsigned __int32 Val;
		struct
		{
			bool Rout : 1; // true if end point is outside
			unsigned int Degraded : 2; // 1 - first vertex is ON; 2 - last vertex is ON; 0 - ordinary edge
			unsigned int IndexBuf : 29; // Index of this point in out (BUF) array
		};
	};
public:
	DXEdge() {};
	DXEdge(bool i, ElemID Surf, int BufInd, unsigned int DegradedEdge);// инициализация полей направление и точки
	~DXEdge();
	void Set(bool i, ElemID Surf, int BufInd, unsigned int DegradedEdge);// инициализация полей направление и точки
	bool GetRout() const {return Rout;}
	void SetRout(bool iVal) {Rout = iVal;}
	int GetPoint() const {return IndexBuf;}
	int GetDegraded() const { return Degraded; }
	void SetDegraded(int iVal) { Degraded = iVal; }
	ElemID GetBaseID() const { return SurfID; }
};

