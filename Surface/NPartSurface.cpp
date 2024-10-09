// NPartSurface.cpp: implementation of the NPartSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BPoint.h"
#include "NUVPoint.h"
#include "NPartSurface.h"
#include "EssayLine.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double	dTangentLen; 

NPartSurface::NPartSurface()
{

}

NPartSurface::~NPartSurface()
{

}

// построение линии видимого контура 
BOOL NPartSurface::BuildEssayLine(BPoint dir, UVPointArray& uvp, NContour &cont)
{
	UVPointArray uv_int;
	
// определение интервалов на uv-линиях, где есть точка линии очерка
		// линия (u,0)
	ThereIsEssayPointOnLine(0.,1.,false,0.,dir,uv_int);	
		// линия (u,1)
	ThereIsEssayPointOnLine(0.,1.,false,1.,dir,uv_int);	
		// линия (0,v)
	ThereIsEssayPointOnLine(0.,1.,true,0.,dir,uv_int);
		// линия (1,v)
	ThereIsEssayPointOnLine(0.,1.,true,1.,dir,uv_int);

// условие прекращения построения:
	// число точек меньше, чем 2
	if (uv_int.GetSize() <= 2)
		return FALSE;

	UVPointArray uv_res;
	NUVPoint uv;
	double rez;
	int i;

	// определение uv-точек линии очерка на соответсвующих интервалах
	for (i=0; i<uv_int.GetSize()/2; i++)
	{	
		rez = ZeroIn(uv_int[2*i].GetU(), uv_int[2*i+1].GetU(),
			         uv_int[2*i].GetV(), uv_int[2*i+1].GetV(),
			         ACCURACY1, dir);
		if (uv_int[2*i].GetU()==uv_int[2*i+1].GetU()) // если u=const т.е. v- линия
			uv.Set(uv_int[2*i].GetU(), rez);
		else					       // если v=const т.е. u- линия
			uv.Set(rez, uv_int[2*i].GetV());	
		
		uv_res.Add(uv);
	}

// условие прекращения построения:

	// число точек меньше, чем 2
	if (uv_res.GetSize() <= 1)
		return FALSE;

	// две найденные точки совпадают
	// (или одна угловая точка породила две при 
	// рассмотрении смежных крайних параметрических линий)
	if (uv_res[0].GetU() == uv_res[1].GetU() &&
		uv_res[0].GetV() == uv_res[1].GetV())
		return FALSE;

// продолжение построения

	// определение массива последовательных точек на кривой
	// получается ломаная кривая

/*	dTangentLen = (double)sqrt((uv_res[1]-uv_res[0]).D2())/7;
	MotionOnEssLines(uv_res[0], 
					 dTangentLen,
					 uvp, dir);
	// проверка достиг ли вторую точку исходного интервала
	// при "движении по линии"
	// если нет (в частности если недостаточная точность)
	// всё равно добавляем эту точку
	// и результирующая линия будет построина 
	// в результате "уточнения"
	if (fabs(uvp[uvp.GetSize()-1].GetU() - uv_res[1].GetU()) > ACCURACY ||
		fabs(uvp[uvp.GetSize()-1].GetV() - uv_res[1].GetV()) > ACCURACY)
		uvp.Add(uv_res[1]);*/

	uvp.Add(uv_res[0]);
	uvp.Add(uv_res[1]);

	// провека найденных точек
	ImprovOfLine(uvp, dir);

	if (uvp.GetSize() > 0)
		InterpolationLine(uvp, cont, dir);
	
	return TRUE;
}

