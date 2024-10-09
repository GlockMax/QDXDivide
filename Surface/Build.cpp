// ���������� ����� �������� �������

#include "stdafx.h"
#include "BPoint.h"
#include "NLine.h"
#include "NContour.h"
#include "NPartSurface.h"
#include "NUVPoint.h"
#include "math.h"

const double MIN_ANGLE = 0.9755; // Cos2 ���� 5 ����
extern double dTangentLen;		 // ����� ������������ �������

// ������� ���������� �������� ����� ������, 
// ������������ �� ������� ������
// ���������:
//  uv - ��������� ����� �� �������������� ��������
//  len - ����� ������������ �������
//  uvps - ������ �������������� UV - �����
//  direct - ������ ����������� �������
void NPartSurface::MotionOnEssLines(NUVPoint uv, double len, 
									UVPointArray &uvps, BPoint direct)
{
	NUVPoint t_uv, p0, p1, T, dir_axes;
	bool finish, scan_u, was_correct;
	double a, d;
	int start;

	// ������ ����� �������
	uvps.Add(uv);
	finish = false;
	was_correct = false;

	p0 = uv;
	// ����������� �� ����� ����� ����� ��������� �����
	if (p0.GetU() == 0.)
		start=1;
	if (p0.GetV() == 0.)
		start=2;
	if (p0.GetU() == 1.)
		start=3;
	if (p0.GetV() == 1.)
		start=4;
	// ���������� ������������ � ��������� �����
	T = TangentToEssLineUV(p0.GetU(), p0.GetV(), direct);
	T = T*(1/sqrt(T.D2()));
	// ����������� ��������� ����� �����������
	p1 = p0 + T*len;
	// ������������� ����������� �����������
	// ��� ����� ���� ���������� ������ �������������� ��������
	if ((start == 1 && p1.GetU() < p0.GetU()) || 
		(start == 2 && p1.GetV() < p0.GetV()) ||
		(start == 3 && p1.GetU() > p0.GetU()) ||
		(start == 4 && p1.GetV() > p0.GetV()))
		T=T*(-1.);

	// ���� �� ������� ��������� �����
	while (!finish)
	{
		// ����������� ��������� ����� �����������
		p1 = p0 + T*len;

		// ��������� ����� (�������� ������ � �� ��������)
		if (p1.GetU() < 0. || p1.GetU() > 1. || 
			p1.GetV() < 0. || p1.GetV() > 1.)
		{
			int index = 0;

			if (p1.GetU() < 0.)
			{
				d = (double)p0.GetU()/fabs(p1.GetU()-p0.GetU());
				p1 = p0 + T*(len*d);
				index = 1;
			}
			if (p1.GetV() < 0. && index == 0)
			{
				d = (double)p0.GetV()/fabs(p1.GetV()-p0.GetV());
				p1 = p0 + T*(len*d);
				index = 2;
			}
			if (p1.GetU() > 1. && index == 0)
			{
				d = (double)(1-p0.GetU())/fabs(p1.GetU()-p0.GetU());
				p1 = p0 + T*(len*d);
				index = 3;
			}
			if (p1.GetV() > 1. && index == 0)
			{
				d = (double)(1-p0.GetV())/fabs(p1.GetV()-p0.GetV());
				p1 = p0 + T*(len*d);
				index = 4;
			}

			if ((index == 1 || index == 3) && index != 0)
				dir_axes.Set(1,0);
			if ((index == 2 || index == 4) && index != 0)
				dir_axes.Set(0,1);

			p1 = p0 + T*(len*d);
			was_correct = true;
		}

	// ����� ����������� ����� 		
		if (fabs(T.GetU()) > fabs(T.GetV()))
		{
		//	scan_u = false;
			if (was_correct)
				if (fabs(dir_axes.Cos2(dir_axes, T)) < MIN_ANGLE)
					scan_u = true;
				else
					scan_u = false;

			if (!was_correct)
				scan_u = false;
		}
		else
		{
		//	scan_u = true;
			if (was_correct)
				if (fabs(dir_axes.Cos2(dir_axes, T)) < MIN_ANGLE)
					scan_u = false;
				else
					scan_u = true;

			if (!was_correct)
				scan_u = true;
		}
		was_correct = false;
	// ����������� �����
		if (scan_u)
		{
			a = ZeroIn(0. , 1.,
					   p1.GetV(), p1.GetV(),
					   ACCURACY1, direct);
			uv.Set(a, p1.GetV());
		}
		else
		{
			a = ZeroIn(p1.GetU(), p1.GetU(),
		 			   0., 1.,
					   ACCURACY1, direct);
			uv.Set(p1.GetU(), a);
		}	
		uvps.Add(uv);

	// ���������� ����������� � UV �����������
		T = TangentToEssLineUV(uv.GetU(), uv.GetV(), direct);
		T = T*(1/sqrt(T.D2()));
	// ����� ����� ��������� �����
		p0 = uv;
	// ����������� ��������� ����� �����������
		p1 = p0 + T*len;
	// ������������� ����������� �����������
	// ��� ����� ���� ���������� ������ �������������� ��������
		if ((start == 1 && p1.GetU() < p0.GetU()) || 
			(start == 2 && p1.GetV() < p0.GetV()) ||
			(start == 3 && p1.GetU() > p0.GetU()) ||
			(start == 4 && p1.GetV() > p0.GetV()))
			T=T*(-1.);

	// ������� ������
		if ((uv.GetU() <= ACCURACY1 || fabs(1-uv.GetU()) <= ACCURACY1) ||
			(uv.GetV() <= ACCURACY1 || fabs(1-uv.GetV()) <= ACCURACY1))
			finish = true;

	}
}

// ������� ��������� ����� ����� �������� �������
bool NPartSurface::ImprovOfLine(UVPointArray &uvps, BPoint direct)
{
	if (uvps.GetSize() <= 0)
		return false;

	UVPointArray cur_uvps, new_uvps, uvp;
	NUVPoint uv;
	double a;
	bool res;
	int ind;

	res = false;
	cur_uvps.RemoveAll();
	cur_uvps.Copy(uvps);
	uvps.RemoveAll();
	
	ind = 0;
	uvps.Add(cur_uvps[0]);
	while (cur_uvps.GetSize() > 1)
	{
		uvp.RemoveAll();
				
		uvp.Add(cur_uvps[ind]);
		uvp.Add(cur_uvps[ind+1]);
		if (ImprovOfSimpleLine(uvp, direct))
		{	// ���� ������� �� ������������� �������
			if (!res)	res = true;
			
			if (fabs(uvp[0].GetU()-uvp[1].GetU()) <= 
				fabs(uvp[0].GetV()-uvp[1].GetV()))
			{
				a = ZeroIn(0. , 1.,
						   uvp[1].GetV(), uvp[1].GetV(),
						   ACCURACY1, direct);
				uv.Set(a, uvp[1].GetV());
			}
			else
			{
				a = ZeroIn(uvp[1].GetU(), uvp[1].GetU(),
		 				   0., 1.,
						   ACCURACY1, direct);
				uv.Set(uvp[1].GetU(), a);
			}	
			cur_uvps.InsertAt(ind+1, uv);
		}
		else
		{	// ���� ������� ������������� �������
			cur_uvps.RemoveAt(ind);

			uvps.Add(uvp[uvp.GetSize()-1]);
		}
	}

	return res; 
}

// ������� ��������� ����� ����� �������� �������

// ������� ��������� ����� ����� �������� ������� 
// ��������� �� ������ �������
// ��������:
//		uvps - ��� UV-����� ����������� � �������
//		direct - ����������� �������
// ����������:
//		���� true - ���� �� ������� ���������� ����� (true - ��, false - ���)
//		uvps - ������ ��������� UV-����� 
//			   (����������� ��������� ����������� ������� ����� ����� 3 - �����)
bool NPartSurface::ImprovOfSimpleLine(UVPointArray &uvps, BPoint direct)
{
	if (uvps.GetSize() != 2)
		return false;

	NUVPoint r0, r1, r2, r3, T0, T3;
	UVPointArray cur_uvps;
	NContour cont;
	double a;
	bool res;

	res = false;
// ����������� ����� � ���������� �����������
	cur_uvps.Copy(uvps);
	if (!InterpolationLine(cur_uvps, cont, direct))
		return false;

// ���������� ����� � UV ����������� (��������� ������)
	r0 = uvps[0];
	r3 = uvps[1];
	T0 = TangentToEssLineUV(r0.GetU(), r0.GetV(), direct);
	T0 = T0*(1/sqrt(T0.D2()));

	a=(r3-r0)*T0;
	if (a < 0.)
		T0=T0*(-1);
	// ����������� � ����� r3
	T3 = TangentToEssLineUV(r3.GetU(), r3.GetV(), direct);
	T3 = T3*(1/sqrt(T3.D2()));
	//	������������� ����������� �������
	a = (r0-r3)*T3;
	if (a < 0.)
		T3 = T3*(-1);
	// ����������� ����� r2 u r3 ������������������ ��������
	a = sqrt((r3-r0).D2());
	r1 = r0+T0*(1./3.*a);
	r2 = r3+T3*(1./3.*a);

// ����������� ����� ������� �� ��������� ������
// ��������������� ��������� ������� 1/2
	BPoint pt_on_uv, pt_on_dec;
	BPoint pt0, pt1, pt2, pt3;
	NLine line;

	pt0.Set(r0.GetU(), r0.GetV(), 0, 1);
	pt1.Set(r1.GetU(), r1.GetV(), 0, 1);
	pt2.Set(r2.GetU(), r2.GetV(), 0, 1);
	pt3.Set(r3.GetU(), r3.GetV(), 0, 1);

	line.Set(pt0, pt1, pt2, pt3);
	pt0 = line.GetPointFromParam(0.5);

	// ����������� ���������� ����� �����. �������� � ��������������� �����������
	pt_on_uv = GetPointFromParams(pt0.GetX(), pt0.GetY());

// ����������� ����� ������� �� ����� � ���������� �����������
// ��������������� ��������� ������� 1/2
	if (!cont.GetLine(0, line))
		return false;
	pt_on_dec = line.GetPointFromParam(0.5);

// ��������� ���������� ����� � �������� ������� 
// � ������������� ������ ������� ��� �� ��� �����
	pt_on_uv.Norm();
	pt_on_dec.Norm();
	a = sqrt(pt_on_uv.Dist2(pt_on_uv, pt_on_dec));
	if (a > ACCURACY_INTER)
	{
		uvps.RemoveAll();
		uvps.Add(r0);
		r1.Set(pt0.GetX(), pt0.GetY());
		uvps.Add(r1);
		uvps.Add(r3);

		res = true;
	}

	return res;
}

// ���������� ���������������� ������
BOOL NPartSurface::InterpolationLine(UVPointArray& uvps, NContour& con, BPoint direction)
{
	if (uvps.GetSize() < 0 || uvps.GetSize() == 0)
		return FALSE;

	BPoint r0,r1,r2,r3,T0,T3;
	double a;
	int i;
	
	con.Clear();

	for (i=0; i<uvps.GetSize()-1; i++)
	{
		// ����������� ����������� � ������ ��������
		r0 = GetPointFromParams(uvps[i].GetU(), uvps[i].GetV());
		r3 = GetPointFromParams(uvps[i+1].GetU(), uvps[i+1].GetV());
		r0.Norm();
		r3.Norm();
		// ����������� � ����� r0
		if (i == 0)
		{
		// ����������� � ����� r0, ��� ������ �����
			T0 = TangentToEssLine(uvps[0].GetU(), uvps[0].GetV(), direction);
			T0 = T0*(1/sqrt(T0.D2()));
			//	������������� ����������� �������
			if ((r3-r0)*T0 < 0.)
				T0=T0*(-1);	
		}
		else
		{	
			T0=T3*(-1);	
		}
		// ����������� � ����� r3
		T3 = TangentToEssLine(uvps[i+1].GetU(), uvps[i+1].GetV(), direction);
		T3 = T3*(1/sqrt(T3.D2()));
	//	������������� ����������� �������
		if ((r0-r3)*T3 < 0.)
			T3 = T3*(-1);
	
		a = sqrt((r3-r0).D2());
		r1 = r0+T0*(1./3.*a);
		r2 = r3+T3*(1./3.*a);

		r1.Norm();
		r2.Norm();
	
	// ���������� ����������� ������� ������� 
	// ������������������ �������� �.�. �������
		if (i == 0)
			con.AddFirst(r0,r1,r2,r3);
		else
			con.AddFollow(r1,r2,r3);
	}

	return TRUE;
}


