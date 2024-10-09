// NSurFMTorahs.cpp: implementation of the NSurFMTorahs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSurFMTorahs.h"
#include "NTrimPiece.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const double PI=3.14159265358979;
const float d = 1.f/3.f;

NSurFMTorahs::NSurFMTorahs()
{
	EssLine.RemoveAll();
	EssLineIsCubic = true;
}

NSurFMTorahs::~NSurFMTorahs()
{

}

// задание параметров образующей 
void NSurFMTorahs::SetForming(const NLine nl, 
							const double angle,
							const double beta,
							bool bl)
{
// nl - линия контура
// angle - не используется
// beta - не используется
// bl - не используется

	Line = nl;
	Alf = angle;
	BAlf = beta;
}

// построение общей поверхности для участка тора
int NSurFMTorahs::BuildSurface(PartArray& pcs, BuildingType BuildType)
{
	if (Line.p[0].GetX() == Line.p[3].GetX() &&
		Line.p[0].GetY() == Line.p[3].GetY() &&
		Line.p[0].GetZ() == Line.p[3].GetZ() &&
		Line.p[0].GetH() == Line.p[3].GetH())
		return -1;

	EssLine.RemoveAll();

	// построение передней части объекта
	if (BuildType == surBuildAll	|| 
		BuildType == surFront		||
		BuildType == surFrontALateral)
		
		if (!BuildObject(pcs, false, false))
			return 2;

	// построение бооковой поверхности
	if (BuildType == surBuildAll	|| 
		BuildType == surFrontALateral ||
		BuildType == surLateralABack)
	{
		if (BuildType == surLateralABack)
			if (!BuildObject(pcs, true, false))
				return 2;

		if (!BuildLateralSurface(pcs))
			return 3;
	}

	// построение задней части объекта
	if (BuildType == surBuildAll	|| 
		BuildType == surBack		||
		BuildType == surLateralABack)
	
		if (!BuildObject(pcs, false, true))
			return 4;

	return 0;
}

// построение объекта типа тор и линии видимого контура
BOOL NSurFMTorahs::BuildObject(PartArray& pc, bool bCrEssLine, bool bBack)
// bCrEssLine - флаг задающий что необходимо построение только ЛВК
// bBack - построение задней поверхности
{

	if (bCrEssLine && bBack)
		return FALSE;

	int i, j;
	double h1, h2, x, alf;
	BMatr m;
	NLine L0, L1, L3;
	BPoint pt, t, direct;
	NTrimPiece *pPiece;

	direct = End - Start;
	pt.Set(0.,0.,0.,1.);
	L3 = Line;
	h1 = sqrt(pt.Dist2(L3.p[0],L3.p[3])/pt.Dist2(L3.p[0],L3.p[1]))*0.5;

	BPoint pt1, n1;

	for (i=0; i<4; i++)
	{
		pPiece = new NTrimPiece;
		L0 = L3;

		// построение поверхности
		pPiece->pc[0][0] = L0.p[0].Norm();
		pPiece->pc[1][0] = L0.p[1].Norm()*h1;
		pPiece->pc[2][0] = L0.p[3].Norm();

		if (i < 3)
		{
			m.RotZ(pt, 90);
			L3 = L0 * m;
		}
		else
			L3 = Line;

		pPiece->pc[0][2] = L3.p[0].Norm();
		pPiece->pc[1][2] = L3.p[1].Norm()*h1;
		pPiece->pc[2][2] = L3.p[3].Norm();

		alf = PI/2.;
		x = 1;
		for (j=0; j<4; j++)
		{
			pt.Set(pt.GetX(), pt.GetY(), L0.p[j].GetZ(), 1);
			t = (L0.p[j]-pt) + (L3.p[j]-pt);
			if (t.D2() > 0.)
				t = t*(1/sqrt(t.D2()));
			x = Line.p[j].GetX()*(1/cos(alf/2.));
			L1.p[j] = pt + t*fabs(x);

			L1.p[j].Norm();
		}
		
		h2 = sqrt(pt.Dist2(L0.p[0],L3.p[0])/pt.Dist2(L0.p[0],L1.p[0]))*0.5;

		pPiece->pc[0][1] = L1.p[0]*h2;
		pPiece->pc[1][1] = L1.p[1]*h1*h2;
		pPiece->pc[2][1] = L1.p[3]*h2;

		if (bBack)
		{
			// перемещение построенной поверхности
			BMatr m;
			BPoint pt, pt1;
	
			pt1 = pt + direct;
			m.Trans(pt, pt1);

			pPiece->Modify(m);
		}
		else // простроение лини видимого контура 
			 // и сохранение её
		{
			NContour cur_cont;

			// движение точно в плоскости Z = const
			if (direct.GetZ() == 0 && i == 0)
			{
				// первая линия
				m.RotZ(pt, BAlf);
				L1 = Line * m;

				h1 = sqrt(L1.p[0].Dist2(L1.p[0],L1.p[3])/L1.p[0].Dist2(L1.p[0],L1.p[1]))*0.5;
				
				cur_cont.Clear();
				cur_cont.AddFirst(L1.p[0].Norm(),
								  L1.p[1].Norm()*h1,
								  L1.p[2].Norm()*h1,
								  L1.p[3].Norm());
				EssLine.Add(cur_cont);

				// вторая линия
				m.RotZ(pt, Alf);
				L1 = L1 * m;

				cur_cont.Clear();
				cur_cont.AddFirst(L1.p[0].Norm(),
								  L1.p[1].Norm()*h1,
								  L1.p[2].Norm()*h1,
								  L1.p[3].Norm());
				EssLine.Add(cur_cont);

				EssLineIsCubic = false;
			}
		// движение точно по оси Z
			else if (direct.GetX() == 0. && direct.GetY() == 0.)
			{
				cur_cont.Clear();
				cur_cont.AddFirst(pPiece->pc[2][0],
								   pPiece->pc[2][1],
								   pPiece->pc[2][1],
								   pPiece->pc[2][2]);
				EssLine.Add(cur_cont);

				EssLineIsCubic = false;
			}
		// произвольная траектория
			else if (direct.GetZ() != 0 && 
				(direct.GetX() != 0. || direct.GetY() != 0.))
			{
				UVPointArray uvp;
				NLine ln;

				if (pPiece->BuildEssayLine(direct, uvp, cur_cont) == TRUE)
				{
					for (j=0; j<uvp.GetSize(); j++)
						pPiece->AddUVPoint(uvp[j]);

					EssLine.Add(cur_cont);

					EssLineIsCubic = true;
				}
			}
		}

		if (!bCrEssLine)
			pc.Add(pPiece);	
		else
			delete pPiece;
	}

	return TRUE;
}

// построение боковой поверхности
BOOL NSurFMTorahs::BuildLateralSurface(PartArray &pcs)
{
	if (EssLine.GetSize() <= 0)
		return FALSE;

	NLine line1, line2;
	BMatr m;
	BPoint pt, pt1, CenTr, direct;
	NCubicPiece *pCubPiece;
	NSquarePiece *pSqPiece;
	DirectionType type = GetTypeDirection();
	int i, j;
	
	direct = End - Start;
	pt.Set(0.,0.,0.,1.);
	pt1 = pt + direct;

	CenTr =  pt - DirLine.p[0];	// центр вращения траектории
	CenTr = Center + CenTr;

	// изменение центра вращения не происходит
	// вопрос в сохранении линии видимого контура
	// решение может в построении порции по методу вращения 
	// начиная с начального оканчиваясь на последнем

	for( i=0; i<EssLine.GetSize(); i++)
		for (j=0; j<EssLine[i].num; j++)
		{		
			// первая линия
			EssLine[i].GetLine(j,line1);
			// вторая линия
			m.Trans(pt, pt1);
		//	line1.Ball2Bernstain();
			line2 = line1 * m;
			
			if (EssLineIsCubic)
			{
				pCubPiece = new NCubicPiece;

				// построение линейчатой поверхности
				if (type == surLINE)
					BuildCubLinearSurface(line1, line2, true, pCubPiece);
				
				// построение поверхности вращения
				if (type == surCWARC || type == surCCWARC)
				//	BuildArcMoveArcSurface(line1, pPiece, pCubPiece, arcAngle*180./PI);

				pcs.Add(pCubPiece);	
			}
			else
			{
				pSqPiece = new NSquarePiece;

				// построение линейчатой поверхности
				if (type == surLINE)
					BuildSqLinearSurface(line1, line2, true, pSqPiece);

				// построение поверхности вращения
				if (type == surCWARC || type == surCCWARC)
					BuildArcMoveArcSurface(line1, pSqPiece, CenTr, arcAngle*180./PI);
								
				pcs.Add(pSqPiece);	
			}
		}

	return TRUE;
}
