#include "stdafx.h"
#include "BObjectManager.h"
#include "BaseRender.h"
#include "BDebugState.h"
#include "ConstDef.h"
#include "MQuadTree.h"
#include "NMeshArray.h"
#include "NMakeStdMesh.h"
#include "NRectMesh.h"
#include "NParamMesh.h"
#include "MBSPInter.h"
#include "SOPlane.h"
#include "GQuadRenderRot.h"
#include "MElemIDProc.h"
#include "math.h"
#include "TBladeCont.h"
#include "MQuadTreeSliced.h"
#include "NStockCont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockCont, NStock, 0)

NStockCont::NStockCont() 
	: BaseCenter(0., 0., 0., 1.), Invert(false), FlipXY(false)
{
	IsCreated = false;
	pOrigRender = NULL;
	FileName.Empty();
}

void NStockCont::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStockCont();
	NStock::CreateCopyTo(stock);

	NStockCont *pCont = (NStockCont*)stock;
	pCont->BaseCenter = this->BaseCenter;
	pCont->Invert = this->Invert;
	pCont->FlipXY = this->FlipXY;
	pCont->Cont.MakeCopy(this->Cont);
	pCont->FileName = this->FileName;
}

NStockCont::~NStockCont(void)
{
	delete pOrigRender;
}
void NStockCont::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	if(!NeedDraw(GlobRMode, ExtRMode))
		return;
	NStock::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);
	if(WFrame)
		BaseRender::SetWFMode();

	if (pOrigRender)
	{
		if (SharpEdgesS)
		{
			glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1., 1.);
		}
		pOrigRender->RenderCh(GlobRMode, 3, DrawSurf);//LATHE
		if (SharpEdgesS)
		{
			const auto Width = GetEdgesAttrs().GetEdgesWidth();
			if (Width > 0.01)
			{
				glLineWidth(GLfloat(Width));
				glColor3fv(GetEdgesAttrs().GetEdgesColor().array);
				RenderSect();
			}
			glPopAttrib();
		}
	}

	if(WFrame)
		BaseRender::RemoveWFMode();
	NStock::RestDrawOriginal();
}

void NStockCont::Serialize(CArchive &ar)
{
	NStock::Serialize(ar);
	SerializeElements(ar,&BaseCenter,1);
	Cont.Serialize(ar);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 5.0))
		SerializeElements(ar, &FlipXY, 1);
	if(ar.IsLoading())
	{
		Create();
		Reset();
	}
}
void NStockCont::Create()
{
	NStock::Create();
	Gabar.Init();
	BBox G = Cont.GetGabar();
	double Xmax = max(fabs(G.Xmin), fabs(G.Xmax));
	Gabar.Expand(- G.Xmax,- G.Xmax, G.Ymin);
	Gabar.Expand(  G.Xmax,  G.Xmax, G.Ymax);
	Gabar.Transform(GetMatr());
	IsCreated = true;
}
void NStockCont::Create( NCadrGeom *GeomArr, int Num, const BPoint &iBase, const CString &iFileName, bool iFlipXY)
{
	IsCreated = false;
	FlipXY = iFlipXY;
	if(Num < 3)
		return;
	BaseCenter = iBase;
	if(!Cont.MakeContour(GeomArr, Num))
		return;
	int sign = Cont.GetXSign(BMatr());
	if(sign == 0)
		return;
	if(sign < 0)
		Cont.Trans(BMatr().RotY(BPoint(0.,0.,0.,1.), -180.));
	Cont.Trans(BMatr().Trans(BPoint(0.,0.,0.,1), BaseCenter));

	Create();
	if(IsCreated)
		FileName = iFileName;
}
bool NStockCont::CreateBody(void)
{
	SetStockType(AT_TURN);
	MakeRot(Cont);
	if(NCMComData::IsMillEnabled() && IsRealStock())
	{
		SetStockType(AT_MILL_TURN);
		// Create body for mill
		CrBody4Mill();
		CreateRenderMill();
	}
	else
	{
		RemoveMillBody();
	}
	MillStockChanged = false;
	TurnStockChanged = false;
	return true;
}



const BPoint & NStockCont::GetBase(void)
{
	return BaseCenter;
}

void NStockCont::MoveBase(double x, double y, double z, bool iInvert)
{
	Invert = iInvert;
	BPoint v = BPoint(x, y, z, 1.) * BMatr().RotX(0., 0., 0., 90.) - BaseCenter;
	BMatr M;
	M.Trans(BPoint(0.,0.,0.,0.), v);
	if(Invert)
		M = BMatr().RotX(0., 0., 0., 180.) * M;
	Cont.Trans(M);
	BaseCenter.Set(x, y, z, 1.);
	Create();
}

bool NStockCont::CreateRenderRot(void)
{
	NStock::CreateRenderRot();

	delete pOrigRender;
	pOrigRender = new GQuadRenderRot(*pRenderRot);
	if (!IsRealStock())
		pOrigRender->SetStockRender(false);

	return pOrigRender != NULL;
}

const BPoint NStockCont::RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	if(!pOrigRender)
		return BPoint(0., 0., 0., -1.);
	return pOrigRender->RayCasting(P, V, MinW, N);
}

void NStockCont::SetColor(const NColor & Col)
{
	if (ColorNum != STOCK_COLOR_NUM || !IsRealStock())
		MElemIDProc::ReplacePC(Color, Col);
	NStock::SetColor(Col);
	CreateBody();
}

NStock::StockCrType NStockCont::GetType() const
{
	return SCT_CONT;
}

