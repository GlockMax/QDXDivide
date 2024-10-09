// NToolDiskStd.cpp: implementation of the NToolDiskStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseRender.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolDiskStd.h"
#include "NToolsCrDiskSheet.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolDiskStd, NToolMill, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const float d = 1.f/3.f;

NToolDiskStd::NToolDiskStd():NToolMill()
{
	D=B=Alf=BAlf=0; R1=R2=R3=R4=0; L1=L2=0; h1=h2=0;
}

NToolDiskStd::~NToolDiskStd()
{
}

NToolDiskStd::NToolDiskStd(const CString &Name, double diam, double b, double alf, double beta,
		double r1, double r2, double r3, double r4,
		double l1, double l2, double hm1, double hm2) :NToolMill(Name)
{
	D=diam; B=b; Alf=alf*PI/180.; BAlf=beta*PI/180.;
	R1=r1; R2=r2; R3=r3; R4=r4;
	L1=l1; L2=l2;
	h1=hm1, h2=hm2;
}

void NToolDiskStd::Serialize(CArchive &ar)
{
	NToolMill::Serialize(ar);
	SerializeElements(ar,&D,1);
	SerializeElements(ar,&B,1);
	SerializeElements(ar,&Alf,1);
	SerializeElements(ar,&BAlf,1);
	SerializeElements(ar,&L1,1);
	SerializeElements(ar,&L2,1);
	SerializeElements(ar,&R1,1);
	SerializeElements(ar,&R2,1);
	SerializeElements(ar,&R3,1);
	SerializeElements(ar,&R4,1);
	SerializeElements(ar,&h1,1);
	SerializeElements(ar,&h2,1);
	SerializeElements(ar,&IsNameAuto,1);
	
	if(ar.IsLoading())
	{
		GenInit();
		double bz = Zdisp;
		Zdisp = 0;
		SetZdisp(bz);
	}
}

int NToolDiskStd::GenInit()
{
// Calculate dimensions in object coord
	Gabar.Expand(-D/2.,-D/2.,0);
	Gabar.Expand(D/2.,D/2.,B);
// проверка геометрических размеров инструмента	
	double x, y; 

	if (PI/2.-Alf < MINAR)
		return 1;	// недопустимый угол
	if (PI/2.-BAlf < MINAR)
		return 1;	// недопустимый угол
	
// проверка параметров фасок если они существуют
	x = y = 0.;
	// определение введённых пользователем L1 и L2,
	// а не преобразованных
	if (L1 != 0.)
		x = L1 + R3*tan((PI/2.-Alf)/2.);
	if (L2 != 0.)
		y = L2 + R4*tan((PI/2.-BAlf)/2.);
	
	if (B-x-y < 0.)
		return 2;
	if (L1 !=0 && Alf != 0.)
		x = L1*tan(PI/2. - Alf);
			
	if (L2 !=0 && BAlf != 0.)
		y = L2*tan(PI/2. - BAlf);
			
	if (L1 != 0. && (B < L1 || D/2. < x))
		return 3;	// неверна фаска L1 или угол Alf
	if (L2 != 0. && (B < L2 || D/2. < y))
		return 4;	// неверна фаска L2 или угол BAlf
// проверка параметров R1 и R2	
	x = R1*tan(Alf/2.);
	y = R2*tan(BAlf/2.);
	
	if (L1*tan(PI/2.-Alf) < x*cos(Alf) ||
		D/2.-L1*tan(PI/2.-Alf) < x || (L1 == 0. && B < R1))
		return 5;	// неверный радиус R1
	if (L2*tan(PI/2.-BAlf) < y*cos(BAlf) ||
		D/2.-L2*tan(PI/2.-BAlf) < y || (L2 == 0. && B < R2))
		return 6;	// неверный радиус R2
	if (L1 == 0. && L2 == 0. && R1+R2 > B)
		return 7;	// неверный радиус R1 или R2
// проверка параметров R3 и R4	
	x = R3*tan((PI/2.-Alf)/2.);
	y = R4*tan((PI/2.-BAlf)/2.);
	
	if (x*sin(Alf) > L1)
		return 8;	// неверный радиус R3
	if (y*sin(BAlf) > L2)
		return 9;	// неверный радиус R4
	if (x*sin(Alf) > L1 - R1*tan(Alf/2.)*sin(Alf))
		return 8;	// неверный радиус R3
	if (y*sin(BAlf) > L2 - R2*tan(BAlf/2.)*sin(BAlf))
		return 9;	// неверный радиус R4
// общая проверка
	(L1 != 0) ?	x = L1 + x : x = R1;
	(L2 != 0) ?	y = L2 + y : y = R2;
	if ( x + y > B + 1.e-6)
		return 12; // неопределённая ошибка
// проверка активной части инструмента
	if (L1 != 0. && Alf != 0.)
		x = L1*tan(PI/2.-Alf) + R1*tan(Alf/2.);
	else
		x = R1;
		
	if (L2 != 0. && BAlf != 0.)
		y = L2*tan(PI/2.-BAlf) + R2*tan(BAlf/2.);
	else
		y = R2;
		
	if (x > h1 || h1 > D/2.)
		return 10; // неправильно задана левая активная часть инструмента
	if (y > h2 || h2 > D/2.)
		return 11; // неправильно задана правая активная часть инструмента

	return 0;
}

void NToolDiskStd::GenRender()
{
//Create NURBS control points
	int LinesNumber = 3; // Lines Number in a contour
	int PointsNumber = LinesNumber*3+1; // Points Number in a contour
	GLfloat *ctlarray = new GLfloat [4*PointsNumber*4];
// определение крайних точек участков контура
// начальная точка
	ctlarray[0*4] = 0;	
	ctlarray[0*4+1] = 0;
	ctlarray[0*4+2] = (float)B;
	ctlarray[0*4+3] = 1;	
// правая сторона
	ctlarray[3*4] = (float)(D/2.-h2);
	ctlarray[3*4+1] = 0;
	ctlarray[3*4+2] = (float)B;
	ctlarray[3*4+3] = 1;
// верхняя часть
	ctlarray[6*4] = (float)(D/2.-h1);
	ctlarray[6*4+1] = 0;
	ctlarray[6*4+2] = (float)0;
	ctlarray[6*4+3] = 1;
// левая сторона
	ctlarray[9*4] = (float)0;
	ctlarray[9*4+1] = 0;
	ctlarray[9*4+2] = (float)0;
	ctlarray[9*4+3] = 1;
// -------------------------	
	GLfloat *uknot = new GLfloat [PointsNumber+4];
	int k = 0;
	for (int i=0; i<=LinesNumber; i++)
	{
		int j = k;
		for (; j < k + 3; j++)
			uknot[j] = (float)i;
		k+=3;
		if (i == 0 || i==LinesNumber)
		{	uknot[j] = (float)i;	k++;	}
		
	}
	GLfloat	vknot[8]={0.f,0.f,0.f,0.f,1.f,1.f,1.f,1.f};
	for(int i=0;i<4;i++)
	{
		ctlarray[1*4+i] = ctlarray[0*4+i]*2.f*d+d*ctlarray[3*4+i];
		ctlarray[2*4+i] = ctlarray[0*4+i]*d+2.f*d*ctlarray[3*4+i];
		ctlarray[4*4+i] = ctlarray[3*4+i]*2.f*d+d*ctlarray[6*4+i];
		ctlarray[5*4+i] = ctlarray[3*4+i]*d+2.f*d*ctlarray[6*4+i];
		ctlarray[7*4+i] = ctlarray[6*4+i]*d+2.f*d*ctlarray[9*4+i];
		ctlarray[8*4+i] = ctlarray[6*4+i]*d+2.f*d*ctlarray[9*4+i];
	}
	
	if (h1 != D / 2. || h2 != D / 2.)
		RenderP.Create(PointsNumber, ctlarray);
	else
		RenderP.MarkEmpty();

///////////////// активная часть инструмента ///////////////////////////
//Create NURBS control points
	double e, f, g;
	(L1 != 0. && Alf !=0.) ? e = L1 + R3*tan((PI/2.-Alf)/2.) 
		: e = R1;
	(L2 != 0. && BAlf != 0.) ? g = L2 + R4*tan((PI/2.-BAlf)/2.)
		: g = R2;
	f = B - e - g;
	LinesNumber = 9; // Lines Number in a contour
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
/*	if (L1+L2 == B	  // пока не решено // нет верхнего участка
		|| (R1 == B/2. && R2 == B/2.))
		LinesNumber--;*/
	
	PointsNumber = LinesNumber*3+1; // Points Number in a contour
	delete []ctlarray;
	ctlarray = new GLfloat [4*PointsNumber*4];
// определение крайних точек участков контура
	double a;
	(BAlf != 0.) ? a = R2*tan(BAlf/2.) : a = R2;
	double b = a*sin(BAlf);
	double c = L2*tan(PI/2.-BAlf);

	k = 6;
// line (point 0)
	ctlarray[0*4] = (float)(D/2.-h2);
	ctlarray[0*4+1] = 0;
	ctlarray[0*4+2] = (float)B;
	ctlarray[0*4+3] = 1;	
// line (point 3)
	ctlarray[3*4] = (float)(D/2.f - c - a);
	ctlarray[3*4+1] = 0;
	ctlarray[3*4+2] = (float)B;
	ctlarray[3*4+3] = 1;
// Arc 2 (If exists) (point 6)
	if (R2 != 0.)
	{
		(BAlf!=0.) ? ctlarray[k*4]=(float)(D/2.f-c+a*cos(BAlf)):ctlarray[k*4]=(float)D/2.f;
		ctlarray[k*4+1] = 0;		
		(BAlf!=0.) ? ctlarray[k*4+2] = (float)(B-b) : ctlarray[k*4+2] = (float)(B-a);
		ctlarray[k*4+3] = 1;

		ctlarray[(k-1)*4] = (float)(D/2.f-c);
		ctlarray[(k-1)*4+1] = 0;
		ctlarray[(k-1)*4+2] = (float)B;
		ctlarray[(k-1)*4+3] = 1;
		k+=3;
	}
// Faset 2 (If exists) (point 9)
	a = R4*tan((PI/2.-BAlf)/2.);
	b = a*sin(BAlf);
	if (L2 != 0. && BAlf != 0.)
	{
		ctlarray[k*4] = (float)(D/2.f-a*cos(BAlf));
		ctlarray[k*4+1] = 0;
		ctlarray[k*4+2] = (float)(B-L2+b);//(B-g+b);
		ctlarray[k*4+3] = 1;
		k+=3;
	}
// Arc 4 (If exists) (point 12)
	if (R4 != 0.)
	{
		ctlarray[k*4] = (float)(D/2.f);
		ctlarray[k*4+1] = 0;
		ctlarray[k*4+2] = (float)(B-L2-a);//(B-g-a);
		ctlarray[k*4+3] = 1;

		ctlarray[(k-1)*4] = (float)(D/2.f);
		ctlarray[(k-1)*4+1] = 0;
		ctlarray[(k-1)*4+2] = (float)(B-L2);
		ctlarray[(k-1)*4+3] = 1;
		k+=3;
	}
	a = R3*tan((PI/2.-Alf)/2.);
	b = a*sin(Alf);
// Top Line (if exists) (point 15)
	if (f != 0.)//&& R2 == 0. && R3 == 0.)//if(L1+L2 != B && R1 != B/2. && R2 != B/2.)
	{
		ctlarray[k*4] = (float)(D/2.f);
		ctlarray[k*4+1] = 0;
		ctlarray[k*4+2] = (float)(B-g-f);//(B-g-f+a);
		ctlarray[k*4+3] = 1;
		k+=3;
	}
// Arc 3 (If exists) (point 18)
	if (R3 != 0.)
	{
		ctlarray[k*4] = (float)(D/2.f-a*cos(Alf));
		ctlarray[k*4+1] = 0;
		ctlarray[k*4+2] = (float)(L1-b);//(B-g-f-b);
		ctlarray[k*4+3] = 1;

		ctlarray[(k-1)*4] = (float)(D/2.f);
		ctlarray[(k-1)*4+1] = 0;
		ctlarray[(k-1)*4+2] = (float)(L1);
		ctlarray[(k-1)*4+3] = 1;
		k+=3;
	}
// Faset 1 (if exists) (point 21)
	(Alf != 0.) ? a = R1*tan(Alf/2.) : a = R1;
	b = a*sin(Alf);
	c = L1*tan(PI/2.-Alf);
	if (L1 != 0. && Alf != 0.)
	{
		ctlarray[k*4] = (float)(D/2.f-c+a*cos(Alf));
		ctlarray[k*4+1] = 0;
		ctlarray[k*4+2] = (float)b;
		ctlarray[k*4+3] = 1;
		k+=3;
	}
// Arc 1 (if exists) (point 24)
	if (R1 != 0.)
	{
		ctlarray[k*4] = (float)(D/2.f-c-a);
		ctlarray[k*4+1] = 0;		
		ctlarray[k*4+2] = 0;
		ctlarray[k*4+3] = 1;

		ctlarray[(k-1)*4] = (float)(D/2.f-c);
		ctlarray[(k-1)*4+1] = 0;
		ctlarray[(k-1)*4+2] = 0;
		ctlarray[(k-1)*4+3] = 1;
		k+=3;
	}
// line (point 27)
//Bottom line (If exists)
	ctlarray[k*4] = (float)(D/2.-h1);
	ctlarray[k*4+1] = 0;
	ctlarray[k*4+2] = 0;
	ctlarray[k*4+3] = 1;
// -------------------------	
	delete []uknot;
	uknot = new GLfloat [PointsNumber+4];
	k = 0;
	for (int i=0; i<=LinesNumber; i++)
	{
		int j;
		for (j=k; j<k+3; j++)
			uknot[j] = (float)i;
		k+=3;
		if (i == 0 || i==LinesNumber)
		{	uknot[j] = (float)i;	k++;	}
		
	}
/*	a = 0;
	for (int i=0; i<PointsNumber+4;i++)
	{
		if (i==4 || i==7 || i==10 || i==13 || i==16 || i==19 || i==22 || i==25 || i==28)
			a++;
		uknot[i] = (float)a;
	}*/
	k = 4;
	for(int i=0;i<4;i++)
	{
		ctlarray[1*4+i] = ctlarray[0*4+i]*2.f*d+d*ctlarray[3*4+i];
		ctlarray[2*4+i] = ctlarray[0*4+i]*d+2.f*d*ctlarray[3*4+i];
		if (R2 != 0.)	// Arc 2
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*d+
				2.f*d*ctlarray[(k+1)*4+i]*(float)(cos((PI/2.-BAlf)/2.));
			ctlarray[(k+1)*4+i] = ctlarray[(k+1)*4+i]*
				2.f*d*(float)(cos((PI/2.f-BAlf)/2.f))+d*ctlarray[(k+2)*4+i];
			k+=3;
		}
		if (L2 != 0. && BAlf != 0.) // Faset 2
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*2.f*d+d*ctlarray[(k+2)*4+i];
			ctlarray[(k+1)*4+i] = ctlarray[(k-1)*4+i]*d+2.f*d*ctlarray[(k+2)*4+i];
			k=k+3;
		}
		if (R4 != 0.)	// Arc 4
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*d+
				2.f*d*ctlarray[(k+1)*4+i]*(float)(cos((PI/2.-BAlf)/2.));
			ctlarray[(k+1)*4+i] = ctlarray[(k+1)*4+i]*
				2.f*d*(float)(cos((PI/2.f-BAlf)/2.f))+d*ctlarray[(k+2)*4+i];
			k=k+3;
		}
		if (f != 0.)// && R2 == 0. && R3 == 0.)//if (L1+L2 != B && R1 != B/2. && R2 != B/2.) // Top line
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*2.f*d+d*ctlarray[(k+2)*4+i];
			ctlarray[(k+1)*4+i] = ctlarray[(k-1)*4+i]*d+2.f*d*ctlarray[(k+2)*4+i];
			k=k+3;
		}
		if (R3 != 0.)	// Arc 3
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*d+
				2.f*d*ctlarray[(k+1)*4+i]*(float)(cos((PI/2.-Alf)/2.));
			ctlarray[(k+1)*4+i] = ctlarray[(k+1)*4+i]*
				2.f*d*(float)(cos((PI/2.f-Alf)/2.f))+d*ctlarray[(k+2)*4+i];
			k=k+3;
		}
		if (L1 != 0 && Alf != 0) // Faset 1
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*2.f*d+d*ctlarray[(k+2)*4+i];
			ctlarray[(k+1)*4+i] = ctlarray[(k-1)*4+i]*d+2.f*d*ctlarray[(k+2)*4+i];
			k=k+3;
		}
		if (R1 != 0.)	// Arc 1
		{
			ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*d+
				2.f*d*ctlarray[(k+1)*4+i]*(float)(cos((PI/2.-Alf)/2.));
			ctlarray[(k+1)*4+i] = ctlarray[(k+1)*4+i]*
				2.f*d*(float)(cos((PI/2.f-Alf)/2.f))+d*ctlarray[(k+2)*4+i];
			k=k+3;
		}
		ctlarray[k*4+i] = ctlarray[(k-1)*4+i]*2.f*d+d*ctlarray[(k+2)*4+i];
		ctlarray[(k+1)*4+i] = ctlarray[(k-1)*4+i]*d+2.f*d*ctlarray[(k+2)*4+i];
		
		k = 4;
	}
	RenderA.Create(PointsNumber, ctlarray);
	CreateSkeleton(PointsNumber, ctlarray);
	delete[] ctlarray;
	delete[] uknot;
}

void NToolDiskStd::FillSheet(CPropertySheet *pDial)
{
	NToolMill::FillSheet(pDial);
	NToolsCrDiskSheet *pD = (NToolsCrDiskSheet *)pDial;
	int TypePage = NToolMill::GetType();
// определение типа инструмента	
	if (TypePage == 20) pD->tab_pos = 5;
	if (TypePage == 21) pD->tab_pos = 0;
	if (TypePage == 22) pD->tab_pos = 1;
	if (TypePage == 23) pD->tab_pos = 2;
	if (TypePage == 24) pD->tab_pos = 3;
	if (TypePage == 10) pD->tab_pos = 4;
	if (TypePage == 71) pD->tab_pos = 6;
	if (TypePage == 72) pD->tab_pos = 7;
	switch(TypePage)
	{
	case 21: // faset
		pD->m_fasetpage.m_diam = RoundP( D );
		pD->m_fasetpage.m_b = RoundP( B );
		pD->m_fasetpage.m_alf1 = RoundP( Alf*180./PI );
		pD->m_fasetpage.m_alf2 = RoundP( BAlf*180./PI );
		pD->m_fasetpage.m_faset1 = RoundP( L1 );
		pD->m_fasetpage.m_faset2 = RoundP( L2 );
		pD->m_fasetpage.m_name = Name;
		pD->m_fasetpage.m_h1 = RoundP( h1 );
		pD->m_fasetpage.m_h2 = RoundP( h2 );
		pD->m_fasetpage.IsNameAuto = IsNameAuto;
		break;
	case 22: // round
		pD->m_roundpage.m_diam = RoundP( D );
		pD->m_roundpage.m_b = RoundP( B );
		if (R1 == R2)
		{
			pD->m_roundpage.m_r1 = RoundP( R1 );
			pD->m_roundpage.m_r2 = 0.;
			pD->m_roundpage.m_r3 = 0.;
		}
		else
		{
			pD->m_roundpage.m_r1 = 0.;
			pD->m_roundpage.m_r2 = RoundP( R1 );
			pD->m_roundpage.m_r3 = RoundP( R2 );
		}
		pD->m_roundpage.m_h1 = RoundP( h1 );
		pD->m_roundpage.m_h2 = RoundP( h2 );
		pD->m_roundpage.m_name = Name;	
		pD->m_roundpage.IsNameAuto = IsNameAuto;
		break;
	case 23: // angle
		pD->m_anglepage.m_diam = RoundP( D );
		pD->m_anglepage.m_b = RoundP( B );
		pD->m_anglepage.m_alf = RoundP( BAlf*180./PI );
		pD->m_anglepage.m_r = RoundP( R1 );
		pD->m_anglepage.m_h1 = RoundP( h1 );
		pD->m_anglepage.m_h2 = RoundP( h2 );
		pD->m_anglepage.m_name = Name;	
		pD->m_anglepage.IsNameAuto = IsNameAuto;
		break;
	case 24: // double angle
		pD->m_anglespage.m_diam = RoundP( D );
		pD->m_anglespage.m_b = RoundP( B );
		pD->m_anglespage.m_alf1 = RoundP( Alf*180./PI );
		pD->m_anglespage.m_alf2 = RoundP( BAlf*180./PI );
		pD->m_anglespage.m_faset = RoundP( L1 + R3*tan((PI/2.-Alf)/2.) );
		pD->m_anglespage.m_r = RoundP( R3 );
		pD->m_anglespage.m_h1 = RoundP( h1 );
		pD->m_anglespage.m_h2 = RoundP( h2 );
		pD->m_anglespage.m_name = Name;	
		pD->m_anglespage.IsNameAuto = IsNameAuto;
		break;
	case 20: // произвольный дисковый
		pD->m_shapedpage.m_diam = RoundP( D );
		pD->m_shapedpage.m_b = RoundP( B );
		pD->m_shapedpage.m_alf1 = RoundP( Alf*180./PI );
		pD->m_shapedpage.m_alf2 = RoundP( BAlf*180./PI );
		if (Alf != 0.)
			pD->m_shapedpage.m_faset1 = RoundP( L1 + R3*tan((PI/2.-Alf)/2.) );
		else
			pD->m_shapedpage.m_faset1 = RoundP( L1 );
		if (BAlf != 0.)
			pD->m_shapedpage.m_faset2 = RoundP( L2 + R4*tan((PI/2.-BAlf)/2.) );
		else
			pD->m_shapedpage.m_faset2 = RoundP( L2 );
		pD->m_shapedpage.m_r1 = RoundP( R1 );
		pD->m_shapedpage.m_r2 = RoundP( R2 );
		pD->m_shapedpage.m_r3 = RoundP( R3 );
		pD->m_shapedpage.m_r4 = RoundP( R4 );
		pD->m_shapedpage.m_h1 = RoundP( h1 );
		pD->m_shapedpage.m_h2 = RoundP( h2 );
		pD->m_shapedpage.m_name = Name;	
		pD->m_shapedpage.IsNameAuto = IsNameAuto;
		break;
	case 71: // 
		pD->m_broundpage.m_diam = RoundP( D );
		pD->m_broundpage.m_b = RoundP( B );
		pD->m_broundpage.m_name = Name;	
		pD->m_broundpage.IsNameAuto = IsNameAuto;
		break;
	case 72: // 
		pD->m_borepage.m_name = Name;	
		pD->m_borepage.pToolVert = nullptr;	
		break;
	}
}

// определение геометрических параметров инструмента
void NToolDiskStd::GetParam(double &diam, double &b, double &alf, double &beta,
		double &r1, double &r2, double &r3, double &r4,
		double &l1, double &l2, double &hm1, double &hm2)
{
	diam = D;
	b = B;
	alf = Alf;
	beta = BAlf;
	r1 = R1; r2 = R2; r3 = R3; r4 = R4;
	l1 = L1; l2 = L2; hm1 = h1; hm2 = h2;
}
bool NToolDiskStd::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = D;
	CornerRadius = 0.;
	ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = B;
	return true;
}
