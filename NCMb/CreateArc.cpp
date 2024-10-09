#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "curve.h"
#include "BMatr.h"
#include "CreateArc.h"
#include "bpoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

int BPoint::CreateSimpleArc(BPoint *ControlP)
{
//	ControlP[0]-Pb
//	ControlP[1]-Pi
//	ControlP[3]-Pe
//
// Calculate control points for arc in Bernstain basis
// arc < 180 is assumed
	double z,d,h;
	const double r = 1.f/3.f;
	BPoint p = ControlP[0] - ControlP[3] + ControlP[1]*2;// main vertex coord
	z=-(p*ControlP[1]);
	if(z == 0.)
		return 1;//error; line will be drawn
	z=1./z;
	d=ControlP[1].D2();
	p=p*(z*d) + ControlP[0] + ControlP[1];

	h=	(ControlP[3]-p).D2();
	if(h == 0.)
		return 2;//error; line will be drawn
	h= ((ControlP[3]-ControlP[0]).D2())/(4.*h);
	if(h == 0.)
		return 3;//error; line will be drawn
	h=sqrt(h);
	z=1./3.;
	ControlP[1] = (ControlP[0]*r + p*(2*r*h));

	ControlP[2] = (ControlP[3]*r + p*(2*r*h));

	return 0;

}

int BPoint::CreateArc(
				const BPoint& pbi,
				const BPoint& pei,
				const BPoint& ii,
				const BPoint& n,
				int type,
				float *&ctlarray,float *&knot,
				double eps,
				Plane pl)

{
// Pb - begin; Pe - end; Pi - Pcenter-Pb

	BMatr Transform;
	Transform.e0cong(n,BPoint(0.,0.,1.,0.));

	BPoint pb = pbi * Transform;
	BPoint pe = pei * Transform;
	BPoint i  = ii  * Transform;
	BPoint dif = (pe-pb).ProjXY();
    double dp1p2 = (dif - BPoint(0.,0.,1.,0.) * (dif*BPoint(0.,0.,1.,0.))).D2();
	BPoint q = (pb + i - pe).ProjXY();// Pq - Pcenter-Pe
	double r2 = i.D2();
	if(r2 == 0.)
		return 0;
	double sp = i * q;
	BPoint nv = q % i;// normal vector

	bool sign = (nv.GetZ() >= 0.);//"sign" of vector cross product
	// find coordinate that has maximum module
	double max = fabs(nv.GetZ());
	int k;//Number of segments
	if (fabs(max) < 1.e-6 * r2)
	{// Angle = 0 or 180
		//if (sp < 0)	// 180
		//	sign = type == ccwarc

		sign = true;
	}
	if (sp > cos(2. * PI / 180.) * r2)// Angle < 2 || >358
	{
		if( (!sign && type == ccwarc) ||
			( sign && type ==  cwarc))// Angle < 120
			k=1;	
		else// Angle > 240
			k=3;
	}
	else// Angle >120 && <240
	{
		k=2;
	}

	if(dp1p2 < eps*eps && sp > 0.)// Angle = 360
		k = 3;
// Create control points for arc


	BPoint ControlP[4];

//

	ctlarray = new float[12*k+4];
	knot = new float[5+k*3];
	knot[0]=0;
	knot[5+k*3-1]=(float)k;
	for(int l=0;l<=k;l++)
		for(int j=0;j<3;j++)
			knot[1+l*3+j]=(float)l;
	ControlP[0] = pb;
	ControlP[1] = i;
	switch(k)
	{
	case 1:// single segment
	{
		ControlP[3] = pe;
		if(0 != CreateSimpleArc(ControlP))
		{
			delete[] ctlarray;
			delete[] knot;
			return 0;
		}
		for(int v=0;v<4;v++)
			for(int j=0;j<4;j++)
				ctlarray[v*4+j]=(float)ControlP[v][j];
//		return 8;
	}
		break;

	case 2:
	{
		double l2 = (i+q).D2();
		BPoint p(0., 0., 0., 1.);
		double w;
		if(l2 < eps*eps) // Angle = 180: Assume that arc lies in XY plane
		{
			p.Set(-i.GetY(),i.GetX(),0.,0.);// 90 degree rotation in XY plane
			if(type == ccwarc)// Pcenter + w*p - middle point of arc
				w=1;
			else
				w=-1;
		}
		else // Angle !=180
		{
			w = sqrt(r2/l2);
			if(!((!sign && type == ccwarc)||( sign && type ==  cwarc)))
				w = -w;
			p=i+q;
		}
// first arc
		ControlP[3] = pb+i-p*w;
		if(0 != CreateSimpleArc(ControlP))
		{
			delete[] ctlarray;
			delete[] knot;
			return 0;
		}
		for(int v=0;v<4;v++)
			for(int j=0;j<4;j++)
				ctlarray[v*4+j]=(float)ControlP[v][j];
//second arc
		ControlP[0] = ControlP[3];
		ControlP[1] = p*w;
		ControlP[3] = pe;
		if(0 != CreateSimpleArc(ControlP))
		{
			delete[] ctlarray;
			delete[] knot;
			return 0;
		}
		for(int v=1;v<4;v++)
			for(int j=0;j<4;j++)
				ctlarray[16+(v-1)*4+j]=(float)ControlP[v][j];
	
//		return 5+3*k;
	}
		break;
	
	case 3:
	{
		double ca=1.;//cos of 1/3 Arc angle with sign
		double sa=0.;//sin of 1/3 Arc angle with sign
		double a = 0;//Arc angle with sign
		{
			ca = max(sp/r2, -1.);
			ca = min(ca, 1.);
			a = 2.*PI - acos(ca);
			if(type == cwarc) 
				a = -a;
//			if(fabs(max)<1.e-6*r2 && sp>0.)// Angle = 360 but never 0
//				sina = 1.;
			ca = cos(a/3.);
			sa = sin(a/3.);
// first arc
			q.Set(	i.GetX()*ca-i.GetY()*sa ,
					i.GetX()*sa+i.GetY()*ca ,
					0. , 0.);
			ControlP[3] = pb+i-q;
			if(0 != CreateSimpleArc(ControlP))
			{
				delete[] ctlarray;
				delete[] knot;
				return 0;
			}
			for(int f=0;f<4;f++)
				for(int j=0;j<4;j++)
					ctlarray[f*4+j]=(float)ControlP[f][j];
//				delete ctlarray;
//				delete knot;
//second arc
			ControlP[0] = ControlP[3];
			ControlP[1] = q;
			q.Set(	q[0]*ca-q[1]*sa,
					q[0]*sa+q[1]*ca,
					0.,
					0.);
			ControlP[3] = pb+i-q;
			if(0 != CreateSimpleArc(ControlP))
			{
				delete[] ctlarray;
				delete[] knot;
				return 0;
			}
			for(int f=1;f<4;f++)
				for(int j=0;j<4;j++)
					ctlarray[16+(f-1)*4+j]=(float)ControlP[f][j];
//third arc
			ControlP[0] = ControlP[3];
			ControlP[1] = q;
			ControlP[3] = pe;
			if(0 != CreateSimpleArc(ControlP))
			{
				delete[] ctlarray;
				delete[] knot;
				return 0;
			}
			for(int f=1;f<4;f++)
				for(int j=0;j<4;j++)
					ctlarray[28+(f-1)*4+j]=(float)ControlP[f][j];
	
//			return 5+3*k;
	
		}
	}
		break;
	default:
		{
			delete[] ctlarray;
			delete[] knot;
			return 0;
		}
		break;
	}
	Transform.e0cong(BPoint(0.,0.,1.,0.),n);
	for(int ll = 0; ll < 1+3*k; ll++)
	{
		double cz = (pe.GetZ() - pb.GetZ())*ll/(3.*k) + pb.GetZ();
		BPoint p = BPoint(ctlarray[ll*4+0],ctlarray[ll*4+1],cz * ctlarray[ll*4+3],ctlarray[ll*4+3])
			*Transform;
		ctlarray[ll*4+0] = (float)p.GetX();
		ctlarray[ll*4+1] = (float)p.GetY();
		ctlarray[ll*4+2] = (float)p.GetZ();
		ctlarray[ll*4+3] = (float)p.GetH();
	}
	return 5+3*k;
}



bool BPoint::Make1(double EPS)
{
	double d = sqrt(D2());
	if(d < EPS)
		return false;
	*this = (*this) * (1. / d);
	return true;
}
