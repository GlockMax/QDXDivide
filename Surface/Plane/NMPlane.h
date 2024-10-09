#pragma once
//#include "c:\vcprojects\ncm25_test\common\mplane.h"
#include "mplane.h"
#include "BPoint.h"

class NMPlane :
	public MPlane
{
public:
	NMPlane(void);
	~NMPlane(void);

protected:
	BPoint m_n;		// нормаль к плоскости
	BPoint m_b;		// точка на плоскости
public:
	void Set(BPoint n, BPoint p);
	const BPoint & GetNormal(void);
	const BPoint & GetPoint(void);
};
