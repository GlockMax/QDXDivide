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
//зная точку на треуг-ке и вектор, находим точку на пов. тора 
/*
BPoint SOTorus::Newton(BPoint a, BPoint A)//!!Метод Ньютона
{
	BPoint T;
	BMatr S;
	double t=0.0,
		f,			//исходная функция
		ft,			//производная по t
		ftt,	    //вторая производная
		f2t,		//производная от квадрата функции = 2*f*ft
		f2tt,		//вторая производная от квадрата функции = 2*(ft*ft+f*ftt)
		mu;			//мю (приращение t на данном шаге приближения)

	//вычисляем исходную функцию
	f = pow(A.GetX(),2)+pow(A.GetY(),2)+pow(A.GetZ(),2)+
		2*(A*a)*t+
		( pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2) )*t*t-
		2*RLarge*sqrt(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)+
		RLarge*RLarge-RSmall*RSmall;

	//вычисляем первую производную
	ft=2*(A*a)+2*(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2))*t-
		2*RLarge*(  (A.GetX()*a.GetX()+A.GetY()*a.GetY())+(pow(a.GetX(),2)+pow(a.GetY(),2))*t  )/
		sqrt(  pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t  ); 

	//вычисляем первую производную от квадрата функции
	f2t = 2*f*ft;

	//выходим из цикла, когда производная от квадрата функции <= 0.001
	while(fabs(f2t)>0.001)
	{
		//вычисляем исходную функцию
		f = pow(A.GetX(),2)+pow(A.GetY(),2)+pow(A.GetZ(),2)+
			2*(A*a)*t+
			( pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2) )*t*t-
			2*RLarge*sqrt(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)+
			RLarge*RLarge-RSmall*RSmall;

		//вычисляем первую производную
		ft=2*(A*a)+2*(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2))*t-
			2*RLarge*(  (A.GetX()*a.GetX()+A.GetY()*a.GetY())+(pow(a.GetX(),2)+pow(a.GetY(),2))*t  )/
			sqrt(  pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t  ); 

		//вычисляем вторую производную
		ftt=2*(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2))-2*RLarge*
			(  (pow(a.GetX(),2)+pow(a.GetY(),2))-
			pow(( A.GetX()*a.GetX()+A.GetY()*a.GetY() + (pow(a.GetX(),2)+pow(a.GetY(),2))*t),2) / 
			(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)  )  /
			sqrt(  abs(pow(A.GetX(),2)+pow(A.GetY(),2)+2*(A.GetX()*a.GetX()+A.GetY()*a.GetY())*t+(pow(a.GetX(),2)+pow(a.GetY(),2))*t*t)  );

		//вычисляем первую производную от квадрата функции
		f2t = 2*f*ft;

		//вычисляем вторую производную от квадрата функции
		f2tt = 2*(ft*ft+f*ftt);

		//вычисляем мю
		mu=-f2t/f2tt;

		//находим очередной t
		t=t+mu;
	}
	S.Set(t,0,0,0,
		0,t,0,0,
		0,0,t,0,			  
		0,0,0,1); 				  				 
	T = A + a*S;
	return(T);
}

//вызывается только, если есть пересечение треугольника с пл.
//передаются (A-точка пересечения тр. с пл.,N-нормаль к пл.,P-точка на пл.)
//возвращает вектор от точки на тр. к точке на пов.тора
BPoint SOTorus::a(BPoint A, BPoint N,BPoint P)//работает!!!
{
	double d, E = 1.0e-06,t;
	BPoint k,O,a,NK,v;
	BMatr S;
	k.Set(0.0,0.0,1.0,0.0);//орт оси Oz
	NK.Set(0.0,0.0,0.0,1.0);//НК
	if(N*k<E)//если пл.параллельна оси Oz
	{
		if(abs(N*(P - NK))<RLarge-RSmall)
		{
			d = sqrt(pow(N.GetX(),2)+pow(N.GetY(),2)+pow(N.GetZ(),2));	//длина нормали
			N.Set(N.GetX()/d, N.GetY()/d, N.GetZ()/d, 0.0);				//делаем нормаль единичной
			t = N*(P-NK);											//расстояние от пл. до центра тора со знаком
			S.Set(t,0,0,0,
				0,t,0,0,
				0,0,t,0,
				0,0,0,1); 
			O = NK + N*S;											//ближайшая к центру тора точка на пл.
			d = sqrt(RLarge*RLarge-t*t);//расстояние от этой точки до начала искомого вектора(знак t не имеет значения)
			a = A - O;
			t = sqrt(pow(a.GetX(),2)+pow(a.GetY(),2)+pow(a.GetZ(),2));	//длина вектора а
			a.Set(a.GetX()*d/t, a.GetY()*d/t, 0.0, 0.0);				//делаем а длиной d(а - паралл. пл. xOy)
			O = O + a;													//начало искомого вектора
		}
		else
		{
			d = sqrt(pow(N.GetX(),2)+pow(N.GetY(),2)+pow(N.GetZ(),2));	//длина нормали
			N.Set(N.GetX()/d, N.GetY()/d, N.GetZ()/d, 0.0);				//делаем нормаль единичной
			t = (P-NK)*N;											//расстояние от пл. до центра тора со знаком
			S.Set(t,0,0,0,
				0,t,0,0,
				0,0,t,0,
				0,0,0,1); 
			O = NK + N*S;											//ближайшая к центру тора точка на пл.
		}
	}
	else//пл. пересекает вблизи ось тора(т.е. ось тора не перпендикулярна нормали плоскости)
	{
		t = (N * (P - NK))/(N * k);
		S.Set(t,0,0,0,
			0,t,0,0,
			0,0,t,0,
			0,0,0,1); 
		O = NK + (k * S);//точка пересечения пл. и оси тора
	}
	v=A-O;
	return (v);//возвращаем вектор,лежащий в пл. и напр-й из т.А к поверхности 
}

*/

void SOTorus::Transform(const BMatr & T)
{
	Center = Center * T;
	Normal = Normal * T;
}

