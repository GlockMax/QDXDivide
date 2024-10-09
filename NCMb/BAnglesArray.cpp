#include "stdafx.h"
#include "BAnglesArray.h"
#include "ConstDef.h"

BAnglesArray::BAnglesArray(void)
{
}

BAnglesArray::~BAnglesArray(void)
{
}
int BAnglesArray::Compare(double a0, double a1)
{
	// Сравниваем 2 угла в диапазоне -180 : +180
	// разница между углами не может быть больше 180
	if(a1 == a0)
		return 0;
	double d = a1 - a0;
	if(d > 0.)
		if(d < 180.)
			return 1;//a0<a1
		else
			return -1;
	else
		if(d > -180.)
			return -1;
		else
			return 1;

	return 0;
}
bool BAnglesArray::IsBetween(double t, double a0, double a1)
{
	// принадлежность ориентированному интервалу от a0 к a1
	if(t == a0 || t == a1)
		return false;
	if(GetDelta(t, a0) >= GetDelta(a1, a0))
		return false;
	return true;
}
double BAnglesArray::GetDelta(double a0, double a1)
{
	// Величина положительного поворота от a1 к a0
	double d = a0 - a1;
	if(d >= 0.)
		return d;
	else
		return 360. + d;
}

double BAnglesArray::GetMidAngle(double a0, double a1)
{
	// Угол, соответствующий середине наименьшего из интервалов между a0, a1.
	if(a0 * a1 > 0.)
		return 0.5 * (a0 + a1);
	double D = GetDelta(a0, a1);
	if(D > 180.)
		D -= 360.;
	double Mid = a1 + 0.5 * D;
	if(Mid > 180.)
		Mid -= 360.;
	else if(Mid < -180.)
		Mid += 360.;
	return Mid;

}
bool BAnglesArray::FindBetween(double a0, double a1)
{
	//проверка на наличие угла в списке между a0 и a1
	if(GetSize() <= 0)
		return false;

	INT_PTR St = 0;
	INT_PTR En = GetUpperBound();

	if(IsBetween(GetAt(St), a0, a1))
		return true;

	if(IsBetween(GetAt(En), a0, a1))
		return true;

	while( En - St > 1 )
	{
		INT_PTR Mid = (St + En) / 2;

		if(IsBetween(GetAt(Mid), a0, a1))
			return true;

		if(IsBetween(GetAt(Mid), a1, GetAt(En)))
			En = Mid;
		else
			St = Mid;
	}
	if(	fabs(a0 - GetAt(En)) < 1.e-6 || 
		fabs(a0 - GetAt(St)) < 1.e-6 ||
		fabs(a1 - GetAt(En)) < 1.e-6 ||
		fabs(a1 - GetAt(St)) < 1.e-6)
		return true;
	return false;
}
