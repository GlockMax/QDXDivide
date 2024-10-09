// NSurFMComTool.cpp: implementation of the NSurFMComTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BPoint.h"
#include "NLine.h"
#include "NSurFMComTool.h"
#include "NSurFMSimTool.h"
#include "NToolCombined.h"
#include "NCadrGeom.h"
#include "NPartSurface.h"
#include "math.h"
#include "surEnums.h"


#include "NToolStd.h"

#include "curve.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const double PI=3.14159265358979;

NSurFMComTool::NSurFMComTool()
{
	pTool = NULL;
//	Start.Set(0.,0.,0.,1.);
//	End.Set(0.,0.,0.,1.);
	Status = EMPTY;
	DirectContour.Clear();
}

NSurFMComTool::~NSurFMComTool()
{
}

// определение статуса приёма
enum JobState NSurFMComTool::GetStatus()
{
	return Status;
}

// построение результирующей поверхности
// для текущего инструмента и траектории
void NSurFMComTool::BuildSurface(PartArray& OutArray)
{
	if (!pTool || Status == EMPTY) 
		return;

	int i, j, k, BegI = OutArray.GetSize(), num_buil, t_n;
	CString str;

	NSurFMSimTool simTool; 
	NLine Line;

	CArray<int, int> arNumBuild;
	arNumBuild.Add(BegI);

	for (i=0; i<Cadrs.GetSize(); i++)
	{
		switch(Cadrs[i]->GetType())
		{
		case line: case fast:
			if (SetDirection(i))
				for (j=0; j<pTool->num_tools; j++)
				{
					num_buil = 0;
					simTool.SetTool(pTool->pTools[j]);
					for (k = 0; k<DirectContour.num; k++)
					{
						if (DirectContour.GetLine(k, Line))
						{
							t_n = OutArray.GetSize();
							// в текущем кадре один участок
							if (k == 0 && DirectContour.num == 1) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surLINE, surBuildAll);
							
							// в текущем кадре больше чем один участок и данный участок первый
							if (k == 0 && DirectContour.num > 1) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surLINE, surFrontALateral);
							
							// в текущем кадре больше чем один участок (промежуточный участок)
							if (k > 0 && k < DirectContour.num-1 ) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surLINE, surLateral);
							
							// в текущем кадре больше чем один участок и данный участок последний
							if (k > 0 && k == DirectContour.num-1 ) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surLINE, surLateralABack);
							
							num_buil = OutArray.GetSize() - t_n;
						}
						arNumBuild.Add(num_buil);
					}
				}
				
			break;
		case cwarc: 
			if (SetDirection(i))
				num_buil = 0;
				for (j=0; j<pTool->num_tools; j++)
				{
					simTool.SetTool(pTool->pTools[j]);
					for (k = 0; k<DirectContour.num; k++)
					{
						if (DirectContour.GetLine(k, Line))
						{
							t_n = OutArray.GetSize();
							// в текущем кадре один участок
							if (k == 0 && DirectContour.num == 1) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCWARC, surBuildAll);

							// в текущем кадре больше чем один участок и данный участок первый
							if (k == 0 && DirectContour.num > 1) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCWARC, surFrontALateral);
							
							// в текущем кадре больше чем один участок (промежуточный участок)
							if (k > 0 && k < DirectContour.num-1 ) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCWARC, surLateral);
							
							// в текущем кадре больше чем один участок и данный участок последний
							if (k > 0 && k == DirectContour.num-1 ) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCWARC, surLateralABack);
							
							num_buil = OutArray.GetSize() - t_n;
						}
						arNumBuild.Add(num_buil);
					}
				}
			break;
		case ccwarc:
			if (SetDirection(i))
				num_buil = 0;
				for (j=0; j<pTool->num_tools; j++)
				{
					simTool.SetTool(pTool->pTools[j]);
					for (k = 0; k<DirectContour.num; k++)
					{
						if (DirectContour.GetLine(k, Line))
						{
							t_n = OutArray.GetSize();
							// в текущем кадре один участок
							if (k == 0 && DirectContour.num == 1) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCCWARC, surBuildAll);

							// в текущем кадре больше чем один участок и данный участок первый
							if (k == 0 && DirectContour.num > 1) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCCWARC, surFrontALateral);
							
							// в текущем кадре больше чем один участок (промежуточный участок)
							if (k > 0 && k < DirectContour.num-1 ) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCCWARC, surLateral);
							
							// в текущем кадре больше чем один участок и данный участок последний
							if (k > 0 && k == DirectContour.num-1 ) 
								simTool.BuildSurface(OutArray, Line, CenterPoint, surCCWARC, surLateralABack);
							
							num_buil = OutArray.GetSize() - t_n;
						}
						arNumBuild.Add(num_buil);
					}
				}
			break;
		}
	

	// восстановление ориентации поверхности
		BMatr m_tr;
		BPoint p0(0.,0.,0.,1.);

		DirectContour.GetLine(i, Line);
		m_tr.Trans(p0, Line.p[0]);
		for(j = BegI; j < OutArray.GetSize(); j++)
			OutArray[j]->Modify(m_tr);
		BegI = OutArray.GetSize();
	/*	j = 0;		// индекс линии
		k = 0;		// индекc последней порции поверхности относящейся к одному участку
		t_n = 0;	// индекс текущей поверхности
		for (j = BegI; j<OutArray.GetSize(); j++)
		{
			if (t_n == k)
			{
				DirectContour.GetLine(i, Line);
				m_tr.Trans(p0, Line.p[0]);
				j ++;
				k += arNumBuild.GetAt(j);
			}
			OutArray[i]->Modify(m_tr);
			t_n++;
		}*/
	}
	Status = BUILD_COMPLETE;
}

// начать передачу кадров
// возвращает: Status
enum JobState NSurFMComTool::BeginIssue( NToolCombined *pT)
{
	Cadrs.RemoveAll();
	pTool = pT;

	Status = POSSIBLEMORE;
	
	return Status;
}

// передать один кадр
// возвращает: Status
enum JobState NSurFMComTool::SendOneCadr(NCadrGeom *pCadr)
{
	if(Status != POSSIBLEMORE && Status != EMPTY)
		return Status;

	Cadrs.Add(pCadr);
	Status = POSSIBLEMORE;

	return Status;
}

// задание направления движения или
// линии задающей траекторию движения 
// (состоит из одного или нескольких участков)
bool NSurFMComTool::SetDirection(int index)
{
// задание отрезка:
// точки 0, 1 - начальная точка
// точки 2, 3 - конечная точка

// задание дуги окружности:
// дуга окружности - квадратичная кривая
// точки 1, 2 - совпадают

	if (Cadrs.GetSize() == 0)
		return false;
	DirectContour.Clear();

	double xe,xb,ye,yb,ze,zb;
	double i,j,k,xn,yn,zn;
	BPoint pb, pe, pn, pi;
	NLine ln;
	double eps = 1.e-5;
	int type;

	Cadrs[index]->GetB(&xb, &yb, &zb);
	Cadrs[index]->GetE(&xe, &ye, &ze);

	type = Cadrs[index]->GetType();

	switch (type)
	{
	case line: case fast:	
		DirectContour.AddFirst(BPoint(xb, yb, zb, 1),
			BPoint(xb, yb, zb, 1),
			BPoint(xb, yb, zb, 1),
			BPoint(xe, ye, ze, 1));
		DirectContour.SetTypeLine(1, LINE_CONT);
		CenterPoint.Set(0.,0.,0.,1.);
		
		break;
	case cwarc: case ccwarc:
		Cadrs[index]->GetI(&i,&j,&k);
		Cadrs[index]->GetN(&xn,&yn,&zn);

		// Pb - begin; Pe - end; Pi - Pcenter-Pb
		pb.Set(xb, yb, zb, 1);
		pe.Set(xe, ye, ze, 1);
		pn.Set(xn, yn, zn, 0);
		pi.Set(i, j, k, 0);

		BMatr Transform;
		BPoint a, b, c, q, tmp;
		double CosAlf, alf, d;

		Transform.e0cong(pn,BPoint(0.,0.,1.,0.));

		pb = pb * Transform;
		pe = pe * Transform;
		pi = pi * Transform;

		c = pb+pi;
		CenterPoint = c;

		Transform.Trans(c, BPoint(0.,0.,0.,1.));

		pb = pb * Transform;
		pe = pe * Transform;
		pi = pi * Transform;
		c = c * Transform;

		// определение числа сегментов окружности
		

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
		
		// проверка на совпадение начальной и конечной точек
		double dp1p2 = (pe-pb).D2();
		if(dp1p2 < eps*eps )// pb=pe
		{
			pb = ( pb + pe )*0.5;
			pe = pb;
		}
		q = pb + pi - pe;// Pq - Pcenter-Pe
		double r2 = pi.D2();
		if(r2 == 0.)
			return 0;
		double sp = pi * q;
		BPoint nv = q % pi;// normal vector

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
		if(sp > cos(120.*PI/180.)*r2)// Angle < 2 || >358
		{
			if( (!sign && type == ccwarc) ||
				( sign && type ==  cwarc))// Angle < 120
				k=1;	
			else// Angle > 240
				k=3;
		}
		else if (sp == 0.) // Angle 90
		{
			k = 1;
		}
		else	// Angle >120 && <240
		{
			k=2;
		}

		if(dp1p2 < eps*eps && sp > 0.)// Angle = 360
			k = 3;

		a = pi*(-1);
		b = (pe-pb)-pi;
		CosAlf = a.Cos2(a, b);
		CosAlf = CosAlf*sqrt(a.D2())*sqrt(b.D2());	// cos alf
		alf = acos(CosAlf);

		// проверка на соответствие траектории окружности
		Transform.RotZ(c, alf*180./PI);
		tmp = pb * Transform;
		d = fabs(tmp.Dist2(tmp, pe));
		if (d > 0.01)
			return 0;

		if (k != 1)
		{
			alf = alf/k;
		
			if(type == cwarc) 
				alf = -alf;
			CosAlf = cos(alf);
			Transform.RotZ(c, alf*180./PI);
		}

		NLine ln;
		// Create control points for arc

		switch(k)
		{
		case 3:
			q = pb * Transform;

			ln.Set(pb, c, c, q);
			ln.BuildBallArc();

			DirectContour.AddFirst(ln);
			
			pb = q;
		case 2:
			q = pb * Transform;

			ln.Set(pb, c, c, q);
			ln.BuildBallArc();

			if(DirectContour.num == 0)
				DirectContour.AddFirst(ln);
			else
				DirectContour.AddFollow(ln);

			pb = q;
		case 1:// single segment
			ln.Set(pb, c, c, pe);
			ln.BuildBallArc();

			if(DirectContour.num == 0)
				DirectContour.AddFirst(ln);
			else
				DirectContour.AddFollow(ln);

		}

		// возвражегние в начальное положение
		Transform.e0cong(BPoint(0.,0.,1.,0.),pn);
		DirectContour = DirectContour*Transform;
		Transform.Trans(BPoint(0.,0.,0.,1.), CenterPoint);
		DirectContour = DirectContour*Transform;

	/*	NLine tLn;
		glColor3d(0,1,0);
		for (i=0; i<DirectContour.num; i++)
			if (DirectContour.GetLine(i,tLn))
			{
				tLn.Ball2Bernstain();
				tLn.Draw();
			}
		glColor3d(0,1,1);*/

	}

	return true;
}

