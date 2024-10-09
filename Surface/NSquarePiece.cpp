// NSquarePiece.cpp: implementation of the NSquarePiece class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NLine.h"
#include "BPoint.h"
#include "BMatr.h"
#include "NSquarePiece.h"
#include "NCubicPiece.h"
#include "NBox.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NSquarePiece::NSquarePiece()
{

}

NSquarePiece::~NSquarePiece()
{

}

void NSquarePiece::Modify(const BMatr m)
{
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			pc[i][j] = pc[i][j] * m;
}

// ����������� ����� �� ������ �����������
BPoint NSquarePiece::GetPointFromParams(double u, double v)
{
	double U[3]={1, u, u*u},
		   V[3]={1, v, v*v}, 
		   a[3], b[3];
	double M[3][3]={1., 0., 0.,	// ������� ������������� ������
					-2.,2.,0.,	// �����
					1.,-2.,1.}, 
		  Mt[3][3]={1.,-2.,1.,	// ����������������� �������
					0.,2.,-2.,	// ������������� ������
					0.,0.,1.};	// �����

	BPoint res(0.,0.,0.,0.);
	BPoint ps[4];
	int i,j;

	for(i=0; i<3; i++)
	{
		a[i] = 0.;
		b[i] = 0.;
		for(j=0; j<3; j++)
		{
			a[i] += U[j]*M[j][i];
			b[i] += Mt[i][j]*V[j];
		}
	}
	for (i=0; i<3; i++)
	{
		ps[i].Set(0.,0.,0.,0.);
		for(j=0; j<3; j++)
			ps[i] = ps[i] + pc[j][i]*a[j];
		
		res = res + ps[i]*b[i];
	}

	return res;
}

void NSquarePiece::Draw()
{
	int i, j=0, l=0, n=3;// Points Number in a contour
	int step = 8;
	bool f = false;
	GLfloat *ctlarray = new GLfloat [3*n*4];

	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			ctlarray[(j+l)*4] = (float)pc[j][i].GetX();
			ctlarray[(j+l)*4+1] = (float)pc[j][i].GetY();
			ctlarray[(j+l)*4+2] = (float)pc[j][i].GetZ();
			ctlarray[(j+l)*4+3] = (float)pc[j][i].GetH();
		}
		l+=3;
	}

	GLint un = 2*step, vn = step;
	GLfloat u1=0.,u2=1.,v1=0.,v2=1.;

//	glDisable(GL_LIGHTING);
//	glDisable(GL_CULL_FACE);

	glPushAttrib(GL_EVAL_BIT);
	glColor3d(0.3, 0.3, 0.3);
	glMapGrid2f( un, u1, u2, vn, v2, v1);
	glEnable(GL_MAP2_VERTEX_4);

	glMap2f(GL_MAP2_VERTEX_4,
		0.,1.,4,3,
		0.,1.,4*n,3,
		ctlarray);
	glEvalMesh2( GL_FILL, 0, un, 0, vn);
//	glEvalMesh2( GL_LINE, 0, un, 0, vn);
		
	glPopAttrib();

	glDisable(GL_LIGHTING);
	glColor3d(1, 0., 0.);
	//glEnable(GL_MAP1_VERTEX_4);
	//glMap1f(GL_MAP1_VERTEX_4, 0., 1., 4, 3, ctlarray);
	//glEvalCoord1d(0.5);
	NLine line;
	for (i=0; i<3; i+=2)
	{
		line.p[0] = pc[i][0];
		line.p[1] = pc[i][1];
		line.p[2] = pc[i][1];
		line.p[3] = pc[i][2];
		line.Ball2Bernstain();
		line.Draw();
	}
	for (i=0; i<3; i+=2)
	{
		line.p[0] = pc[0][i];
		line.p[1] = pc[1][i];
		line.p[2] = pc[1][i];
		line.p[3] = pc[2][i];
		line.Ball2Bernstain();
		line.Draw();
	}
	glColor3d(0.3, 0.3, 0.3);

	glEnable(GL_LIGHTING);

	delete []ctlarray;
}

// ������� ���������� ���� �� �� ������ ���������������
// ����� �.�. (u,const), (const,v) ����� ����� ������
// ���������� ��������:
//		k0 - ���������� ��������� �����
//		k2 - ���������� �������� �����
//		type - ���� ��������������� ��� ��������������� ������ 
//				U ��� V (true - U, false - V)
//		dir - ������ ����������� �������
// ����������:
//		����� ��������� �����
//		uvp - ������ ��������� UV-�����
int NSquarePiece::ThereIsEssayPointOnLine(double k0, double k2, 
										  bool type, double con,
										  BPoint dir, UVPointArray& uvp)
{
	double k1;
	double a, b, c;
	NUVPoint uv1, uv2;

	// ����������� ���������� ��� ���������� ����� ���������� ������������
	k1 = (k0 + k2)*0.5;

	if (type) // ����������� �������� ������� ������� ��� ��������
	{
		// u-�����
		a = GetNormalOnDirection(con, k0, dir);
		b = GetNormalOnDirection(con, k1, dir);
		c = GetNormalOnDirection(con, k2, dir);
	}
	else
	{
		// v-�����
		a = GetNormalOnDirection(k0, con, dir);
		b = GetNormalOnDirection(k1, con, dir);
		c = GetNormalOnDirection(k2, con, dir);
	}

// ���������� ���������� ��������� ������ ������
// � ���������� �� �������� �����
// �������� a-b	
	if ((a>0.0 && b<=0.0)||(a<0.0 && b>=0.0))
	{
		if (type)	// u-�����
		{
			uv1.Set(con, k0);
			uv2.Set(con, k1);
		}
		else		// v-�����
		{
			uv1.Set(k0, con);
			uv2.Set(k1, con);
		}
		uvp.Add(uv1);
		uvp.Add(uv2);
	}
// �������� b-c
	if ((b>0.0 && c<=0.0)||(b<0.0 && c>=0.0))
	{
		if (type)	// u-�����
		{
			uv1.Set(con, k1);
			uv2.Set(con, k2);
		}
		else		// v-�����
		{
			uv1.Set(k1, con);
			uv2.Set(k2, con);
		}
		uvp.Add(uv1);
		uvp.Add(uv2);
	}

	return uvp.GetSize();
}

// ����������� ������� � ����������� � �������� ����� 
BPoint NSquarePiece::Normal(double u, double v)
{	
	BPoint dRu,dRv,Norm;

	// ����������� � u-������
	dRu = TangentRu(u,v);
// ����������� � v-������
	dRv = TangentRv(u,v);
// ������ ������� � ����������� � ����� � ������������ (u,v)
	Norm=dRu%dRv;

	return Norm;
}

// ����������� ����������� � ��������������� u-������ 
BPoint NSquarePiece::TangentRu(double u, double v)
{
	// �������:		  dU*M*P*Mt*V - dU*M*H*Mt*V*r(u,v)
	//			dRu = -------------------------------
	//							U*M*H*Mt*V
	double U[3]={1, u, u*u},
		   V[3]={1, v, v*v}, 
		   dU[3]={0, 1, 2*u},
		   a[3], b[3];//, t[3];
	double M[3][3]={1., 0., 0.,	// ������� ������������� ������
					-2.,2.,0.,	// �����
					1.,-2.,1.}, 
		  Mt[3][3]={1.,-2.,1.,	// ����������������� �������
					0.,2.,-2.,	// ������������� ������
					0.,0.,1.};	// �����
	BPoint c1(0.,0.,0.,0.),
		   c2(0.,0.,0.,0.),
		   r;
	BPoint ps[3];
	BPoint dRu;
	int i,j;

	for(i=0; i<3; i++)
	{
		a[i] = 0.;
		b[i] = 0.;
		for(j=0; j<3; j++)
		{
			a[i] += dU[j]*M[j][i];
			b[i] += Mt[i][j]*V[j];
		}
	}
	for (i=0; i<3; i++)
	{
		ps[i].Set(0.,0.,0.,0.);
		for(j=0; j<3; j++)
			ps[i] = ps[i] + pc[j][i]*a[j];
		
		c1 = c1 + ps[i]*b[i];
	}
	c2=GetPointFromParams(u,v);
	r = c2;
	dRu = (c1-r.Norm()*c1.GetH())*(1/c2.GetH());

	return dRu;
}

// ����������� ����������� � ��������������� v-������ 
BPoint NSquarePiece::TangentRv(double u, double v)
{
	// �������:		  U*M*P*Mt*dV - U*M*H*Mt*dV*r(u,v)
	//			dRv = -------------------------------
	//							U*M*H*Mt*V
	double U[3]={1, u, u*u},
		   V[3]={1, v, v*v}, 
		   dV[3]={0, 1, 2*v},
		   a[3], b[3];//, t[3];
	double M[3][3]={1., 0., 0.,	// ������� ������������� ������
					-2.,2.,0.,	// �����
					1.,-2.,1.}, 
		  Mt[3][3]={1.,-2.,1.,	// ����������������� �������
					0.,2.,-2.,	// ������������� ������
					0.,0.,1.};	// �����
	BPoint c1(0.,0.,0.,0.),
		   c2(0.,0.,0.,0.);
	BPoint ps[3], r;
	BPoint dRv;
	int i,j;

	for(i=0; i<3; i++)
	{
		a[i] = 0.;
		b[i] = 0.;
		for(j=0; j<3; j++)
		{
			a[i] += U[j]*M[j][i];
			b[i] += Mt[i][j]*dV[j];
		}
	}	
	for (i=0; i<3; i++)
	{
		ps[i].Set(0.,0.,0.,0.);
		for(j=0; j<3; j++)
			ps[i] = ps[i] + pc[j][i]*a[j];
		
		c1 = c1 + ps[i]*b[i];
	}
	c2 = GetPointFromParams(u,v);
	r = c2;
	dRv = (c1-r.Norm()*c1.GetH())*(1/c2.GetH());

	return dRv;
}

// ����������� ���������� ������������ ������� � ����������� 
// � ������� ����������� �������
double NSquarePiece::GetNormalOnDirection(double u, double v, BPoint dir)
{
// ������ ������� � ����������� � ����� � ������������ (u,v)
	BPoint Norm = Normal(u, v), d;
// ��������� ������������ ������� � ������������ �������
	d = dir*(1/sqrt(dir.D2()));
	return Norm*dir;
}

// ����������� ����������� � ����� ������
// � ���������� �����������
BPoint NSquarePiece::TangentToEssLine(double u, double v, BPoint direct)
{
	NUVPoint D;

	return TangentToEssayLine(u,v,direct,D);
}

// ����������� ����������� � ����� ������
// � ��������������� �����������
NUVPoint NSquarePiece::TangentToEssLineUV(double u, double v, BPoint direct)
{
	NUVPoint res;

	TangentToEssayLine(u,v,direct,res);

	return res;
}

// ����������� ����������� � ����� ������
BPoint NSquarePiece::TangentToEssayLine(double u, double v, 
										BPoint direct, NUVPoint &D)
{
	BPoint T,dRu,dRv,dRuu,dRvv,dRuv;
	BMatr M,Mt,H;
	BPoint U,V,dU,dV,ddU,ddV;
	BPoint pt_param;
	BPoint c1(0.,0.,0.,0.),
		   c2(0.,0.,0.,0.),
		   c3(0.,0.,0.,0.),
		   c4(0.,0.,0.,0.);
	BPoint u2m, u1m, um,
		   mtv2, mtv1, mtv;
	BPoint ps[3], p;
	double d,ut,vt, dHu, dHv;
	int i;
// ������� ��������� �������
	M.Set(1., 0., 0.,0.,	// ������� ������������� ������
		 -2., 2., 0.,0.,	// �����
		  1.,-2., 1.,0.,
		  0., 0., 0.,1.);
	Mt.Set(1.,-2., 1., 0.,	// ����������������� �������
		   0., 2.,-2., 0.,	// ������������� ������
		   0., 0., 1., 0.,	// �����
		   0., 0., 0., 1.);
	H.Set(pc[0][0].GetH(), pc[0][1].GetH(), pc[0][2].GetH(), 0.,
		  pc[1][0].GetH(), pc[1][1].GetH(), pc[1][2].GetH(), 0.,
		  pc[2][0].GetH(), pc[2][1].GetH(), pc[2][2].GetH(), 0.,
				0.,				0.,				0.,			 1.);
	U.Set(1,u,u*u,0);
	V.Set(1,v,v*v,0);
	dU.Set(0,1,2*u,0);
	dV.Set(0,1,2*v,0);
	ddU.Set(0,0,2,0);
	ddV.Set(0,0,2,0);
	pt_param = GetPointFromParams(u,v);
	p = pt_param;
	p.Norm();
// �������������� ����������
	um = U*M;
	u1m = dU*M;
	u2m = ddU*M;
	mtv = Mt*V;
	mtv1 = Mt*dV;
	mtv2 = Mt*ddV;
// ����������� ������ �����������
	dRu = TangentRu(u,v);
	dRv = TangentRv(u,v);
// ����������� ������ ����������� � ����� �����������
// ����������� dRuu
	// �������:			ddU*M*P*Mt*V - 2*(dU*M*H*Mt*V)*dRu - ddU*M*H*Mt*V*r(u,v)
	//			dRuu =	-------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u2m.GetX()+pc[1][i]*u2m.GetY()+
			  pc[2][i]*u2m.GetZ();

	c1 = ps[0]*mtv.GetX()+ps[1]*mtv.GetY()+ps[2]*mtv.GetZ();
	c2 = u1m*H;
	dHu = c2.GetX()*mtv.GetX()+c2.GetY()*mtv.GetY()+c2.GetZ()*mtv.GetZ();
	
	dRuu = (c1-dRu*2*dHu-p*c1.GetH())*(1/pt_param.GetH());

// ����������� dRvv
	// �������:			U*M*P*Mt*ddV - 2*(U*M*H*Mt*dV)*dRv - U*M*H*Mt*ddV*r(u,v)
	//			dRuu =	-------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*um.GetX()+pc[1][i]*um.GetY()+
			  pc[2][i]*um.GetZ();

	c1 = ps[0]*mtv2.GetX()+ps[1]*mtv2.GetY()+ps[2]*mtv2.GetZ();
	c2 = um*H;
	dHv = c2.GetX()*mtv1.GetX()+c2.GetY()*mtv1.GetY()+c2.GetZ()*mtv1.GetZ();

	dRvv = (c1-dRv*2*dHv-p*c1.GetH())*(1/pt_param.GetH());

// ����������� dRuv
	// �������:		   dU*M*P*Mt*dV - dU*M*H*Mt*V*dRu - U*M*H*Mt*dV*dRv - dU*M*H*Mt*dV*r(u,v)
	//			dRuu = ---------------------------------------------------------------------
	//											U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u1m.GetX()+pc[1][i]*u1m.GetY()+
			  pc[2][i]*u1m.GetZ();
	
	c1 = ps[0]*mtv1.GetX()+ps[1]*mtv1.GetY()+ps[2]*mtv1.GetZ();
	
	dRuv = (c1-dRv*dHu-dRu*dHv-p*c1.GetH())*(1/pt_param.GetH());

// ����������� ��������� ut:vt
	dRuu = dRuu*(1/sqrt(dRuu.D2()));
	dRvv = dRvv*(1/sqrt(dRvv.D2()));
//	dRuv = dRuv*(1/sqrt(dRuv.D2()));
	c1 = dRu%dRuv;
	c2 = dRu%dRvv;
	c3 = dRuu%dRv;
	c4 = dRuv%dRv;
	c1 = c1+c3;
	c2 = c2+c4;
	d = c1*direct;
	if (d != 0.) // ��������� ut:vt
	{
		vt=1.;
		d=(c2*direct)*(1./d);
		ut=-d*vt;
	}
	else		 // ��������� vt:ut
	{
		if (c2*direct != 0.)
		{
			ut=1.;
			d=d*(1./(c2*direct));
			vt=-d*ut;
		}
	}
// ������������� ����������� ����������� � ����� ������
	D.Set(ut,vt);
	dRu = dRu*(1/sqrt(dRu.D2()));
	dRv = dRv*(1/sqrt(dRv.D2()));
	T = dRu*ut+dRv*vt;

	return T;
}

// ����������� ������� �������, �������� � ��������� ��� ��� � ����� ����������� 
// ����������
//		u, v - ��������������� ����������
// ������������ 
//		T - �������� ����������� � ���
//		N - �������� ������� ������� � ������
//		B - ������� ���������
//		K - �������� ��������
void NSquarePiece::GetParamForInitLine(double u, double v, 
									   BPoint direct,
									   BPoint &T, BPoint &N, 
									   BPoint &B, double &K)
{
	NUVPoint uvt, uvtt;

	GetParamForInitEssLine(u, v, direct, T, N, B, K, uvt, uvtt);
}


void NSquarePiece::GetParamForInitLineUV(double u, double v, 
									   BPoint direct,
									   NUVPoint &T, NUVPoint &N, 
									   NUVPoint &B, double &K)
{
	NUVPoint uvt, uvtt;
	BPoint Tp, Np, Bp;
	double Kp;

	GetParamForInitEssLine(u, v, direct, Tp, Np, Bp, Kp, uvt, uvtt);

	T.Set(uvt.GetU(), uvt.GetV());
	N.Set(uvt.GetV(), -uvt.GetU());
	B.Set(0., 0.);
	
	K =  Kp;
}

void NSquarePiece::GetParamForInitEssLine(double u, double v, 
										 BPoint direct, 
										 BPoint &T, BPoint &N, 
										 BPoint &B, double &K, 
										 NUVPoint &uvt, NUVPoint &uvtt)
{
	BMatr m;
	NLine line;
	BPoint n,L,Ln,Lt,Ltt,dT,ddT,pt;

	// ����������� ����������
	BPoint dRu,dRv,dRuu,dRvv,dRuv,dRuuu,dRuuv,dRuvv,dRvvv;
	BMatr M,Mt,H;
	BPoint U,V,dU,dV,ddU,ddV, Zer;
	BPoint pt_param;
	BPoint c1(0.,0.,0.,0.),
		   c2(0.,0.,0.,0.),
		   c3(0.,0.,0.,0.),
		   c4(0.,0.,0.,0.);
	BPoint u3m, u2m, u1m, um,
		   mtv3, mtv2, mtv1, mtv;
	BPoint ps[3], p;
	double d,ut,vt,utt,vtt, 
		   dHu, dHv, dHuu, dHuv, dHvv,
		   dHuuu, dHvvv, dHuuv, dHuvv;
	int i;

	// ������� ��������� �������
	M.Set(1., 0., 0.,0.,	// ������� ������������� ������
		 -2., 2., 0.,0.,	// �����
		  1.,-2., 1.,0.,
		  0., 0., 0.,1.);
	Mt.Set(1.,-2., 1., 0.,	// ����������������� �������
		   0., 2.,-2., 0.,	// ������������� ������
		   0., 0., 1., 0.,	// �����
		   0., 0., 0., 1.);
	H.Set(pc[0][0].GetH(), pc[0][1].GetH(), pc[0][2].GetH(), 0.,
		  pc[1][0].GetH(), pc[1][1].GetH(), pc[1][2].GetH(), 0.,
		  pc[2][0].GetH(), pc[2][1].GetH(), pc[2][2].GetH(), 0.,
				0.,				0.,				0.,			 1.);
	U.Set(1,u,u*u,0);
	V.Set(1,v,v*v,0);
	dU.Set(0,1,2*u,0);
	dV.Set(0,1,2*v,0);
	ddU.Set(0,0,2,0);
	ddV.Set(0,0,2,0);
	Zer.Set(0,0,0,0);
	pt_param = GetPointFromParams(u,v);
	p = pt_param;
	p.Norm();
// �������������� ����������
	um = U*M;
	u1m = dU*M;
	u2m = ddU*M;
	u3m = Zer*M;
	mtv = Mt*V;
	mtv1 = Mt*dV;
	mtv2 = Mt*ddV;
	mtv3 = Mt*Zer;
// ����������� ������� � �����������
	n = Normal(u, v);
	
// ����������� ������ �����������
	dRu = TangentRu(u,v);
	dRv = TangentRv(u,v);

// ����������� ������ ����������� � ����� �����������
// ����������� dRuu
	// �������:			ddU*M*P*Mt*V - 2*(dU*M*H*Mt*V)*dRu - ddU*M*H*Mt*V*r(u,v)
	//			dRuu =	-------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u2m.GetX()+pc[1][i]*u2m.GetY()+
			  pc[2][i]*u2m.GetZ();

	c1 = ps[0]*mtv.GetX()+ps[1]*mtv.GetY()+ps[2]*mtv.GetZ();
	c2 = u1m*H;
	dHu = c2.GetX()*mtv.GetX()+c2.GetY()*mtv.GetY()+c2.GetZ()*mtv.GetZ();
	
	dRuu = (c1-dRu*2*dHu-p*c1.GetH())*(1/pt_param.GetH());

// ����������� dRvv
	// �������:			U*M*P*Mt*ddV - 2*(U*M*H*Mt*dV)*dRv - U*M*H*Mt*ddV*r(u,v)
	//			dRvv =	-------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*um.GetX()+pc[1][i]*um.GetY()+
			  pc[2][i]*um.GetZ();

	c1 = ps[0]*mtv2.GetX()+ps[1]*mtv2.GetY()+ps[2]*mtv2.GetZ();
	c2 = um*H;
	dHv = c2.GetX()*mtv1.GetX()+c2.GetY()*mtv1.GetY()+c2.GetZ()*mtv1.GetZ();

	dRvv = (c1-dRv*2*dHv-p*c1.GetH())*(1/pt_param.GetH());

// ����������� dRuv
	// �������:		   dU*M*P*Mt*dV - dU*M*H*Mt*V*dRu - U*M*H*Mt*dV*dRv - dU*M*H*Mt*dV*r(u,v)
	//			dRuv = ----------------------------------------------------------------------
	//											U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u1m.GetX()+pc[1][i]*u1m.GetY()+
			  pc[2][i]*u1m.GetZ();
	
	c1 = ps[0]*mtv1.GetX()+ps[1]*mtv1.GetY()+ps[2]*mtv1.GetZ();
	
	dRuv = (c1-dRv*dHu-dRu*dHv-p*c1.GetH())*(1/pt_param.GetH());

// ���������� ������� ����������� 
// ����������� dRuuu
	// �������:			dddU*M*P*Mt*V - 3*(ddU*M*H*Mt*V)*dRu - 3*(dU*M*H*Mt*V)*dRuu - dddU*M*H*Mt*V*r(u,v)
	//			dRuuu =	----------------------------------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u3m.GetX()+pc[1][i]*u3m.GetY()+
			  pc[2][i]*u3m.GetZ();
	
	c1 = ps[0]*mtv.GetX()+ps[1]*mtv.GetY()+ps[2]*mtv.GetZ();
	c2 = u2m*H;
	dHuu = c2.GetX()*mtv.GetX()+c2.GetY()*mtv.GetY()+c2.GetZ()*mtv.GetZ();
	c2 = u3m*H;
	dHuuu = c2.GetX()*mtv.GetX()+c2.GetY()*mtv.GetY()+c2.GetZ()*mtv.GetZ();
	
	dRuuu = (c1-dRu*dHuu*3-dRuu*dHu*3-p*c1.GetH())*(1/pt_param.GetH());
// ����������� dRvvv
	// �������:			U*M*P*Mt*dddV - 3*(U*M*H*Mt*ddV)*dRv - 3*(U*M*H*Mt*dV)*dRvv - U*M*H*Mt*dddV*r(u,v)
	//			dRvvv =	----------------------------------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*um.GetX()+pc[1][i]*um.GetY()+
			  pc[2][i]*um.GetZ();
	
	c1 = ps[0]*mtv3.GetX()+ps[1]*mtv3.GetY()+ps[2]*mtv3.GetZ();
	c2 = um*H;
	dHvv = c2.GetX()*mtv2.GetX()+c2.GetY()*mtv2.GetY()+c2.GetZ()*mtv2.GetZ();
	c2 = um*H;
	dHvvv = c2.GetX()*mtv3.GetX()+c2.GetY()*mtv3.GetY()+c2.GetZ()*mtv3.GetZ();
	
	dRvvv = (c1-dRv*dHvv*3-dRvv*dHv*3-p*c1.GetH())*(1/pt_param.GetH());
// ����������� dRuuv
	// �������:			ddU*M*P*Mt*dV - 2*(dU*M*H*Mt*dV)*dRu - 2*(dU*M*H*Mt*V)*dRuv - U*M*H*Mt*dV*dRuu - ddU*M*H*Mt*V*dRv - ddU*M*H*Mt*dV*r(u,v)
	//			dRuuv =	------------------------------------------------------------------------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u2m.GetX()+pc[1][i]*u2m.GetY()+
			  pc[2][i]*u2m.GetZ();
	
	c1 = ps[0]*mtv1.GetX()+ps[1]*mtv1.GetY()+ps[2]*mtv1.GetZ();
	c2 = u1m*H;
	dHuv = c2.GetX()*mtv1.GetX()+c2.GetY()*mtv1.GetY()+c2.GetZ()*mtv1.GetZ();
	c2 = u2m*H;
	dHuuv = c2.GetX()*mtv1.GetX()+c2.GetY()*mtv1.GetY()+c2.GetZ()*mtv1.GetZ();
	
	dRuuv = (c1-dRu*dHuv*2-dRuv*dHu*2-dRuu*dHv-dRv*dHuu-p*c1.GetH())*(1/pt_param.GetH());
// ����������� dRuvv
	// �������:			dU*M*P*Mt*ddV - 2*(dU*M*H*Mt*dV)*dRv - 2*(U*M*H*Mt*dV)*dRuv - dU*M*H*Mt*V*dRvv - U*M*H*Mt*ddV*dRu - dU*M*H*Mt*ddV*r(u,v)
	//			dRuvv =	------------------------------------------------------------------------------------------------------------------------
	//										U*M*H*Mt*V
	for (i=0; i<3; i++)
		ps[i]=pc[0][i]*u1m.GetX()+pc[1][i]*u1m.GetY()+
			  pc[2][i]*u1m.GetZ();
	
	c1 = ps[0]*mtv2.GetX()+ps[1]*mtv2.GetY()+ps[2]*mtv2.GetZ();
	c2 = u1m*H;
	dHuvv = c2.GetX()*mtv2.GetX()+c2.GetY()*mtv2.GetY()+c2.GetZ()*mtv2.GetZ();
	
	dRuvv = (c1-dRv*dHuv*2-dRuv*dHv*2-dRvv*dHu-dRv*dHvv-p*c1.GetH())*(1/pt_param.GetH());
// ����������� ��������� ut:vt
//	dRuu = dRuu*(1/sqrt(dRuu.D2()));
//	dRvv = dRvv*(1/sqrt(dRvv.D2()));
//	dRuv = dRuv*(1/sqrt(dRuv.D2()));
	c1 = dRu%dRuv;
	c2 = dRu%dRvv;
	c3 = dRuu%dRv;
	c4 = dRuv%dRv;
	c1 = c1+c3;
	c2 = c2+c4;
	d = c1*direct;

	if (d != 0.) // ��������� ut:vt
	{
		vt = 1.;
		d = (c2*direct)*(1./d);
		ut = -d*vt;
	}
	else		 // ��������� vt:ut
	{
		if (c2*direct != 0.)
		{
			ut=1.;
			d=d*(1./(c2*direct));
			vt=-d*ut;
		}
	}
// ����������� ����������� � ����� ������
// �������: T = dRu*ut + dRv*vt
//	dRu = dRu*(1/sqrt(dRu.D2()));
//	dRv = dRv*(1/sqrt(dRv.D2()));
	T = dRu*ut+dRv*vt;
	uvt.Set(ut, vt);
// ����������� �������� utt vtt
	BPoint E, Q, G;

	c3 = ((dRuu%dRuv)*2+dRuuu%dRuv+dRu%dRuuv)*ut+
		 (dRuu%dRvv+dRuuv%dRv+dRu%dRuvv)*vt;
	c4 = (dRu%dRuvv+dRuu%dRvv+dRuuv%dRv)*ut+
		 ((dRuv%dRvv)*2+dRu%dRvv+dRuvv%dRv)*vt;
	if (vt == 1)
	{
		d = (double)((c3*direct)/(c1*direct))*ut;
		E = dRuu*ut*ut-dRu*d;
		d = (double)((c4*direct)/(c1*direct))*vt;
		Q = dRuv*2*ut*vt+dRvv*vt*vt-dRu*d;
		d = (double)(1-(c2*direct)/(c1*direct));
		G = dRv*d;

		vtt = (double)(E*T+Q*T)/(G*T);
		vtt = -vtt;
		utt = (c3*ut+c4*vt+c2*vtt)*direct/(c1*direct);
	}
	else if (ut == 1)
	{
		d = (double)((c4*direct)/(c4*direct))*vt;
		E = dRvv*vt*vt-dRv*d;
		d = (double)((c3*direct)/(c2*direct))*ut;
		Q = dRuv*2*ut*vt+dRuu*ut*ut-dRv*d;
		d = (double)(1-(c1*direct)/(c2*direct));
		G = dRu*d;

		utt = (double)(E*T+Q*T)/(G*T);
		utt = -utt;
		vtt = (c3*ut+c4*vt+c1*utt)*direct/(c2*direct);
	}
	
// ����������� ������� ������� � ���
	// �������: N = dT/dt = dRuu*ut^2 + 2*dRuv*ut*vt + dRvv*vt^2 + dRu*utt + dRv*vtt
	N = dRuu*ut*ut + dRuv*2*ut*vt + dRvv*vt*vt + dRu*utt + dRv*vtt;
	uvtt.Set(utt, vtt);

// ����������� ��������� 
	// �������:		T x N
	//			B =	-----
	//			   |T x N|
	B = T%N;
	B = B*(1/sqrt(B.D2()));

// ���������� �������� ��������
	d = sqrt(T.D2());

//	K = (n*dRuu*ut*ut+n*2*dRuv*ut*vt+n*dRvv*vt*vt)/((n*N)*d*d);
	
	K = (T%N*B)/(d*d*d);

	K = fabs(K);
}

NBox NSquarePiece::GetGabar()
{
	NBox res;

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			res.Expand(pc[i][j]);

	return res;
}

NBox NSquarePiece::GetProjGabar()
{
	NBox res;
	GLdouble x,y,z;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
		{
			gluProject(pc[i][j].GetX(), pc[i][j].GetY(), pc[i][j].GetZ(),
			           modelMatrix, projMatrix,viewport,&x,&y,&z); 
			res.Expand(BPoint(x, y, z, 1));
		}

	return res;
}
