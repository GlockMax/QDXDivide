#include "..\Common\NContour.h"
// NContour.cpp: implementation of the NContour class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCadrGeom.h"
#include "NContour.h"
#include "NLine.h"
#include "BPoint.h"
#include "ConstDef.h"
#include "math.h"
#include "NSurfSettings.h"
#include "ncontour.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NContour::NContour()
{
	num = 0;
}

NContour::NContour(const NContour &c)
{
	lns.Copy(c.lns);
	Types.Copy(c.Types);
	num = c.num;
}

NContour::~NContour()
{
	Clear();
}

int NContour::Append(const NContour &c)
{
	NLine line;
	int n;
	for (int i=0; i<c.num; i++)
	{
		if (!c.GetLine(i, line))
			return num;

		if (num	== 0)
		{
			AddFirst(line);
			n = 1;
		}
		else
			n = AddFollow(line);

		SetTypeLine(n-1, c.Types.GetAt(i));
	}
	return num;
}

// добавление первой линии к контуру
void NContour::AddFirst(const BPoint p0,const BPoint p1,
						const BPoint p2,const BPoint p3)
{
	lns.RemoveAll();
	lns.Add(p0);
	lns.Add(p1);
	lns.Add(p2);
	lns.Add(p3);

	Types.Add(NOTKNOWN);

	num = 1;
}

// добавление первой линии к контуру
void NContour::AddFirst(const NLine &ln)
{
	lns.Add(ln.p[0]);
	lns.Add(ln.p[1]);
	lns.Add(ln.p[2]);
	lns.Add(ln.p[3]);

	Types.Add(NOTKNOWN);

	num = 1;
}

// добавление линии в массив
int NContour::AddFollow(const BPoint p1,const BPoint p2,const BPoint p3)
{
	lns.Add(p1);
	lns.Add(p2);
	lns.Add(p3);

	Types.Add(NOTKNOWN);

	return ++num;
}

// добавление линии к контуру
int NContour::Add(const NLine &ln)
{
	if (lns.IsEmpty())
		lns.Add(ln.p[0]);
	lns.Add(ln.p[1]);
	lns.Add(ln.p[2]);
	lns.Add(ln.p[3]);

	Types.Add(NOTKNOWN);

	return ++num;
}


int NContour::AddFollow(const NLine &ln)
{
	lns.Add(ln.p[1]);
	lns.Add(ln.p[2]);
	lns.Add(ln.p[3]);

	Types.Add(NOTKNOWN);

	return ++num;
}

// удаление линии из контура
bool NContour::Remove(int index)
{
	if (index <0 || index>=num || num==0)
		return false;

	lns.RemoveAt(index * 3,3);
	Types.RemoveAt(index, 1);
	num--;
	return true;

}

// очистка массива линий
void NContour::Clear()
{
	if (num == 0) return;
	num = 0;
	lns.RemoveAll();
	Types.RemoveAll();
}

// умножение массива линий (контура) на матрицу
//NContour NContour::operator *(const NMatrix m) const
NContour NContour::operator *(const BMatr &m) const
{
	NContour con;
	
	if (num>0)
	{
		con.AddFirst(lns[0]*m, lns[1]*m, lns[2]*m, lns[3]*m);
		con.Types[0] = Types[0];
	}

	for (int i=4; i<3*num+1; i+=3)
	{
		con.AddFollow(lns[i]*m, lns[i+1]*m, lns[i+2]*m);
		con.Types[(i - 1) / 3] = Types[(i - 1) / 3];
	}
	return con;
}

NContour& NContour::operator =(const NContour &c)
{
	lns.Copy(c.lns);
	Types.Copy(c.Types);
	num = c.num;

	return *this;
}

void NContour::SetTypeLine(int index, TypeLineInContour type)
{
	if (index <0 || index>=num || num==0)
		return;
	int k;
	switch (type)
	{
	case CONE:
		k = index*3;
		if(fabs(lns[k].GetZ() - lns[k+3].GetZ()) < MIND)
			Types[index] = HORIZONTAL;
		else
			Types[index] = CONE;
		break;
	case SPHERE:
		Types[index] = SPHERE;
		break;
	case TORAHS:
		Types[index] = TORAHS;
		break;
	case HORIZONTAL:
		Types[index] = HORIZONTAL;
		break;
	case NOTKNOWN:
		Types[index] = NOTKNOWN;
		break;
	}
}

// определить линию в контуре по заданному индексу
bool NContour::GetLine(int index, NLine& line) const
{
	if (index <0 || index>=num || num==0)
		return false;

	if (index == 0)
		line.Set(lns[0], lns[1], lns[2], lns[3]);
	else
		line.Set(lns[3*index], 
				 lns[3*index+1], 
				 lns[3*index+2],
				 lns[3*index+3]);

	return true;
}

double NContour::GetMaxX(void) const
{
	double Xmax = 0.;
	for(int i = 0; i < lns.GetSize(); ++i)
		Xmax = max(Xmax, lns[i].GetX()/lns[i].GetH());
	return Xmax;
}

void NContour::MoveLines(int Start, int End, const BPoint &e)
{
	if(Start >= End)
		return;
	int StartPoint = Start * 3;
	int EndPoint = End * 3 + 1;
	BMatr m;
	m.Trans(BPoint(0.,0.,0.,0.), e);
	for(int i = StartPoint; i < EndPoint; ++i)
		lns[i] = lns[i] * m;
}

void NContour::CrVertMovCont(const NLine &Line)
{
	CrVertMovCont(Line.p[0], Line.p[3]);
}

void NContour::CrVertMovCont(const BPoint &P0, const BPoint &P1)
{
	// Find strict extremum
	int Top,Bottom;
	bool Stop = false;
	for (Top = 0; Top < num && !Stop; ++Top)
	{
		if (lns.GetAt(Top * 3).GetX() > lns.GetAt((Top + 1) * 3).GetX())
		{
			Bottom = Top;
			break;
		}
		if (lns.GetAt(Top * 3).GetX() == lns.GetAt((Top + 1) * 3).GetX())
		{
			for (Bottom = Top + 1; Bottom < num - 1; ++Bottom)
			{
				if (lns.GetAt(Bottom * 3).GetX() == lns.GetAt((Bottom + 1) * 3).GetX())
					continue;
				Stop = (lns.GetAt(Bottom * 3).GetX() > lns.GetAt((Bottom + 1) * 3).GetX());
				break;
			}
		}
	}
	if(Top == Bottom)
	{
		lns.InsertAt(Top*3+1,lns[Top*3],3);
		Types.InsertAt( Top, CONE);
		num++;
	}
	else
	{
		if(Bottom > Top+1)
		{
			lns.RemoveAt((Top+1)*3+1, (Bottom-Top-1)*3);
			Types.RemoveAt(Top+1, (Bottom-Top-1));
			num -= (Bottom-Top-1);
		}
	}
	Types.SetAt(Top, CONE);
	if(P1.GetZ() < P0.GetZ())
		MoveLines(Top+1, num, P1 - P0);
	else
		MoveLines(0, Top, P1 - P0);
	lns[Top*3+1] = (lns[Top*3]+lns[Top*3+3])*0.5;
	lns[Top*3+2] = lns[Top*3+1];
}

void * NContour::GetDump()
{
	BPoint *pDump = new BPoint[1 + num*3 + 1 + num];
	pDump[0].Set(num,1.,1.,1.);
	for(int ic = 0; ic < num*3 + 1; ++ic)
		pDump[ic + 1] = lns[ic];

	int Shift = num*3 + 1 + 1;
	for(int it = 0; it < num; ++it)
		pDump[Shift + it].Set(int(Types[it]),2.,2.,2.);

	return pDump;
}

void NContour::SetDump(void * pDump)
{
	BPoint *pPoint = (BPoint *)pDump;
	int LinesNum = int(pPoint->GetX());
	num = LinesNum;

	lns.SetSize(LinesNum * 3 + 1);
	pPoint++;
	for(int ic = 0; ic < LinesNum*3 + 1; ++ic, ++pPoint)
		lns[ic] = BPoint(*pPoint);

	Types.SetSize(LinesNum);
	for(int it = 0; it < LinesNum; ++it, ++pPoint)
		Types[it] = TypeLineInContour(int(pPoint->GetX()));
}

void NContour::Approximate(CArray <BPoint, const BPoint &> *pCenters)
{
	int InitNum = num;// num will be changed in the cycle body
	int k = 0;
	for(int i = 0; i < InitNum; ++i,++k)// i - init cont index
	{
		if(pCenters)
			pCenters->Add(BPoint(0.,0.,0.,-1.));
		if(Types[k] != SPHERE && Types[k] != TORAHS)
			continue;
		NLine Line;
		GetLine(k, Line);
		// Sphere or torus
		if(fabs(Line.p[3].GetX()) < MIND)
			if(fabs(Line.GetCenter().GetX()) < MIND)
				continue; // is sphere
		int AppNum = GetNumAproxLine(Line);
		for(int l = 1; l < AppNum; ++l)
		{
			if(pCenters)
				pCenters->Add(Line.GetCenter());
			Types.InsertAt(k, CONE);
			BPoint P = Line.GetPointFromParam(1. - double(l)/AppNum).Norm();
			lns.InsertAt(3*k + 1, P);
			lns.InsertAt(3*k + 1, (P + lns[3*k]) * 0.5);
			lns.InsertAt(3*k + 1, lns[3*k + 1]);
		}
		num += AppNum - 1;
		k += AppNum - 1;
		if(pCenters)
			pCenters->SetAt(k - AppNum + 1, Line.GetCenter());
		Types[k] = CONE;
		lns[3*k + 1] = (lns[3*k] + lns[3*k + 3]) *  0.5;
		lns[3*k + 2] = lns[3*k + 1];
	}
	num = k;
}

// определение числа разбиения дуги окружности
int NContour::GetNumAproxLine(const NLine &Line)
{
#ifdef _DEBUG
	if ((Line.p[0]-Line.p[3]).D2() <= 0)
		return 1;
#endif

	double R, alf;
	{ // EK start
	double cosA2 = Line.p[1].GetH();
	alf = acos(cosA2);
	BPoint T1 = Line.p[1];
	T1.Norm();
    R = sqrt((Line.p[3] - Line.p[0]).D2())*0.5;
	R = R/sin(alf);
	alf = alf * 2.;
	} // EK end

	alf = alf*180./PI;
	int n = NSurfSettings::GetNumAproxLine(R, alf);
	return n;
}

//bool NContour::MakeContour(NCadrGeom * GeomArr, int Num)
//{
//	// clear current
//	Clear();
//	//make new
//	if(Num < 1)
//		return false;
//	lns.Add(GeomArr[0].GetB());
//	for(int i = 0; i < Num; ++i)
//	{
//		NCadrGeom *pGeom = GeomArr + i;
//		if(pGeom->IsFast())
//			continue;
//		BPoint p0 = pGeom->GetB();
//		BPoint p3 = pGeom->GetE();
//		if(pGeom->IsLine())
//		{
//			AddFollow((p0 + p3) * 0.5, (p0 + p3) * 0.5, p3);
//			if(fabs(p0.GetY() - p3.GetY()) < MIND)
//				Types[num - 1] = HORIZONTAL;
//			else
//				Types[num - 1] = CONE;
//		}
//		else if(pGeom->IsArc())
//		{
//			NCadrGeom *pGeomArr = NULL;
//			int Cnt = pGeom->DivideQuad(pGeomArr);
//			if(Cnt < 0)
//				return false;
//			if(Cnt == 0)
//			{
//				NLine Line;
//				Line.p[0] = p0;
//				Line.p[1] = pGeom->GetC();
//				Line.p[3] = p3;
//				Line.BuildBallArc();
//				AddFollow(Line);
//				if(fabs(Line.p[1].GetX()) < MIND
//					&& (fabs(Line.p[3].GetX()) < MIND || fabs(Line.p[3].GetX()) < MIND))
//					Types[num - 1] = SPHERE;
//				else
//					Types[num - 1] = TORAHS;
//
//			}
//			else
//			{
//				for(int i = 0; i < Cnt; ++i)
//				{
//					NLine Line;
//					Line.p[0] = p0;
//					Line.p[1] = pGeomArr[i].GetC();
//					Line.p[3] = p3;
//					Line.BuildBallArc();
//					AddFollow(Line);
//					if(fabs(Line.p[1].GetX()) < MIND
//						&& (fabs(Line.p[3].GetX()) < MIND || fabs(Line.p[3].GetX()) < MIND))
//						Types[num - 1] = SPHERE;
//					else
//						Types[num - 1] = TORAHS;
//				}
//				delete[] pGeomArr;
//			}
//		}
//	}
//	if(num < 2 || fabs(lns[0].GetX()) > MIND || fabs(lns[3 * (num - 1)].GetX()) > MIND)
//	{
//		Clear();
//		return false;
//	}
//
//	return true;
//}
void NContour::Invert(void)
{
	for(int i = 0; i < lns.GetSize() / 2; ++i)
	{
		BPoint P = lns.GetAt(i);
		lns.SetAt(i, lns.GetAt(lns.GetSize() - 1 - i));
		lns.SetAt(lns.GetSize() - 1 - i, P);
	}
	for(int k = 0; k < num / 2; ++k)
	{
		TypeLineInContour B = Types[k];
		Types[k] = Types[Types.GetSize() - 1 - k];
		Types[Types.GetSize() - 1 - k] = B;
	}
}
void NContour::SetMiddlePoints(void)
{
	// Set right middle points on lines
	for(int k = 0; k < num; ++k)
	{
		if(Types[k] == HORIZONTAL || Types[k] == CONE)
		{
			int Is = k * 3, Ie = k * 3 + 3;
			lns[Is + 1] = (lns[Is] + lns[Ie]) * 0.5;
			lns[Is + 2] = lns[Is + 1];
		}
	}
}

void NContour::Serialize(CArchive &ar)
{
	SerializeElements(ar, &num, 1);
	lns.Serialize(ar);
	Types.Serialize(ar);
}

int NContour::MakeFromCadrGeom(const NCadrGeom * GeomArr, int Num)
{
	// Contour lies in XY plane

	// clear current
	Clear();
	//make new
	int count = 0;
	for(int i = 0; i < Num; ++i)
	{
		const NCadrGeom *pGeom = GeomArr + i;
		if(pGeom->IsLine() || pGeom->IsArc())
		{
			if(!AddGeomElem(pGeom))
			{
				Clear();
				return 0;
			}
			++count;
		}
		else
			continue;
	}
	if(count < 2)
	{
		Clear();
		return 0;
	}
	// Test angle
	return num;
}

bool NContour::AddGeomElem(const NCadrGeom *pGeom)
{
	// Invert pGeom
	NLine Line;
	Line.p[3] = pGeom->GetE();
	Line.p[0] = pGeom->GetB();
	TypeLineInContour Type = NOTKNOWN;
	if(pGeom->IsArc())
	{
		Line.p[1] = pGeom->GetC();
		Type = TORAHS;
		if(fabs(Line.p[1].GetX()) < MIND && (fabs(Line.p[0].GetX()) < MIND || fabs(Line.p[3].GetX())))
			Type = SPHERE;
		Line.BuildBallArc();
	}
	else
	{
		Type = CONE;
		if(fabs(Line.p[0].GetY() - Line.p[3].GetY()) < MIND)
			Type = HORIZONTAL;
		Line.p[1] = Line.p[2] = (Line.p[0] + Line.p[3]) * 0.5;
	}
	// add start if empty
	if(num == 0)
		AddFirst(Line);
	else
		AddFollow(Line);
	Types[num - 1] = Type;
	return true;
}

bool NContour::GetApt7(double& Alf, double& H, double& DBot, double& R, double& BAlf, double& ELength, double& FLength) const
{
	// contour is up -> down oriented
	if (num < 2 || num > 4)
		return false;
	if (fabs(lns[0].GetX()) > MIND)
		return false;
	if (fabs(lns[lns.GetSize() - 1].GetX()) > MIND)
		return false;
	if (Types[0] != HORIZONTAL)
		return false;
	Alf = 0.;
	DBot = 0.;
	BAlf = 0.;
	ELength = 0.;
	FLength = 0.;
	R = 0.;
	H = fabs(lns[0].GetZ() - lns[lns.GetSize() - 1].GetZ());
	const double DTop = 2. * sqrt((lns[0] - lns[3]).D2());
	switch (num)
	{
	case 2:
		if (Types[1] == SPHERE)
		{
			NLine Line;
			for (int i = 0; i < 4; ++i)
				Line.p[i] = lns[i + 6];
			R = Line.GetRad();
			DBot = 2. * fabs(Line.p[1].Norm().GetX());
			return true;
		}
		if (Types[1] == CONE)
		{
			BPoint V = lns[6] - lns[3];
			BAlf = atan2(V.GetZ(), V.GetX()) * 180. / PI;
			DBot = DTop;
			return true;
		}
		return false;
	case 3:
		if (Types[1] == TORAHS)
		{
			NLine Line;
			for (int i = 0; i < 4; ++i)
				Line.p[i] = lns[i + 3];
			R = Line.GetRad();
			DBot = 2. * fabs(Line.p[1].Norm().GetX());
			Alf = atan2(0.5 * (DTop - DBot), H) * 180. / PI;
			if (Types[2] == HORIZONTAL)
			{
				return true;
			}
			if (Types[2] == CONE)
			{
				BPoint V = lns[6] - lns[3];
				BAlf = atan2(V.GetZ(), V.GetX()) * 180. / PI;
				return true;
			}
			return false;
		}
		if (Types[1] == CONE)
		{
			if (Types[2] == HORIZONTAL)
			{
				DBot = 2. * fabs(lns[6].GetX());
				Alf = atan2(0.5 * (DTop - DBot), H) * 180. / PI;
				return true;
			}
			if (Types[2] == CONE)
			{
				DBot = 2. * fabs(lns[6].GetX());
				BPoint V = lns[6] - lns[3];
				BAlf = atan2(V.GetZ(), V.GetX()) * 180. / PI;
				return true;
			}
			if (Types[2] == SPHERE)
			{
				NLine Line;
				for (int i = 0; i < 4; ++i)
					Line.p[i] = lns[i + 6];
				R = Line.GetRad();
				DBot = 2. * fabs(Line.p[1].Norm().GetX());
				return true;
			}
			return false;
		}
		return false;
	case 4:
		if (Types[1] != CONE)
			return false;
		if (Types[2] != TORAHS)
			return false;
		NLine Line;
		for (int i = 0; i < 4; ++i)
			Line.p[i] = lns[i + 6];
		R = Line.GetRad();
		DBot = 2. * fabs(Line.p[1].Norm().GetX());
		Alf = atan2(0.5 * (DTop - DBot), H) * 180. / PI;
		if (Types[3] == HORIZONTAL)
		{
			return true;
		}
		if (Types[3] == CONE)
		{
			BPoint V = lns[9] - lns[6];
			BAlf = atan2(V.GetZ(), V.GetX()) * 180. / PI;
			return true;
		}
		return false;
	}
	return false;
}

void NContour::ExchangeYZ(void)
{
	for(int i = 0; i < lns.GetSize(); ++i)
		lns[i].XY2XZ();
}

bool NContour::HaveSphere(void) const
{
	for (int k = 0; k < num; ++k)
	{
		if (Types[k] == SPHERE)
			return true;
	}
	return false;
}
