#include "stdafx.h"
#include "NStockStl.h"
#include "GQuadRenderDX5X.h"
#include "GQuadRenderRot.h"
#include "NToolCombined.h"
#include "NContourOfTool.h"
#include "NToolTurnBladeMill.h"
#include "NToolMill.h"
#include "MTMashineTool.h"
#include "NMachUnitPair.h"
#include "NCUnit.h"
#include "MTPart.h"
#include "NOMScaner.h"
#include "BObjectManager.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "GQuadRenderMill.h"
#include "NToolTurnHolderSTL.h"
#include "NToolTurnHolderBar.h"
#include "NToolTurnHolderCyl.h"
#include "NCMDoc.h"
#include "GDynamicCadr.h"
#include "MTConfig.h"
#include "NCollManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCollManager::NCollManager(CNCMDoc* pD) :
	pGDoc(pD),
	pCollisionScene(nullptr),
	IgnoreMargins(false),
	MarginMach(0.),
	MarginPart(0.),
	MarginFixture(0.),
	MarginTool(0.),
	MarginStock(0.),
	CheckStep(2.),
	GroupMask(OG_MACHINE | OG_TOOLS | OG_FIXTURES | OG_STOCKS),
	pMainMach(nullptr),
	StopOnColl(false),
	IgnoreWrongAxis(false)
{
}

NCollManager::~NCollManager(void)
{
	Clear();
	C_OBJECT_MANAGER((*this))->DeleteActiveObject(this);
}

void NCollManager::Clear(void)
{
	delete pCollisionScene;
	pCollisionScene = NULL;
}

bool NCollManager::IsEnabled()
{
	return (pCollisionScene != NULL);
}

NCollisionScene* NCollManager::GetScene(void) const
{
	return pCollisionScene;
}

int NCollManager::MakeCollisionScene()
{
	// Add machine
	if (GroupMask & OG_MACHINE)
	{
		for (NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
		{
			if (Sc.GetPassiveGroup() != OG_MACHINE)
				continue;
			MTPart* pPart = (MTPart*)Sc.GetPassiveObject();
			int AxInd = pMainMach->GetCurConfig().GetAxisIndex(pPart->GetAxisName());
			if (AxInd < 0)
				continue;// internal error
			NCollisionObjectID ID;
			ID.SetGroup(OG_MACHINE);
			ID.SetID(AxInd);
			ID.SetBehav(CalcBehav(ID.GetID(), false, false));
			ID.SetCollidableObj(Sc.GetPassiveObject());
			size_t SRVersion = 0;
			const float* TriPtr = pPart->GetTriangles(pMainMach->GetCurConfig(), SRVersion);
			ID.SetSpinRendVersion(SRVersion);
			double Marg = (fabs(pPart->GetMargin()) < 1.e-6) ? MarginMach : pPart->GetMargin();
			if (IgnoreMargins)
				Marg = 0.;
			pCollisionScene->AddStlObject(ID, TriPtr, sizeof(float) * 6, -1, float(Marg), false, false);
		}
	}
	// Add parts and fixtures
	for (NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
	{
		PObjectGroup Group = Sc.GetPassiveGroup();
		if (!(Group & GroupMask))
			continue;
		if (Group != OG_PARTS && Group != OG_FIXTURES)
			continue;
		NStock* pStock = static_cast<NStock*>(Sc.GetPassiveObject());
		NCollisionObjectID ID;
		ID.SetGroup(Group);
		ID.SetID(pMainMach->GetStockMovedAxisInd(0));
		ID.SetBehav(CalcBehav(ID.GetID(), false, false));
		ID.SetCollidableObj(pStock);
		pCollisionScene->AddStlObject(ID,
			((NStockStl*)pStock)->GetTriangles(), sizeof(float) * 6, -1,
			IgnoreMargins ? 0.f : float((Group == OG_PARTS) ? MarginPart : MarginFixture), false, false);
	}
	// Add tools in place positions
	MakeToolsInPlacePos();
	// Add tools
	MakeToolsInActPos();

	return 0;
}

int NCollManager::MakeToolsInPlacePos(void)
{
	if (GroupMask & OG_TOOLS)
	{
		// Add tools
		for (NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
		{
			PObjectGroup Group = Sc.GetPassiveGroup();
			if (Group != OG_TOOLS)
				continue;
			NToolCombined* pTool = (NToolCombined*)Sc.GetPassiveObject();
			int AxisIndex = pMainMach->GetCurConfig().GetAxisIndex(MTConfig::GetToolPlaceAxisName(pTool->Position, pTool->Turret, pTool->Attach));
			if (AxisIndex >= 0)
				MakeTool(AxisIndex, pTool, false);
		}
	}
	return 0;
}

int NCollManager::MakeToolsInActPos(void)
{
	if (GroupMask & OG_TOOLS)
	{
		// Add tools
		for (NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
		{
			PObjectGroup Group = Sc.GetPassiveGroup();
			if (Group != OG_TOOLS)
				continue;
			NToolCombined* pTool = (NToolCombined*)Sc.GetPassiveObject();
			int AxisIndex = pMainMach->GetCurConfig().GetAxisIndex(MTConfig::GetToolAxisName(pTool->Turret, pTool->Attach));
			if (AxisIndex >= 0)
				MakeTool(AxisIndex, pTool, true);
		}
	}
	return 0;
}

void NCollManager::MakeTool(int AxisIndex, NToolCombined* pTool, bool Active, bool ProbeOnly /*= false*/)
{
	if (AxisIndex < 0)
		return;
	std::vector<float*> PassiveTri;
	std::vector<float*> ActiveTri;
	NCollisionObjectID ID;
	ID.SetGroup(OG_TOOLS);
	ID.SetID(AxisIndex);
	ID.SetCollidableObj(pTool);
	const NToolCombined* pOffTool = pTool;
	if (Active && !IgnoreMargins)
		pOffTool = pTool->CreateOffsetTool(true);
	for (int ToolI = 0; ToolI < pOffTool->num_tools; ++ToolI)
	{
		NTool* pToolI = pOffTool->GetTool(ToolI);
		if (((GroupMask & OG_HOLD) == 0 && !pToolI->IsCutting()) && (ProbeOnly && !pToolI->IsProbe()))
			continue;
		const bool IsTurnMillTool = (pToolI->GetType() > START_TURNBLADEMILL && pToolI->GetType() <= END_TURNBLADEMILL);
		bool MakeGabarObject = true;
		if (pOffTool->GetClass() == AT_MILL || IsTurnMillTool)
		{//Mill tool
			NContourOfTool Contour;
			NTool* pToolMill = IsTurnMillTool ? ((NToolTurnBladeMill*)pToolI)->GetMillTool() : pToolI;
			NTool::CreateSimGeneratrix(pToolMill, Contour);
			if (Contour.num > 0)
			{// Make solid of revolution
				MakeGabarObject = false;
				MeshArr RMeshArr;
				Contour.MakeToolMeshs(RMeshArr, BMatr(), pToolI->IsCutting(), pToolI->IsShank());
				for (int i = 0; i < RMeshArr.GetSize(); ++i)
				{
					NRectMesh* pMesh = static_cast<NRectMesh*>(RMeshArr.GetAt(i));
					if (!pMesh)
						continue;
					float* ToolTriangles = nullptr;
					if (IsTurnMillTool)
						ToolTriangles = pMesh->MakeTriangles(&BMatr().RotY(0., 0., 0., 90.));
					else
						ToolTriangles = pMesh->MakeTriangles();
					const bool ProcAsCuttingActive = Active && pToolI->IsCutting();
					if (ProcAsCuttingActive)
						ActiveTri.push_back(ToolTriangles);
					else
						PassiveTri.push_back(ToolTriangles);
					delete pMesh;
				}
			}
		}
		else
		{
			switch (pToolI->GetType())
			{
			case 62: // turn holder STL
			{
				NToolTurnHolderSTL* pToolTHSTL = dynamic_cast<NToolTurnHolderSTL*>(pToolI);
				if (pToolTHSTL != nullptr)
				{
					MakeGabarObject = false;
					const auto pTriangles = pToolTHSTL->GetTriangles();
					if (pTriangles == nullptr)
						break;
					int NTri = int(pTriangles[0]);
					if (NTri <= 0)
						break;
					GLfloat* pNewTriangles = new GLfloat[9 * NTri];
					BMatr M(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.));
					for (int TriInd = 0; TriInd < NTri; ++TriInd)
					{
						const int j = TriInd * 18 + 1;
						const int l = TriInd * 9;
						for (int k = 0; k < 18; k += 6)
						{
							int i = j + k;
							int m = l + k / 2;
							BPoint Pc = (BPoint(pTriangles[i + 3], pTriangles[i + 4], pTriangles[i + 5], 1.)) * M;
							pNewTriangles[m + 0] = GLfloat(Pc.GetX());
							pNewTriangles[m + 1] = GLfloat(Pc.GetY());
							pNewTriangles[m + 2] = GLfloat(Pc.GetZ());
						}
					}
					ID.SetBehav(CalcBehav(ID.GetID(), false, false));
					pCollisionScene->AddStlObject(ID, pNewTriangles - 4, 0, NTri, IgnoreMargins ? 0.f : (Active ? float(NTool::GetTurnOffset()) : float(MarginTool)), true, Active);
				}
			}
			break;
			case 60: // turn holder bar
			{
				NToolTurnHolderBar* pHold = dynamic_cast<NToolTurnHolderBar*>(pToolI);
				if (pHold != nullptr)
				{
					const auto pTriangles = pHold->GetCompTriangles();
					if (pTriangles != nullptr)
						PassiveTri.push_back(pTriangles);
					MakeGabarObject = false;
				}
			}
			case 61: // turn holder cyl
			{
				NToolTurnHolderCyl* pHold = dynamic_cast<NToolTurnHolderCyl*>(pToolI);
				if (pHold != nullptr)
				{
					const auto pTriangles = pHold->GetCompTriangles();
					if (pTriangles != nullptr)
						PassiveTri.push_back(pTriangles);
					MakeGabarObject = false;
				}
			}
			break;
			default:
			{
				// Turn tool
				NToolTurnBlade* pBlade = dynamic_cast<NToolTurnBlade*>(pToolI);
				if (pBlade != nullptr)
				{
					MeshArr RMeshArr;
					pBlade->MakeBladeMesh(RMeshArr, pBlade->IsCutting(), false);
					const bool ProcAsCuttingActive = Active;
					for (int i = 0; i < RMeshArr.GetSize(); ++i)
					{
						NRectMesh* pMesh = static_cast<NRectMesh*>(RMeshArr.GetAt(i));
						if (!pMesh)
							continue;
						float* ToolTriangles = ToolTriangles = pMesh->MakeTriangles();
						if (ProcAsCuttingActive)
							ActiveTri.push_back(ToolTriangles);
						else
							PassiveTri.push_back(ToolTriangles);
						delete pMesh;
					}
					MakeGabarObject = false;
				}
			}
			break;
			}
		}
		if (MakeGabarObject)
		{
			float* ToolTriangles = pToolI->GetGabar().MakeTriangles();
			const bool ProcAsCuttingActive = Active && pToolI->IsCutting();
			if (ProcAsCuttingActive)
				ActiveTri.push_back(ToolTriangles);
			else
				PassiveTri.push_back(ToolTriangles);
		}
	}
	// make passive tool as single object (for all passive tools and for passive part of active tool
	ID.SetBehav(CalcBehav(ID.GetID(), false, false));
	pCollisionScene->AddToolObject(ID, PassiveTri, IgnoreMargins ? 0.f : (Active ? 0.f : float(MarginTool)), Active);
	for each (auto p in PassiveTri)
		delete[] p;
	if (Active)
	{
		// make cutting part of active tool as single object
		ID.SetBehav(CalcBehav(ID.GetID(), false, true));
		pCollisionScene->AddToolObject(ID, ActiveTri, 0.f, true);
		for each (auto p in ActiveTri)
			delete[] p;
	}
}

void NCollManager::Serialize(CArchive& ar)
{
	SerializeElements(ar, &MarginMach, 1);
	SerializeElements(ar, &MarginPart, 1);
	SerializeElements(ar, &MarginFixture, 1);
	SerializeElements(ar, &MarginTool, 1);
	SerializeElements(ar, &MarginStock, 1);
	// special case for version 5.3 only (IgnoreWrongAxis is coded in the sign of CheckStep
	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() == atof("5.3"))
	{
		SerializeElements(ar, &CheckStep, 1);
		if (CheckStep < 0)
		{
			CheckStep = -CheckStep;
			IgnoreWrongAxis = true;
		}
	}
	else
	{
		SerializeElements(ar, &CheckStep, 1);
		SerializeElements(ar, &IgnoreWrongAxis, 1);
	}
	SerializeElements(ar, &GroupMask, 1);
	BOOL S = StopOnColl;
	SerializeElements(ar, &S, 1);
	StopOnColl = S;
	S = false;
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("2025")))// TO DELETE
		SerializeElements(ar, &S, 1);
}

bool NCollManager::ChangeTool(class NCollisionScene* pCollisionScene, const class MTMachineTool& MT, const class NToolCombined* pTool)
{
	if (!pCollisionScene)
		return true;
	int ToolTur = 0;
	int ToolAtt = 0;
	if (pTool)
	{
		ToolTur = pTool->Turret;
		ToolAtt = pTool->Attach;
	}

	auto AxisIndex = MT.GetCurConfig().GetAxisIndex(MTConfig::GetToolAxisName(ToolTur, ToolAtt));

	return pCollisionScene->ChangeTool(static_cast<const NCollidableObject*>(pTool), AxisIndex);// pTool and static_cast<const NCollidableObject*>(pTool) are not equal
}

bool NCollManager::ChangeActiveToolResp(NCollisionScene* pCollisionScene, bool IsFast5X, bool Restore)
{
	pCollisionScene->ChangeActiveToolResp(IsFast5X, Restore);
	return false;
}

bool NCollManager::SetCurMatrices(NCollisionScene* pCollisionScene, MTMachineTool* pMach, const NToolCombined* pActiveTool)
{
	auto StockAxisInd0 = pMach->GetStockMovedAxisInd(0);
	BMatrPair StockMatr = pMach->GetStockMovedMatr(0, true);// Do it here to ensure MakeActual called
	const NCollisionObjectID* pFirstID = pCollisionScene->GetObjID(0);
	if (!pFirstID)
		return false;
	int k = 0;
	if (pFirstID->GetGroup() == OG_MACHINE)
	{
		const MTPartArray& Parts = pMach->GetParts();
		// Machine parts must be first
		for (int i = 0; i < Parts.GetSize(); ++i)
		{
			const float* Triangles = nullptr;
			size_t SRVersion = 0;
			BMatr CollMatr = Parts[i]->GetCollMatr(pMach->GetCurConfig(), Triangles, SRVersion);
			if (Triangles != nullptr)
			{// part can be spinning
				pCollisionScene->ReplaceStlObjectIfChanged(i, Triangles, SRVersion);
			}
			pCollisionScene->SetMatrix(i, CollMatr * StockMatr.Inv());
		}
		k += Parts.GetSize();
	}
	BMatr ToolMatr = pMach->GetToolMatr(
		(pActiveTool == NULL) ? 0 : pActiveTool->Turret,
		(pActiveTool == NULL) ? 0 : pActiveTool->Attach,
		true);
	ToolMatr = ToolMatr * StockMatr.Inv();
	const NCollisionObjectID* pID = pCollisionScene->GetObjID(k);
	int PrevPos = -1;
	int PrevTur = -1;
	int PrevAtt = -1;
	BMatr PlaceMatr;
	bool PassiveToolFound = true;
	for (++k; pID; ++k)
	{
		/*		if (pID->GetGroup() == OG_PARTS || pID->GetGroup() == OG_FIXTURES)
					pCollisionScene->SetMatrix(k - 1, StockMatr);
				else*/ if (pID->GetGroup() == OG_STOCKS)
				{
					if (StockAxisInd0 != pID->GetID()) // if stock matrix != stock 0 matrix
						pCollisionScene->SetMatrix(k - 1, pMach->GetCurConfig()[pID->GetID()]->GetMatr() * StockMatr.Inv());
				}
				else if (pID->GetGroup() == OG_TOOLS)
				{
					bool IsActiveID = (pCollisionScene->GetActiveToolID() == pID->GetID());
					const NToolCombined* pPassTool = static_cast<NToolCombined*>(pID->GetCollidableObj());
					if (pActiveTool == pPassTool)
					{
						if (IsActiveID)
							pCollisionScene->SetMatrix(k - 1, ToolMatr);
					}
					else
					{
						if (!IsActiveID)
						{
							if (pPassTool != nullptr)
							{
								if (!(pPassTool->Position == PrevPos && pPassTool->Turret == PrevTur && pPassTool->Attach == PrevAtt))
								{// New matrix needed
									PlaceMatr = pPassTool->GetCompMatr()
										* pMach->GetToolPlaceMatr(pPassTool->Position, pPassTool->Turret, pPassTool->Attach, &PassiveToolFound)
										* StockMatr.Inv();
									PrevPos = pPassTool->Position;
									PrevTur = pPassTool->Turret;
									PrevAtt = pPassTool->Attach;
								}
							}
							else
								PassiveToolFound = false;

							if (PassiveToolFound)
								pCollisionScene->SetMatrix(k - 1, PlaceMatr);
						}
					}
				}
				pID = pCollisionScene->GetObjID(k);
	}
	return true;
}

int NCollManager::MakeStocks()
{
	if (!(GroupMask & OG_STOCKS))
		return 0;
	auto pDynCadr = pGDoc->GetChannel(0)->GetDynamicCadr();
	GBodyRender* pStart = nullptr;
	GBodyRender* pEnd = nullptr;
	if (pDynCadr == nullptr)
	{
		pStart = pGDoc->NGetStockList()->MakeVisibleRendersList();
	}
	else
	{
		pStart = pDynCadr->GetStart();
		pEnd = pDynCadr->GetEnd();
		if (pStart == nullptr && pEnd == nullptr)
			pStart = pGDoc->NGetStockList()->MakeVisibleRendersList();
	}

	if (pEnd == nullptr)
	{
		for (GBodyRender* pRender = pStart; pRender != nullptr; pRender = pRender->GetNextRender())
		{
			pRender->SetCollided(false);
			const int AxInd = pMainMach->GetStockMovedAxisIndSGr(pRender->GetStockPos());
			if (AxInd < 0)
				continue;// internal error
			NCollisionObjectID ID;
			ID.SetGroup(OG_STOCKS);
			ID.SetID(AxInd);
			ID.SetBehav(CalcBehav(ID.GetID(), true, false));
			ID.SetSpinRendVersion(pRender->GetHASH(nullptr));
			ID.SetCollidableObj(pRender);
			MakeStock(ID, pRender, nullptr, false);
		}
	}
	else
	{
		auto pRenderEnd = pEnd;
		for (GBodyRender* pRender = pStart; pRender != nullptr; pRender = pRender->GetNextRender())
		{
			pRender->SetCollided(false);
			const int AxInd = pMainMach->GetStockMovedAxisIndSGr(pRender->GetStockPos());
			if (AxInd < 0)
				continue;// internal error
			NCollisionObjectID ID;
			ID.SetGroup(OG_STOCKS);
			ID.SetID(AxInd);
			ID.SetBehav(CalcBehav(ID.GetID(), true, false));
			ID.SetSpinRendVersion(pRender->GetHASH(pRenderEnd));
			ID.SetCollidableObj(pRender);
			MakeStock(ID, pRender, pRenderEnd, false);
			if (pRenderEnd != nullptr)
				pRenderEnd = pRenderEnd->GetNextRender();
		}
	}
	return 0;
}

int NCollManager::ReplaceStocks(GBodyRender* pStart, GBodyRender* pEnd)
{
	if (!(GroupMask & OG_STOCKS))
		return 0;

	int CurObj = 0;
	auto pRenderEnd = pEnd;
	for (GBodyRender* pRender = pStart; pRender != nullptr; pRender = pRender->GetNextRender())
	{
		pRender->SetCollided(false);
		const int AxInd = pMainMach->GetStockMovedAxisIndSGr(pRender->GetStockPos());
		if (AxInd < 0)
			continue;// internal error
		// find this stock
		NCollisionObjectID* pID = pCollisionScene->GetObjID(CurObj);
		for (++CurObj; pID; ++CurObj)
		{
			if (pID->GetGroup() == OG_STOCKS && pID->GetID() == AxInd)
				break;
			pID = pCollisionScene->GetObjID(CurObj);
		}
		if (pID != nullptr)
		{
			auto HASH = pRender->GetHASH(pRenderEnd);
			if (pID->GetSpinRendVersion() != HASH)
			{
				pID->SetSpinRendVersion(HASH);
				pID->SetCollidableObj(pRender);
				MakeStock(*pID, pRender, pRenderEnd, true);
			}
		}
		if (pRenderEnd != nullptr)
			pRenderEnd = pRenderEnd->GetNextRender();
	}
	return 0;
}

int NCollManager::MakeStock(const NCollisionObjectID& ID, const GBodyRender* pStart, const GBodyRender* pEnd, bool Replace)
{
	if (pStart == nullptr)
		return -1;
	const GQuadRenderMill* pRenderMill = dynamic_cast<const GQuadRenderMill*>(pStart);
	if (pRenderMill != nullptr)
	{
		const GQuadRenderMill* pRenderMillEnd = dynamic_cast<const GQuadRenderMill*>(pEnd);
		auto Array = pRenderMill->GetArray();
		int WholeTriNum = 0;
		for (size_t ix = 0; ix < pRenderMill->GetColsNum(); ++ix)
			for (size_t iy = 0; iy < pRenderMill->GetRowsNum(); ++iy)
			{
				float* pCEnd = (pRenderMillEnd == nullptr) ? (float*)QR_EMPTY : pRenderMillEnd->GetArray()[ix][iy];
				float* pC = pRenderMill->NotDefined(pCEnd) ? Array[ix][iy] : pCEnd;
				if (pRenderMill->Empty(pC))
					continue;
				WholeTriNum += int(pC[0]);
			}
		if (WholeTriNum <= 0)
			return -2;
		int WholeSize = 3 * 3 * WholeTriNum;
		GLfloat* buf = new GLfloat[WholeSize];
		GLfloat* Dest = buf;
		for (size_t ix = 0; ix < pRenderMill->GetColsNum(); ++ix)
			for (size_t iy = 0; iy < pRenderMill->GetRowsNum(); ++iy)
			{
				float* pCEnd = (pRenderMillEnd == nullptr) ? (float*)QR_EMPTY : pRenderMillEnd->GetArray()[ix][iy];
				float* pC = pRenderMill->NotDefined(pCEnd) ? Array[ix][iy] : pCEnd;
				if (pRenderMill->Empty(pC))
					continue;
				int LocSize = int(pC[0]) * 3;
				pC += 4;
				for (int i = 0; i < LocSize; ++i)
				{
					memcpy_s(buf, WholeSize * sizeof(buf[0]), pC, 3 * sizeof(pC[0]));
					pC += 7;
					buf += 3;
					WholeSize -= 3;
				}
			}
		if (Replace)
			pCollisionScene->ReplaceStlObject(ID, Dest - 4, 0, WholeTriNum, IgnoreMargins ? 0.f : float(MarginStock), true);
		else
			pCollisionScene->AddStlObject(ID, Dest - 4, 0, WholeTriNum, IgnoreMargins ? 0.f : float(MarginStock), true);
	}
	else
	{
		const GQuadRenderDX5X* pRenderDX = dynamic_cast<const GQuadRenderDX5X*>(pStart);
		if (pRenderDX != nullptr)
		{
			const NCollisionObjectID& NID = Replace
				? pCollisionScene->GetComplexObject(ID)
				: pCollisionScene->StartComplexObject(ID, pRenderDX->GetColsNum(), pRenderDX->GetRowsNum(), pRenderDX->GetLevelsNum());
			constexpr int IndStep = NCollisionObjectID::IndStep;
			for (int ix = 0; ix < pRenderDX->GetColsNum(); ix += IndStep)
				for (int iy = 0; iy < pRenderDX->GetRowsNum(); iy += IndStep)
					for (int iz = 0; iz < pRenderDX->GetLevelsNum(); iz += IndStep)
					{
						RTriRandMesh* Meshs[IndStep * IndStep * IndStep];
						for (int kx = 0; kx < IndStep; ++kx)
							for (int ky = 0; ky < IndStep; ++ky)
								for (int kz = 0; kz < IndStep; ++kz)
								{
									RTriRandMesh* pMesh = pRenderDX->GetBigCellMesh(ix + kx, iy + ky, iz + kz);
									Meshs[kx * IndStep * IndStep + ky * IndStep + kz] = pMesh;
								}
						pCollisionScene->ProcCell(NID, ix, iy, iz, Meshs, IgnoreMargins ? 0.f : float(MarginStock));
					}
		}
		else
		{
			const GQuadRenderRot* pRenderRot = dynamic_cast<const GQuadRenderRot*>(pStart);
			if (pRenderRot != nullptr)
			{
				int TriNum;
				const float* Triangles = pRenderRot->MakeSTLArray(TriNum);
				if (Replace)
					pCollisionScene->ReplaceStlObject(ID, Triangles - 4, 0, TriNum, IgnoreMargins ? 0.f : float(MarginStock), true);
				else
					pCollisionScene->AddStlObject(ID, Triangles - 4, 0, TriNum, IgnoreMargins ? 0.f : float(MarginStock), true);
			}
		}
	}
	return 0;
}

int NCollManager::CalcBehav(int AxisInd, bool Stock, bool ActTool)
{
	//int CurNearestMoveIndex = 0;
	//const MTAxis* pCurAxis = pMainMach->GetNearestMoveRotAxis(pMainMach->GetCurConfig()[AxisInd]);
	//if(pCurAxis != nullptr)
	//	CurNearestMoveIndex = 1 + pMainMach->GetCurConfig().GetAxisIndex(pCurAxis->GetName());// 1 added to allow index 0 for machine tool base
	return NCollisionObjectID::MakeBehav(AxisInd + 1, Stock, ActTool);// 1 added to allow index 0 for machine tool base
}

