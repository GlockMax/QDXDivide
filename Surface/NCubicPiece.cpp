// NCubicPiece.cpp: implementation of the NCubicPiece class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NLine.h"
#include "NCubicPiece.h"
#include "BMatr.h"
#include "NBox.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCubicPiece::NCubicPiece()
{

}

NCubicPiece::~NCubicPiece()
{

}

// умножение координат хар-их точек порции на матрицу
void NCubicPiece::Modify(const BMatr m)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			pc[i][j] = pc[i][j] * m;
}

void NCubicPiece::operator*(const BMatr m)
{

}

/*NCubicPiece NCubicPiece::operator *(BMatr m) const
{
	int i,j;
	NCubicPiece r;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			r.pc[i][j]=pc[i][j]*m;
	return r;
}*/

// определение точки на порции поверхности
BPoint NCubicPiece::GetPointFromParams(double u, double v)
{
	double a,b,c,d;
	int i;
	NLine l;
	a = (1-v)*(1-v);
	b = 2*v*(1-v)*(1-v);
	c = 2*v*v*(1-v);
	d = v*v;
	for(i = 0; i < 4;i++)
	{
		l.p[i].Set(a*pc[i][0].GetH()+b*pc[i][1].GetH()+c*pc[i][2].GetH()+d*pc[i][3].GetH(),
				   a*pc[i][0].GetX()+b*pc[i][1].GetX()+c*pc[i][2].GetX()+d*pc[i][3].GetX(),
				   a*pc[i][0].GetY()+b*pc[i][1].GetY()+c*pc[i][2].GetY()+d*pc[i][3].GetY(),
				   a*pc[i][0].GetZ()+b*pc[i][1].GetZ()+c*pc[i][2].GetZ()+d*pc[i][3].GetZ());
	}
	return(l.GetPointFromParam(u));
}

void NCubicPiece::Draw()
{
	int i, j=0, l=0, n=4;// Points Number in a contour
	int step = 8;
	bool f = false;
	GLfloat *ctlarray = new GLfloat [4*n*4];

	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			ctlarray[(j+l)*4] = (float)pc[j][i].GetX();
			ctlarray[(j+l)*4+1] = (float)pc[j][i].GetY();
			ctlarray[(j+l)*4+2] = (float)pc[j][i].GetZ();
			ctlarray[(j+l)*4+3] = (float)pc[j][i].GetH();
		}
		l+=4;
	}

	GLint un = 2*step, vn = step;
	GLfloat u1=0.,u2=1.,v1=0.,v2=1.;

//	glDisable(GL_LIGHTING);
//	glDisable(GL_CULL_FACE);

	glPushAttrib(GL_EVAL_BIT);
	glMapGrid2f( un, u1, u2, vn, v2, v1);
	glEnable(GL_MAP2_VERTEX_4);

	glMap2f(GL_MAP2_VERTEX_4,
		0.,1.,4,4,
		0.,1.,4*n,4,
		ctlarray);
	glEvalMesh2( GL_FILL, 0, un, 0, vn);
//	glEvalMesh2( GL_LINE, 0, un, 0, vn);

	glPopAttrib();

	glDisable(GL_LIGHTING);
	glColor3d(1, 0., 0.);
	NLine line;
	for (i=0; i<4; i+=3)
	{
		for (j=0; j<4; j++)
			line.p[j] = pc[i][j];
		line.Draw();
	}
	for (i=0; i<4; i+=3)
	{
		for (j=0; j<4; j++)
			line.p[j] = pc[j][i];
		line.Draw();
	}
	glEnable(GL_LIGHTING);

	delete []ctlarray;
}


int NCubicPiece::ThereIsEssayPointOnLine(double a, double b, 
										 bool type, double con,
										 BPoint dir, UVPointArray& uvp)
{

	return uvp.GetSize();
}

// определение нормали к поверхности в заданной точке 
BPoint NCubicPiece::Normal(double u, double v)
{
	BPoint dRu,dRv,Norm;

	// касательная к u-кривой
	dRu = TangentRu(u,v);
// касательная к v-кривой
	dRv = TangentRv(u,v);
// вектор нормали к поверхности в точке с координатами (u,v)
	Norm=dRu%dRv;

	return Norm;
}

// определение касательной к параметрической u-кривой 
BPoint NCubicPiece::TangentRu(double u, double v)
{
	BMatr M,Mt,H;
	BPoint U,V,dU;
	BPoint a,b,c1,c2;
	BPoint ps[4];
	BPoint dRu;
	double d;
	int i;
// задание начальных условий
	M.Set(1., 0., 0.,0.,	// матрица коэффициентов базиса
		 -2., 2., 0.,0.,	// Безье
		  1.,-4., 2.,1.,
		  0., 2.,-2.,0.);
	Mt.Set(1.,-2., 1., 0.,	// транспонированная матрица
		   0., 2.,-4., 2.,	// коэффициентов базиса
		   0., 0., 2.,-2.,	// Безье
		   0., 0., 1., 0.);
	H.Set(pc[0][0].GetH(), pc[0][1].GetH(), pc[0][2].GetH(), pc[0][3].GetH(),
		  pc[1][0].GetH(), pc[1][1].GetH(), pc[1][2].GetH(), pc[1][3].GetH(),
		  pc[2][0].GetH(), pc[2][1].GetH(), pc[2][2].GetH(), pc[2][3].GetH(),
		  pc[3][0].GetH(), pc[3][1].GetH(), pc[3][2].GetH(), pc[3][3].GetH());
	U.Set(1,u,u*u,u*u*u);
	V.Set(1,v,v*v,v*v*v);
	dU.Set(0,1,2*u,3*u*u);
	a=dU*M;
	b=Mt*V;
	for (i=0; i<4; i++)
		ps[i]=pc[0][i]*a.GetX()+
			  pc[1][i]*a.GetY()+
			  pc[2][i]*a.GetZ()+
			  pc[3][i]*a.GetH();
	
	c1=ps[0]*b.GetX()+ps[1]*b.GetY()+ps[2]*b.GetZ()+ps[3]*b.GetH();
	c2 = a*H;
	d = c2.GetX()*b.GetX()+c2.GetY()*b.GetY()+c2.GetZ()*b.GetZ()+c2.GetH()*b.GetH();
	c2=GetPointFromParams(u,v);
	c2.Norm();
	c2=c2*d;
	dRu=(c1-c2);
	a=U*M;
	c2=a*H;
	d = c2.GetX()*b.GetX()+c2.GetY()*b.GetY()+c2.GetZ()*b.GetZ()+c2.GetH()*b.GetH();
	dRu=dRu*(1/d);

	return dRu;
}

// определение касательной к параметрической v-кривой 
BPoint NCubicPiece::TangentRv(double u, double v)
{
	BMatr M,Mt,H;
	BPoint U,V,dV;
	BPoint a,b,c1,c2;
	BPoint ps[4];
	BPoint dRv;
	double d;
	int i;
// задание начальных условий
	M.Set(1., 0., 0.,0.,	// матрица коэффициентов базиса
		 -2., 2., 0.,0.,	// Безье
		  1.,-4., 2.,1.,
		  0., 2.,-2.,0.);
	Mt.Set(1.,-2., 1., 0.,	// транспонированная матрица
		   0., 2.,-4., 2.,	// коэффициентов базиса
		   0., 0., 2.,-2.,	// Безье
		   0., 0., 1., 0.);
	H.Set(pc[0][0].GetH(), pc[0][1].GetH(), pc[0][2].GetH(), pc[0][3].GetH(),
		  pc[1][0].GetH(), pc[1][1].GetH(), pc[1][2].GetH(), pc[1][3].GetH(),
		  pc[2][0].GetH(), pc[2][1].GetH(), pc[2][2].GetH(), pc[2][3].GetH(),
		  pc[3][0].GetH(), pc[3][1].GetH(), pc[3][2].GetH(), pc[3][3].GetH());
	U.Set(1,u,u*u,u*u*u);
	V.Set(1,v,v*v,v*v*v);
	dV.Set(0,1,2*v,3*v*v);
	a=U*M;
	b=Mt*dV;
	for (i=0; i<4; i++)
		ps[i]=pc[0][i]*a.GetX()+pc[1][i]*a.GetY()+
			  pc[2][i]*a.GetZ()+pc[3][i]*a.GetH();
	
	c1=ps[0]*b.GetX()+ps[1]*b.GetY()+ps[2]*b.GetZ()+ps[3]*b.GetH();
	c2 = a*H;
	d = c2.GetX()*b.GetX()+c2.GetY()*b.GetY()+c2.GetZ()*b.GetZ()+c2.GetH()*b.GetH();
	c2=GetPointFromParams(u,v);
	c2.Norm();
	c2=c2*d;
	dRv=(c1-c2);
	b=Mt*V;
	c2=a*H;
	d = c2.GetX()*b.GetX()+c2.GetY()*b.GetY()+c2.GetZ()*b.GetZ()+c2.GetH()*b.GetH();
	dRv=dRv*(1/d);

	return dRv;
}

// определение скалярного произведения нормали к поверхности 
// и вектора направления взгляда
double NCubicPiece::GetNormalOnDirection(double u, double v, BPoint dir)
{
// вектор нормали к поверхности в точке с координатами (u,v)
	BPoint Norm = Normal(u, v);
// скалярное произведение нормали и направлением взгляда
	return Norm*dir;
}

// определение касательной к линии очерка
// в декартовых координатах
BPoint NCubicPiece::TangentToEssLine(double u, double v, BPoint direct)
{
	NUVPoint D;

	return TangentToEssayLine(u,v,direct,D);
}

// определение касательной к линии очерка
// в параметрических координатах
NUVPoint NCubicPiece::TangentToEssLineUV(double u, double v, BPoint direct)
{
	NUVPoint res;

	TangentToEssayLine(u,v,direct,res);

	return res;
}

// определение касательной к линии очерка
BPoint NCubicPiece::TangentToEssayLine(double u, double v, 
									   BPoint direct, NUVPoint &D)
{
	BPoint T,dRu,dRv,dRuu,dRvv,dRuv;
	BMatr M,Mt,H;
	BPoint U,V,dU,dV,ddU,ddV;
	BPoint a,b,e,g,c1,c2,c3,c4;
	BPoint ps[4];
	double d,ut,vt;
	int i;
// задание начальных условий
	M.Set(1., 0., 0.,0.,	// матрица коэффициентов базиса
		 -2., 2., 0.,0.,	// Безье
		  1.,-4., 2.,1.,
		  0., 2.,-2.,0.);
	Mt.Set(1.,-2., 1., 0.,	// транспонированная матрица
		   0., 2.,-4., 2.,	// коэффициентов базиса
		   0., 0., 2.,-2.,	// Безье
		   0., 0., 1., 0.);
	H.Set(pc[0][0].GetH(), pc[0][1].GetH(), pc[0][2].GetH(), pc[0][3].GetH(),
		  pc[1][0].GetH(), pc[1][1].GetH(), pc[1][2].GetH(), pc[1][3].GetH(),
		  pc[2][0].GetH(), pc[2][1].GetH(), pc[2][2].GetH(), pc[2][3].GetH(),
		  pc[3][0].GetH(), pc[3][1].GetH(), pc[3][2].GetH(), pc[3][3].GetH());
	U.Set(1,u,u*u,u*u*u);
	V.Set(1,v,v*v,v*v*v);
	dU.Set(0,1,2*u,3*u*u);
	dV.Set(0,1,2*v,3*v*v);
	ddU.Set(0,0,2,6*u);
	ddV.Set(0,0,2,6*v);
// определение первых производных
	dRu = TangentRu(u,v);
	dRv = TangentRv(u,v);
// определение вторых производных в точке поверхности
// производная dRuu
	a=ddU*M;
	b=Mt*V;
	for (i=0; i<4; i++)
		ps[i]=pc[0][i]*a.GetX()+pc[1][i]*a.GetY()+
			  pc[2][i]*a.GetZ()+pc[3][i]*a.GetH();
	c1=ps[0]*b.GetX()+ps[1]*b.GetY()+ps[2]*b.GetZ()+ps[3]*b.GetH();
	e=dU*M;
	c2=e*H;
	d=c2.GetX()*b.GetX()+c2.GetY()*b.GetY()+c2.GetZ()*b.GetZ()+c2.GetH()*b.GetH();
	d=2*d;
	c2=dRu*d;
	c3=a*H;
	d=c3.GetX()*b.GetX()+c3.GetY()*b.GetY()+c3.GetZ()*b.GetZ()+c3.GetH()*b.GetH();
	c3=GetPointFromParams(u,v);
	c3.Norm();
	c3=c3*d;
	dRuu=(c1-c2-c3);
	g=U*M;
	c4=g*H;
	d=c4.GetX()*b.GetX()+c4.GetY()*b.GetY()+c4.GetZ()*b.GetZ()+c4.GetH()*b.GetH();
	dRuu=dRuu*(1/d); // производная dRuu
// производная dRvv
	a=U*M;
	b=Mt*ddV;
	for (i=0; i<4; i++)
		ps[i]=pc[0][i]*a.GetX()+pc[1][i]*a.GetY()+
			  pc[2][i]*a.GetZ()+pc[3][i]*a.GetH();
	c1=ps[0]*b.GetX()+ps[1]*b.GetY()+ps[2]*b.GetZ()+ps[3]*b.GetH();
	e=Mt*dV;
	c2=a*H;
	d=c2.GetX()*e.GetX()+c2.GetY()*e.GetY()+c2.GetZ()*e.GetZ()+c2.GetH()*e.GetH();
	d=2*d;
	c2=dRv*d;
	c3=a*H;
	d=c3.GetX()*b.GetX()+c3.GetY()*b.GetY()+c3.GetZ()*b.GetZ()+c3.GetH()*b.GetH();
	c3=GetPointFromParams(u,v);
	c3.Norm();
	c3=c3*d;
	dRvv=(c1-c2-c3);
	g=Mt*V;
	c4=a*H;
	d=c4.GetX()*g.GetX()+c4.GetY()*g.GetY()+c4.GetZ()*g.GetZ()+c4.GetH()*g.GetH();
	dRvv=dRvv*(1/d); // производная dRvv
// производная dRuv
	a=dU*M;
	b=Mt*dV;
	for (i=0; i<4; i++)
		ps[i]=pc[0][i]*a.GetX()+pc[1][i]*a.GetY()+
			  pc[2][i]*a.GetZ()+pc[3][i]*a.GetH();
	c1=ps[0]*b.GetX()+ps[1]*b.GetY()+ps[2]*b.GetZ()+ps[3]*b.GetH();
	e=Mt*V;
	c2=a*H;
	d=c2.GetX()*e.GetX()+c2.GetY()*e.GetY()+c2.GetZ()*e.GetZ()+c2.GetH()*e.GetH();
	c2=dRv*d;
	g=U*M;
	c3=g*H;
	d=c3.GetX()*b.GetX()+c3.GetY()*b.GetY()+c3.GetZ()*b.GetZ()+c3.GetH()*b.GetH();
	c3=dRu*d;
	c4=a*H;
	d=c4.GetX()*b.GetX()+c4.GetY()*b.GetY()+c4.GetZ()*b.GetZ()+c4.GetH()*b.GetH();
	c4=GetPointFromParams(u,v);
	c4.Norm();
	c4=c4*d;
	dRuv=(c1-c2-c3-c4);
	c1=g*H;
	d=c1.GetX()*e.GetX()+c1.GetY()*e.GetY()+c1.GetZ()*e.GetZ()+c1.GetH()*e.GetH();
	dRuv=dRuv*(1/d); // производная dRuv
// определение отношения ut:vt
	c1=dRu%dRuv;
	c2=dRu%dRvv;
	c3=dRuu%dRv;
	c4=dRuv%dRv;
	c1=c1+c3;
	c2=c2+c4;
	d=c1*direct;
	if (d != 0.) // отношение ut:vt
	{
		vt=1.;
		d=(c2*direct)*(1./d);
		ut=-d*vt;
	}
	else		 // отношение vt:ut
	{
		if (c2*direct != 0.)
		{
			ut=1.;
			d=(d)*(1./(c2*direct));
			vt=-d*ut;
		}
	}
// окончательное определение касательной к линии очерка
//	(*D).set(ut,vt);
	dRu=dRu*(1/sqrt(dRu.D2()));
	dRv=dRv*(1/sqrt(dRv.D2()));
	T=dRu*ut+dRv*vt;
	return T;
}

// определение главной нормали, кривизны и бинормали для ЛВК в точке поверхности 
// ПЕРЕДАЁТСЯ
//		u, v - параматрические координаты
// ВОЗВРАЩАЕТСЯ 
//		T - значение касательной к ЛВК
//		N - значение главной нормали к кривой
//		B - значени бинормали
//		K - значение кривизны
void NCubicPiece::GetParamForInitLine(double u, double v, 
									   BPoint direct,
									   BPoint &T, BPoint &N,
									   BPoint &B, double &K)
{

}

void NCubicPiece::GetParamForInitLineUV(double u, double v, 
									   BPoint direct,
									   NUVPoint &T, NUVPoint &N, 
									   NUVPoint &B, double &K)
{
}

NBox NCubicPiece::GetGabar()
{
	NBox res;

	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			res.Expand(pc[i][j]);

	return res;
}

NBox NCubicPiece::GetProjGabar()
{
	NBox res;
	GLdouble x,y,z;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
		{
			gluProject(pc[i][j].GetX(), pc[i][j].GetY(), pc[i][j].GetZ(),
			           modelMatrix, projMatrix,viewport,&x,&y,&z); 
			res.Expand(BPoint(x, y, z, 1));
		}

	return res;
}
