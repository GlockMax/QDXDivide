// MTInterpretator.cpp: implementation of the MTInterpretator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MachineTool.h"
#include "MTInterpretator.h"
#include "MTGOTOCommand.h"
#include "MTCIRCLECommand.h"
#include "MTCoordinate.h"
#include "NCadrGeom.h"
#include "BMatr.h"
#include "math.h"
#include "gl/gl.h"
#include "gl/glu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//const double PI = 3.14159265358979;

MTInterpretator::MTInterpretator()
{
//	NumCoor=3;
	CoorN.Add("x");
	CoorN.Add("y");
	CoorN.Add("z");
	CoorN.Add("a");
	CoorN.Add("b");
	Flag=0;
	Counter=-1;
//	CoorN=NULL;
}

MTInterpretator::~MTInterpretator()
{
	CoorN.RemoveAll();
}

BOOL MTInterpretator::NewProgram(CString File)
{
	CStdioFile f;
	EraseAll();
	if(f.Open(File,CFile::modeRead|CFile::typeText))
	{
		Flag=0;
		ProgramFile=File;
		f.Close();
		Counter=-1;
		return TRUE;
	}
	return FALSE;
}



BOOL MTInterpretator::FirstCommand(/*MTCommand *retCommand*/)
{
	CStdioFile f;
	CString str;
	Counter=0;
	if(!f.Open(ProgramFile,CFile::typeText|CFile::modeRead))
		return FALSE;
	f.SeekToBegin();
	while(f.ReadString(str))
	{
		if(Interpretate(str))
		{
			f.Close();
			Counter=0;
			return TRUE;
		}
		
	}
	f.Close();
	return FALSE;

}


BOOL MTInterpretator::Interpretate(CString str)
{
	int i,type=0;
	CString temp;
	float d[7];
	for(i=0;i<5;i++)
		d[i]=0;
	if(str.Find("GOTO/")!=-1)
		type=1;
	else if(str.Find("CIRCLE/")!=-1)
		type=2;
	else
		return FALSE;
	MTCoordinate cor;
	switch (type)
	{
	case 1:
		{
			CString s;
			int m;
			//m=sscanf(str,"%f, %f, %f, %f, %f",&d[0],&d[1],&d[2],&d[3],&d[4]);
			m=sscanf(str,"%s %f, %f, %f,%f,%f",s,&d[0],&d[1],&d[2],&d[3],&d[4]);
			/*if(m!=NumCoor)
				return FALSE;*/
			m--;
			Command.CommandParam.RemoveAll();
			if(Flag==1)
					Flag=2;
				else
					Flag=0;
			for(i=0;i<m/*NumCoor*/;i++)
			{
				cor.AxisName=CoorN[i];
				cor.Value=d[i];
				Command.CommandParam.Add(cor);
			}
			return TRUE;
		}
	case 2:
		{
			CString s;
			if(sscanf(str,"%s %f,%f,%f,%f,%f,%f,%f",s,&d[0],&d[1],&d[2],&d[3],&d[4],&d[5],&d[6])<7)
				return FALSE;
			Flag=1;
			cor.AxisName="cx";
			cor.Value=d[0];
			Circle.CommandParam.Add(cor);
			//*
			cor.AxisName="cy";
			cor.Value=d[1];
			Circle.CommandParam.Add(cor);
			//*
			cor.AxisName="cz";
			cor.Value=d[2];
			Circle.CommandParam.Add(cor);
			//*
			cor.AxisName="nx";
			cor.Value=d[3];
			Circle.CommandParam.Add(cor);
			//*
			cor.AxisName="ny";
			cor.Value=d[4];
			Circle.CommandParam.Add(cor);
			//*
			cor.AxisName="nz";
			cor.Value=d[5];
			Circle.CommandParam.Add(cor);
			//*
			cor.AxisName="R";
			cor.Value=d[6];
			Circle.CommandParam.Add(cor);
			//*
			return FALSE;
		}
	}
	return FALSE;
}

void MTInterpretator::SetParametrs(CString &Names, int Num)
{
	if(Num==0)
		return;
	if(CoorN.GetSize()!=0)
		CoorN.RemoveAll();
	NumCoor=Num;
//DELETED
//	for(i=0;i<Num;i++)
//		CoorN.Add(Names[i]);
	Counter=-1;
}



BOOL MTInterpretator::NextCommand(/*MTCommand *retCommand*/)
{
	CStdioFile f;
	CString str;
	int k=-1;
	Flag=0;
	if(!f.Open(ProgramFile,CFile::typeText|CFile::modeRead))
		return FALSE;
	f.SeekToBegin();
	while(f.ReadString(str) && k<Counter+1)
	{
		if(Interpretate(str))
		{
			k++;
		}
	}
	if (k==Counter+1)
	{
		Counter++;
		f.Close();
		return TRUE;
	}
	f.Close();
	return FALSE;
}

int MTInterpretator::GetCount()
{
	return Counter;
}
int MTInterpretator::CreateArc(BPoint pb,BPoint pe,BPoint i,BPoint n,int type,float *&ctlarray,float *&knot,double eps,Plane pl)
{
// Pb - begin; Pe - end; Pi - Pcenter-Pb

	BMatr Transform;
	Transform.e0cong(n,BPoint(0.,0.,1.,0.));

	pb = pb * Transform;
	pe = pe * Transform;
	i  = i  * Transform;
/*	switch(pl)
	{
	case XY:
		i.Set(i.GetX(),i.GetY(),0.,0.);
		break;
	case XZ:
		i.Set(i.GetX(),0.,i.GetZ(),0.);
		break;
	case YZ:
		i.Set(0.,i.GetY(),i.GetZ(),0.);
		break;
	}
*/	
	double dp1p2 = (pe-pb).D2();
	if(dp1p2 < eps*eps )// pb=pe
	{
		pb = ( pb + pe )*0.5;
		pe = pb;
	}
	BPoint q = pb + i - pe;// Pq - Pcenter-Pe
	double r2 = i.D2();
	if(r2 == 0.)
		return 0;
	double sp = i * q;
	BPoint nv = q % i;// normal vector

	bool sign = (nv.GetZ() >= 0.);//"sign" of vector cross product
/*	switch(pl)
	{
	case XY:
		sign = (n.GetZ() >= 0.);
		break;
	case XZ:
		sign = (n.GetY() >= 0.);
		break;
	case YZ:
		sign = (n.GetX() >= 0.);
		break;
	}
*/
	// find coordinate that has maximum module
	double max = fabs(nv.GetZ());
//	max = fabs(n.GetX())>fabs(n.GetY())?n.GetX():n.GetY();
//	max = fabs(max)>fabs(n.GetZ())?max:n.GetZ();
	if(fabs(max)<1.e-6*r2)
		sign = true;
	int k;//Number of segments
	if(sp>cos(2.*3.1415/180.)*r2)// Angle < 2 || >358
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
			delete ctlarray;
			delete knot;
			return 0;
		}
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				ctlarray[i*4+j]=(float)ControlP[i][j];
//		return 8;
	}
		break;

	case 2:
	{
		double l2 = (i+q).D2();
		BPoint p;
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
			delete ctlarray;
			delete knot;
			return 0;
		}
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				ctlarray[i*4+j]=(float)ControlP[i][j];
//second arc
		ControlP[0] = ControlP[3];
		ControlP[1] = p*w;
		ControlP[3] = pe;
		if(0 != CreateSimpleArc(ControlP))
		{
			delete ctlarray;
			delete knot;
			return 0;
		}
		for( i=1;i<4;i++)
			for(int j=0;j<4;j++)
				ctlarray[16+(i-1)*4+j]=(float)ControlP[i][j];
	
//		return 5+3*k;
	}
		break;
	
	case 3:
	{
		double ca=1.;//cos of 1/3 Arc angle with sign
		double sa=0.;//sin of 1/3 Arc angle with sign
		double a = 0;//Arc angle with sign
		{
			ca = sp/r2;
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
				delete ctlarray;
				delete knot;
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
				delete ctlarray;
				delete knot;
				return 0;
			}
			for( f=1;f<4;f++)
				for(int j=0;j<4;j++)
					ctlarray[16+(f-1)*4+j]=(float)ControlP[f][j];
//third arc
			ControlP[0] = ControlP[3];
			ControlP[1] = q;
			ControlP[3] = pe;
			if(0 != CreateSimpleArc(ControlP))
			{
				delete ctlarray;
				delete knot;
				return 0;
			}
			for( f=1;f<4;f++)
				for(int j=0;j<4;j++)
					ctlarray[28+(f-1)*4+j]=(float)ControlP[f][j];
	
//			return 5+3*k;
	
		}
	}
		break;
	default:
		{
			delete ctlarray;
			delete knot;
			return 0;
		}
		break;
	}
	Transform.e0cong(BPoint(0.,0.,1.,0.),n);
	BPoint p;
	for(int ll = 0; ll < 1+3*k; ll++)
	{
		p = BPoint(ctlarray[ll*4+0],ctlarray[ll*4+1],ctlarray[ll*4+2],ctlarray[ll*4+3])
			*Transform;
		ctlarray[ll*4+0] = (float)p.GetX();
		ctlarray[ll*4+1] = (float)p.GetY();
		ctlarray[ll*4+2] = (float)p.GetZ();
		ctlarray[ll*4+3] = (float)p.GetH();
	}
	return 5+3*k;
}


int MTInterpretator::CreateSimpleArc(BPoint *ControlP)
{
//	ControlP[0]-Pb
//	ControlP[1]-Pi
//	ControlP[3]-Pe
//
// Calculate control points for arc in Bernstain basis
// arc < 180 is assumed
	BPoint p;// main vertex coord
	double z,d,h;
	const double r = 1.f/3.f;
	p=ControlP[0]-ControlP[3]+ControlP[1]*2;
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

void MTInterpretator::GetArcPoint(double u, int nknots, float *ctlarray, float *knot, double &x, double &y, double &z)
{
	int k = (nknots - 5)/3;// the segments number
	BPoint p,p0,p1,p2,p3;
	int i = int(u*k-10e-6); // number of selected segment
	double t = u*k - i;// parameter on selected segment;
	p0.Set(ctlarray[12*i+0],ctlarray[12*i+1],ctlarray[12*i+2],ctlarray[12*i+3]);
	p1.Set(ctlarray[12*i+0+4],ctlarray[12*i+1+4],ctlarray[12*i+2+4],ctlarray[12*i+3+4]);
	p2.Set(ctlarray[12*i+0+8],ctlarray[12*i+1+8],ctlarray[12*i+2+8],ctlarray[12*i+3+8]);
	p3.Set(ctlarray[12*i+0+12],ctlarray[12*i+1+12],ctlarray[12*i+2+12],ctlarray[12*i+3+12]);
	p = p0*((1.-t)*(1.-t)*(1.-t)) + p1*(3.*(1.-t)*(1.-t)*t)
		+ p2*(3.*(1.-t)*t*t) + p3*(t*t*t);
	x=p.GetX()/p.GetH();
	y=p.GetY()/p.GetH();
	z=p.GetZ()/p.GetH();
}

BOOL MTInterpretator::GetNewPosition(int steps,int step,MTGOTOCommand &coord,const MTGOTOCommand Start)
{
	if(Counter==-1)
		return FALSE;
	int j,k;
	MTCoordinate NC;
	coord.CommandParam.RemoveAll();
	if(Flag<2)
	{
//		coord.CommandParam.RemoveAll();
		double value;
		for(j=0;j<Start.CommandParam.GetSize();j++)
		{
			BOOL ftest;
			ftest=FALSE;
			for(k=0;k<Command.CommandParam.GetSize();k++)
			{
				
				if(Command.CommandParam[k].AxisName==Start.CommandParam[j].AxisName)
				{
					value=Command.CommandParam[k].Value-Start.CommandParam[j].Value;
					ftest=TRUE;
					break;//**
				}
			}
			if(ftest)
			{
				value=Start.CommandParam[j].Value+value*step/steps;
				NC.AxisName=Command.CommandParam[j].AxisName;
				NC.Value=value;
				coord.CommandParam.Add(NC);
			}
		}
		return TRUE;
	}
	if(Flag==2)
	{
		if(Circle.CommandParam.GetSize()==0 || Command.CommandParam.GetSize()==0)
			return FALSE;
		float *ctlarray,*knot;
		ctlarray=NULL;
		knot=NULL;
		int nknots;
		BPoint ps,pe,vc,norm;
		vc.Set(Circle.CommandParam[0].Value,Circle.CommandParam[1].Value,Circle.CommandParam[2].Value,1);
		norm.Set(Circle.CommandParam[3].Value,Circle.CommandParam[4].Value,Circle.CommandParam[5].Value,0);
		double d[5];
		d[0]=0;
		d[1]=0;
		d[2]=0;
		d[3]=0;
		d[4]=0;
		if(Start.CommandParam.GetSize()>=1)
			d[0]=Start.CommandParam[0].Value;
		if(Start.CommandParam.GetSize()>=2)
			d[1]=Start.CommandParam[1].Value;
		if(Start.CommandParam.GetSize()>=3)
			d[2]=Start.CommandParam[2].Value;
		ps.Set(d[0],d[1],d[2],1);
		
		/*if(Start.CommandParam.GetSize()<3)
		{
			ps.Set(Start.CommandParam[0].Value,Start.CommandParam[1].Value,0,1);
		}
		else
		{
				ps.Set(Start.CommandParam[0].Value,Start.CommandParam[1].Value,Start.CommandParam[2].Value,1);
		}*/
		pe.Set(Command.CommandParam[0].Value,Command.CommandParam[1].Value,Command.CommandParam[2].Value,1);
		vc=vc-ps;
		BPoint test1,test2,tn,tnorm;
		tnorm.Set(0,0,1,0);
		tn.Set(-1,0,0,0);
		test1.Set(1,0,0,1);
		test2.Set(0,1,0,1);
	
	
//		nknots=CreateArc(test1,test2,tn,tnorm,ccwarc,ctlarray,knot,(1.e-05),XY);
//		GetArcPoint(0.5,nknots,ctlarray,knot,d[0],d[1],d[2]);
		if(Circle.CommandParam[3].Value==1)
			nknots=CreateArc(ps,pe,vc,norm,ccwarc,ctlarray,knot,1.e-05,YZ);
		if(Circle.CommandParam[3].Value==-1)
			nknots=CreateArc(ps,pe,vc,norm,cwarc,ctlarray,knot,1.e-05,YZ);
		if(Circle.CommandParam[4].Value==1)
			nknots=CreateArc(ps,pe,vc,norm,ccwarc,ctlarray,knot,1.e-05,XZ);
		if(Circle.CommandParam[4].Value==-1)
			nknots=CreateArc(ps,pe,vc,norm,cwarc,ctlarray,knot,1.e-05,XZ);
		if(Circle.CommandParam[5].Value==1)
			nknots=CreateArc(ps,pe,vc,norm,ccwarc,ctlarray,knot,1.e-05,XY);
		if(Circle.CommandParam[5].Value==-1)
			nknots=CreateArc(ps,pe,vc,norm,cwarc,ctlarray,knot,1.e-05,XY);
		/*if(Circle.CommandParam[3].Value==1 ||Circle.CommandParam[4].Value==1||Circle.CommandParam[5].Value==1)
			nknots=CreateArc(ps,pe,vc,norm,ccwarc,ctlarray,knot);
		if(Circle.CommandParam[3].Value==-1 ||Circle.CommandParam[4].Value==-1||Circle.CommandParam[5].Value==-1)
			nknots=CreateArc(ps,pe,vc,norm,cwarc,ctlarray,knot);*/
		if(nknots==0)
			return FALSE;
//		double d[5];
		GetArcPoint(((double)step/steps),nknots,ctlarray,knot,d[0],d[1],d[2]);
		MTCoordinate crd;
		crd.AxisName="x";
		crd.Value=d[0];
		coord.CommandParam.Add(crd);
		crd.AxisName="y";
		crd.Value=d[1];
		coord.CommandParam.Add(crd);
		crd.AxisName="z";
		crd.Value=d[2];
		coord.CommandParam.Add(crd);
		if(Start.CommandParam.GetSize()>=4 && Command.CommandParam.GetSize()>=4)
		{
			double dl;
			dl=(Command.CommandParam[3].Value-Start.CommandParam[3].Value)*step/steps;
			d[3]=Start.CommandParam[3].Value+dl;
			crd.AxisName="a";
			crd.Value=d[3];
			coord.CommandParam.Add(crd);
		}
		if(Start.CommandParam.GetSize()>=5 && Command.CommandParam.GetSize()>=5)
		{
			double dl;
			dl=(Command.CommandParam[4].Value-Start.CommandParam[4].Value)*step/steps;
			d[3]=Start.CommandParam[4].Value+dl;
			crd.AxisName="b";
			crd.Value=d[4];
			coord.CommandParam.Add(crd);
		}
		if(ctlarray)
			delete ctlarray;
		if(knot)
			delete knot;
		return TRUE;		
	}
	return FALSE;
}

/*int MTInterpretator::GetCadrLength (MTGOTOCommand start)
{
	if(Command==NULL)
		return -1;
	double L,La,Lb,d[5];
	int i;
	for(i=0;i<start.CommandParam.GetSize();i++)
	{
		d[i]=start.CommandParam[i].Value;
	}
	BPoint ps,pe;
	pe.Set(Command->CommandParam[0].Value,Command->CommandParam[1].Value,Command->CommandParam[2].Value,1);
	int k;
	k=0;
	k=start.CommandParam.GetSize();
	switch(k)
	{
	case 0:
		{
			return -1;
		}
	case 1:
		{
			ps.Set(d[0],0,0,1);
			break;
		}
	case 2:
		{
			ps.Set(d[0],d[1],0,1);
			break;
		}
	default:
		{
			ps.Set(d[0],d[1],d[2],1);
			break;
		}
	}//end switch
	if(Flag==1)
	{
		pe=pe-ps;
		L=sqrt(pe.D2());
	}//end if
	if(Flag==2)
	{
	/*	NCadrGeom cg;
		cg.Set(ccwarc,ps.GetX(),ps.GetY(),ps.GetZ(),pe.GetX(),pe.GetY(),pe.GetZ(),
			Command->CommandParam[0].Value,

	}

	}*/

void MTInterpretator::Remote()
{
	Flag=0;
	Counter=-1;
}

CString MTInterpretator::GetProgrammName()
{
	return ProgramFile;
}

int MTInterpretator::GetNumberOfCadr()
{
	CStdioFile F;
	if(!F.Open(ProgramFile,CFile::modeRead|CFile::typeText))
		return 0;
	CString str;
	int k;
	k=0;
	while(F.ReadString(str))
	{
		if(str.Find("GOTO/")!=-1)
			k++;
	}
	F.Close();
	return k;
}

void MTInterpretator::EraseAll()
{
	Counter=-1;
	Flag=0;
	Command.CommandParam.RemoveAll();
}
