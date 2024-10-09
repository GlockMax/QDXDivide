#include "StdAfx.h"
#include ".\qstatuspoint.h"

QStatusPoint::QStatusPoint(void)
{
}
QStatusPoint::QStatusPoint(unsigned int NumTrian, unsigned int NumPoint, unsigned int TypePoint)
{
	NumTr = NumTrian;
	NumP = NumPoint;
	TypeP = TypePoint;
}
QStatusPoint::~QStatusPoint(void)
{
}
void QStatusPoint::SetStPoint(unsigned int NumTrian, unsigned int NumPoint, unsigned int TypePoint)
{
	NumTr = NumTrian;
	NumP = NumPoint;
	TypeP = TypePoint;
}

