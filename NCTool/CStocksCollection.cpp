#include "stdafx.h"
//#include "NCEnums.h"
#include "PropList.h"
#include "GQuadRenderDX5X.h"
#include "NStockBSP.h"
#include "ConstDef.h"
#include "NStockStl.h"
#include "NStock.h"
#include "NStockCont.h"
#include "NStockSweep.h"
#include "GQuadRenderRot.h"
#include "BaseRender.h"
#include "NTool.h"
#include "MTMashineTool.h"
#include "CStocksCollection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CStocksCollection::CStocksCollection(void) : CMapStringToNStock()
{
	WFrameAllStocks = true;
	DrawSharpEdgesDX = false;
	StocksColor.SetRGB(RGB(0,0,255));
	Collided = false;
}


CStocksCollection::~CStocksCollection(void)
{
}

void CStocksCollection::SetAlpha(double Alpha)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	StocksColor.A() = (float)Alpha;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetAlpha(Alpha);
	}
}

bool CStocksCollection::NeedDraw(enum RenderMode GlobRMode, enum RenderMode ExtRMode)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if(!Visible)
		return false;
	RenderMode ActRMode = (ExtRMode == RM_UNDEF) ? GetRMode() : ExtRMode;

	if(GlobRMode == RM_TRANSLUCENT && ActRMode != RM_TRANSLUCENT) 
		return false;
	if (GlobRMode != RM_TRANSLUCENT && ActRMode == RM_TRANSLUCENT)
		return false;
	if(GlobRMode == RM_TRANSLUCENT)
		SetAlpha(Transparency);
	BaseRender::SetShader(ActRMode);
	return true;
}

void CStocksCollection::SetCollided(bool Val)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	Collided = Val;
	for (INT_PTR i = 0; i < GetSize(); ++i)
	{
		CString key;
		NStock* pStock = nullptr;
		GetAt(i, key, (CObject*&)pStock);
		if (pStock != nullptr)
			pStock->SetCollided(Val);
	}
}

void CStocksCollection::DrawStocks(RenderMode GlobRMode, int Type, BOOL CuttingMode, bool DrawSurf, MTMachineTool& MT)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	ResetRendersLists();
	glPushName(OG_STOCKS);
	for (INT_PTR i = 0; i < GetSize(); ++i)
	{
		CString key;
		NStock* pStock = nullptr;
		GetAt(i, key, (CObject*&)pStock);
		if (pStock == nullptr)
			continue;
		if (!pStock->IsVisible())
			continue;

		bool wFrameBackp = false;
		if (WFrameAllStocks) // Apply global wFrame value
		{
			wFrameBackp = (pStock->GetWFrame() == TRUE);
			pStock->SetWFrame(WFrameAllStocks);
		}
		const BStockGrID StPos = pStock->GetStockGr();
		glPushMatrix();
		BMatr M = MT.GetStockMovedMatrSGr(StPos, true) * MT.GetStockMovedMatr(0, true).invr();
		glMultMatrixd(M.GetArray());

		glPushName(GLuint(i));
		DrawStock(GlobRMode, pStock, Type, CuttingMode, DrawSurf);
		glPopName();

		if (IsSelectedKey(key)) // Selected stocks bound box drawing
		{
			if (GlobRMode != RM_TRANSLUCENT)
			{
				glColor3fv(pStock->GetColor().array);
				pStock->DrawSelected();
			}
		}

		glPopMatrix();
		if (WFrameAllStocks) // Recover pStock wFrame value
			pStock->SetWFrame(wFrameBackp);
	}
	glPopName();// OG_STOCKS
	CreateMixedRendersList();
}

void CStocksCollection::DrawStocksOld(RenderMode GlobRMode, int Type, BOOL CuttingMode, bool DrawSurf)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	bool wFrameBackp;
	ResetRendersLists();
	glPushName(OG_STOCKS);
	for (INT_PTR i = 0; i < GetSize(); ++i)
	{
		CString key;
		NStock* pStock = nullptr;
		GetAt(i, key, (CObject*&)pStock);
		if (pStock == nullptr)
			continue;
		if (!pStock->IsVisible())
			continue;

		if (WFrameAllStocks) // Apply global wFrame value
		{
			wFrameBackp = (pStock->GetWFrame() == TRUE);
			pStock->SetWFrame(WFrameAllStocks);
		}
		glPushName(GLuint(i));
		DrawStock(GlobRMode, pStock, Type, CuttingMode, DrawSurf);
		glPopName();

		if (IsSelectedKey(key)) // Selected stocks bound box drawing
		{
			if (GlobRMode != RM_TRANSLUCENT)
			{
				glColor3fv(pStock->GetColor().array);
				pStock->DrawSelected();
			}
		}
		if (WFrameAllStocks) // Recover pStock wFrame value
			pStock->SetWFrame(wFrameBackp);
	}
	glPopName();// OG_STOCKS
	CreateMixedRendersList();
}

unsigned int CStocksCollection::GetUuid()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (!NCMComData::IsTurnEnabled())
		return 0;
	unsigned int Res = 0;
	for (int i = 0; i < GetSize(); ++i)
	{
		NStock *pStock = GetStock(i);
		if (pStock == nullptr)
			continue;
		Res += pStock->GetTurnSkeleton().GetUuid();
	}
	return Res;
}

void CStocksCollection::DrawStock(RenderMode GlobRMode, NStock *pStock, int Type, BOOL CuttingMode, bool DrawSurf)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if(BDebugState::Get().DrawFaces)
	{
		if(pStock->WFrame)
			BaseRender::SetWFMode();
		((MQuadTree *)pStock->GetBody())->DrawFaces();

		if(pStock->WFrame)
			BaseRender::RemoveWFMode();
		return;
	}

	if(CuttingMode)
	{
		glColor4fv(pStock->GetColor().array);
		if(pStock->NeedDraw(GlobRMode, pStock->GetRMode()))
			pStock->Render(GlobRMode, Type, DrawSurf);
	}
	else
	{
		BOOL BSharpEdgesS = pStock->SharpEdgesS;
		BOOL BSharpEdgesDX = pStock->SharpEdgesDX;
		if (!(Type & 4))
		{
			pStock->SharpEdgesS = FALSE;
			pStock->SharpEdgesDX = FALSE;
		}

		pStock->DrawOriginal(GlobRMode, pStock->GetRMode(), DrawSurf);
		pStock->SharpEdgesS = BSharpEdgesS;
		pStock->SharpEdgesDX = BSharpEdgesDX;
	}
}

void CStocksCollection::CreateStocksRender()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->CreateRender();
	}
	CreateMixedRendersList();
}

void CStocksCollection::CreateModifStocksRender()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->CreateModifRender();
	}
	CreateMixedRendersList();
}

void CStocksCollection::ChangeVisibleRendersToNULL(bool MakeCopy)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (pS->IsVisible() && !pS->IsFrozen())
		{
			if (MakeCopy)
				pS->MakeRenderCopy();
			else
				pS->NullRender();
		}
	}
	if (MakeCopy)
		CreateMixedRendersList();
}

void CStocksCollection::AddStock(const CString& name, int& ind, NStock* pStock)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (pStock == nullptr)
		return;
	if (pStock->IsTemporary())
		SetFrozen(true);
	CString TrunName = name.Right(name.GetLength() - CMapStringToNStock::PrefixStock.GetLength());
	MakeName(TrunName);
	if (ind == -1)
	{
		Order.push_back(TrunName);
		ind = int(Order.size()) - 1;
	}
	else
		Order.insert(Order.begin() + ind, TrunName);
	if (!TrunName.IsEmpty())
	{
		pStock->SetGroup(OG_STOCKS);
//		pStock->Transform(matr);
		pStock->SetColor(GetColor());
		pStock->SetEdgesS(IsSharpEdges());
		pStock->SetEdgesDX(IsSharpEdgesDX());
		pStock->SetRMode(GetRMode());
		pStock->Reset();
		std::map<CString, CObject*>::insert(std::make_pair(TrunName, pStock));// to prevent addition new element to Order
		ReCreateLists();
	}
}

CString CStocksCollection::DeleteStockByInd(INT_PTR index)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	CString key;
	if (index < GetSize() && index >= 0)
	{
		CObject* stock = nullptr;
		GetAt(index, key, stock);

		ResetBodiesAndRendersLists();
		((NStock*)stock)->Deleted();
		RemoveKey(key);
		CreateBodiesAndRendersLists();
		if (GetSize() == 1)
			SyncProperties();
		if (((NStock*)stock)->IsTemporary())
			SetFrozen(false);
	}
	return key;
}


void CStocksCollection::DeleteStocks()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	ResetBodiesAndRendersLists();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		delete pS;
	}
	RemoveAll();
}

void CStocksCollection::CreateBodiesAndRendersLists()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	bool mill = false;
	bool turn = false;
	switch(NCMComData::GetType())
	{
		case AT_MILL:
			mill = true;
			break;
		case AT_TURN:
			turn = true;
			break;
		case AT_MILL_TURN:
			mill = true;
			turn = true;
			break;
	}
	if (mill)
	{
		CreateBodiesList(AT_MILL);
	}
	if (turn)
	{
		CreateBodiesList(AT_TURN);
	}
	CreateMixedRendersList();
}

void CStocksCollection::ResetBodiesAndRendersLists()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	bool mill = false;
	bool turn = false;
	switch(NCMComData::GetType())
	{
		case AT_MILL:
			mill = true;
			break;
		case AT_TURN:
			turn = true;
			break;
		case AT_MILL_TURN:
			mill = true;
			turn = true;
			break;
	}
	if (mill)
	{
		ResetBodiesList(AT_MILL);
		ResetRendersLists(AT_MILL);
	}
	if (turn)
	{
		ResetBodiesList(AT_TURN);
		ResetRendersLists(AT_TURN);
	}
	/*CString key;
	class NStock *pS = nullptr;

	for (POSITION i = GetStartPosition(); i != nullptr; )
	{
		GetNextAssoc(i, key, pS);
		pS->GetBody()->SetNextMBody(nullptr);

		if (!pS->GetRender())
			continue;
		((GQuadRender*)pS->GetRender())->SetPreviousRender(nullptr);
		((GQuadRender*)pS->GetRender())->SetNextRender(nullptr);
	}*/
}

void CStocksCollection::ResetRendersLists()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	switch(NCMComData::GetType())
	{
		case AT_MILL:
			ResetRendersLists(AT_MILL);
			break;
		case AT_TURN:
			ResetRendersLists(AT_TURN);
			break;
		case AT_MILL_TURN:
			ResetRendersLists(AT_MILL);
			ResetRendersLists(AT_TURN);
			break;
	}
}

void CStocksCollection::ResetRendersLists(NCMApplicationType type)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (!pS->GetRender(type))
			continue;
		((GQuadRender*)pS->GetRender(type))->SetPreviousRender(nullptr);
		((GQuadRender*)pS->GetRender(type))->SetNextRender(nullptr);
	}
}

void CStocksCollection::ResetBodiesList(NCMApplicationType type)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		MBody* pBody = nullptr;
		switch (type)
		{
		case AT_MILL:
			pBody = pS->GetBodyMill();
			break;
		case AT_TURN:
			pBody = pS->GetBodyRot();
			break;
		}
		if(pBody != nullptr)
			pBody->SetNextMBody(nullptr);
	}
}

void CStocksCollection::CreateBodiesList(NCMApplicationType type)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	MBody *firstBody = nullptr;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (!(pS->IsVisible() && !pS->IsFrozen()))
			continue;
		if (pS->GetStockState() != type)
			continue;
		auto pBody = pS->GetBody();
		if (pBody == nullptr)
			continue;
		if (!firstBody)
		{
			firstBody = pBody;
			continue;
		}
		firstBody->AddBodyInList(pBody);
	}
}

void CStocksCollection::CreateMixedRendersList()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	ResetRendersLists();

	class NStock* previous = nullptr;

	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (!(pS->IsVisible() && !pS->IsFrozen()))
			continue;
		if (!pS->GetRender())
			pS->CreateStateRender();
		if (!pS->GetRender())
			continue;

		if (previous)
		{
			((GQuadRender*)pS->GetRender())->SetPreviousRender((GQuadRender*)previous->GetRender());
			((GQuadRender*)previous->GetRender())->SetNextRender((GQuadRender*)pS->GetRender());
		}
		previous = pS;
	}
}

bool CStocksCollection::Swap(INT_PTR first, INT_PTR second)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if ( (first >= GetSize()) || (second >= GetSize()) || (first == second))
		return false;
	CString firstKey;
	CString secondKey;
	NStock *backup;
	NStock *firstStock;
	NStock *secondStock;	

	GetAt(first, firstKey, (CObject*&)firstStock);
	GetAt(second, secondKey, (CObject*&)secondStock);

	//BMatr tempMatr = firstStock->GetMatr();
	//firstStock->Matr = secondStock->GetMatr();
	//secondStock->Matr = tempMatr;

	backup = firstStock;
	SetAt(firstKey, secondStock);
	SetAt(secondKey, backup);
	return true;
}

void CStocksCollection::ReCreateLists()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	ResetBodiesAndRendersLists();
	CreateBodiesAndRendersLists();
	if (GetSize() == 1)
		SyncProperties();
}

NStock * CStocksCollection::GetStock(int index)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	return GetStockUnsafe(index);
}

NStock* CStocksCollection::GetStockUnsafe(int index) const
{
	if (index >= GetSize() || index < 0)
		return nullptr;
	CString key;
	CObject* pObj = nullptr;
	GetAt(index, key, pObj);
	return (NStock*)pObj;
}

const NStock* CStocksCollection::GetLastStock()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (empty())
		return nullptr;
	auto index = GetSize() - 1;
	return GetStock(int(index));
}

PropList * CStocksCollection::Create(PropList * List)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	List->Clear();
	// Visibility 1
	PropSingle * h = new PropSingle();
	h->ID=1;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_MACH;
	h->Group=IDS_PROP_STOCK;
	h->For_Enum.Add(IDS_FALSE);
	if (GetSize() > 1)
	{
		h->For_Enum.Add(IDS_MIX);
		h->ST_Value= (Visible ? IDS_MIX : IDS_FALSE);
	}
	else
	{
		h->For_Enum.Add(IDS_TRUE);
		if (GetSize() == 1)
			Visible = GetStock(0)->IsVisible();
		h->ST_Value= (Visible ? IDS_TRUE : IDS_FALSE);
	}
	List->Add(h);
	// Edges 2
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_EDGE;
	h->Type=1;
	h->Comment=IDS_COMM_STOCK_EDGE;
	h->Group=IDS_PROP_STOCK;
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		h->ST_Value = IsSharpEdges() ? IDS_TRUE : IDS_FALSE;
	else
		h->ST_Value = IsSharpEdgesDX() ? IDS_TRUE : IDS_FALSE;
	List->Add(h);
	// Edges color 3
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_EDGE_COLOR;
	h->Type=3;
	h->Comment=IDS_COMM_MACH_COLOR;
	h->Group=IDS_PROP_STOCK;
	h->V_Value = GQuadRender::GetEdgesColor().GetRGB();
	List->Add(h);
	// Edges thickness 4
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_EDGE_THIKNESS;
	h->Type=4;
	h->Comment=IDS_COMM_MACH_THICK;
	h->Group=IDS_PROP_STOCK;
	h->Min=1.;
	h->Max=10.;
	h->V_Value = GQuadRender::GetEdgesWidth();
	List->Add(h);
	// Smooth angle 5
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_SMOOTHING_ANGLE;
	h->Type=4;
	h->Comment=IDS_COMM_MACH_SMOOTHANGL;
	h->Group=IDS_PROP_STOCK;
	if(NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		h->V_Value = GQuadRender::GetSmoothAngle();
	else
		h->V_Value = GQuadRenderDX5X::GetSmoothAngle();
	h->Max=180;
	h->Min=0;
	List->Add(h);
	// Transparency 6
	h = new PropSingle();
	h->ID=6;
	h->Name=IDS_TRANSPARENCY_DEGREE;
	h->Type=5;
	h->Comment=IDS_COMM_MACH_TRANSP;
	h->Group=IDS_PROP_STOCK;
	h->Max=100;
	h->Min=0;
	h->V_Value=int((1. - GetTransparency()) * 100. + 0.5);
	List->Add(h);
	// color 7
	h = new PropSingle();
	h->ID=7;
	h->Name=IDS_COLOR;
	h->Type=3;
	h->Group=IDS_PROP_STOCK;
	h->Comment=IDS_COMM_MODEL_COLOR;
	h->V_Value = GetColor().GetRGB();	
	List->Add(h);
	// polychrome 8
	h = new PropSingle();
	h->ID=8;
	h->Name=IDS_PROG_COLORMODE;
	h->Type=1;
	h->Comment=IDS_COMM_STOCK_COLORMODE;
	h->Group=IDS_PROP_STOCK;
	h->ST_Value = (MElemIDProc::GetAllowPCPalUse() ? IDS_POLYCHROME : IDS_MONOCHROME);
	h->For_Enum.Add(IDS_POLYCHROME);
	h->For_Enum.Add(IDS_MONOCHROME);
	List->Add(h);
	// Fast color mode
	h = new PropSingle();
	h->ID = 14;
	h->Name = IDS_FASTCOLORMODE;
	h->Type = 1;
	h->Comment = IDS_COMM_FASTCOLORMODE;
	h->Group = IDS_PROP_STOCK;
	h->ST_Value = NTool::GetFastColorMode();
	h->For_Enum.Add(IDS_FCM_YES);
	h->For_Enum.Add(IDS_FCM_NO);
	h->For_Enum.Add(IDS_FCM_IGNORE);
	h->Color = 1;
	List->Add(h);
	// Render mode 9
	h = new PropSingle();
	h->ID=9;
	h->Name = IDS_DISPLAY_MODE ;
	h->Type=1;
	h->Comment = IDS_COMM_MACH_PART_RENDER;
	h->Group=IDS_PROP_STOCK;
	h->ST_Value = GetRMode();
	h->For_Enum.Add(IDS_RM_SHADE);
	h->For_Enum.Add(IDS_RM_GLITTER);
	h->For_Enum.Add(IDS_RM_TRANSLUCENT);
	h->Color = 1;
	List->Add(h);
	// wire frame 10
	h = new PropSingle();
	h->ID=10;
	h->Name=IDS_WIRE_FRAME;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART_WIRE;
	h->Group=IDS_PROP_STOCK;
	h->For_Enum.Add(IDS_TRUE);
	if (GetSize() > 1)
	{
		h->For_Enum.Add(IDS_MIX);
		h->ST_Value = WFrameAllStocks ? IDS_TRUE : IDS_MIX;
	}
	else
	{
		h->For_Enum.Add(IDS_FALSE);
		if (GetSize() == 1)
			WFrameAllStocks = (GetStock(0)->GetWFrame() == TRUE);
		h->ST_Value = WFrameAllStocks ? IDS_TRUE : IDS_FALSE;
	}
	List->Add(h);

	//Single stock file name
	if (GetSize() == 1)
	{
	//stock type and form
		NStock* pStock = GetStock(0);		
		NStock::StockCrType sType = pStock->GetType();
		h = new PropSingle();
		h->ID = 16;
		h->Name = IDS_STOCK_TYPE;
		h->Type = 2;
		h->Comment = IDS_COMM_STOCK_TYPE;
		h->Group = IDS_PROP_STOCK;
		switch (sType)
		{
		case NStock::SCT_EMPTY:
			h->Value = "SCT_EMPTY";
			break;
		case NStock::SCT_BOX:
			h->Value.LoadString(IDS_SCT_BOX);
			break;
		case NStock::SCT_BAR:
			if (NCMComData::GetType() == AT_MILL)
				h->Value.LoadString(IDS_SCT_BAR);
			else
				h->Value.LoadString(IDS_SCT_BAR);
			break;
		case NStock::SCT_BAR_SM:
			if (NCMComData::GetType() == AT_MILL)
				h->Value.LoadString(IDS_SCT_BAR);
			else
				h->Value.LoadString(IDS_SCT_BAR_SM);
			break;
		case NStock::SCT_CONT:
			h->Value.LoadString(IDS_SCT_CONT);
			break;
		case NStock::SCT_CYL:
			h->Value.LoadString(IDS_SCT_BAR_SM);
			break;
		case NStock::SCT_STL:
			h->Value.LoadString(IDS_SCT_STL);
			break;
		case NStock::SCT_STLMT:
			h->Value.LoadString(IDS_SCT_STL);
			break;
		case NStock::SCT_STLT:
			h->Value.LoadString(IDS_SCT_STL);
			break;
		case NStock::SCT_SWEEP:
			h->Value.LoadString(IDS_SCT_SWEEP);
			break;
		case NStock::SCT_IMPEXP:
			h->Value.LoadString(IDS_SCT_BSP);
			break;
		case NStock::SCT_WELD:
			h->Value = "SCT_WELD";
			break;
		default:			
			h->Value = "SCT_EMPTY";
			break;
		}
		h->Color = 0;
		List->Add(h);
	// File path
		CString FileName;
		if (pStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
			FileName = ((NStockStl *)pStock)->GetFileName();
		else if (pStock->IsKindOf(RUNTIME_CLASS(NStockSweep)))
			FileName = ((NStockSweep *)pStock)->GetFileName();
		else if (pStock->IsKindOf(RUNTIME_CLASS(NStockCont)))
			FileName = ((NStockCont *)pStock)->GetFileName();
		else if (pStock->IsKindOf(RUNTIME_CLASS(NStockBSP)))
			FileName = ((NStockBSP*)pStock)->GetFileName();
		if (!FileName.IsEmpty())
		{
			h = new PropSingle();
			h->ID = 15;
			h->Name = IDS_PROP_STLFILE;
			h->Type = 2;
			h->Comment = IDS_COMM_PROP_STLFILE;
			h->Value = FileName;
			h->Color = 0;
			List->Add(h);
		}
		//IDC_STOCK_POS 
		h = new PropSingle();
		h->ID = 13;
		h->Name = IDS_PROP_STOCKPOS;
		h->Type = 5;
		h->Comment = IDS_COMM_PROP_STOCKPOS;
		h->V_Value = pStock->StockGr.Get();
		h->Color = 0;
		List->Add(h);
	}
	// Section angle
	if(NCMComData::GetType() == AT_MILL_TURN || NCMComData::GetType() == AT_TURN)
	{
		//Angle A 11
		{
			h = new PropSingle();
			h->ID=11;
			h->Name=IDS_ANGLE_A;
			h->Type=5;
			h->Comment=IDS_COMM_ANGLE_A;
			h->Group=IDS_PROP_STOCK;
			h->Min=0.;
			h->Max=360.;
			double StartAngle , Angle, AngleStep;
			int AngleStepsNum;
			GQuadRenderRot::GetVisParam(StartAngle , Angle, AngleStep, AngleStepsNum);
			h->V_Value = StartAngle;
			List->Add(h);
		}
		//Angle B 12
		{
			h = new PropSingle();
			h->ID=12;
			h->Name=IDS_ANGLE_B;
			h->Type=5;
			h->Comment=IDS_COMM_ANGLE_B;
			h->Group=IDS_PROP_STOCK;
			h->Min=0.;
			h->Max=360.;
			double StartAngle , Angle, AngleStep;
			int AngleStepsNum;
			GQuadRenderRot::GetVisParam(StartAngle , Angle, AngleStep, AngleStepsNum);
			h->V_Value = Angle;
			List->Add(h);
		}
	}



	// type
	//if(IsRealStock())
	//{
		/*h = new PropSingle();
		h->ID=8;
		h->Name=IDS_STOCK_TYPE;
		h->Type=1;
		h->Comment=IDS_COMM_STOCK_TYPE;
		h->Group=IDS_PROP_STOCK;
		h->ST_Value=(GetStockType() == AT_MILL ? IDS_AT_MILL : IDS_AT_TURN);
		h->Color = 0;
		List->Add(h);*/
	//}
	//else
	//{
	//	h = new PropSingle();
	//	h->ID=12;
	//	h->Name=IDS_SMOOTHING;
	//	h->Type=1;
	//	h->Comment=IDS_COMM_MACH_SMOOTH;
	//	h->Group=IDS_PROP_STL;
	//	h->ST_Value= (GetDrawSmoothSurf() ? IDS_TRUE : IDS_FALSE);
	//	h->For_Enum.Add(IDS_TRUE);
	//	h->For_Enum.Add(IDS_FALSE);
	//	List->Add(h);
	//}
	// DX params
	if ((NCMComData::GetType() == AT_MILL || NCMComData::GetType() == AT_MILL_TURN)
		&& (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX))
	{
		//FastCutMode tol
		h = new PropSingle();
		h->ID = 25;
		h->Name = IDS_FAST_TOL;
		h->Type = 5;
		h->Comment = IDS_COMM_FAST_TOL;
		h->Group = IDS_TUNING;
		h->V_Value = GQuadRenderDX5X::GetWholeCellsNum() / GQuadRenderDX5X::CellsNumScale;
		NTiParams &Par = NCM_PROJECT.Defaults;
		CString Key(_T("Defaults/Stock/Mill/DXSize"));
		CString SubKeyMax(_T("@DXMax"));
		CString SubKeyMin(_T("@DXMin"));
		h->Max = Par.GetDouble((Key + SubKeyMax).GetBuffer(), 1000);
		h->Min = Par.GetDouble((Key + SubKeyMin).GetBuffer(), 1);
		List->Add(h);
		//Scalop
		h = new PropSingle();
		h->ID=23;
		h->Name=IDS_FIND_VERGE;
		h->Type=1;
		h->Comment=IDS_COMM_FIND_VERGE;
		h->Group=IDS_TUNING;
		h->ST_Value = (GBodyRender::GetFindVergPoint() ? IDS_TRUE : IDS_FALSE);
		h->For_Enum.Add(IDS_TRUE);
		h->For_Enum.Add(IDS_FALSE);
		List->Add(h);
		//Vert
		h = new PropSingle();
		h->ID=20;
		h->Name=IDS_DichotDegree;
		h->Type=5;
		h->Comment=IDS_DichotDegree;
		h->Group=IDS_TUNING;
		h->Min = 0;
		h->Max = 20;
		h->V_Value = GQuadRender::GetDichotDegree();
		List->Add(h);
		//
		h = new PropSingle();
		h->ID=21;
		h->Name=IDS_NORM_PAR;
		h->Type=4;
		h->Comment=IDS_NORM_PAR;
		h->Group=IDS_TUNING;
		h->V_Value = sqrt(GBodyRender::GetMinSin2()) * 180. / PI;
		List->Add(h);
		//
	}

return List;
}

PropList * CStocksCollection::Change(PropList * List,PropSingle * Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;
	//NStock* pStock = GetStock(0);

	switch(id)
	{
	case 1: // Visability
		{
			Visible = (Single->ST_Value != IDS_FALSE);
			if (GetSize() == 1)
				GetStock(0)->SetVisible(Visible);
		}
		break;
	case 2: // Sharp Edges
		{
			if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
			{
				SetSharpEdges(Single->ST_Value == IDS_TRUE);
				Group = OG_STOCKS;
			}
			else
				SetSharpEdgesDX(Single->ST_Value == IDS_TRUE);
		}
		break;
	case 3: // Edges Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			GQuadRender::SetEdgesColor(Col);
			SetEdgesColor(Col);
			break;
		}
	case 4: // Edges thickness
		GQuadRender::SetEdgesWidth(Single->V_Value);
		SetEdgesWidth(Single->V_Value);
		break;
	case 5: // Smoothing angle
		if (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		{
			Group = OG_STOCKS;
			GQuadRender::SetSmoothAngle(Single->V_Value);
		}
		else
			GQuadRenderDX5X::SetSmoothAngle(Single->V_Value);
		Group = OG_STOCKS;
		break;
	case 6: // Transparency
		SetStocksTransparency(1. - Single->V_Value / 100.);
		Group = OG_STOCKS;
		break;
	case 7: // Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetColor(Col);
			Group = OG_STOCKS;
		}
		break;
	case 8: // Polychrome
		{
			MElemIDProc::SetAllowPCPalUse(Single->ST_Value == IDS_POLYCHROME);
			Group = OG_STOCKS;//?????
		}
		break;
	case 9: // Render mode
		SetRMode(RenderMode(Single->ST_Value));
		break;
	case 10: // Wireframe
		WFrameAllStocks = (Single->ST_Value == IDS_TRUE);
		if (GetSize() == 1)
			GetStock(0)->SetWFrame(WFrameAllStocks);
		break;
	// Angle property
	case 11: 
		{
			double StartAngle , Angle, AngleStep;
			int AngleStepsNum;
			GQuadRenderRot::GetVisParam(StartAngle, Angle, AngleStep, AngleStepsNum);
			GQuadRenderRot::SetVisParam(Single->V_Value, Angle, AngleStepsNum);
			Group = OG_STOCKS | OG_PARTS;
	}
		break;
	case 12: 
		{
			double StartAngle , Angle, AngleStep;
			int AngleStepsNum;
			GQuadRenderRot::GetVisParam(StartAngle, Angle, AngleStep, AngleStepsNum);
			GQuadRenderRot::SetVisParam(StartAngle, Single->V_Value, AngleStepsNum);
			Group = OG_STOCKS | OG_PARTS;
		}
		break;
	//case 13: //IDC_STOCK_POS 
	//	pStock->SetPosition(Single->V_Value);
	//	break;
	case 14: // Fast color
		NTool::SetFastColorMode(NFastColorMode(Single->ST_Value));
		Group = OG_STOCKS;
		break;
	case 15:
		break;
	case 16:
		break;
		// DX prop	
	case 20: 
		{
			GQuadRender::SetDichotDegree(int(Single->V_Value));
			Group = OG_STOCKS;
		}
		break;
	case 21: 
		{
			double b = Single->V_Value * PI / 180.;
			GBodyRender::SetMinSin2(b * b);
			Group = OG_STOCKS;
		}
		break;
	case 23: 
		{
			GQuadRender::SetFindVergPoint(Single->ST_Value == IDS_TRUE);
			Group = OG_STOCKS;
		}
		break;
	case 25: 
		{
			GQuadRenderDX5X::SetWholeCellsNum(int(Single->V_Value * GQuadRenderDX5X::CellsNumScale));
			Group = OG_NO_GROUP; // Reset all
		}
		break;	
	}
	return 	Create(List);
}


void CStocksCollection::ApplyStocksState(NCMApplicationType NewState, const std::vector<BStockGrID>& StGroups)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		bool pSOK = false;
		for each(auto el in StGroups)
			if (el == pS->GetStockGr() || el.Get()  == -1)
			{
				pSOK = true;
				break;
			}
		if (pSOK)
		{
			pS->SetStockState(NewState);
			if (pS->GetStockType() != AT_MILL_TURN)
				continue;
			if (pS->IsVisible() && !pS->IsFrozen())
			{
				if (pS->GetStockState() == AT_MILL)
				{
					pS->IntersectBodyes();
				}
				else if (pS->GetStockState() == AT_TURN)
				{
					TurnGenMode TGMode = NStock::GetTGMode();
					if (TGMode == TGM_NO)
						continue;
					pS->MakeTurnFromMill(false, TGMode);
				}
			}
		}
	}
	CreateBodiesAndRendersLists();
}

NCMApplicationType CStocksCollection::GetStocksState(void)
{
	if (empty())
		return AT_UNDEF;
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	return GetStock(0)->GetStockState();
}

void CStocksCollection::SetStocksTransparency(double value)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	Transparency = value;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetTransparency(value);
	}
//	CreateStocksRender();
}

void CStocksCollection::SetStocksColor(const NColor &Col)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetColor(Col);
	}
//	CreateStocksRender();
}

void CStocksCollection::ResetStocks()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	SetCollided(false);
	ResetBodiesAndRendersLists();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->Reset();
	}
	CreateBodiesAndRendersLists();
}

void CStocksCollection::ClearStocksRender()
{
	if (empty())
		return;
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	ResetRendersLists();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->ClearRender();
	}
}

MBody* CStocksCollection::GetVisibleBodiesList(NCMApplicationType State)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (pS->IsVisible() && !pS->IsFrozen())
			if(pS->GetStockState() == State)
				return pS->GetBody();
	}
	return nullptr;
}

GBodyRender* CStocksCollection::GetVisibleRendersList()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (pS->IsVisible() && !pS->IsFrozen())
			return pS->GetRender();
	}
	return nullptr;
}

GBodyRender* CStocksCollection::MakeVisibleRendersList()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (pS->IsVisible() && !pS->IsFrozen() && pS->GetRender() == nullptr)
			pS->CreateStateRender();
	}
	return GetVisibleRendersList();
}

void CStocksCollection::SetFrozen(bool Val)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	bool result = true;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetFrozen(Val);
	}
}
const NColor & CStocksCollection::GetColor(void)
{
	return StocksColor;
}

void CStocksCollection::SetColor(const NColor & Col)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	StocksColor = Col;
	StocksColor.A() = float(Transparency);
	SetStocksColor(StocksColor);
}

void CStocksCollection::SetColor(COLORREF Col)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	NColor buf;
	buf.SetRGB(Col);
	SetColor(buf);
}

void CStocksCollection::Serialize(CArchive &ar)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	CString TempStockName;
	NStock *pTempStock = nullptr;
	if (ar.IsStoring())
	{// Remove temporary stock to restore it after serializing
		if(FindTempStock(TempStockName, pTempStock))
			RemoveKey(TempStockName);
	}
	CMapStringToNStock::Serialize(ar);
	StocksColor.Serialize(ar);
	SerializeElements(ar, &WFrameAllStocks, 1);
	if (ar.IsLoading())
		WFrameAllStocks = false;
	if (!empty())
		NStock::SerializeExt(ar);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.5")))
		SerializeElements(ar, &DrawSharpEdgesDX, 1);
	if (ar.IsStoring())
	{// Restore temporary stock
		if(!TempStockName.IsEmpty())
			SetAt(TempStockName, pTempStock);
	}

	if (ar.IsLoading())
	{
		EdgesAttrs.SetEdgesColor(GQuadRender::GetEdgesColor());
		EdgesAttrs.SetEdgesWidth(GQuadRender::GetEdgesWidth());
		EdgesAttrs.SetSmoothAngle(GQuadRender::GetSmoothAngle());
		CString key;
		class NStock *pS = nullptr;
		for (int l = 0; l < GetSize(); ++l)// Remove bad stocks if any
		{
			GetAt(l, key, (CObject *&)pS);
			if (pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
			{
				if (!((NStockStl *)pS)->IsCreated)
				{
					delete pS;
					RemoveAt(l);
					--l;
					continue;
				}
			}
			pS->WFrame = TRUE;
		}
	}
}

void CStocksCollection::PostSerialize()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	ResetStocks();
	if (GetSize() == 1)
		SyncProperties();
}

bool CStocksCollection::RenderSect(class MTMachineTool& MT)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	CString key;
	NStock *pS = nullptr;
	bool result;
	for (int i = 0; i < GetSize(); ++i )
	{
		if (!GetAt(i, key, (CObject*&)pS))
			continue;
		if (pS->IsVisible() && !pS->IsFrozen())
		{
			BStockGrID StPos = pS->GetStockGr();
//			if (StPos != 0)
			{
				glPushMatrix();
				glMultMatrixd((MT.GetStockMovedMatrSGr(StPos, true) * MT.GetStockMovedMatr(0, true).invr()).GetArray());
			}
			result = pS->RenderSect();
//			if (StPos != 0)
				glPopMatrix();
		}
	}
	return result;
}

void CStocksCollection::SetSharpEdges(bool value)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	DrawSharpEdgesS = value;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetEdgesS(DrawSharpEdgesS);
	}
}

void CStocksCollection::SetSharpEdgesDX(bool value)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	DrawSharpEdgesDX = value;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetEdgesDX(DrawSharpEdgesDX);
	}
}

void CStocksCollection::SyncProperties()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if(WFrameAllStocks)
		GetStock(0)->SetWFrame(true);
//	GetStock(0)->SetVisible(Visible);
}

void CStocksCollection::SetRMode(RenderMode Mode)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	RMode = Mode;
	if(RMode == RM_TRANSLUCENT)
	{
		//SetAlpha(Transparency);
		StocksColor.A() = (float)Transparency;
	}
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetRMode(Mode);
	}
}

bool CStocksCollection::FindTempStock(CString & Name, NStock *& pStock)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (pS->IsTemporary())
		{
			Name = el.first;
			pStock = pS;
			return true;
		}
	}
	return false;
}

bool CStocksCollection::HaveTempStock(void)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if(pS != nullptr)
			if(pS->IsTemporary())
				return true;
	}
	return false;
}

void CStocksCollection::SetMillStockChanged(bool Val)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetMillStockChanged(Val);
	}
}

void CStocksCollection::SetTurnStockChanged(bool Val)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		pS->SetTurnStockChanged(Val);
	}
}
