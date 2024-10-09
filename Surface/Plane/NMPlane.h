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
	BPoint m_n;		// ������� � ���������
	BPoint m_b;		// ����� �� ���������
public:
	void Set(BPoint n, BPoint p);
	const BPoint & GetNormal(void);
	const BPoint & GetPoint(void);
};
