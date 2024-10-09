// NPlane.cpp: implementation of the NPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPlane.h"
#include "BMatr.h"
#include "NBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NPlane::NPlane()
{
	BPoint pt;
	pc[0] = pc[1] = pc[2] = pc[3] = pt;
}


NPlane::NPlane(const NPlane &pl)
{
	*this = pl;
}

NPlane::~NPlane()
{

}

// задание образующих точек плоскости
void NPlane::Set(BPoint pt1, BPoint pt2, BPoint pt3, BPoint pt4)
{
	pc[0] = pt1;
	pc[1] = pt2;
	pc[2] = pt3;
	pc[3] = pt4;
}

// умножение плоскости на матрицу
void NPlane::Modify(const BMatr m)
{
	for (int i=0; i<4; i++)
		pc[i] = pc[i] * m;
}

void NPlane::Draw()
{
//	glBegin(GL_LINE_LOOP);
	BPoint n;
	n = (pc[1] - pc[0])%(pc[3] - pc[0]);
	glBegin(GL_QUADS);
	
	glNormal3d(n.GetX(), n.GetY(), n.GetZ());
	glVertex3d(pc[0].GetX(), pc[0].GetY(), pc[0].GetZ());
	glVertex3d(pc[1].GetX(), pc[1].GetY(), pc[1].GetZ());
	glVertex3d(pc[2].GetX(), pc[2].GetY(), pc[2].GetZ());
	glVertex3d(pc[3].GetX(), pc[3].GetY(), pc[3].GetZ());
	
	glEnd();	
}

// определение точки на порции
BPoint NPlane::GetPointFromParams(double u, double v)
{
	// определить еЄ возможное назначение 
	// пока возвращаетс€ перва€ точка плоскости
	return pc[0];
}

int NPlane::ThereIsEssayPointOnLine(double a, double b, 
									bool type, double con,
									BPoint dir, UVPointArray& uvp)
{

	return uvp.GetSize();
}

// определение нормали
BPoint NPlane::Normal(double u, double v)
{
	BPoint dRu,dRv,Norm;

// касательна€ к u-кривой
	dRu = TangentRu(u,v);
// касательна€ к v-кривой
	dRv = TangentRv(u,v);
// вектор нормали к поверхности в точке с координатами (u,v)
	Norm=dRu%dRv;

	return Norm;
}

// определение касательной к параметрической u-кривой 
BPoint NPlane::TangentRu(double u, double v)
{
	BPoint dRu;

	return dRu;
}

// определение касательной к параметрической v-кривой 
BPoint NPlane::TangentRv(double u, double v)
{
	BPoint dRv;

	return dRv;
}

// определение скал€рного произведени€ нормали к поверхности 
// и вектора направлени€ взгл€да
double NPlane::GetNormalOnDirection(double u, double v, BPoint dir)
{
// вектор нормали к поверхности в точке с координатами (u,v)
	BPoint Norm = Normal(u, v);
// скал€рное произведение нормали и направлением взгл€да
	return Norm*dir;
}

// определение касательной к линии очерка
// в декартовых координатах
BPoint NPlane::TangentToEssLine(double u, double v, BPoint direct)
{
	NUVPoint D;

	return TangentToEssayLine(u,v,direct,D);
}

// определение касательной к линии очерка
// в параметрических координатах
NUVPoint NPlane::TangentToEssLineUV(double u, double v, BPoint direct)
{
	NUVPoint res;

	TangentToEssayLine(u,v,direct,res);

	return res;
}

// определение касательной к линии очерка
BPoint NPlane::TangentToEssayLine(double u, double v, 
								  BPoint direct, NUVPoint &D)
{
	BPoint pt;

	return pt;
}

// определение главной нормали, кривизны и бинормали дл€ Ћ¬  в точке поверхности 
// ѕ≈–≈ƒј®“—я
//		u, v - параматрические координаты
// ¬ќ«¬–јўј≈“—я 
//		T - значение касательной к Ћ¬ 
//		N - значение главной нормали к кривой
//		B - значени бинормали
//		K - значение кривизны
void NPlane::GetParamForInitLine(double u, double v, 
								   BPoint direct,
								   BPoint &T, BPoint &N, 
								   BPoint &B, double &K)
{
}

void NPlane::GetParamForInitLineUV(double u, double v, 
								   BPoint direct,
								   NUVPoint &T, NUVPoint &N, 
								   NUVPoint &B, double &K)
{
}

NBox NPlane::GetGabar()
{
	NBox res;

	for (int i=0; i<4; i++)
		res.Expand(pc[i]);

	return res;
}

NBox NPlane::GetProjGabar()
{
	NBox res;
	GLdouble x,y,z;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

	for (int i=0; i<4; i++)
	{
		gluProject(pc[i].GetX(), pc[i].GetY(), pc[i].GetZ(),
			       modelMatrix, projMatrix,viewport,&x,&y,&z); 
		res.Expand(BPoint(x, y, z, 1));
	}

	return res;
}
