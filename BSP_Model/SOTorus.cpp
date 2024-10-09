#include "stdafx.h"
#include "ResLocal.h"
#include "ConstDef.h"
#include ".\sotorus.h"
#include "math.h"

SOTorus::SOTorus(double Rl, double Rs, const BPoint & C, const BPoint & N) : Center(C), Normal(N)
{
	RLarge = Rl;
	RSmall = Rs;
}

SOTorus::~SOTorus(void)
{
}

const BPoint SOTorus::GetNormal(const BPoint & P) const
{
	// Rsmall > 0 - external normal; < 0 - internal

	BPoint SCCenter = P - Center;
	SCCenter = SCCenter - Normal * (SCCenter * Normal);// Project SCCenter to torus main plane
	double d2 = SCCenter.D2();
	if(d2 < DMIN * DMIN)
		return BPoint(0,0,-1,0);
	SCCenter = SCCenter*((1./sqrt(d2))*RLarge);
	BPoint N = P - SCCenter - Center;
	double RSmallP = sqrt(N.D2());
	if(fabs(fabs(RSmallP) - fabs(RSmall)) > fabs(RSmall) * 0.5)
		return BPoint(0., 0., 0., -1.);
	N = N*((RSmall > 0 ? 1. : -1.)/RSmallP);
	return N;
}

const BPoint SOTorus::GetPoint(const BPoint & P) const
{
	BPoint SCCenter = P - Center;
	SCCenter = SCCenter - Normal * (SCCenter * Normal);// Project SCCenter to torus main plane
	double d2 = SCCenter.D2();
	if(d2 < DMIN * DMIN)
		return P;
	SCCenter = SCCenter*((1./sqrt(d2))*RLarge);
	BPoint N = P - SCCenter - Center;
	return P + N * (1. - RSmall/sqrt(N.D2()));
}
CString SOTorus::GetTypeName() const
{
	CString tmp;
	tmp.LoadString(IDS_TYPE_TORUS);
	return tmp;
}

void SOTorus::Save(CFile * f, const BMatr &M) const
{
	SSurfType i = GetType();
	f->Write((void *)&i,sizeof(i));
	BPoint Tmp;
	Tmp = Center * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	Tmp = Normal * M;
	f->Write((void *)&Tmp, sizeof(Tmp));
	f->Write((void *)&RLarge,sizeof(RLarge));
	f->Write((void *)&RSmall,sizeof(RSmall));
	return;
}

void SOTorus::Load(CFile * f)
{
	f->Read((void *)&Center,sizeof(Center));
	f->Read((void *)&Normal,sizeof(Normal));
	f->Read((void *)&RLarge,sizeof(RLarge));
	f->Read((void *)&RSmall,sizeof(RSmall));
	return;
}

bool SOTorus::RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const
{
	DirectVector;
	RefPoint;
	return false;
}
void SOTorus::Invert()
{
	RSmall *= -1.;
}
//���� ����� �� �����-�� � ������, ������� ����� �� ���. ���� 
/*
BPoint SOTorus::Newton(BPoint a, BPoint A)//!!����� �������
{
	BPoint T;
	BMatr S;
	double t=0.0,
		f,			//�������� �������
		ft,			//����������� �� t
		ftt,	    //������ �����������
		f2t,		//����������� �� �������� ������� = 2*f*ft
		f2tt,		//������ ����������� �� �������� ������� = 2*(ft*ft+f*ftt)
		mu;			//�� (���������� t �� ������ ���� �����������)

	//��������� �������� �������
	f = pow(A.GetX(),2)+pow(A.GetY(),2)+pow(A.GetZ(),2)+
		2*(A*a)*t+
		( pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2) )*t*t-
		2*RLarge*sqrt(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)+
		RLarge*RLarge-RSmall*RSmall;

	//��������� ������ �����������
	ft=2*(A*a)+2*(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2))*t-
		2*RLarge*(  (A.GetX()*a.GetX()+A.GetY()*a.GetY())+(pow(a.GetX(),2)+pow(a.GetY(),2))*t  )/
		sqrt(  pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t  ); 

	//��������� ������ ����������� �� �������� �������
	f2t = 2*f*ft;

	//������� �� �����, ����� ����������� �� �������� ������� <= 0.001
	while(fabs(f2t)>0.001)
	{
		//��������� �������� �������
		f = pow(A.GetX(),2)+pow(A.GetY(),2)+pow(A.GetZ(),2)+
			2*(A*a)*t+
			( pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2) )*t*t-
			2*RLarge*sqrt(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)+
			RLarge*RLarge-RSmall*RSmall;

		//��������� ������ �����������
		ft=2*(A*a)+2*(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2))*t-
			2*RLarge*(  (A.GetX()*a.GetX()+A.GetY()*a.GetY())+(pow(a.GetX(),2)+pow(a.GetY(),2))*t  )/
			sqrt(  pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t  ); 

		//��������� ������ �����������
		ftt=2*(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2))-2*RLarge*
			(  (pow(a.GetX(),2)+pow(a.GetY(),2))-
			pow(( A.GetX()*a.GetX()+A.GetY()*a.GetY() + (pow(a.GetX(),2)+pow(a.GetY(),2))*t),2) / 
			(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)  )  /
			sqrt(  abs(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)  );

		//��������� ������ ����������� �� �������� �������
		f2t = 2*f*ft;

		//��������� ������ ����������� �� �������� �������
		f2tt = 2*(ft*ft+f*ftt);

		//��������� ��
		mu=-f2t/f2tt;

		//������� ��������� t
		t=t+mu;
	}
	S.Set(t,0,0,0,
		0,t,0,0,
		0,0,t,0,			  
		0,0,0,1); 				  				 
	T = A + a*S;
	return(T);
}

//���������� ������, ���� ���� ����������� ������������ � ��.
//���������� (A-����� ����������� ��. � ��.,N-������� � ��.,P-����� �� ��.)
//���������� ������ �� ����� �� ��. � ����� �� ���.����
BPoint SOTorus::a(BPoint A, BPoint N,BPoint P)//��������!!!
{
	double d, E = 1.0e-06,t;
	BPoint k,O,a,NK,v;
	BMatr S;
	k.Set(0.0,0.0,1.0,0.0);//��� ��� Oz
	NK.Set(0.0,0.0,0.0,1.0);//��
	if(N*k<E)//���� ��.����������� ��� Oz
	{
		if(abs(N*(P - NK))<RLarge-RSmall)
		{
			d = sqrt(pow(N.GetX(),2)+pow(N.GetY(),2)+pow(N.GetZ(),2));	//����� �������
			N.Set(N.GetX()/d, N.GetY()/d, N.GetZ()/d, 0.0);				//������ ������� ���������
			t = N*(P-NK);											//���������� �� ��. �� ������ ���� �� ������
			S.Set(t,0,0,0,
				0,t,0,0,
				0,0,t,0,
				0,0,0,1); 
			O = NK + N*S;											//��������� � ������ ���� ����� �� ��.
			d = sqrt(RLarge*RLarge-t*t);//���������� �� ���� ����� �� ������ �������� �������(���� t �� ����� ��������)
			a = A - O;
			t = sqrt(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2));	//����� ������� �
			a.Set(a.GetX()*d/t, a.GetY()*d/t, 0.0, 0.0);				//������ � ������ d(� - ������. ��. xOy)
			O = O + a;													//������ �������� �������
		}
		else
		{
			d = sqrt(pow(N.GetX(),2)+pow(N.GetY(),2)+pow(N.GetZ(),2));	//����� �������
			N.Set(N.GetX()/d, N.GetY()/d, N.GetZ()/d, 0.0);				//������ ������� ���������
			t = (P-NK)*N;											//���������� �� ��. �� ������ ���� �� ������
			S.Set(t,0,0,0,
				0,t,0,0,
				0,0,t,0,
				0,0,0,1); 
			O = NK + N*S;											//��������� � ������ ���� ����� �� ��.
		}
	}
	else//��. ���������� ������ ��� ����(�.�. ��� ���� �� ��������������� ������� ���������)
	{
		t = (N * (P - NK))/(N * k);
		S.Set(t,0,0,0,
			0,t,0,0,
			0,0,t,0,
			0,0,0,1); 
		O = NK + (k * S);//����� ����������� ��. � ��� ����
	}
	v=A-O;
	return (v);//���������� ������,������� � ��. � ����-� �� �.� � ����������� 
}

*/

void SOTorus::Transform(const BMatr & T)
{
	Center = Center * T;
	Normal = Normal * T;
}

