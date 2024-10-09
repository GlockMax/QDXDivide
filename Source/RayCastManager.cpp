#include "stdafx.h"
#include "NCMDoc.h"
#include "RayCastManager.h"

RayCastManager::RayCastManager()
{
	StPos = 0;
}


void RayCastManager::InitializeProg(CNCMDoc* pDoc, int iStPos)
{
	StPos = iStPos;
	octreeProg.Initialize(pDoc, StPos);
}

RPickID RayCastManager::PickRCM(const BPoint& irayPoint, const BPoint& irayVector, double Epsilon, double MaxL2, CNCMDoc *pDoc, const bool& StopPick)
{
	BPoint rayPoint = irayPoint;
	BPoint rayVector = irayVector;
	if (StPos != 0)
	{
		MTMachineTool& Mach = pDoc->GetMachTool();
		BMatr M = Mach.GetStockMatr(0, true) * Mach.GetStockMatr(StPos, true).invr();
		rayPoint = rayPoint * M;
		rayVector = rayVector * M;

	}
	RPickID id;
	id.SetStPos(StPos);
	BPoint res;
	cadrID cadrId = octreeProg.RayCast(rayPoint, rayVector, Epsilon, res, MaxL2, StopPick, StPos);
	if (!cadrId.IsEmpty())
	{
		cadrId.Cadr /= 2;
		unsigned data[3];
		data[0] = OG_PROGRAMS;
		data[1] = cadrId.Prog;
		data[2] = cadrId.Cadr;
		id.Set(3, data);
		return id;
	}
	if (StopPick)
		return id;
	octreeSkel.Initialize(pDoc, StPos);

	cadrId = octreeSkel.RayCast(rayPoint, rayVector, Epsilon, res, MaxL2, StopPick, StPos);
	if (!cadrId.IsEmpty())
	{
		unsigned data[4];
		data[0] = OG_STOCKS;
		data[1] = cadrId.Prog;
		data[2] = cadrId.Cadr % 4;
		data[3] = cadrId.Cadr / 4;
		id.Set(4, data);
		return id;
	}
	return id;
}

RayCastManager::~RayCastManager()
{
}

void RayCastManPack::Initialize(CNCMDoc* pDoc)
{
	StopPick = false;
	const auto& posSet = pDoc->GetMachTool().GetStPosSet();
	Mans.clear();
	Mans.resize(posSet.size());
	int i = 0;
	for each (auto pos in posSet)
	{
		Mans.at(i++).InitializeProg(pDoc, pos);
	}
}

RPickID RayCastManPack::PickRCM(const BPoint& rayPoint, const BPoint& rayVector, double Epsilon, double MaxL2, CNCMDoc* pDoc)
{
	ResetStopPick();
	for (int i = 0; i < Mans.size(); ++i)
	{
		RPickID pickID = Mans.at(i).PickRCM(rayPoint, rayVector, Epsilon, MaxL2, pDoc, StopPick);
		if(StopPick)
			return pickID;
		if (pickID.GetSize() > 0)
			return pickID;
	}
	return RPickID();
}

RPickID RayCastManPack::PickRCMSync(const BPoint& rayPoint, const BPoint& rayVector, double Epsilon, double MaxL2, CNCMDoc* pDoc)
{
	ResetStopPick();
	for (int i = 0; i < Mans.size(); ++i)
	{
		RPickID pickID = Mans.at(i).PickRCM(rayPoint, rayVector, Epsilon, MaxL2, pDoc, StopPick);
		if (StopPick)
			return pickID;
		if (pickID.GetSize() > 0)
			return pickID;
	}

	BPoint res;
	IDost Id = octreeSkelTool.RayCast(rayPoint, rayVector, Epsilon, res, MaxL2, StopPick, 0);
	if (!Id.IsEmpty())
	{
		unsigned data[4];
		data[0] = OG_TOOLS;
		data[1] = Id.GetToolInd();
		data[2] = Id.GetCadrInd();
		data[3] = Id.GetActive() ? 1 : 0;
		RPickID id;
		id.SetStPos(0);
		id.Set(4, data);
		return id;
	}
	return RPickID();
}

void RayCastManPack::InitializeTools(CNCMDoc* pDoc, bool StockView)
{
	octreeSkelTool.Initialize(pDoc, StockView);
}

