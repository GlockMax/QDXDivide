#include "stdafx.h"
#include "ConstDef.h"
#include "NToolStd.h"
#include "NToolDiskStd.h"
#include "NToolBRound.h"
#include "math.h"
#include "NToolShaped.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const float d = 1.f/3.f;


bool NToolStd::CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f)
{
	if (sizeof(pts) == 0 || f == -1)
		return false;

//	int size = sizeof(type)/sizeof(type[0]);
	int *int_type = new int[size];
	bool res = CreateToolContour(pts, int_type, f);
	for (int i=0; i<size; i++)
	{
		if (int_type[i] == 1)
			type[i] = HORIZONTAL;
		if (int_type[i] == 2)
			type[i] = CONE;
		if (int_type[i] == 3)
		{
			if(fabs(pts[(i + 1) * 3 * 4]) < MIND)
				type[i] = SPHERE;
			else
				type[i] = TORAHS;
		}
		if (int_type[i] == 4)
			type[i] = TORAHS;
	}
	delete []int_type;
	return res;
}

// определение точек задающих котур инструмента
bool NToolStd::CreateToolContour(GLfloat *pts,int *type, BOOL f)
{
// получает:
	// pt - массив координат точек(размерность уже задана,
	//		т.е. число точек контура известно)
	// type - массив описываюший типы линий контура 
	//		1 - горизнтальная линия
	//		2 - конус
	//		3 - сфера
	// f - тип контура (рабочая или нерабочая часть)

// примечание:
	//	контур определяется только для активной части инструмента
	// Zdisp = 0 - построение в нуле


	if (sizeof(pts) == 0 || f == -1)
		return false;

	double cx = 0., cy = 0.;//учёт смещения для типа 10
	if (GetType() == 10)
	{
		cy = DBot/2.*tan(BAlf);
		cx = cy * tan(Alf);
		DTop = DBot + 2.*(H-cy)*tan(Alf);
	}
	else
		DTop = DBot + 2.*H*tan(Alf);

	double y = h;
	double x = y*tan(Alf) - cx;
	int i;

	if( f == FALSE) // нерабочая часть инструмента
	{
	// крайние точки участков контура
	// Start point
		pts[0*4] = 0;	
		pts[0*4+1] = 0;
		pts[0*4+2] = (float)H;
		pts[0*4+3] = 1;	
	//Top line
		pts[3*4] = (float)DTop/2.f;
		pts[3*4+1] = 0;
		pts[3*4+2] = (float)H;
		pts[3*4+3] = 1;
		type[0] = 1;
	// Draft line
		if (h != H && h < H)
		{
			pts[6*4] = (float)(DBot/2.+x);
			pts[6*4+1] = 0;
			pts[6*4+2] = (float)y;
			pts[6*4+3] = 1;
			type[1] = 2;
		}
	// -------------------------	
	// остальные точки
		for(i=0;i<4;i++)
		{
			pts[1*4+i] = (pts[0*4+i] + pts[3*4+i])*0.5f;
			pts[2*4+i] = pts[1*4+i];
			pts[4*4+i] = (pts[3*4+i] + pts[6*4+i])*0.5f;
			pts[5*4+i] = pts[4*4+i];
		}
	}
	if ( f == TRUE) //рабочая часть инструмента
	{
	// определение крайних точек участков контура
		int k = 3;
	// Draft line
		pts[0*4] = (float)(DBot/2.+x);
		pts[0*4+1] = 0;
		pts[0*4+2] = (float)y;
		pts[0*4+3] = 1;
		type[0] = 2;
		
		x = R*tan((PI/2.-Alf-BAlf)/2.);// EK
		y = x*sin(Alf);

	// Arc or Faset (If exists)
//		if (R != 0.)
		{
			if ((BAlf == 0. && GetType() != 10) ||
				GetType() == 10)		// Arc
			{
				pts[k*4] = (float)(DBot/2.+y);		
				pts[k*4+1] = 0;
				if (GetType() != 10)
				{
					pts[k*4+2] = (float)(x*cos(Alf));
				}			
				else	// APT 7
				{
					pts[k*4+2] = (float)(cy+x*cos(Alf));
				}
				pts[k*4+3] = 1;
				type[k/3] = 3;

				pts[(k+2)*4] = (float)(DBot/2.);
				pts[(k+2)*4+1] = 0;
				if (GetType() != 10)
					pts[(k+2)*4+2] = 0;
				else
					pts[(k+2)*4+2] = (float)cy;
				pts[(k+2)*4+3] = 1;
			}
			if (BAlf != 0. && GetType() != 10)	// Faset
			{
				x = R;		// в переменной R содержится значение Faset
				y = x*tan(BAlf);
				pts[k*4] = (float)DBot/2.f;
				pts[k*4+1] = 0;		
				pts[k*4+2] = (float)y;
				pts[k*4+3] = 1;
				type[k/3] = 2;
			}
		}

		k = 6;
	//Bottom line (If exists)
//		if ( x*cos(BAlf) < DBot*0.5 )// EK
		{
			if ((BAlf != 0. && R == 0) || GetType() == 10)	
			{
				if (GetType() != 10)	// сверло
				{
					pts[3*4] = (float)(DBot/2.);
					pts[3*4+1] = 0;
					pts[3*4+2] = (float)((DBot/2.)*tan(BAlf));
					type[3/3] = 2;
					pts[(3+2)*4+2] = pts[3*4+2];
					pts[k*4] = (float)(DBot/2.);
					pts[k*4+1] = 0;
					pts[k*4+2] = (float)((DBot/2.)*tan(BAlf));
					type[k/3] = 2;
				}
				else	// APT 7
				{
					pts[k*4] = (float)(DBot/2.-x*cos(BAlf));
					pts[k*4+1] = 0;
					pts[k*4+2] = (float)(cy-x*sin(BAlf));
					type[k/3] = 2;
				}
			}
			else			// не сверло и не APT 7
			{
				pts[k*4] = (float)(DBot/2.-x);
				pts[k*4+1] = 0;
				pts[k*4+2] = 0;
				type[k/3] = 1;
			}
			pts[k*4+3] = 1;
		}
		k = 3;
		{
			if (BAlf != 0. && GetType() != 10)	// Faset
			{
				pts[(k + 2) * 4 + 0] = (pts[k * 4 + 0] + pts[(k + 3) * 4 + 0]) * 0.5f;
				pts[(k + 2) * 4 + 1] = (pts[k * 4 + 1] + pts[(k + 3) * 4 + 1]) * 0.5f;
				pts[(k + 2) * 4 + 2] = (pts[k * 4 + 2] + pts[(k + 3) * 4 + 2]) * 0.5f;
				pts[(k + 2) * 4 + 3] = 1.;
			}
		}
		k = 9;
		pts[k*4] = 0.;
		pts[k*4+1] = 0.;
		pts[k*4+2] = 0.;
		pts[k*4+3] = 1.;
	// --------------
	// остальные точки
		for(i=0;i<4;i++)
		{
			pts[1*4+i] = (pts[0*4+i] + pts[3*4+i])*0.5f;
			pts[2*4+i] = pts[1*4+i];
			pts[4*4+i] = pts[5*4+i];
			pts[7*4+i] = (pts[6*4+i] + pts[9*4+i])*0.5f;
			pts[8*4+i] = pts[7*4+i];
		}
	}

	return true;
}

bool NToolDiskStd::CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f)
{
	if (sizeof(pts) == 0 || f == -1)
		return false;

//	int size = sizeof(type)/sizeof(type[0]);
	int *int_type = new int[size];
	bool res = CreateToolContour(pts, int_type, f);
	for (int i=0; i<size; i++)
	{
		if (int_type[i] == 1)
			type[i] = HORIZONTAL;
		if (int_type[i] == 2)
			type[i] = CONE;
		if (int_type[i] == 3)
			type[i] = SPHERE;
		if (int_type[i] == 4)
			type[i] = TORAHS;
	}
	delete []int_type;

	return true;
}

// определение точек задающих котур инструмента
bool NToolDiskStd::CreateToolContour(GLfloat *pts,int *type, BOOL f)
{
	// получает:
	// pt - массив координат точек(размерность уже задана,
	//		т.е. число точек контура известно)
	// type - массив описываюший типы линий контура 
	//		1 - горизнтальная линия
	//		2 - конус
	//		3 - сфера
	// f - тип контура (рабочая или нерабочая часть)

// примечание:
	//	контур определяется только для активной части инструмента
	// Zdisp = 0 - построение в нуле

	if (sizeof(pts) == 0 || f == -1)
		return false;

	//Create NURBS control points


// определение крайних точек участков контура

///////////////// нерабочая часть инструмента ///////////////////////////
	if (f == FALSE)	// 
	{
	// начальная точка
		pts[0*4] = 0;	
		pts[0*4+1] = 0;
		pts[0*4+2] = (float)B;
		pts[0*4+3] = 1;
	// правая сторона
		pts[3*4] = (float)(D/2.-h2);
		pts[3*4+1] = 0;
		pts[3*4+2] = (float)B;
		pts[3*4+3] = 1;
		type[0] = 1;
	// верхняя часть
		pts[6*4] = (float)(D/2.-h1);
		pts[6*4+1] = 0;
		pts[6*4+2] = (float)0;
		pts[6*4+3] = 1;
		type[1] = 1;
	// левая сторона
		pts[9*4] = (float)0;
		pts[9*4+1] = 0;
		pts[9*4+2] = (float)0;
		pts[9*4+3] = 1;
		type[2] = 1;
	// -------------------------	

		for(int i=0;i<4;i++)
		{
			pts[1*4+i] = pts[0*4+i]*2.f*d+d*pts[3*4+i];
			pts[2*4+i] = pts[0*4+i]*d+2.f*d*pts[3*4+i];
			pts[4*4+i] = pts[3*4+i]*2.f*d+d*pts[6*4+i];
			pts[5*4+i] = pts[3*4+i]*d+2.f*d*pts[6*4+i];
			pts[7*4+i] = pts[6*4+i]*d+2.f*d*pts[9*4+i];
			pts[8*4+i] = pts[6*4+i]*d+2.f*d*pts[9*4+i];
		}
	}
	
///////////////// активная часть инструмента ///////////////////////////
	if (f == TRUE)
	{
		double e, f, g;
		(L1 != 0. && Alf !=0.) ? e = L1 + R3*tan((PI/2.-Alf)/2.) 
			: e = R1;
		(L2 != 0. && BAlf != 0.) ? g = L2 + R4*tan((PI/2.-BAlf)/2.)
			: g = R2;
		f = B - e - g;
/*		int LinesNumber = 9; // Lines Number in a contour
		if ( R1 == 0.)				// нет скруглений на инструменте
			LinesNumber--;
		if ( R2 == 0.)
			LinesNumber--;
		if ( R3 == 0.)
			LinesNumber--;
		if (R4 == 0. )
			LinesNumber--;
		if (L1 == 0 && Alf == 0.)	// нет фаски c правой стороны
			LinesNumber--;
		if (L2 == 0 && BAlf == 0.) 	// нет фаски с левой стороны
			LinesNumber--;
		if (f == 0. )//|| (R2 != 0. && R3 != 0.))
			LinesNumber--;
*/
	/*	if (L1+L2 == B	  // пока не решено // нет верхнего участка
			|| (R1 == B/2. && R2 == B/2.))
			LinesNumber--;*/
		
	// определение крайних точек участков контура
		double a;
		(BAlf != 0.) ? a = R2*tan(BAlf/2.) : a = R2;
		double b = a*sin(BAlf);
		double c = L2*tan(PI/2.-BAlf);

		int k = 6;
	// line (point 0)
		pts[0*4] = (float)(D/2.-h2);
		pts[0*4+1] = 0;
		pts[0*4+2] = (float)B;
		pts[0*4+3] = 1;	
	// line (point 3)
		pts[3*4] = (float)(D/2.f - c - a);
		pts[3*4+1] = 0;
		pts[3*4+2] = (float)B;
		pts[3*4+3] = 1;
		type[0] = 1;
	// Arc 2 (If exists) (point 6)
		if (R2 != 0.)
		{
			(BAlf!=0.) ? pts[k*4]=(float)(D/2.f-c+a*cos(BAlf)):pts[k*4]=(float)D/2.f;
			pts[k*4+1] = 0;		
			(BAlf!=0.) ? pts[k*4+2] = (float)(B-b) : pts[k*4+2] = (float)(B-a);
			pts[k*4+3] = 1;

			pts[(k-1)*4] = (float)(D/2.f-c);
			pts[(k-1)*4+1] = 0;
			pts[(k-1)*4+2] = (float)B;
			pts[(k-1)*4+3] = 1;
			type[k/3-1] = 4;
			k+=3;
		}
	// Faset 2 (If exists) (point 9)
		a = R4*tan((PI/2.-BAlf)/2.);
		b = a*sin(BAlf);
		if (L2 != 0. && BAlf != 0.)
		{
			pts[k*4] = (float)(D/2.f-a*cos(BAlf));
			pts[k*4+1] = 0;
			pts[k*4+2] = (float)(B-L2+b);//(B-g+b);
			pts[k*4+3] = 1;
			type[k/3-1] = 2;
			k+=3;
		}
	// Arc 4 (If exists) (point 12)
		if (R4 != 0.)
		{
			pts[k*4] = (float)(D/2.f);
			pts[k*4+1] = 0;
			pts[k*4+2] = (float)(B-L2-a);//(B-g-a);
			pts[k*4+3] = 1;

			pts[(k-1)*4] = (float)(D/2.f);
			pts[(k-1)*4+1] = 0;
			pts[(k-1)*4+2] = (float)(B-L2);
			pts[(k-1)*4+3] = 1;
			type[k/3-1] = 4;
			k+=3;
		}
		a = R3*tan((PI/2.-Alf)/2.);
		b = a*sin(Alf);
	// Top Line (if exists) (point 15)
		if (f != 0.)//&& R2 == 0. && R3 == 0.)//if(L1+L2 != B && R1 != B/2. && R2 != B/2.)
		{
			pts[k*4] = (float)(D/2.f);
			pts[k*4+1] = 0;
			pts[k*4+2] = (float)(B-g-f);//(B-g-f+a);
			pts[k*4+3] = 1;
			type[k/3-1] = 2;
			k+=3;
		}
	// Arc 3 (If exists) (point 18)
		if (R3 != 0.)
		{
			pts[k*4] = (float)(D/2.f-a*cos(Alf));
			pts[k*4+1] = 0;
			pts[k*4+2] = (float)(L1-b);//(B-g-f-b);
			pts[k*4+3] = 1;

			pts[(k-1)*4] = (float)(D/2.f);
			pts[(k-1)*4+1] = 0;
			pts[(k-1)*4+2] = (float)(L1);
			pts[(k-1)*4+3] = 1;
			type[k/3-1] = 4;
			k+=3;
		}
	// Faset 1 (if exists) (point 21)
		(Alf != 0.) ? a = R1*tan(Alf/2.) : a = R1;
		b = a*sin(Alf);
		c = L1*tan(PI/2.-Alf);
		if (L1 != 0. && Alf != 0.)
		{
			pts[k*4] = (float)(D/2.f-c+a*cos(Alf));
			pts[k*4+1] = 0;
			pts[k*4+2] = (float)b;
			pts[k*4+3] = 1;
			type[k/3-1] = 2;
			k+=3;
		}
	// Arc 1 (if exists) (point 24)
		if (R1 != 0.)
		{
			pts[k*4] = (float)(D/2.f-c-a);
			pts[k*4+1] = 0;		
			pts[k*4+2] = 0;
			pts[k*4+3] = 1;

			pts[(k-1)*4] = (float)(D/2.f-c);
			pts[(k-1)*4+1] = 0;
			pts[(k-1)*4+2] = 0;
			pts[(k-1)*4+3] = 1;
			type[k/3-1] = 4;
			k+=3;
		}
	// line (point 27)
	//Bottom line (If exists)
		pts[k*4] = (float)(D/2.-h1);
		pts[k*4+1] = 0;
		pts[k*4+2] = 0;
		pts[k*4+3] = 1;
		type[k/3-1] = 1;
			
		k = 4;
		for(int i=0;i<4;i++)
		{
			pts[1*4+i] = pts[0*4+i]*2.f*d+d*pts[3*4+i];
			pts[2*4+i] = pts[0*4+i]*d+2.f*d*pts[3*4+i];
			if (R2 != 0.)	// Arc 2
			{
				// дуга окружности - квадратичная кривая
				pts[k*4+i] = pts[(k+1)*4+i];
			/*	pts[k*4+i] = pts[(k-1)*4+i]*d+
					2.f*d*pts[(k+1)*4+i]*(float)(cos((PI/2.-BAlf)/2.));
				pts[(k+1)*4+i] = pts[(k+1)*4+i]*
					2.f*d*(float)(cos((PI/2.f-BAlf)/2.f))+d*pts[(k+2)*4+i];*/
				k+=3;
			}
			if (L2 != 0. && BAlf != 0.) // Faset 2
			{
				pts[k*4+i] = pts[(k-1)*4+i]*2.f*d+d*pts[(k+2)*4+i];
				pts[(k+1)*4+i] = pts[(k-1)*4+i]*d+2.f*d*pts[(k+2)*4+i];
				k=k+3;
			}
			if (R4 != 0.)	// Arc 4
			{
				// дуга окружности - квадратичная кривая
				pts[k*4+i] = pts[(k+1)*4+i];
			/*	pts[k*4+i] = pts[(k-1)*4+i]*d+
					2.f*d*pts[(k+1)*4+i]*(float)(cos((PI/2.-BAlf)/2.));
				pts[(k+1)*4+i] = pts[(k+1)*4+i]*
					2.f*d*(float)(cos((PI/2.f-BAlf)/2.f))+d*pts[(k+2)*4+i];*/
				k=k+3;
			}
			if (f != 0.)// && R2 == 0. && R3 == 0.)//if (L1+L2 != B && R1 != B/2. && R2 != B/2.) // Top line
			{
				pts[k*4+i] = pts[(k-1)*4+i]*2.f*d+d*pts[(k+2)*4+i];
				pts[(k+1)*4+i] = pts[(k-1)*4+i]*d+2.f*d*pts[(k+2)*4+i];
				k=k+3;
			}
			if (R3 != 0.)	// Arc 3
			{
				// дуга окружности - квадратичная кривая
				pts[k*4+i] = pts[(k+1)*4+i];
			/*	pts[k*4+i] = pts[(k-1)*4+i]*d+
					2.f*d*pts[(k+1)*4+i]*(float)(cos((PI/2.-Alf)/2.));
				pts[(k+1)*4+i] = pts[(k+1)*4+i]*
					2.f*d*(float)(cos((PI/2.f-Alf)/2.f))+d*pts[(k+2)*4+i];*/
				k=k+3;
			}
			if (L1 != 0 && Alf != 0) // Faset 1
			{
				pts[k*4+i] = pts[(k-1)*4+i]*2.f*d+d*pts[(k+2)*4+i];
				pts[(k+1)*4+i] = pts[(k-1)*4+i]*d+2.f*d*pts[(k+2)*4+i];
				k=k+3;
			}
			if (R1 != 0.)	// Arc 1
			{
				// дуга окружности - квадратичная кривая
				pts[k*4+i] = pts[(k+1)*4+i];
			/*	pts[k*4+i] = pts[(k-1)*4+i]*d+
					2.f*d*pts[(k+1)*4+i]*(float)(cos((PI/2.-Alf)/2.));
				pts[(k+1)*4+i] = pts[(k+1)*4+i]*
					2.f*d*(float)(cos((PI/2.f-Alf)/2.f))+d*pts[(k+2)*4+i];*/
				k=k+3;
			}
			pts[k*4+i] = pts[(k-1)*4+i]*2.f*d+d*pts[(k+2)*4+i];
			pts[(k+1)*4+i] = pts[(k-1)*4+i]*d+2.f*d*pts[(k+2)*4+i];
			
			k = 4;
		}
	}

	return true;
}
bool NToolShaped::CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f)
{
	if (sizeof(pts) == 0 || f == -1)
		return false;

//	int size = sizeof(type)/sizeof(type[0]);
	int *int_type = new int[size];
	bool res = CreateToolContour(pts, int_type, f);
	for (int i=0; i<size; i++)
	{
		if (int_type[i] == 1)
			type[i] = HORIZONTAL;
		if (int_type[i] == 2)
			type[i] = CONE;
		if (int_type[i] == 3)
			type[i] = SPHERE;
		if (int_type[i] == 4)
			type[i] = TORAHS;
	}
	delete []int_type;

	return true;
}

// определение точек задающих котур инструмента
bool NToolShaped::CreateToolContour(GLfloat *pts,int *type, BOOL f)
{
	return true;
}

bool NToolBRound::CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f)
{
	return true;
}

// определение точек задающих котур инструмента
bool NToolBRound::CreateToolContour(GLfloat *pts,int *type, BOOL f)
{
	return true;
}

