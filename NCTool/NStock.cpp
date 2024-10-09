#include "NStock.h"
// NStock.cpp: implementation of the NStock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "ppltasks.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "..\BSP_Model\MForestSave.h"
#include "ConstDef.h"
#include "BaseRender.h"
#include "NColor.h"
#include "RXYZArrows.h"
#include "GQuadRenderRot.h"
#include "GQuadRenderMill.h"
#include "GAnimateCadrRot.h"
#include "GAnimateCadrMill.h"
#include "NSurfSettings.h"
#include "MBSPInter.h"
#include "NStockStl.h"
#include "NStockCont.h"
#include "NStockSweep.h"
#include "NStockBSP.h"
#include "PropList.h"
#include "GQuadRenderDX5X.h"
#include "StContOut.h"
#include "DXFOut.h"
#include "BFileDialogWPath.h"
#include "TBladeCont.h"
#include "NParamMesh.h"
#include "MCubeMask.h"
#include "NStockBox.h"
#include "WM_APP_messages.h"
#include "NFastColorMode.h"
#include "DXSubModel5X.h"
#include "..\BSP_Model\MBSPUnion.h"
#include "RRopes.h"
#include "NStock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(NStock, NMovableObj, 0)

NStock::NStock()
{
	Visible = TRUE;
	WFrame = TRUE;
	DrawSmoothSurf = true;
	//Glitter = FALSE;
	Transparent = FALSE;
	Transparency = 0.5;
	SharpEdgesS = TRUE;
	SharpEdgesDX = FALSE;
	Color.SetRGB(RGB(0,0,255));

	BSPTNum = -1;
	BSPTNumRot = -1;
	BSPTNumDX = -1;
	pRender = NULL;
	pRenderRot = NULL;
	Type = AT_MILL;
	State = AT_MILL;
	ColorNum = STOCK_COLOR_NUM;
	RMode = RM_SHADE;
	SetAlpha(Transparency);
	Frozen = false;
	Temporary = false;
	WholeCellsKoef = 1.;
	StockGr.Set(0);
	MillStockChanged = true; // For mill turn only
	TurnStockChanged = true; // For mill turn only
}

void NStock::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStock();
	stock->SetStockGr(GetStockGr());

	// NCollidableObject variables
	stock->Collided = this->Collided;

	// NPassiveObject variables
	stock->EdgesAttrs = this->EdgesAttrs;
	
	// NMovableObj variables
	stock->Moving = this->Moving;
	stock->pos_X = this->pos_X;
	stock->pos_Y = this->pos_Y;
	stock->pos_Z = this->pos_Z;
	stock->Matr = this->Matr;
	
	// NStock variables
	stock->Visible = this->Visible;
	stock->WFrame = this->WFrame;
	stock->DrawSmoothSurf = this->DrawSmoothSurf;
	stock->Transparent = this->Transparent;
	stock->Transparency = this->Transparency;
	stock->SharpEdgesS = this->SharpEdgesS;
	stock->SharpEdgesDX = this->SharpEdgesDX;
	stock->Color = this->Color;
	stock->Type = this->Type;
	stock->State = this->State;
	stock->ColorNum = this->ColorNum;
	stock->RMode = this->RMode;
	stock->StockGr = this->StockGr;
	stock->MillStockChanged = this->MillStockChanged;
	stock->TurnStockChanged = this->TurnStockChanged;
	stock->Gabar = this->Gabar;
}

void NStock::MakeRenderCopy()
{
	switch (State)
	{
	case AT_MILL:
		if(pRender != nullptr)
			pRender = pRender->MakeCopy();
		break;
	case AT_TURN:
		if (pRenderRot != nullptr)
			pRenderRot = static_cast<GQuadRenderRot*>(pRenderRot->MakeCopy());
		break;
	}
}

NStock::~NStock()
{
	if(BSPTNum > -1)
	{
		MFOREST.DeleteTree(BSPTNum);
		BSPTNum = -1;
	}
	delete pRender;
	if(BSPTNumRot > -1)
	{
		MFOREST.DeleteTree(BSPTNumRot);
		BSPTNumRot = -1;
	}
	delete pRenderRot;
	if(BSPTNumDX > -1)
	{
		MFOREST.DeleteTree(BSPTNumDX);
		BSPTNumDX = -1;
	}
}
BEdgesAttrs BufAttrs;
void NStock::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSurf)
{
	if (IsMoving())
	{
		glPushMatrix();
		BPoint p = GetGabar().GetRelPoint(pos_X, pos_Y, pos_Z);
		glTranslated(p.GetX(), p.GetY(), p.GetZ());
		RXYZArrows BaseXYZ;
		BaseXYZ.Draw(Color.Inv());
		glPopMatrix();
		auto& Attrs = GetSelectAttrs();
		BufAttrs = GetEdgesAttrs();
		BEdgesAttrs NewEdgesAttrs;
		NewEdgesAttrs.SetEdgesColor(Attrs.GetEdge());
		NewEdgesAttrs.SetEdgesWidth(Attrs.GetEdgeWidth());
		SetEdgesAttrs(NewEdgesAttrs);
		glColor4fv(Attrs.GetMain().array);
	}
	else if (IsCollided() || (GetRender() != nullptr && GetRender()->IsCollided()))
	{
		glColor4fv(GetCollidedColor().array);
	}
	else
	{
		glColor4fv(Color.array);
	}
}

void NStock::RestDrawOriginal()
{
	if (IsMoving())
	{
		SetEdgesAttrs(BufAttrs);
	}
}


void NStock::SerializeExt(CArchive &ar)
{
	if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() >= atof("4.0") && NCM_PROJECT.GetInFileVersion() < atof("4.5"))
	{
		int BDichotDegree = 5;
		double BMinSin2 = 5.;
		double BVertCellRatio = 15.;
		bool BFindVergPoint = false;
		bool BFindFootstep = true;
		int BWholeCellsNum = 512 * 512;
		SerializeElements(ar, &BDichotDegree, 1);
		SerializeElements(ar, &BMinSin2, 1);
		SerializeElements(ar, &BVertCellRatio, 1);
		SerializeElements(ar, &BFindVergPoint, 1);
		SerializeElements(ar, &BFindFootstep, 1);
		SerializeElements(ar, &BWholeCellsNum, 1);
		GBodyRender::SetDichotDegree(BDichotDegree);
		GBodyRender::SetMinSin2(BMinSin2);
		GBodyRender::SetFindVergPoint(BFindVergPoint);
		GQuadRenderDX5X::SetWholeCellsNum(BWholeCellsNum);
	}

	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.5")))
	{
		int BDichotDegree = 5;
		double BMinSin2 = 5.;
		bool BFindVergPoint = false;
		int BWholeCellsNum = 512 * 512;
		if (ar.IsStoring())
		{
			BDichotDegree = GBodyRender::GetDichotDegree();
			BMinSin2 = GBodyRender::GetMinSin2();
			BFindVergPoint = GBodyRender::GetFindVergPoint();
			BWholeCellsNum = GQuadRenderDX5X::GetWholeCellsNum();
		}
		SerializeElements(ar, &BDichotDegree, 1);
		SerializeElements(ar, &BMinSin2, 1);
		SerializeElements(ar, &BFindVergPoint, 1);
		SerializeElements(ar, &BWholeCellsNum, 1);
		if (ar.IsLoading())
		{
			GBodyRender::SetDichotDegree(BDichotDegree);
			GBodyRender::SetMinSin2(BMinSin2);
			GBodyRender::SetFindVergPoint(BFindVergPoint);
			GQuadRenderDX5X::SetWholeCellsNum(BWholeCellsNum);
		}
	}

	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		double BSmoothAngle = 0.;
		NColor BEdgesCol;
		double EdgesWidth = 1;
		bool Polychrome = true;
		if(ar.IsStoring())
		{
			BSmoothAngle = GQuadRender::GetSmoothAngle();
			BEdgesCol = GQuadRender::GetEdgesColor();
			EdgesWidth = GQuadRender::GetEdgesWidth();
			Polychrome = MElemIDProc::GetAllowPCPalUse();
		}
		SerializeElements(ar, &BSmoothAngle, 1);

		//SerializeElements(ar, &BEdgesCol, 1);
		int a=0;
		if (ar.IsLoading())
			ar>>a;
		else
			ar<<a;
		BEdgesCol.Serialize(ar);

		SerializeElements(ar, &EdgesWidth, 1);
		SerializeElements(ar, &Polychrome, 1);
		if(ar.IsLoading())
		{
			GQuadRender::SetSmoothAngle(BSmoothAngle);
			GQuadRender::SetEdgesColor(BEdgesCol);
			GQuadRender::SetEdgesWidth(EdgesWidth);
			MElemIDProc::SetAllowPCPalUse(Polychrome);
		}
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.6")))
	{
		double BSmoothAngle = 0.;
		if (ar.IsStoring())
			BSmoothAngle = GQuadRenderDX5X::GetSmoothAngle();
		SerializeElements(ar, &BSmoothAngle, 1);
		if (ar.IsLoading())
			GQuadRenderDX5X::SetSmoothAngle(BSmoothAngle);
	}
}
void NStock::Serialize(CArchive &ar)
{
	NMovableObj::Serialize(ar);
	Color.Serialize(ar);
	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.8"))
	{
		double B[6];
		BOOL Def;
		SerializeElements(ar, B, 6);
		SerializeElements(ar, &Def, 1);
	}
//	Gabar.Serialize(ar);
	SerializeElements(ar,&Visible,1);
	SerializeElements(ar,&WFrame,1);
	if (ar.IsLoading() && IsRealStock())
		WFrame = true;
	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.0"))
	{
		BPoint buf;
		double pbuf;
		SerializeElements(ar, &pbuf, 1);
		SerializeElements(ar,&buf,1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.5")))
	{
		SerializeElements(ar, &SharpEdgesDX, 1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.2")))
		{
			SerializeElements(ar, &SharpEdgesS, 1);
			bool b;
		if(ar.IsStoring())
			b = MElemIDProc::GetAllowPCPalUse();
		SerializeElements(ar, &b, 1);
		if(ar.IsLoading())
			MElemIDProc::SetAllowPCPalUse(b);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.6")))
	{
		double StartAngle , Angle, AngleStep;
		int AngleStepsNum;
		if(ar.IsStoring())
			GQuadRenderRot::GetVisParam(StartAngle , Angle, AngleStep, AngleStepsNum);
		SerializeElements(ar, &StartAngle, 1);
		SerializeElements(ar, &Angle, 1);
		SerializeElements(ar, &AngleStepsNum, 1);
		if(ar.IsLoading())
			GQuadRenderRot::SetVisParam(StartAngle , Angle, AngleStepsNum);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		SerializeElements(ar, &RMode, 1);
		SerializeElements(ar, &SharpEdgesS, 1);
		SerializeElements(ar, &Transparency, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.1")))
	{
		SerializeElements(ar, &ColorNum, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.4")))
	{
		SerializeElements(ar, &WholeCellsKoef, 1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")))
	{
		SerializeElements(ar, &StockGr, 1);
	}
}

const NColor & NStock::GetColor(void) const
{
	return Color;
}

void NStock::SetColor(const NColor & Col)
{
	Color = Col;
	Color.A() = float(Transparency);
}

void NStock::SetColor(COLORREF Col)
{
	NColor buf;
	buf.SetRGB(Col);
	SetColor(buf);
}

void NStock::SetAlpha(double Alpha)
{
	Color.A() = (float)Alpha;
}

bool NStock::CreateRenderMill(void)
{
	// For single stock in list
	MQuadBody *pLocBody = (MQuadBody *)GetBodyMill();
	if (!pLocBody)
		return false;

	auto *previous = (pRender) ? pRender->GetPreviousRender() : NULL;
	auto *next = (pRender) ? pRender->GetNextRender() : NULL;
	if(pRender)
		pRender->SetNextRender(NULL);
	auto* pDeleteThread = new std::thread([](auto pToDel) {delete pToDel; }, pRender);
	SetThreadPriority(pDeleteThread->native_handle(), THREAD_PRIORITY_IDLE);
	pDeleteThread->detach();
//	delete pRender;
	switch(NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		pRender = new GQuadRenderDX5X(static_cast<DXSubModel5X &>(*pLocBody));
		break;
	case NGlobalState::BSP:
		pRender = new GQuadRenderMill(*pLocBody);
		break;
	}
	pRender->SetCollided(IsCollided());

	MBody *nextBody = pLocBody->GetNextMBody();
	pLocBody->SetNextMBody(NULL);
	
	pRender->FillAllArraysCh(*pLocBody);

	pRender->SetNextRender(next);
	pRender->SetPreviousRender(previous);
	if (previous)
		previous->SetNextRender(pRender);
	if (next)
		next->SetPreviousRender(pRender);

	pLocBody->ClearModified();
	pLocBody->SetNextMBody(nextBody);
//	MFOREST.Compress(true);// don't use compress while QDivide may be active
	return true;
}

bool NStock::CreateStateRender(void)
{
	switch (GetStockState())
	{
	case AT_MILL:
		return CreateRenderMill();
		break;
	case AT_TURN:
		return CreateRenderRot();
	}
	return false;
}

bool NStock::CreateModifRenderMill(void)
{
	MQuadBody *pLocBody = (MQuadBody *)GetBodyMill();
	if (!pLocBody || !pRender)
		return false;

	auto *previous = (pRender) ? pRender->GetPreviousRender() : NULL;
	auto *next = (pRender) ? pRender->GetNextRender() : NULL;
	if(pRender)
		pRender->SetNextRender(NULL);
	
	MBody *nextBody = pLocBody->GetNextMBody();
	pLocBody->SetNextMBody(NULL);
	
	pRender->FillModifArraysCh(*pLocBody);

	pRender->SetNextRender(next);
	pRender->SetPreviousRender(previous);
	if (previous)
		previous->SetNextRender(pRender);
	if (next)
		next->SetPreviousRender(pRender);

	pLocBody->ClearModified();
	pLocBody->SetNextMBody(nextBody);

	return true;
}

void NStock::Render(enum RenderMode GlobRMode, int Type, bool DrawSurf)
{
	if(!Visible)
		return;
//	glColor4fv(Color.array);

	if(WFrame)
	{
		BaseRender::SetWFMode();
	}
	if(GetStockState() == AT_TURN)
	{
		if(!pRenderRot)
			if(BSPTNumRot >= 0)
				if(!CreateRenderRot())
					return;
		if (!pRenderRot)
			return;
		BOOL SharpEdges = FALSE;
		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
		case NGlobalState::DX:
			SharpEdges = SharpEdgesDX;
			break;
		case NGlobalState::BSP:
			SharpEdges = SharpEdgesS;
			break;
		}

		if (SharpEdges)
		{
			glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1., 1.);
		}
		pRenderRot->RenderCh(GlobRMode, 3, DrawSurf);//LATHE
		if(SharpEdges)
		{
			glPopAttrib();
			if (EdgesAttrs.GetEdgesWidth() > 0.01)
			{
				glPushAttrib(GL_CURRENT_BIT);
				glColor4fv(EdgesAttrs.GetEdgesColor().array);
				glLineWidth(GLfloat(EdgesAttrs.GetEdgesWidth()));
				RenderSect();
				glPopAttrib();
			}
		}
	}
	else
	{
		if (!pRender)
			CreateRender();
		if (pRender)
			pRender->RenderCh(GlobRMode, Type, DrawSurf);
	}
	
	if(WFrame)
	{
		BaseRender::RemoveWFMode();
	}
}

void NStock::Reset(void)
{
	if(Frozen)
		return;
	CreateBody();
	NColor *pCol;
	if(NCM_PROJECT.Colors.Lookup(_T("ЗаготовкаОбработаннаяПоУмолчанию"),pCol))
		MElemIDProc::ClearPCP(*pCol);
	else
		MElemIDProc::ClearPCP(NColor(0., 1., 0., 1.));
}

MBody * NStock::GetBody() const
{
	switch(Type)
	{
	case AT_MILL:
		return GetBodyMill();
	case AT_TURN:
		return GetBodyRot();
	case AT_MILL_TURN:
		switch(State)
		{
		case AT_MILL:
			return GetBodyMill();
		case AT_TURN:
			return GetBodyRot();
		}
		return NULL;
	default:
		return NULL;
	}
	return NULL;
}
MBody * NStock::GetBodyRot(void) const 
{
	if(BSPTNumRot < 0)
		return NULL;
	return MFOREST.GetTree(BSPTNumRot);
}
MBody * NStock::GetBodyMill(void) const
{
	int Num = -1;
	switch(NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		Num = BSPTNumDX;
		break;
	case NGlobalState::BSP:
		Num = BSPTNum;
		break;
	}
	if(Num < 0)
		return NULL;
	return MFOREST.GetTree(Num);
}

void NStock::CreateFromBSP(void)
{
	if(MFOREST.GetPointsNum() > 0)
		BSPTNum = 0;
	else
		BSPTNum = -1;
	CreateRender();
}

void NStock::ExportSTL(CStdioFile *f)
{
	const MQuadBody *pTree = static_cast<MQuadTree *>(GetBody());
	if (!pTree)
		return;

	bool buf = GBodyRender::GetSmooth();
	GBodyRender::SetSmooth(false);
	GBodyRender *pSTLRender;
	switch (State)
	{
	case AT_MILL:
		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
		case NGlobalState::DX:
			pSTLRender = new GQuadRenderDX5X((DXSubModel5X &)*pTree);
			break;
		case NGlobalState::BSP:
			pSTLRender = new GQuadRenderMill(*pTree);
			break;
		}
		break;
	case AT_TURN:
		pSTLRender = new GQuadRenderRot(*pTree);
		break;
	}
	pSTLRender->FillAllArrays(*static_cast<MQuadTree*>(GetBody()));
	pSTLRender->WriteSTL(*f);
	delete pSTLRender;
	GQuadRender::SetSmooth(buf);

}

void NStock::ExportBSP(CFile *f)
{
	if (f == nullptr)
		return;
	f->Write(&Type, sizeof(Type));
	switch (Type)
	{
	case AT_MILL:
		Save1Tree(*f, BSPTNum, BMatr());
		break;
	case AT_TURN:
		Save1Tree(*f, BSPTNumRot, BMatr());
		break;
	case AT_MILL_TURN:
		if (SetStockState(AT_MILL))
		{
			IntersectBodyes();
			SetStockState(AT_TURN);// restore previous state
		}
		Save1Tree(*f, BSPTNum, BMatr());
		Save1Tree(*f, BSPTNumRot, BMatr());
		break;
	}
}

void NStock::ExportDX(CFile* f)
{
	if (f == nullptr)
		return;
	f->Write(&Type, sizeof(Type));
	switch (Type)
	{
	case AT_MILL:
		Save1Tree(*f, BSPTNumDX, BMatr());
		break;
	case AT_TURN:
		Save1Tree(*f, BSPTNumRot, BMatr());
		break;
	case AT_MILL_TURN:
		if (SetStockState(AT_MILL))
		{
			IntersectBodyes();
			SetStockState(AT_TURN);// restore previous state
		}
		Save1Tree(*f, BSPTNumDX, BMatr());
		Save1Tree(*f, BSPTNumRot, BMatr());
		break;
	}
}

void NStock::ClearRender(void)
{
	switch(State)
	{
	case AT_MILL:
		if(!pRender)
			return;
		delete pRender;
		pRender = NULL;
		break;
	case AT_TURN:
		if(!pRenderRot)
			return;
		delete pRenderRot;
		pRenderRot = NULL;
		break;
	}
}

GBodyRender * NStock::GetRender() const
{
	switch(State)
	{
	case AT_MILL:
		return pRender;
	case AT_TURN:
		return pRenderRot;
	default:
		return NULL;
	}
}

void NStock::NullRender(void)
{
	switch(State)
	{
	case AT_MILL:
		pRender = NULL;
		break;
	case AT_TURN:
		pRenderRot = NULL;
		break;
	}
}

GBodyRender * NStock::GetRender(NCMApplicationType Type) const
{
	switch(Type)
	{
	case AT_MILL:
		return pRender;
	case AT_TURN:
		return pRenderRot;
	default:
		return nullptr;
	}
}

BBox NStock::GetGabar(void) const
{
	return Gabar;
}

BBox NStock::GetGabarRender(void) const
{
	static const BMatr M = BMatr().RotX(0., 0., 0., 90.);
	BBox G;
	if (pRender)
		G = pRender->GetGabar();
	else if (pRenderRot)
		G = pRenderRot->GetGabar();
	else
		G = GetGabar();
	if (G.IsDefined() && (Type == AT_TURN || Type == AT_MILL_TURN))
		G.Transform(M);
	return G;
}

void NStock::Create(void)
{
	if(BSPTNum > -1)
		MFOREST.DeleteTree(BSPTNum);
	BSPTNum = -1;
	if(BSPTNumRot > -1)
		MFOREST.DeleteTree(BSPTNumRot);
	BSPTNumRot = -1;
	if(BSPTNumDX > -1)
		MFOREST.DeleteTree(BSPTNumDX);
	BSPTNumDX = -1;
}
bool NStock::CreateRenderRot(void)
{
	// For single stock in list
	auto *previous = (pRenderRot) ? pRenderRot->GetPreviousRender() : NULL;
	auto *next = (pRenderRot) ? pRenderRot->GetNextRender() : NULL;
	if (pRenderRot)
		pRenderRot->SetNextRender(NULL);
	delete pRenderRot;
	if( BSPTNumRot < 0)
	{
		pRenderRot = NULL;
		return false;
	}
	MQuadTree *pLocTree = (MQuadTree *)MFOREST.GetTree(BSPTNumRot);
	MBody *nextBody = pLocTree->GetNextMBody();
	pLocTree->SetNextMBody(NULL);

	pRenderRot = new GQuadRenderRot(*pLocTree);
	pRenderRot->FillAllArraysCh(*pLocTree);
	CreateTurnSkeleton();
	pRenderRot->SetCollided(IsCollided());

	pRenderRot->SetNextRender(next);
	pRenderRot->SetPreviousRender(previous);
	if (previous)
		previous->SetNextRender(pRenderRot);
	if (next)
		next->SetPreviousRender(pRenderRot);

	pLocTree->ClearModified();
	pLocTree->SetNextMBody(nextBody);
	return true;
}
bool NStock::CreateRender(void)
{
	bool RMill = CreateRenderMill();
	bool RRot = CreateRenderRot();
	return RMill || RRot;
}

bool NStock::CreateModifRender(void)
{
	bool RMill = CreateModifRenderMill();
	bool RRot = CreateRenderRot();
	return RMill || RRot;
}

bool NStock::IntersectBodyes(void)
{
	if (!TurnStockChanged)
		return true;
	TurnStockChanged = false;
	MQuadTreeRot *pTurnTree = (MQuadTreeRot *)GetBodyRot();
	if(!pTurnTree)
		return true;
	MQuadBody *pMillTree = (MQuadBody *)GetBodyMill();
	if(!pMillTree)
		return false;

	// Find max radius
	if(!pRenderRot)
		CreateRenderRot();
	double Rmax = pRenderRot->GetMaxX();
	// Calc a number of sectors
	int Ns = NSurfSettings::GetNumAproxLine(Rmax, 360.);

	bool res = pTurnTree->Combine(pMillTree, Ns);

	MFOREST.Compress();

	CreateRenderMill();

	return res;
}

bool NStock::MakeTurnFromMill(bool RecalcGabar, TurnGenMode Mode)
{
	if (!MillStockChanged)
		return true;
	MillStockChanged = false;

	if(!CreateRenderMill())
		return false;
	std::list<BClrContour> Contours;
	pRender->MakeEnvelopeCont(Contours, Mode);
	if(Contours.empty())
		return false;
	if (RecalcGabar)
	{
		Gabar.Init();
		for each (const auto& Cont in Contours)
		{
			BBox G;
			for each (const auto& P in Cont)
				G.Expand(P);
			Gabar.Expand(-G.Xmax, -G.Xmax, G.Ymin);
			Gabar.Expand(G.Xmax, G.Xmax, G.Ymax);
		}
	}

	MakeRotMulti(Contours);

	return true;
}

void NStock::MakeRot(const TBladeCont &Cont)
{
	int PCNum = ColorNum;
	if(ColorNum != STOCK_COLOR_NUM || !IsRealStock())
		PCNum = MElemIDProc::AddPC(MElemIDProc::MakePC(Color)) + RESERVED_PCS;	

	BPoint DeltaMin(-ST_DEC * 1.1, -ST_DEC * 1.1, -ST_DEC * 1.1, 0.);
	BPoint DeltaMax(ST_INC * 1.1, ST_INC * 1.1, ST_INC * 1.1, 0.);

	// Create body for turn
	BPoint Pn(Gabar.GetMinPoint());
	BPoint Px(Gabar.GetMaxPoint());
	BPoint MinPointRot((Pn.GetX() + Px.GetX()) * 0.5 + DeltaMin.GetX(), Pn.GetZ() + DeltaMin.GetZ(), -1., 1.);
	BPoint MaxPointRot(Px.GetX() + DeltaMax.GetX(), Px.GetZ() + DeltaMax.GetZ(), 1., 1.);
	if (this->IsKindOf(RUNTIME_CLASS(NStockBox)))
	{
		BBox NBox = Cont.GetGabar();
		BPoint Pn = NBox.GetMinPoint();
		BPoint Px = NBox.GetMaxPoint();
		MinPointRot.Set(Pn.GetX() + DeltaMin.GetX(), Pn.GetY() + DeltaMin.GetY(), -1., 1.);
		MaxPointRot.Set(Px.GetX() + DeltaMax.GetX(), Px.GetY() + DeltaMax.GetY(), 1., 1.);
	}
	// Create meshs for bar stock cylinder
	MeshArr MeshArray;
	MeshArray.RemoveAll();

	Cont.CrMesh4Turn(MeshArray, PCNum, PCNum, -1);

	int Num = (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) ?
		int(BDebugState::Get().CellSize + 0.5) :
		int(BDebugState::Get().DXNumber + 0.5);

	int Num1 = BDebugState::Get().UseSlicedTree ? 1 : Num;

	MQuadTreeRot *pTreeRot = new MQuadTreeRot(MinPointRot, MaxPointRot, Num, Num1, GetStockGr(), false);// SlicedTree

	for(int l = 0; l < MeshArray.GetSize(); ++l)
	{
		pTreeRot->StoreMesh(*MeshArray[l], MeshArray[l]->GetMeshID(), cadrID(-1), false);
		delete MeshArray[l];
	}
	BBox Bounds;
	Bounds.Expand(MinPointRot); 
	Bounds.Expand(MaxPointRot);
	pTreeRot->CreateBodyStd(Bounds,false, false);
	pTreeRot->Invert(false);
	pTreeRot->CrStandard(false);

	if(BSPTNumRot < 0)
		BSPTNumRot = MFOREST.AddTree(pTreeRot);
	else
	{
		MFOREST.DeleteTree(BSPTNumRot);
		MFOREST.SetTree(BSPTNumRot, pTreeRot);
	}
	CreateRenderRot();
}

void NStock::MakeRotMulti(const std::list<BClrContour>& Contours)
{
	BPoint DeltaMin(-ST_DEC * 1.1, -ST_DEC * 1.1, -ST_DEC * 1.1, 0.);
	BPoint DeltaMax(ST_INC * 1.1, ST_INC * 1.1, ST_INC * 1.1, 0.);

	// Create body for turn
	BPoint Pn(Gabar.GetMinPoint());
	BPoint Px(Gabar.GetMaxPoint());
	BPoint MinPointRot((Pn.GetX() + Px.GetX()) * 0.5 + DeltaMin.GetX(), Pn.GetZ() + DeltaMin.GetZ(), -1., 1.);
	BPoint MaxPointRot(Px.GetX() + DeltaMax.GetX(), Px.GetZ() + DeltaMax.GetZ(), 1., 1.);

	int Num = (NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) ?
		int(BDebugState::Get().CellSize + 0.5) :
		int(BDebugState::Get().DXNumber + 0.5);

	int Num1 = BDebugState::Get().UseSlicedTree ? 1 : Num;


	MQuadTreeRot* pTreeRot = new MQuadTreeRot(MinPointRot, MaxPointRot, Num, Num1, GetStockGr(), false);
	bool FirstCont = true;
	for (auto it = Contours.begin(); it != Contours.end(); ++it)
	{
		MeshArr MeshArray;
		MeshArray.CrMesh4Turn(*it, -1);
		for (int l = 0; l < MeshArray.GetSize(); ++l)
		{
			pTreeRot->StoreMesh(*MeshArray[l], MeshArray[l]->GetMeshID(), cadrID(-1), !FirstCont);
			delete MeshArray[l];
		}
		BBox Bounds;
		Bounds.Expand(MinPointRot);
		Bounds.Expand(MaxPointRot);
		pTreeRot->CreateBodyStd(Bounds, false, !FirstCont);
		if (FirstCont)
			pTreeRot->CrStandard(false);
		else
			pTreeRot->SafeMerge(MBSPInter());
		FirstCont = false;
	}
	pTreeRot->Invert(false);
	if (BSPTNumRot < 0)
		BSPTNumRot = MFOREST.AddTree(pTreeRot);
	else
	{
		MFOREST.DeleteTree(BSPTNumRot);
		MFOREST.SetTree(BSPTNumRot, pTreeRot);
	}
	CreateRenderRot();
}

bool NStock::TransformCutted(const BMatr & CuMatr)
{
//	NMovableObj::Transform(CuMatr.GetTran() * CuMatr.GetRot().invr());
	BMatr M = CuMatr;
	if (Type == AT_TURN || Type == AT_MILL_TURN)
		M = BMatr().RotX(0., 0., 0., 90.) * M * BMatr().RotX(0., 0., 0., -90.);
	int TNum = -1;
	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
		TNum = BSPTNumDX;
		break;
	case NGlobalState::BSP:
		TNum = BSPTNum;
		break;
	}
	MFOREST.Transform1Tree(TNum, M);
	MFOREST.Transform1Tree(BSPTNumRot, M);
	CreateRender();
	return false;
}

void NStock::SetCollided(bool Val)
{
	if (pRender != nullptr)
		pRender->SetCollided(Val);
	if (pRenderRot != nullptr)
		pRenderRot->SetCollided(Val);
	NCollidableObject::SetCollided(Val);
}

TurnGenMode NStock::GetTGMode()
{
	static TurnGenMode TGMode = TGM_EMPTY;
	if (TGMode == TGM_EMPTY)
	{
		NTiParams& Par = NCM_PROJECT.Defaults;
		CString Key(_T("Defaults/Stock/MillTurn/TurnStockGen@MODE"));
		CString Mode(Par.GetString(Key.GetBuffer(), _T("No")));
		if (Mode == _T("No"))
			TGMode = TGM_NO;
		else
			TGMode = (Mode == _T("ConnectedOnly")) ? TGM_CONLY : TGM_FULL;
	}
	return TGMode;
}



bool NStock::SetStockType(NCMApplicationType NewType)
{
	Type = NewType;
	if (Type == AT_MILL || Type == AT_TURN)
		State = Type;
	else
		State = AT_MILL;
	return true;
}

bool NStock::SetStockState(NCMApplicationType NewState)
{
	if(Type != AT_MILL_TURN)
		return false;

	if(State == NewState)
		return false;

	if(NewState == AT_MILL || NewState == AT_TURN)
		State = NewState;
	else
		return false;
	return true;
}

NCMApplicationType NStock::GetStockType(void)
{
	return Type;
}
NCMApplicationType NStock::GetStockState(void)
{
	if(Type != AT_MILL_TURN)
		return Type;
	return State;
}

void NStock::CrBody4Mill(void)
{
	MQuadTreeRot *pBodyRot = (MQuadTreeRot *)GetBodyRot();
	if(!pBodyRot)
		return;
	BPoint MinPointRot = pBodyRot->GetMin();
	BPoint MaxPointRot = pBodyRot->GetMax();
	int Num = pBodyRot->GetRowsNum();
	BBox Box;
	Box.Expand(BPoint(-MaxPointRot.GetX()
		, MinPointRot.GetY()
		, -MaxPointRot.GetX()
		, 1.));
	Box.Expand(BPoint(MaxPointRot.GetX() + 1.
		, MaxPointRot.GetY()
		, MaxPointRot.GetX() + 1.
		, 1.));


	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::DX:
	{
		MakeGabarBoxDX5X(Num);
		DXSubModel5X* pBodyDX = (DXSubModel5X*)MFOREST.GetTree(BSPTNumDX);
		if(pBodyDX != nullptr)
			pBodyDX->Reset();
	}
	break;
	case NGlobalState::BSP:
	{
		if (BSPTNum < 0)
			BSPTNum = MFOREST.AddTree(new MQuadTree(Box.GetMinPoint(), Box.GetMaxPoint(), Num, Num, GetStockGr(), false));
		else
		{
			MFOREST.DeleteTree(BSPTNum);
			MFOREST.SetTree(BSPTNum, new MQuadTree(Box.GetMinPoint(), Box.GetMaxPoint(), Num, Num, GetStockGr(), false));
		}
		break;
	}
	}
	TurnStockChanged = true;
	IntersectBodyes();
}

void NStock::RemoveMillBody(void)
{
	if (BSPTNum > -1)
	{
		MBody *pBody = MFOREST.GetTree(BSPTNum);
		pBody->SetNextMBody(NULL);
		MFOREST.DeleteTree(BSPTNum);
		BSPTNum = -1;
	}
	if (BSPTNumDX > -1)
	{
		MBody *pBody = MFOREST.GetTree(BSPTNumDX);
		pBody->SetNextMBody(NULL);
		MFOREST.DeleteTree(BSPTNumDX);
		BSPTNumDX = -1;
	}
}

void NStock::RemoveRotBody(void)
{
	if (BSPTNumRot > -1)
	{
		MBody *pBody = MFOREST.GetTree(BSPTNumRot);
		pBody->SetNextMBody(NULL);
		MFOREST.DeleteTree(BSPTNumRot);
		BSPTNumRot = -1;
	}
}

bool NStock::RenderSect(void)
{
	if (GetStockState() == AT_TURN)
	{
		BaseRender::SetWFMode();
		TurnSkeleton.Draw();
		BaseRender::RemoveWFMode();
	}
	return false;
}

const BGeomArray * NStock::GetSect(int Part) const
{
	return TurnSkeleton.GetSectGeom(Part);
}

bool NStock::CreateTurnSkeleton(void)
{
	if(GetStockType() != AT_TURN && GetStockType() != AT_MILL_TURN)
		return true;
	if(BSPTNumRot < 0)
		return false;
	TurnSkeleton.Create(*((MQuadTreeRot *)GetBodyRot()));
	return true;
}

double NStock::GetVolume()
{
	switch(Type)
	{
	case AT_TURN:
		if(!pRenderRot)
			if(!CreateRenderRot())
				return 0.;
		return TurnSkeleton.GetSectGeom(NTurnStockSkeleton::SkelContour1)->GetVolumeRot();
	case AT_MILL_TURN:
	case AT_MILL:
		if(!pRender)
			if(!CreateRenderMill())
				return 0.;
		if(!pRender)
			return 0.;
		return pRender->GetVolume();
	default:
		return 0.;
	}
}

void NStock::DrawSelected(void) const
{
	DrawGabar();
}

void NStock::DrawGabar(void) const
{
	glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
	auto& Attrs = GetSelectAttrs();
	glLineWidth(GLfloat(Attrs.GetBoxWidth()));
	if (Attrs.GetBox().GetA() == 0.)
		glColor3fv(GetColor().array);
	else
		glColor3fv(Attrs.GetBox().array);
	BaseRender::DrawBox(Gabar);
	glPopAttrib();
}

const BPoint NStock::RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	GBodyRender *pRender = GetRender();
	if(!pRender)
		return BPoint(0., 0., 0., -1.);
	return pRender->RayCasting(P, V, MinW, N);
}

int NStock::SubtractStl(const BStl & Stl)
{
	MQuadTree *pMillTree = (MQuadTree *)GetBodyMill();
	if(!pMillTree)
		return -1;

	const GLfloat *Triangles = Stl.GetTriangles();

	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint pn(Triangles[i],Triangles[i+1],Triangles[i+2],0.);
		pn = pn * (-1./sqrt(pn.D2()));
		BPoint p0(Triangles[i+3],Triangles[i+4],Triangles[i+5],1.);
		BPoint p1(Triangles[i+9],Triangles[i+10],Triangles[i+11],1.);
		BPoint p2(Triangles[i+15],Triangles[i+16],Triangles[i+17],1.);
		int n = MFOREST.AddPoints(pn);
		int b = MFOREST.AddPoints(p0);
		MFace Face( n, b);
		Face.SetArraySize(3);
		Face.SetPointAt(0, b);
		Face.SetPointAt(1, MFOREST.AddPoints(p1));
		Face.SetPointAt(2, MFOREST.AddPoints(p2));
		pMillTree->StoreFace(&Face, MElemIDProc::ID_NULL, cadrID(-1), true);
	}
	BBox Bounds;
	Bounds.Expand(pMillTree->GetMin());
	Bounds.Expand(pMillTree->GetMax());
	pMillTree->CreateBodyStd(Bounds, false, true);
	pMillTree->Invert(true);
	MBSPInter Inter;
	pMillTree->SafeMerge(Inter);

	return 0;
}
PropList * NStock::Create(PropList * List)
{
	List->Clear();

	PropSingle * h = new PropSingle();
	// Visibility
	h->ID=1;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	if(IsRealStock()) 
	{
		h->Comment=IDS_COMM_STOCK;
		h->Group=IDS_PROP_STOCK;
	}
	else 
	{
		h->Comment=IDS_COMM_MACH;
		h->Group=IDS_PROP_STL;
	}
	h->ST_Value= (IsVisible() ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	List->Add(h);
	if (!IsRealStock())
	{
		// Edges
		h = new PropSingle();
		h->ID=2;
		h->Name=IDS_EDGE;
		h->Type=1;
		h->Comment=IDS_COMM_MACH_EDGE;
		h->Group=IDS_PROP_STL;
		h->ST_Value = (SharpEdgesS ? IDS_TRUE : IDS_FALSE);
		h->For_Enum.Add(IDS_TRUE);
		h->For_Enum.Add(IDS_FALSE);
		List->Add(h);
	}
	// color
	if (!IsRealStock())
	{
		h = new PropSingle();
		h->ID=3;
		h->Name=IDS_COLOR;
		h->Type=3;
		if(IsRealStock()) 
		{
			h->Group=IDS_PROP_STOCK;
		}
		else 
		{
			h->Group=IDS_PROP_STL;
		}
		h->Comment=IDS_COMM_MODEL_COLOR;
		h->V_Value=GetColor().GetRGB();
		List->Add(h);
	}
	// Render mode
	if (!IsRealStock())
	{
		h = new PropSingle();
		h->ID=10;
		h->Name=IDS_DISPLAY_MODE;
		h->Type=1;
		h->Comment=IDS_COMM_MACH_PART_RENDER;
		h->Group=IDS_PROP_STL;
		h->ST_Value = GetRMode();
		h->For_Enum.Add(IDS_RM_SHADE);
		h->For_Enum.Add(IDS_RM_GLITTER);
		h->For_Enum.Add(IDS_RM_TRANSLUCENT);
		h->Color = 1;
		List->Add(h);
	}
	// wire frame
	h = new PropSingle();
	h->ID=11;
	h->Name=IDS_WIRE_FRAME;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART_WIRE;
	if(IsRealStock()) 
	{
		h->Group=IDS_PROP_STOCK;
	}
	else 
	{
		h->Group=IDS_PROP_STL;
	}
	h->ST_Value= (WFrame ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	List->Add(h);
	if(IsRealStock())
	{
	// Коэффициент точности для заготовки.
		h = new PropSingle();
		h->ID=8;
		h->Name=IDS_STOCK_COEF;
		h->Type=4;
		h->Comment=IDS_COMM_STOCK_COEF;
		h->Group=IDS_PROP_STOCK;
		h->Min=0.01;
		h->Max=100.;
		h->V_Value=GetWholeCellsKoef();
		h->Color = 1;
		List->Add(h);
	}
	// type
	if(IsRealStock())
	{
		//stock type and form
		//NStock *pStock = GetStock(0);
		//NStock::StockCrType sType = pStock->GetType();
		h = new PropSingle();
		h->ID = 13;
		h->Name = IDS_STOCK_TYPE;
		h->Type = 2;
		h->Comment = IDS_COMM_STOCK_TYPE;
		h->Group = IDS_PROP_STOCK;
		switch (GetType())
		{
		case NStock::SCT_EMPTY:
			h->Value = "SCT_EMPTY";
			break;
		case NStock::SCT_BOX:
			h->Value.LoadString(IDS_SCT_BOX);
			break;
		case NStock::SCT_BAR:
			if (GetStockType() == AT_MILL)
				h->Value.LoadString(IDS_SCT_BAR);
			else
				h->Value.LoadString(IDS_SCT_BAR);
			break;
		case NStock::SCT_BAR_SM:
			if (GetStockType() == AT_MILL)
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
	}
	else
	{
		h = new PropSingle();
		h->ID=12;
		h->Name=IDS_SMOOTHING;
		h->Type=1;
		h->Comment=IDS_COMM_MACH_SMOOTH;
		h->Group=IDS_PROP_STL;
		h->ST_Value= (GetDrawSmoothSurf() ? IDS_TRUE : IDS_FALSE);
		h->For_Enum.Add(IDS_TRUE);
		h->For_Enum.Add(IDS_FALSE);
		List->Add(h);
	}
	// File path
	CString FileName;
	if (IsKindOf(RUNTIME_CLASS(NStockStl)))
		FileName = ((NStockStl *)this)->GetFileName();
	else if(IsKindOf(RUNTIME_CLASS(NStockSweep)))
		FileName = ((NStockSweep *)this)->GetFileName();
	else if(IsKindOf(RUNTIME_CLASS(NStockCont)))
		FileName = ((NStockCont *)this)->GetFileName();
	else if(IsKindOf(RUNTIME_CLASS(NStockBSP)))
		FileName = ((NStockBSP *)this)->GetFileName();
	if(!FileName.IsEmpty())
	{
		h = new PropSingle();
		h->ID = 4;
		h->Name = IDS_PROP_STLFILE;
		h->Type = 2;
		h->Comment = IDS_COMM_PROP_STLFILE;
		if (IsRealStock())
		{
			h->Group = IDS_PROP_STOCK;
		}
		else
		{
			h->Group = IDS_PROP_STL;
		}
		h->Value = FileName;
		h->Color = 0;
		List->Add(h);
	}
	if (IsRealStock())
	{
	//IDC_STOCK_POS 
		h = new PropSingle();
		h->ID = 5;
		h->Name = IDS_PROP_STOCKPOS;
		h->Type = 5;
		h->Comment = IDS_COMM_PROP_STOCKPOS;
		h->V_Value = StockGr.Get();
		h->Color = 0;
		List->Add(h);
	}
	return List;
}

PropList * NStock::Change(PropList *List, PropSingle *Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_STOCKS;
	switch(id)
	{
	case 1: // IDS_VISIBILITY
		Visible = (Single->ST_Value == IDS_TRUE);
		AfxGetMainWnd()->SendMessage(WM_APP_STOCK_VISIBLE);
		break;
	case 2: // Edges
		SharpEdgesS = (Single->ST_Value == IDS_TRUE);
		CreateRender();
		break;
	case 3: // Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetColor(Col);
			CreateRender();
			break;
		}
	case 4:
		break;
	//case 5: //IDC_STOCK_POS 
	//	SetPosition(Single->V_Value);
	//	break;
	case 8: // Коэффициент точности
		SetWholeCellsKoef(Single->V_Value);
		break;
	/*case 9: // Polychrome
		MElemIDProc::SetAllowPCPalUse(Single->ST_Value == IDS_POLYCHROME);
		CreateRender();
		break;*/
	case 10: // Render mode
		SetRMode(RenderMode(Single->ST_Value));
		break;
	case 11: // wire frame
		WFrame = (Single->ST_Value == IDS_TRUE);
		break;
	case 12: // smooth surf
		SetDrawSmoothSurf(Single->ST_Value == IDS_TRUE);
		break;
	}
	return 	Create(List);
}

void NStock::SetRMode(RenderMode Mode)
{
	RMode = Mode;
	if(RMode == RM_TRANSLUCENT)
	{
		SetAlpha(Transparency);
		CreateRender();
	}
}

bool NStock::IsRealStock(void) const
{
	return GetGroup() == OG_STOCKS;
//	return (BSPTNumRot >= 0) || (BSPTNum >= 0) || (BSPTNumDX >= 0);
}

bool NStock::NeedDraw(enum RenderMode GlobRMode, enum RenderMode ExtRMode)
{
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

bool NStock::ExportStC(void)
{
	if(!pRenderRot)
		CreateRenderRot();
	if(!pRenderRot)
		return 1;

	const BGeomArray* pGeomArray = TurnSkeleton.GetSectGeom(NTurnStockSkeleton::SkelContour1);
	BGeomArray BufArray;
	for (int i = 0; i < pGeomArray->GetSize(); i++)
	{
		NCadrGeom* pCadr = new NCadrGeom(*pGeomArray->GetAt(i));
		pCadr->MoveToXY();
		pCadr->RepairArc();
		BufArray.Add(pCadr);
	}
	StContOut Export(&BufArray);

	return TurnSkeleton.IsSorted();
}

bool NStock::ExportDXF(void)
{
	if(!pRenderRot)
		CreateRenderRot();
	if(!pRenderRot)
		return 1;

	const BGeomArray* pGeomArray = TurnSkeleton.GetSectGeom(NTurnStockSkeleton::SkelContour1);
	BGeomArray BufArray;
	for (int i = 0; i < pGeomArray->GetSize(); i++)
	{
		NCadrGeom* pCadr = new NCadrGeom(*pGeomArray->GetAt(i));
		pCadr->MoveToXY();
		pCadr->RepairArc();
		BufArray.Add(pCadr);
	}
	DXFOut Export(&BufArray);

	return TurnSkeleton.IsSorted();
}

void NStock::MakeGabarBoxDX5X(int Num)
{
	BBox LBox = GetGabar();
	if (NCMComData::IsTurnEnabled())
		LBox.Transform(BMatr().RotX(0., 0., 0., -90.));
	LBox.Offset(+0.001, +0.001, +0.001);
	MakeBoxDX5X(Num, LBox);
}

void NStock::MakeBoxDX5X(int Num, const BBox &LBox)
{
	DXModel B;
	B.Set(LBox, int(GQuadRenderDX5X::GetWholeCellsNum() * WholeCellsKoef), Num, Num, Num);
	if (BSPTNumDX < 0)
	{
		DXWorld = B;
		BSPTNumDX = MFOREST.AddTree(new DXSubModel5X(DXWorld, GetStockGr()));
	}
	else
	{
		MBody *pBody = MFOREST.GetTree(BSPTNumDX);
		if (pBody->GetModelMode() != NGlobalState::DX)
		{
			MFOREST.DeleteTree(BSPTNumDX);
			BSPTNumDX = -1;
			DXWorld = B;
			BSPTNumDX = MFOREST.AddTree(new DXSubModel5X(DXWorld, GetStockGr()));
		}
		else
		{
			if (DXWorld != B)
			{
				DXWorld = B;
				((DXSubModel5X *)pBody)->ReCreate();
			}
			else
				((DXSubModel5X *)pBody)->MakeEmpty();
		}
	}
}

bool NStock::FindSegmentsWithRender(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr)
{
	const auto pRender = GetRender();
	if (pRender == nullptr)
		return false;
	// check for ropes
	if(pRender->GetRopes().IsEmpty())
		CreateRenderMill();
	auto Res = GetRender()->FindSegments(oSegments);
	const auto Rest = oSegments.size() - oMatrsInvr.size();
	for(int i = 0; i < Rest; ++i)
		oMatrsInvr.push_back(BMatr().SetE());
	return Res;
}

MQuadBody* NStock::Load1Tree(CFile& f, bool GabarOnly/* = false*/)
{
	int LoadedVersion;
	f.Read(&LoadedVersion, sizeof(LoadedVersion));
	if (LoadedVersion != IOVersion)
		return nullptr;
	NGlobalState::ModelModes Mode = NGlobalState::MM_EMPTY;
	f.Read(&Mode, sizeof(Mode));
	MQuadBody* pQuadBody = nullptr;
	switch (Mode)
	{
	case NGlobalState::BSP:
		pQuadBody = new MQuadTree;
		break;
	case NGlobalState::DX:
		DXWorld.Load(&f);
		pQuadBody = new DXSubModel5X(DXWorld, BStockGrID());
		break;
	default:
		break;
	}
	if (GabarOnly)
	{
		pQuadBody->MBody::Load(&f);
		return pQuadBody;
	}
	if (Mode == NGlobalState::DX && NCM_PROJECT.GetGlobalState().GetModelMode() != NGlobalState::DX)
	{
		delete pQuadBody;
		pQuadBody = nullptr;
	}
	if (pQuadBody != nullptr)
		pQuadBody->Load(&f);
	return pQuadBody;
}

BOOL NStock::Save1Tree(CFile& f, int TreeInd, const BMatr& M)
{
	if (TreeInd < 0 || TreeInd >= MFOREST.GetTreeCount())
		return FALSE;
	f.Write(&IOVersion, sizeof(IOVersion));
	MBody* pBody = MFOREST.GetTree(TreeInd);
	auto pDXBody = dynamic_cast<DXSubModel*>(pBody);
	NGlobalState::ModelModes Mode = NGlobalState::BSP;
	if (pDXBody != nullptr)
		Mode = NGlobalState::DX;
	f.Write(&Mode, sizeof(Mode));
	if(Mode == NGlobalState::DX)
		DXWorld.Save2File(&f);
	MForestSave SaveData(&f, M);
	pBody->Save(SaveData);
	return TRUE;
}

