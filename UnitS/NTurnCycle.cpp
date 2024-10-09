#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "BFixedSizeStack.h"
#include "nturncycle.h"

double NTurnCycle::ZDir = 0.;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NTurnCycle::NTurnCycle(BGeomArray &Cont, double XStep, double XRetr, double iSign) : BasePoint(0., 0., 0., 1.)
{
	pCont = &Cont;

	if(pCont->GetSize() <= 1)
		return;

	Step = XStep;
	Retr = XRetr;
	Sign = iSign;
	OrientCont();
	bool Monotone = (pCont->GetAt(0)->GetAttr() != 4);
	pCont->GetAt(0)->SetAttr(1);
	int Res = Monotone ? MakePath() : MakePathNonMonotone();
	if(Res != 0)
	{
		bOK = false;
		Cont.RemoveAll();
	}

	RestoreCont();
}

NTurnCycle::~NTurnCycle(void)
{
}

int NTurnCycle::MakePath(void)
{
	BPoint V = pCont->GetAt(0)->GetStartDir();
	bool XIncr = true;
	if(V.GetX() < 0)
	{
		Step *= -1.;
		XIncr = false;
	}
	else
		Retr *= -1.;
	BGeomArray OneStep, Path;
	BPoint StartPoint = pCont->GetAt(0)->GetB();
	//Find last geom cadr
	NCadrGeom *pLastCadr = NULL;
	for(auto i = pCont->GetSize() - 1; i >= 0; --i)
	{
		pLastCadr = pCont->GetAt(i);
		if(pLastCadr->HaveGeom())
			break;
	}
	if(!pLastCadr)
		return -1;
	double Diff = (XIncr ?  -1. : 1.) * (fabs(StartPoint.GetX()) - fabs(pLastCadr->GetE().GetX()));
	if(Diff > 0.)
	{
		NCadrGeom *pAddCadr = new NCadrGeom;
		pAddCadr->SetType(fast);
		pAddCadr->SetAttr(1);
		pAddCadr->SetB(pLastCadr->GetE());
		pAddCadr->SetE(BPoint(StartPoint.GetX(), 0., pLastCadr->GetE().GetZ(), 1.));
		pCont->Add(pAddCadr);
	}

	bool StopFlag = !MakeOneStep(OneStep, StartPoint, Step);
	for(int i = 0; !StopFlag && i < 500; ++i)
	{
		Path.Append(OneStep);
		StartPoint = OneStep[OneStep.GetSize() - 1]->GetE();
		OneStep.SetSize(0);
		StopFlag = !MakeOneStep(OneStep, StartPoint, Step - Retr);
	}
	pCont->GetAt(0)->SetAttr(1);
	pCont->GetAt(0)->SetB(StartPoint);
	pCont->InsertAt(0, &Path);
	Path.SetSize(0);
	return 0;
}

bool NTurnCycle::MakeOneStep(BGeomArray & OneStep, const BPoint & StartPoint, double StepX)
{
	OneStep.RemoveAll();
	
	double SDelta = StartPoint.GetX() + StepX - pCont->GetAt(0)->GetE().GetX();
	if(StepX < 0.)
	{
		if(SDelta <= MIND)
			return false;
	}
	else
	{
		if(SDelta >= -MIND)
			return false;
	}
	

	double Z;
	if(!GetZ(StartPoint.GetX() + StepX, &Z))
		return false;
	double Len = Z - StartPoint.GetZ();
	if(fabs(Len) < LMIN)
		return false;
	double RetrX = Retr;
	double RetrZ = fabs(Retr) * (Len < 0. ? 1. : -1.);

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(pCont->GetAt(0)->GetType());
	pCadr->SetB(StartPoint);
	pCadr->SetE(StartPoint + BPoint(StepX, 0., 0., 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(StepX, 0., 0., 0.));
	pCadr->SetE(StartPoint + BPoint(StepX, 0., Len, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(line);
	pCadr->SetB(StartPoint + BPoint(StepX, 0., Len, 0.));
	pCadr->SetE(StartPoint + BPoint(StepX + RetrX, 0., Len + RetrZ, 0.));
	OneStep.Add(pCadr);
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(StartPoint + BPoint(StepX + RetrX, 0., Len + RetrZ, 0.));
	pCadr->SetE(StartPoint + BPoint(StepX + RetrX, 0., 0., 0.));
	OneStep.Add(pCadr);

	return true;
}

bool NTurnCycle::GetZ(double X, double * pZ)
{
// Find Z value corresponded to given X value on the pCont excluding first cadr
//	returns false if X is outside contour projection
	for(int i = 1; i < pCont->GetSize(); ++i)
	{
		NCadrGeom *pCadr = pCont->GetAt(i);
		if(!pCadr->HaveGeom())
			continue;
		double X0 = pCadr->GetB().GetX();
		double dX = pCadr->GetE().GetX() - X0;
		if (fabs(X - X0) < MIND)
		{// Совпал с начальной точкой
			*pZ = pCadr->GetB().GetZ();
			return true;
		}
		if (fabs(X - X0 - dX) < MIND)
		{// Совпал с конечной точкой
			*pZ = pCadr->GetE().GetZ();
			return true;
		}
		if ((X - X0) * (X - pCadr->GetE().GetX()) >= 0)
			continue;// Лежит за пределами

		double Z0 = pCadr->GetB().GetZ();
		double dZ = pCadr->GetE().GetZ() - Z0;
		if(pCadr->IsLine())
		{
			*pZ = Z0 + (X - X0) * dZ / dX;// dX не может быть 0, т.к. сработает первый или второй if
			return true;
		}
		else if(pCadr->IsArc())
		{
			double R = pCadr->GetR();
			BPoint C = pCadr->GetC();
			double d = R * R - (X - C.GetX()) * (X - C.GetX());
			double sign = 
				((dX > 0) == (pCadr->IsCCWArc())) 
				? 1. : -1.;
            *pZ = C.GetZ() + sign * sqrt(fabs(d));
			return true;
		}
		else
			continue;
	}
	return false;
}

void NTurnCycle::OrientCont(void)
{
	orient = false;

	NCadrGeom *pCadr = pCont->GetAt(0);
	if(fabs(pCadr->GetB().GetX() - pCadr->GetE().GetX()) > MIND)
		return; 

	// Change orientation
	BasePoint = BPoint(0., 0., pCadr->GetE().GetZ(), 1.);
	orient = true;
	RotSign = ((pCadr->GetE() - pCadr->GetB()).GetZ() < 0.) ? 1. : -1.;
	BMatr Matr;
	Matr.RotY(BasePoint, 90. * RotSign);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}
void NTurnCycle::RestoreCont(void)
{
	if(!orient)
		return;

	NCadrGeom *pCadr = pCont->GetAt(0);

	// Change orientation
	BMatr Matr;
	Matr.RotY(BasePoint, -90. * RotSign);
	for(int i = 0; i < pCont->GetSize(); ++i)
		pCont->GetAt(i)->Tr(Matr);
}

NTurnCycle::ContElement::ContElement()
{
	Type = Undef;
	StartGeomNum = -1;
	NextElemNum = -1;
}

int NTurnCycle::MakePathNonMonotone(void)
{
	pCont->RemoveAt(0);
	BPoint V = pCont->GetAt(0)->GetStartDir();
	if(Sign < 0.)
	{
		Step *= -1.;
		Retr *= -1.;
	}
	BPoint StartPoint = pCont->GetAt(0)->GetB();
	// Divide by quadrant points
	BGeomArray GeomArray;
	double XMax = StartPoint.GetX() * Sign;
	for(int i = 0; i < pCont->GetSize(); ++i)
	{
		NCadrGeom *pGeom = pCont->GetAt(i);
		XMax = max(XMax, pGeom->GetB().GetX() * Sign);
		XMax = max(XMax, pGeom->GetE().GetX() * Sign);
		if(!pGeom->IsArc())
		{
			GeomArray.Add(new NCadrGeom(*pGeom));
			continue;
		}

		NCadrGeom *BufGeom = NULL;
		int PartsNum = pGeom->DivideQuad(BufGeom);
		if(PartsNum > 1)
		{
			for(int k = 0; k < PartsNum; ++k)
				GeomArray.Add(new NCadrGeom(BufGeom[k]));
			delete[] BufGeom;
		}
		else
			GeomArray.Add(new NCadrGeom(*pGeom));
	}
	//END: Divide by quadrant points
	//Set Start point
	StartPoint.SetX(XMax * Sign);
	BPoint LastPoint = GeomArray[GeomArray.GetSize() - 1]->GetE();
	// Add vertical (X parallel) lines if needed
	if(GeomArray[0]->GetB().GetX() * Sign < StartPoint.GetX() * Sign)
	{
		GeomArray.InsertAt(0, new NCadrGeom);
		GeomArray[0]->Set(line, StartPoint.GetX(), StartPoint.GetY(), StartPoint.GetZ(),
			0., 0., 0.);
		GeomArray[0]->SetE(GeomArray[1]->GetB());
		GeomArray[0]->SetAttr(1);
		pCont->InsertAt(0, new NCadrGeom(*GeomArray[0]));
	}
	if(GeomArray[GeomArray.GetSize() - 1]->GetE().GetX() * Sign < StartPoint.GetX() * Sign)
	{
		GeomArray.Add(new NCadrGeom);
		GeomArray[GeomArray.GetSize() - 1]->Set(line, 0., 0., 0.,
			StartPoint.GetX(), LastPoint.GetY(), LastPoint.GetZ());
		GeomArray[GeomArray.GetSize() - 1]->SetB(LastPoint);
		GeomArray[GeomArray.GetSize() - 1]->SetAttr(1);
		pCont->Add(new NCadrGeom(*GeomArray.GetLast()));
	}
	//Change (close) pCont
	NCadrGeom *pGeom = new NCadrGeom;
	pGeom->Set(fast, 0., 0., 0., 0., 0., 0.);
	pGeom->SetAttr(1);
	pGeom->SetB(pCont->GetLast()->GetE());
	pGeom->SetE(pCont->GetAt(0)->GetB());
	pCont->Add(pGeom);
	// Make Elements
	ContElement newElement;
	newElement.Point = StartPoint;
	newElement.StartGeomNum = 0;
	Elements.Add(newElement);
	BGeomArray *pBCont = new BGeomArray;
	double dX = Step * Sign;
	for(int i = 0; i < GeomArray.GetSize(); ++i)
	{
		NCadrGeom *pGeom = GeomArray[i];
		if(!pGeom)
			continue;
		if(!pGeom->HaveGeom())
			continue;
		NCadrGeom Source = *pGeom;
		double XDir = (pGeom->GetB().GetX() < pGeom->GetE().GetX()) ? 1. : -1.;
		double XStart = StartPoint.GetX() + 100000. * dX - (int((StartPoint.GetX() + 100000.* dX - pGeom->GetB().GetX() + dX - 1.e-6 * XDir) / dX) + ((XDir > 0.) ? -1 : 0)) * dX;
		bool CanDivide = (pGeom->GetE().GetX() * XDir + MIND > XStart * XDir);
		CanDivide &= (fabs(pGeom->GetB().GetX() - pGeom->GetE().GetX()) > MIND); // Not horizontal
		if(CanDivide)
		{
			NCadrGeom Dest;
			for(double XCur = XStart; XCur * XDir < pGeom->GetE().GetX() * XDir + MIND; XCur += dX * XDir)
			{
				if(fabs(XCur - pGeom->GetE().GetX()) < MIND)
				{// End point of pGeom
					// Check next dir
					bool IgnorePoint = false;
					for(int k = i + 1; k < GeomArray.GetSize(); ++k)
					{
						NCadrGeom *pNext = GeomArray[k];
						if(!pNext)
							continue;
						if(!pNext->HaveGeom())
							continue;
						if(fabs(pNext->GetB().GetX() - pNext->GetE().GetX()) < MIND) // Horizontal
							continue;
						double NextDir = (pNext->GetB().GetX() < pNext->GetE().GetX()) ? 1. : -1.;
						if(XDir * NextDir < 0.)
							IgnorePoint = true;
						break;
					}
					if(IgnorePoint)
						continue;
				}
				NCadrGeom::DivideX(Source, XCur, Dest);
				if(Source.Length() > MIND && !Source.IsFullArc(1.e-5))
					pBCont->Add(new NCadrGeom(Source));
				ContElement newElement;
				newElement.Point = Dest.GetB();
				newElement.StartGeomNum = int(pBCont->GetSize());
				Elements.Add(newElement);
				Source = Dest;
			}
			if(Dest.Length() > MIND && !Source.IsFullArc(1.e-5))
				pBCont->Add(new NCadrGeom(Dest));
		}
		else
		{
			if(Source.Length() > MIND)
				pBCont->Add(new NCadrGeom(Source));
		}
	}
	if(Elements[Elements.GetSize() - 1].StartGeomNum == pBCont->GetSize())
		Elements.RemoveAt(Elements.GetSize() - 1);
	GeomArray.RemoveAll();
	// Set element type
	for(int e = 0; e < Elements.GetSize(); ++e)
	{
		CEType Type = Undef;
		ContElement &Element = Elements[e];
		auto iMax = (e == Elements.GetSize() - 1) ? pBCont->GetSize() : Elements[e + 1].StartGeomNum;
		CEType FirstType = Undef;
		CEType LastType = Undef;
		for(int i = Element.StartGeomNum; i < iMax; ++i)// For each cadr in Elements[e]
		{
			NCadrGeom *pCur = (*pBCont)[i];
			if(!pCur->HaveGeom())
				continue;
			double dXCur = Sign * (pCur->GetE().GetX() - pCur->GetB().GetX());
			if(FirstType == Undef)
			{
				if(dXCur != 0.)
				{
					FirstType = ((dXCur > 0.) ? Up : Down);
					Type = FirstType;
				}
			}
			else
			{
				if(dXCur != 0.)
				{
					LastType = ((dXCur > 0.) ? Up : Down);
				}
			}
		}
		if(FirstType == LastType || LastType == Undef)
			Type = FirstType;
		else
			Type = (LastType == Down) ? UpDown : DownUp;
		if(FirstType == Undef) // Internal error
			return -1;
		Element.Type = Type;
	}
	// Sort elements by X,Z
	ZDir = LastPoint.GetZ() - StartPoint.GetZ();
	ZDir /= fabs(ZDir);
	typedef ContElement *LPCE;
	ContElement **Sort = new LPCE[Elements.GetSize()];
	for(int l = 0; l < Elements.GetSize(); ++l)
		Sort[l] = &Elements[l];
	qsort(Sort, Elements.GetSize(), sizeof(Sort[0]), CompareElements);
	// Define NextElemNum
	for(int e = 0; e < Elements.GetSize(); ++e)
		Sort[e]->NextElemNum = -1;
	for(int e = 0; e < Elements.GetSize() - 1;)
	{
		double XCur = Sort[e]->Point.GetX();
		if(fabs(XCur - Sort[e + 1]->Point.GetX()) < MIND)
		{
			while(fabs(XCur - Sort[e + 1]->Point.GetX()) < MIND)
			{
				Sort[e]->NextElemNum = int(Sort[e + 1] - &Elements[0]);
				e += 2;
				if(e > Elements.GetSize() - 2)
					break;
			}
		}
		else
			++e;
	}
	delete[] Sort;
	// Make path
	BFixedSizeStack<int, 512> SPoints;
	int CurElI = 0;
	SPoints.Push(CurElI);
	bool Stop = false;
	while(SPoints.GetSize() > 0)
	{
		ContElement Element = Elements[CurElI];
		if(Element.Type == Down)
		{
			MakeElementStep(CurElI, pBCont, GeomArray);// Down step
			++CurElI;
			while(1)
			{
				SPoints.Push(CurElI);
				int NextElI = Elements[CurElI].NextElemNum;
				if(NextElI < 0)
					return -1;// Internal error
				NCadrGeom *pGeom = new NCadrGeom;
				pGeom->SetAttr(1);
				pGeom->Set(line, 0., 0., 0., 0., 0., 0.);
				pGeom->SetB(Elements[CurElI].Point);
				pGeom->SetE(Elements[NextElI].Point);
				GeomArray.Add(pGeom);
				CurElI = NextElI;
				if(Elements[CurElI].Type == UpDown)
				{
					MakeElementStep(CurElI, pBCont, GeomArray); //
					++CurElI;
				}
				else
				{
					MakeElementStep(CurElI, pBCont, GeomArray);// Up step
					break;
				}
			}
		}
		else
		{
			if(Element.Type == DownUp)
				MakeElementStep(CurElI, pBCont, GeomArray);
		}
		SPoints.Pop(CurElI);
		if(SPoints.GetSize() > 0)
			MakeReturn(GeomArray.GetLast()->GetE(), Elements[CurElI].Point, GeomArray);
		else
		{// Return to start point
			BPoint P0 = GeomArray.GetLast()->GetE();
			BPoint P1 = GeomArray.GetAt(0)->GetB();
			NCadrGeom *pGeom = new NCadrGeom;
			pGeom->SetAttr(1);
			pGeom->Set(fast, 0., 0., 0., 0., 0., 0.);
			pGeom->SetB(P0);
			pGeom->SetE(BPoint(P1.GetX(), 0., P0.GetZ(), 1.));
			GeomArray.Add(pGeom);
			pGeom = new NCadrGeom;
			pGeom->SetAttr(1);
			pGeom->Set(fast, 0., 0., 0., 0., 0., 0.);
			pGeom->SetB(BPoint(P1.GetX(), 0., P0.GetZ(), 1.));
			pGeom->SetE(P1);
			GeomArray.Add(pGeom);
		}
	}
	pBCont->RemoveAll();
	delete pBCont;
//	pCont->SetSize(0);// SHOULD BE REMOVED
	pCont->InsertAt(0, &GeomArray);
	GeomArray.SetSize(0);
	return 0;
}

int NTurnCycle::CompareElements( const void *a, const void *b)
{
	NTurnCycle::ContElement *p0 = *(NTurnCycle::ContElement **)a;
	NTurnCycle::ContElement *p1 = *(NTurnCycle::ContElement **)b;
	double X0 = p0->Point.GetX();
	double X1 = p1->Point.GetX();
	if(fabs(X0 - X1) < MIND)
	{
		return (p0->Point.GetZ() * NTurnCycle::ZDir > p1->Point.GetZ() * NTurnCycle::ZDir) ? 1 : -1;
	}
	else if(X0 < X1)
		return -1;
	return 1;

}

int NTurnCycle::MakeReturn(const BPoint &P0, const BPoint &P1, BGeomArray &GeomArray)
{
	NCadrGeom Prot;
	Prot.Set(fast, 0., 0., 0., 0., 0., 0.);
	Prot.SetAttr(1);
	BPoint PInt(Sign * (max(Sign * P0.GetX(), Sign * (P1.GetX() + Step))) + Retr, 0., P1.GetZ(), 1.);

	NCadrGeom *pGeom = new NCadrGeom(Prot);
	pGeom->SetB(P0);
	pGeom->SetE(BPoint(PInt.GetX(), 0., P0.GetZ(), 0.));
	GeomArray.Add(pGeom);

	pGeom = new NCadrGeom(Prot);
	pGeom->SetB(BPoint(PInt.GetX(), 0., P0.GetZ(), 0.));
	pGeom->SetE(PInt);
	GeomArray.Add(pGeom);

	pGeom = new NCadrGeom(Prot);
	pGeom->SetType(line);
	pGeom->SetB(PInt);
	pGeom->SetE(P1);
	GeomArray.Add(pGeom);
	return 3;
}
int NTurnCycle::MakeElementStep(int e, const BGeomArray *pBCont, BGeomArray &GeomArray)
{
	ContElement &Element = Elements[e];
	int iMax = (e == Elements.GetSize() - 1) ? int(pBCont->GetSize()) : Elements[e + 1].StartGeomNum;
	for(int i = Element.StartGeomNum; i < iMax; ++i)// For each cadr in Elements[e]
	{
		GeomArray.Add(new NCadrGeom(*(*pBCont)[i]));
	}
	return iMax - Element.StartGeomNum;
}
int NTurnCycle::MakeZStep(int e, const BGeomArray *pBCont, BGeomArray &GeomArray)
{
	return 0;
}
