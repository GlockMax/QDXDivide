// реализация численного метода

#include "stdafx.h"
#include "NPartSurface.h"
#include "BPoint.h"
#include "math.h"

// определение точки на параметрической кривой 
// принадлежащей линии очерка (численный метод)
// Au - координата начала участка u-кривой
// Bu - координата конца участка u-кривой
// Av - координата начала участка v-кривой
// Bv - координата конца участка v-кривой
double NPartSurface::ZeroIn(double Au, double Bu, double Av, double Bv, 
							double Tol, BPoint direct)
{
	double zer;
	double A,B,C,D,E,Eps,Fa,Fb,Fc,Tol1,Xm,p,q,r,s,con;
	bool type_line, f_goto;
	int num_step;
	// A - точка начала интервала или предыдущее приближение к корню
	// В - точка конца интервала или последнее и лучшее приближение
	// С - предыдущее или более раннее приближение 
	// Fa - значение искомой функции в точке А
	// Fb - значение искомой функции в точке В
	// Fc - значение искомой функции в точке С
	// Eps - относительная машинная погрешность
	// type_line - флаг если v=const т.е. u- линия (0-нет, 1-да)

// определение Eps относительной машинной погрешности
	Eps=1.0;
	Eps=Eps/2.0;
	Tol1=1.0+Eps;
	while (Tol1 > 1.0)
	{
		Eps=Eps/2.0;
		Tol1=1.0+Eps;
	}
// присвоение начальных значений
// и определение на какой u- или v- линии мы ищем корень
	if (Au==Bu)	// если u=const т.е. v- линия
	{
		A=Av;	B=Bv;	con=Au;		type_line=false;
	}
	else		// если v=const т.е. u- линия
	{
		A=Au;	B=Bu;	con=Av;		type_line=true;
	}
	if (type_line==true)
	{
		Fa = GetNormalOnDirection(A,con,direct);
		Fb = GetNormalOnDirection(B,con,direct);
	}
	else
	{
		Fa = GetNormalOnDirection(con,A,direct);
		Fb = GetNormalOnDirection(con,B,direct);
	}
	f_goto = false;
	num_step = 0;
// начать щаг
	C=A;
	Fc=Fa;
	D=B-A;
	E=D;
	if (fabs(Fc) <= fabs(Fb))
	{
		A=B;
		B=C;
		C=A;
		Fa=Fb;
		Fb=Fc;
		Fc=Fa;
	}
	Tol1=2.0*Eps*fabs(B)+0.5*Tol;
	Xm=0.5*(C-B);
	num_step++;
	// проверка сходимости
	// и реализация основного алгоритма
	while ((fabs(Xm) >= Tol1) && (Fb != 0.0) || num_step >= 100.)
	//while ((fabs(Xm) >= Tol1) && (Fb != 0.0) && (fabs(A-B) != 0.0))
	{
		// необходима ли бисекция
		if (fabs(E) < Tol1)
		{
			D=Xm;
			E=D;
			f_goto = true;
		}
		// необходима ли бисекция
		if (fabs(Fa) <= fabs(Fb) && !f_goto)
		{
			D=Xm;
			E=D;
			f_goto = true;
		}
		if (!f_goto)
		{
			//возможна ли квадратичная интерполяция
			if (A != C) // обратная квадратичная интерполяция
			{	
				q=Fa/Fc;
				r=Fb/Fc;
				s=Fb/Fa;
				p=s*(2.0*Xm*q*(q-r)-(B-A)*(r-1.0));
				q=(q-1.0)*(r-1.0)*(s-1.0);
			}
			else // линейная интерполяция
			{
				s=Fb/Fa;
				p=2.0*Xm*s;
				q=1.0-s;
			}
			// выбрать знаки
			if (p > 0.0) q=-q;
			p=fabs(p);
			// приемлима ли интерполяция
			if ((2.0*p) >= (3.0*Xm*q-fabs(Tol1*q)))
			{
				D=Xm;
				E=D;
				f_goto = true;
			}
			if (p >= fabs(0.5*E*q) && !f_goto)
			{
				D=Xm;
				E=D;
				f_goto = true;
			}

			if (!f_goto)
			{
				E=D;
				D=p/q;
			}
		}
		// завершить шаг

		A=B;
		Fa=Fb;
		if (fabs(D) > Tol1) B=B+D;
		if (fabs(D) <= Tol1) B=B+Xm*Tol1;
		if (type_line==true)
		{
			Fb = GetNormalOnDirection(B,con,direct);
		}
		else
		{
			Fb = GetNormalOnDirection(con,B,direct);
		}
		if ((Fb*(Fc/fabs(Fc))) > 0.0) 
		{
			C=A;
			Fc=Fa;
    		D=B-A;
			E=D;
		}
		if (fabs(Fc) < fabs(Fb))
		{
			A=B;
			B=C;
			C=A;
			Fa=Fb;
			Fb=Fc;
			Fc=Fa;
		}
		Tol1=2.0*Eps*fabs(B)+0.5*Tol;
		Xm=0.5*(C-B);
		f_goto = false;
		num_step++;
	}
	// конец
	zer=B;

	return zer;
}