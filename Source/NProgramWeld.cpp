#include "stdafx.h"
#include "NCMProject.h"
#include "math.h"
#include "NSurfSettings.h"
#include "BObjectManager.h"
#include "CMapStringToNStock.h"
#include "BSpacePos.h"
#include "MTMashineTool.h"
#include "NStockStl.h"
#include "SMachStateFix.h"
#include "BPointsBuf.h"
#include "NCUnitStatePrev.h"
#include "NProgramWeld.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NProgramWeld, NProgram, 0)


NProgramWeld::NProgramWeld(void) : ProgSections(), Trajectories(*this)
{
	NTiParams &Par = NCM_PROJECT.Defaults;

	VisSections = Par.GetBool("Defaults/Program/Weld/Sections@Visible", false);
	IntSections = int(Par.GetDouble("Defaults/Program/Weld/Sections@Step", 5.));
	ProgSections.SetThickness(GLfloat(Par.GetDouble("Defaults/Program/Weld/Sections@Thick", 1.5)));

	pDialog = NULL;
}

NProgramWeld::~NProgramWeld(void)
{
	for(int i = 0; i < WeldArray.GetSize(); ++i)
		delete WeldArray[i];
	if(pDialog)
		pDialog->DestroyWindow();
}

void NProgramWeld::DrawInt(bool CompOn, bool ProgOn, bool Draw4Pick, const std::vector<BProgVisInterval> & VisInt
	, int BoldCadr /*= -1*/, double t, bool UseAxes /*= false*/, bool DrawNodes /*= false*/)
{
	//NProgram::DrawInt(CompOn, ProgOn, Draw4Pick, VisInt, BoldCadr, t);
	//if(VisSections)
	//	ProgSections.DrawSections();
	//Trajectories.DrawInt(Draw4Pick, VisInt, BoldCadr, t);
}


int NProgramWeld::MakeSectionPlanes(MTMachineTool &MT)
{
//	if(!VisSections)
//	{
//		ProgSections.Clear();
//		return 0;
//	}
//
//	IntSections = max(1, IntSections);
//	int Start = 1;
//	int End = 0;
//	int PlanesNum = 0;
//	for(bool Continue = SectNums.GetFirst(&Start, &End); Continue; Continue = SectNums.GetNext(&Start, &End))
//	{
//		if(End == 0)
//			End = int(GeomArray.GetSize() - 1);
//		else
//			End = min(End, int(GeomArray.GetSize() - 1));
//		PlanesNum += ((End - Start) / IntSections) + 1;
//	}
//	ProgSections.SetNPlanes(PlanesNum);
//
//	int PlaneInd = 0;
//	for(bool Continue = SectNums.GetFirst(&Start, &End); Continue; Continue = SectNums.GetNext(&Start, &End))
//	{
//		if(Start < 1)
//			Start = 1;
//		if(End == 0)
//			End = int(GeomArray.GetSize() - 1);
//		else
//			End = min(End, int(GeomArray.GetSize() - 1));
//		//PlanesNum += ((End - Start) / IntSections) + 1;
//		for(int i = Start - 1; i < End; i += IntSections)
//		{
//			ASSERT(PlaneInd < PlanesNum);
//			NCadrGeom *pGeom = GeomArray.GetAt(i);
//			for(int b = i; pGeom->IsUndef() && b < End + 1; ++b)
//			{// Find nearest geom cadr
//				pGeom = GeomArray.GetAt(b);
//			}
//			BPoint ToolDir(0., 0., 0., 0.);
//			BPoint ToolPoint(0., 0., 0., -1.);
//			if(!pGeom->IsUndef())
//			{
//				SMachStateFix1 Start;
//				if(pGeom->CalcCurCoordMS(BMatr().SetE(), 0., Start))
//				{
//					MT.SetState(pGeom->GetMTLink(), Start);
//					BMatr M(MT.GetStock2ToolMatr(0, 0));// Works for single tool in the project only
//					ToolDir = (BPoint(0., 0., 1., 0.) * M) % BPoint(0., 0., 1., 0.);
//					ToolPoint = BPoint(0., 0., 0., 1.) * M;
//				}
//			}
//			ProgSections.SetPosition(ToolPoint, ToolDir, PlaneInd);
//			++PlaneInd;
//		}
//	}
//	return PlanesNum;
	return 0;
}
void NProgramWeld::PrepareSections(const MTMachineTool &MT, CMapStringToNStock *pPartsList)
{
	MTMachineTool LocMT;
	LocMT.CopyKinematics(MT);
	MakeSectionPlanes(LocMT);
//	MakeSectionPlanes(MT);
	//for(POSITION pos = pPartsList->GetStartPosition(); pos != NULL;)
	//{
	//	CString str;
	//	NStock *pPart;
	//	pPartsList->GetNextAssoc(pos, str, pPart);
	//	ASSERT(pPart->IsKindOf(RUNTIME_CLASS(NStockStl)));
	//	ProgSections.MakeStockSections(pPart);
	//}
}

void NProgramWeld::Serialize(CArchive& ar)
{ 
	NProgram::Serialize(ar);
	SerializeElements(ar, &IntSections, 1);
	SerializeElements(ar, &VisSections, 1);
	SectNums.Serialize(ar);
	Trajectories.Serialize(ar);
}

BPoint NProgramWeld::CalcPtsTriplet(double t, int Ind, class MTMachineTool &MT, CMapStringToNStock *pPartsList, class BPoint &FirstP, BPoint &SecondP, BPoint &RootP, double MinW)
{
//	// H возвращаемой точки содержит Т1, H ветора RootP содержит глубину проплава
//	NCadrGeom *pGeom = GeomArray.GetAt(Ind);
//	NWeldCadr *pWeldCadr = WeldArray.GetAt(Ind);
////	Trajectories.GetCadrLenghs(Ind, F, S,
//	SMachStateFix1 State;
//	if(!pGeom->CalcCurCoordMS(BMatr().SetE(), t, State))
//	{
//		return BPoint(0., 0., 0., -1.);
//	}
//	MT.SetState(pGeom->GetMTLink(), State);
//	BMatr M(MT.GetStock2ToolMatr(0, 0));// Works for single tool in the project only
//	BPoint ToolDir = BPoint(0., 0., -1., 0.) * M;
//	BPoint ToolPoint = BPoint(0., 0., 0., 1.) * M;
//	FirstP.Set(0., 0., 0., -1.);
//	SecondP.Set(0., 0., 0., -1.);
//	double FirstD2 = 1.e12;
//	double SecondD2 = 1.e12;
//	int PtsNum = 0; // Number of points found
//	for(POSITION pos = pPartsList->GetStartPosition(); pos != NULL;)
//	{
//		CString str;
//		NStock* pPart;
//		pPartsList->GetNextAssoc(pos, str, pPart);
//		ASSERT(pPart->IsKindOf(RUNTIME_CLASS(NStockStl)));
//		BPoint Bn;
//		BPoint BP = ((NStockStl *)pPart)->RayCastingOr(ToolPoint - ToolDir * 100., ToolDir, MinW, Bn);
//		if(BP.IsPoint())
//		{
//			++PtsNum;
//			double D2 = (ToolPoint - BP).D2();
//			if(D2 < FirstD2)
//			{
//				SecondD2 = FirstD2;
//				SecondP = FirstP;
//				FirstD2 = (ToolPoint - BP).D2();
//				FirstP = BP;
//			}
//			else if(D2 < SecondD2)
//			{
//				SecondD2 = D2;
//				SecondP = BP;
//			}
//		}
//	}
//	RootP.Set(0., 0., 0., -1.);
//	double RootL, T1;
//	if(pWeldCadr->GetRootDist(t, &RootL, &T1))
//	{
//		RootP = ToolDir;
//		RootP.SetH(RootL);
//		ToolPoint = ToolPoint + ToolDir * T1;
//		ToolPoint.SetH(T1);
//	}
//	return ToolPoint;
	return 0;
}
int NProgramWeld::MakePaths(class MTMachineTool &iMT, CMapStringToNStock *pPartsList)
{
//	if(!IsEval())
//		return 0;
//
//	double MinW = 0.1;
//	double *pDouble;
//	if((NCM_PROJECT.DoubleParams.Lookup("WeldBeamR", pDouble)))
//		MinW = *pDouble;
//
//	MTMachineTool MT;
//	MT.CopyKinematics(iMT);
//
//
//	double MaxLength = NSurfSettings::GetTolerance() * 5;
//	BPointsBuf PointsBuf1;
//	BPointsBuf PointsBuf2;
//	BPointsBuf PointsBuf3;
//	BPointsBuf PointsBuf4;
//	NColor Col = GetColor();
//	Trajectories.Create(int(GeomArray.GetSize()), Col.Inv());
//	int PointsNum = 0;
//	bool VeryFirst = true;
//	for(int i = 0; i < GeomArray.GetSize(); ++i)
//	{
//		NCadrGeom *pGeom = GeomArray.GetAt(i);
//		if(pGeom->IsUndef())
//			continue;
//		PointsBuf1.SetSize(2);
//		PointsBuf2.SetSize(2);
//		PointsBuf3.SetSize(2);
//		PointsBuf4.SetSize(2);// Temporary for base tool point
//		BPoint FirstP;
//		BPoint SecondP;
//		BPoint RootP;
//		if(VeryFirst)// very first cadr
//		{
//			VeryFirst = false;
//			PointsBuf4.SetPoint(0, CalcPtsTriplet(0., i, MT, pPartsList, FirstP, SecondP, RootP, MinW));
//			// H возвращаемой точки содержит Т1, H ветора RootP содержит глубину проплава
//			PointsBuf1.SetPoint(0, FirstP);
//			PointsBuf2.SetPoint(0, SecondP);
//			PointsBuf3.SetPoint(0, RootP);
//		}
//		PointsBuf4.SetPoint(1, CalcPtsTriplet(1., i, MT, pPartsList, FirstP, SecondP, RootP, MinW));
//			// H возвращаемой точки содержит Т1, H ветора RootP содержит глубину проплава
//		PointsBuf1.SetPoint(1, FirstP);
//		PointsBuf2.SetPoint(1, SecondP);
//		PointsBuf3.SetPoint(1, RootP);
//		double TrajLen  = max(max(
//			(PointsBuf1.GetPoint(1) - PointsBuf1.GetPoint(0)).D2(),
//			(PointsBuf2.GetPoint(1) - PointsBuf2.GetPoint(0)).D2()),
//			(PointsBuf4.GetPoint(1) - PointsBuf4.GetPoint(0)).D2()
//			);
//		int PtsCount = min(BPointsBuf::GetMainSize(), int(sqrt(TrajLen) / MaxLength) + 2);
//		if(PtsCount > 2)
//		{
//			PointsBuf1.SetSize(PtsCount);
//			PointsBuf2.SetSize(PtsCount);
//			PointsBuf3.SetSize(PtsCount);
//			PointsBuf4.SetSize(PtsCount);
//			PointsBuf1.SetPoint(PtsCount - 1, PointsBuf1.GetPoint(1));
//			PointsBuf2.SetPoint(PtsCount - 1, PointsBuf2.GetPoint(1));
//			PointsBuf3.SetPoint(PtsCount - 1, PointsBuf3.GetPoint(1));
//			PointsBuf4.SetPoint(PtsCount - 1, PointsBuf4.GetPoint(1));
//			for(int ip = 1; ip < PtsCount - 1; ++ip)
//			{
//				PointsBuf4.SetPoint(ip, CalcPtsTriplet(double(ip) / (PtsCount - 1), i, MT, pPartsList, FirstP, SecondP, RootP, MinW));
//				// H возвращаемой точки содержит Т1, H ветора RootP содержит глубину проплава
//				PointsBuf1.SetPoint(ip, FirstP);
//				PointsBuf2.SetPoint(ip, SecondP);
//				PointsBuf3.SetPoint(ip, RootP);
//			}
//		}
//		// Recalc PointsBuf3 and PointsBuf4 to take velocity into account
//		double OneStepTime = max(CompGeomArray.GetAt(i)->GetTime(), 0.00001) / (PointsBuf3.GetSize() - 1);
//		for(int k = (i == 0) ? 0 : 1; k < PointsBuf3.GetSize(); ++k)
//		{
//			int k1 = (k == 0) ? k + 1 : k - 1;
//			double OneStepLength = -1.;
//			if(PointsBuf2.GetPoint(k).IsPoint() && PointsBuf2.GetPoint(k - 1).IsPoint())
//				OneStepLength = sqrt((PointsBuf2.GetPoint(k) - PointsBuf2.GetPoint(k - 1)).D2());
//			else if(PointsBuf1.GetPoint(k).IsPoint() && PointsBuf1.GetPoint(k - 1).IsPoint())
//				OneStepLength = sqrt((PointsBuf1.GetPoint(k) - PointsBuf1.GetPoint(k - 1)).D2());
//
//			if((OneStepLength < 0. && WeldArray.GetAt(i)->ConsiderF) || PointsBuf4.GetPoint(k).GetH() < 0. || PointsBuf3.GetPoint(k).GetH() < 0.)
//				PointsBuf3.SetPoint(k, BPoint(0., 0., 0., -1.));
//			else
//			{
//				double Vel = 1.;
//				if(WeldArray.GetAt(i)->ConsiderF)
//					Vel = OneStepLength * 60. / OneStepTime;
//				BPoint BufP3 = PointsBuf3.GetPoint(k);
//				BufP3.SetH(BufP3.GetH() / Vel);
//				PointsBuf3.SetPoint(k, BufP3);
//				BPoint BufP4 = PointsBuf4.GetPoint(k);
//				double BufT1 = BufP4.GetH();
//				BufP4 = PointsBuf3.GetPoint(k) * PointsBuf3.GetPoint(k).GetH() + BufP4;
//				BufP4.SetH(BufT1);
//				PointsBuf4.SetPoint(k, BufP4);
//			}
//		}
//		Trajectories.AddElem(PointsBuf1, PointsBuf2, PointsBuf3, PointsBuf4, i, 0, i, Col);
//		PointsBuf1.SetPoint(0, PointsBuf1.GetPoint(PtsCount - 1));
//		PointsBuf2.SetPoint(0, PointsBuf2.GetPoint(PtsCount - 1));
//		PointsBuf3.SetPoint(0, PointsBuf3.GetPoint(PtsCount - 1));
//		PointsBuf4.SetPoint(0, PointsBuf4.GetPoint(PtsCount - 1));
//	}
//	Trajectories.Close();
	return 0;
}
void NProgramWeld::PreparePaths(MTMachineTool &MT, CMapStringToNStock *pPartsList)
{
	MakePaths(MT, pPartsList);
	FillGabar(BSpacePos::GetRotMatrs());
}
//void NProgramWeld::PrepRenders(double Eps)
//{
//	NProgram::PrepRenders(Eps);
//}
NProgramWeld::NSectNums::NSectNums(void) 
	: Delim(',')
{
	CurrentStrPos = 0;
}
const NProgramWeld::NSectNums & NProgramWeld::NSectNums::operator =(const NSectNums &In)
{
	CString::operator = (In);
	CurrentStrPos = 0;
	return *this;
}
const NProgramWeld::NSectNums & NProgramWeld::NSectNums::operator =(const CString &In)
{
	CString::operator = (In);
	CurrentStrPos = 0;
	return *this;
}
NProgramWeld::NSectNums::~NSectNums(void)
{
}
void NProgramWeld::NSectNums::Serialize(CArchive &ar)
{
	if(ar.IsLoading())
		CurrentStrPos = 0;
	SerializeElements(ar, (CString *)this, 1);
}

bool NProgramWeld::NSectNums::GetFirst(int *Start, int *End)
{
	CurrentStrPos = 0;
	return GetNext(Start, End);
}

bool NProgramWeld::NSectNums::GetNext(int *Start, int *End)
{
	int EndPos = Find(Delim, CurrentStrPos);
	if(EndPos < 0)
		EndPos = GetLength();
	CString Substr = Mid(CurrentStrPos, EndPos - CurrentStrPos);
	if(Substr.IsEmpty())
		return false;
	CurrentStrPos += Substr.GetLength() + 1;
	int MPos = Substr.Find('-');
	if(MPos == 0)
		return false;
	if(MPos > 0)
	{
		*Start = atoi(Substr.Left(MPos));
		Substr.Delete(0, MPos + 1);
		*End = atoi(Substr);
	}
	else
	{
		*Start = atoi(Substr);
		*End = *Start;
	}
	return true;
}

bool NProgramWeld::NSectNums::AddPair(int Start, int End)
{
	CString Pair;
	Pair.Format("%d-%d,", Start, End); 
	Insert(0, Pair);
	return true;
}
const CString NProgramWeld::NSectNums::GetString()
{
	int EndPos = Find(Delim, 0);
	if(EndPos < 0)
		EndPos = GetLength();
	return Mid(EndPos + 1);
}
//void NProgramWeld::FillWeldCadrs(const NCUnitStatePrev &Start, const NCUnitStatePrev &End, const NCadrGeomArray &CadrGeomArray)
//{
//	//double Length = 0.;
//	//auto BegI = WeldArray.GetSize();
//	//for(auto i = BegI; i < GeomArray.GetSize(); ++i)
//	//{
//	//	Length += GeomArray[i]->Length();
//	//}
//	//double DistS = Start.GetPenetrDist();
//	//double DistE = End.GetPenetrDist();
//	//bool ConsiderF = (Start.ElectrSF == 0. && End.ElectrSF == 0.);
//	//double Dist0 = DistS;
//	//double T1_S = Start.ElectrT1;
//	//double T1_E = End.ElectrT1;
//	//double T1_0 = T1_S;
//	//double CurL = 0.;
//	//for(auto i = BegI; i < GeomArray.GetSize(); ++i)
//	//{
//	//	NCadrGeom *pGeom = GeomArray[i];
//	//	double L = pGeom->Length();
//	//	NWeldCadr *pWeldCadr = new NWeldCadr;
//	//	pWeldCadr->ConsiderF = ConsiderF;
//	//	CurL += L;
//	//	if(CurL > 0.)
//	//	{
//	//		double v = CurL / Length;
//	//		pWeldCadr->RootDist0 = Dist0;
//	//		pWeldCadr->RootDist1 = DistS * (1. - v) + DistE * v;
//	//		Dist0 = pWeldCadr->RootDist1;
//	//		pWeldCadr->RootT1_0 = T1_0;
//	//		pWeldCadr->RootT1_1 = T1_S * (1. - v) + T1_E * v;
//	//		T1_0 = pWeldCadr->RootT1_1;
//	//	}
//	//	else
//	//	{
//	//		pWeldCadr->RootDist0 = -1.;
//	//		pWeldCadr->RootDist1 = -1.;
//	//		pWeldCadr->RootT1_0 = T1_S;
//	//		pWeldCadr->RootT1_1 = T1_S;
//	//	}
//	//	WeldArray.Add(pWeldCadr);
//	//}
//}

void NProgramWeld::ClearEvaluated()
{
	NProgram::ClearEvaluated();
	for(int i = 0; i < WeldArray.GetSize(); ++i)
		delete WeldArray[i];
	WeldArray.RemoveAll();
}

void NProgramWeld::FillGabar(const BMatr *MatrArr)
{
	NProgram::FillGabar(MatrArr);
	Gabar.Expand(Trajectories.GetGabar());
}

void NProgramWeld::DrawPenetration(int Cadr, double t)
{
	Trajectories.DrawPenetration(Cadr, t);
}
