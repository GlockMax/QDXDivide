#include "stdafx.h"
#include "BProgram.h"
#include "NCStatesHist.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NCStatesHist::NCStatesHist()
{
	for (int i = 0; i < MAX_STATES_NUM; ++i)
	{
		States[i] = new NCUnitState;
		AddStates[i] = new NCUnitState;
		PrevGeomStates[i] = new NCUnitStatePrevGeom;
	}
	Clear();
}


NCStatesHist::~NCStatesHist(void)
{
	for (int i = 0; i < MAX_STATES_NUM; ++i)
	{
		delete States[i];
		delete AddStates[i];
		delete PrevGeomStates[i];
	}
}

void NCStatesHist::Clear()
{
	Size = 0;
	StoreStep = 1;
	StoreInd = 0;
	Closed = false;
}

void NCStatesHist::AddState(const NCUnitState &St, const NCUnitState &AddSt, const NCUnitStatePrevGeom &GeomSt)
{
	if(Closed)
		return;
	if (St.ProbeCadr) // may contain improper data
		return;
	if(Size >= MAX_STATES_NUM)
		Halve();
	*(States[Size]) = St;
	*(AddStates[Size]) = AddSt;
	*(PrevGeomStates[Size]) = GeomSt;
	++Size;
}

void NCStatesHist::Swap(int i, int j)
{
	NCUnitState *pB = States[i];
	States[i] = States[j];
	States[j] = pB;
	pB = AddStates[i];
	AddStates[i] = AddStates[j];
	AddStates[j] = pB;
	NCUnitStatePrevGeom *pC = PrevGeomStates[i];
	PrevGeomStates[i] = PrevGeomStates[j];
	PrevGeomStates[j] = pC;
}

void NCStatesHist::Halve(void)
{
	// Compress States to a half
	for(int i = 1; i < Size / 2; ++i)
		Swap(i, 2 * i);
	Size /= 2;
	StoreStep *= 2;
}

void NCStatesHist::SendState(const NCUnitState &St, const NCUnitState &AddSt, const NCUnitStatePrevGeom &GeomSt)
{// Store the very first St and every StoreStep's St
	if(Closed)
		return;
	if(Size == 0)
		AddState(St, AddSt, GeomSt);
	++StoreInd;
	if(StoreInd == StoreStep)
	{
		StoreInd = 0;
		AddState(St, AddSt, GeomSt);
	}
}

void NCStatesHist::Init(int EstimatedProgSize)
{
	Clear();
	StoreStep = EstimatedProgSize / (MAX_STATES_NUM - 1) + 1;
	StoreStep = max(1, StoreStep);
	StoreInd = 0;
}

bool NCStatesHist::IsGrater(int Ind, cadrID SearchArg, bool AbsHCadr) const
{// Returns true if SearchArg is grater than or equal to Ind
	if(SearchArg >= (AbsHCadr ?
		++States[Ind]->GetCurHCadrAbsEx() : States[Ind]->GetCurCadrIDEx())// -
		)
		return true;
	return false;
}

int NCStatesHist::FindMaxLowerInd(cadrID SearchArg, bool AbsHCadr) const
{// Returns a maximum index State lower than or equal to SearchArg
	if (Size == 0)
		return 0;
	int End = Size - 1;
	if(IsGrater(End, SearchArg, AbsHCadr) || SearchArg.Prog < 0)
		return End;
	int Start = 0;
	while(End > Start + 1)
	{
		int Mid = (End + Start) / 2;
		if(IsGrater(Mid, SearchArg, AbsHCadr))
			Start = Mid;
		else
			End = Mid;
	}
	return Start;
}

void NCStatesHist::GetStatePair(int Ind, const NCUnitState *&pSt, const NCUnitState *&pAddSt, const NCUnitStatePrevGeom *&pGeomSt) const
{
	Ind = max(0, Ind);
	Ind = min(Size - 1, Ind);
	pSt = States[Ind];
	pAddSt = AddStates[Ind];
	pGeomSt = PrevGeomStates[Ind];
}

//const NCUnitState &NCStatesHist::GetLastState(void) const
//{
//	return *States[max(0, Size - 1)];
//}

void NCStatesHist::Close(void)
{
	Closed = true;
}

void NCStatesHist::Open(void)
{
	Closed = false;
}

void NCStatesHist::SetAddCoords(const SMachState &MachState)
{
	for (int i = 0; i < MAX_STATES_NUM; ++i)
	{
		States[i]->AddCoords = MachState;
		AddStates[i]->AddCoords = MachState;
		States[i]->AddCoordsFlags = MachState;
		AddStates[i]->AddCoordsFlags = MachState;
	}
}
