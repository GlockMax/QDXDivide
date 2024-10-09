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

// ���������� ����� �������� ������� 
BOOL NPartSurface::BuildEssayLine(BPoint dir, UVPointArray& uvp, NContour &cont)
{
	UVPointArray uv_int;
	
// ����������� ���������� �� uv-������, ��� ���� ����� ����� ������
		// ����� (u,0)
	ThereIsEssayPointOnLine(0.,1.,false,0.,dir,uv_int);	
		// ����� (u,1)
	ThereIsEssayPointOnLine(0.,1.,false,1.,dir,uv_int);	
		// ����� (0,v)
	ThereIsEssayPointOnLine(0.,1.,true,0.,dir,uv_int);
		// ����� (1,v)
	ThereIsEssayPointOnLine(0.,1.,true,1.,dir,uv_int);

// ������� ����������� ����������:
	// ����� ����� ������, ��� 2
	if (uv_int.GetSize() <= 2)
		return FALSE;

	UVPointArray uv_res;
	NUVPoint uv;
	double rez;
	int i;

	// ����������� uv-����� ����� ������ �� �������������� ����������
	for (i=0; i<uv_int.GetSize()/2; i++)
	{	
		rez = ZeroIn(uv_int[2*i].GetU(), uv_int[2*i+1].GetU(),
			         uv_int[2*i].GetV(), uv_int[2*i+1].GetV(),
			         ACCURACY1, dir);
		if (uv_int[2*i].GetU()==uv_int[2*i+1].GetU()) // ���� u=const �.�. v- �����
			uv.Set(uv_int[2*i].GetU(), rez);
		else					       // ���� v=const �.�. u- �����
			uv.Set(rez, uv_int[2*i].GetV());	
		
		uv_res.Add(uv);
	}

// ������� ����������� ����������:

	// ����� ����� ������, ��� 2
	if (uv_res.GetSize() <= 1)
		return FALSE;

	// ��� ��������� ����� ���������
	// (��� ���� ������� ����� �������� ��� ��� 
	// ������������ ������� ������� ��������������� �����)
	if (uv_res[0].GetU() == uv_res[1].GetU() &&
		uv_res[0].GetV() == uv_res[1].GetV())
		return FALSE;

// ����������� ����������

	// ����������� ������� ���������������� ����� �� ������
	// ���������� ������� ������

/*	dTangentLen = (double)sqrt((uv_res[1]-uv_res[0]).D2())/7;
	MotionOnEssLines(uv_res[0], 
					 dTangentLen,
					 uvp, dir);
	// �������� ������ �� ������ ����� ��������� ���������
	// ��� "�������� �� �����"
	// ���� ��� (� ��������� ���� ������������� ��������)
	// �� ����� ��������� ��� �����
	// � �������������� ����� ����� ��������� 
	// � ���������� "���������"
	if (fabs(uvp[uvp.GetSize()-1].GetU() - uv_res[1].GetU()) > ACCURACY ||
		fabs(uvp[uvp.GetSize()-1].GetV() - uv_res[1].GetV()) > ACCURACY)
		uvp.Add(uv_res[1]);*/

	uvp.Add(uv_res[0]);
	uvp.Add(uv_res[1]);

	// ������� ��������� �����
	ImprovOfLine(uvp, dir);

	if (uvp.GetSize() > 0)
		InterpolationLine(uvp, cont, dir);
	
	return TRUE;
}

