// MTPart.cpp: implementation of the MTPart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PropList.h"
#include "MTPart.h"
#include "MTPartAxis.h"
#include "gl\gl.h"
#include "math.h"
#include "BMatrPair.h"
#include "BaseRender.h"
#include "MTPartVisProp.h"
#include "MTConfig.h"
#include "MTSpinAxis.h"
#include "RSpinning.h"
#include "mtpart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
IMPLEMENT_SERIAL(MTPart, NPassiveObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTPart::MTPart()
{
	CollisionMargin = 0.;
	Visible = true;
	WFrame = false;
	pSpinRend = nullptr;
	SpinRendVersion = 1;// RendVersion for STL is 0
}

MTPart::MTPart(const MTPartAxis *PAx) : MTPart()
{
	AxisName = PAx->GetName();
}

MTPart::~MTPart()
{
	Deleted();
	delete pSpinRend;
	pSpinRend = nullptr;
}

void MTPart::Draw(const MTConfig& Conf, enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSharpEdges, bool DrawSurf,  bool DrawSmoothSurf)
{
	if(!Visible)
		return;

	const MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
	if (pAxis == nullptr)
		return;
	
	if(IsCollided())
		glColor4fv(GetCollidedColor().array);
	else
		glColor4fv(Color.array);

	BMatr M = GetCollMatr(Conf);
	glPushMatrix();
	glMultMatrixd(M.GetArray());
	if (pAxis->IsSpinning())
	{
		const MTSpinAxis* pSpinAxis = GetSpinAncestor(Conf);
		if (pSpinAxis == nullptr)
			return;
		ReCreateSpinRand(pAxis, pSpinAxis);
		if (pSpinRend != nullptr)
			pSpinRend->Draw(GlobRMode, RMode, WFrame);
	}
	else
	{
		Stl.DrawOriginal(GlobRMode, RMode, WFrame, DrawSharpEdges, DrawSurf, DrawSmoothSurf);
	}
	glPopMatrix();
}

const MTSpinAxis* MTPart::GetSpinAncestor(const MTConfig& Conf) const
{
	MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
	if (pAxis == nullptr)
		return nullptr;
	if (!pAxis->GetCanBeSpinned())
		return nullptr;
	MTAxis* pSpAxis = pAxis;
	while (pSpAxis != nullptr && pSpAxis->GetType() != MTAxis::AT_SPIN)
		pSpAxis = Conf.GetAxis(pSpAxis->GetParent());
	MTSpinAxis* pSpinAxis = dynamic_cast<MTSpinAxis*>(pSpAxis);
	return pSpinAxis;
}

void MTPart::DrawGabar(const MTConfig& Conf)
{
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
	auto& Attrs = GetSelectAttrs();
	glLineWidth(GLfloat(Attrs.GetBoxWidth()));
	if(Attrs.GetBox().GetA() == 0.)
		glColor3fv(GetColor().array);
	else
		glColor3fv(Attrs.GetBox().array);
	glDisable(GL_BLEND);
	glPushMatrix();
	glMultMatrixd(GetMatr(Conf).GetArray());
	BaseRender::DrawBox(Stl.GetGabar());
	glPopMatrix();
	glPopAttrib();
}

void MTPart::LoadFromStl(const CString& n)
{
	delete pSpinRend;
	pSpinRend = nullptr;
	Stl.Clear();
	Stl.CreateEx(n);
}

const CString &MTPart::GetAxisName() const
{
	return AxisName;
}

void MTPart::SetAxisName(const CString &NewName)
{
	AxisName = NewName;
}//void MTPart::SetAxisName(CString AxisName)

const CString& MTPart::GetFile() const
{	
	return Stl.GetFileName();
}

BBox MTPart::GetGabar(const MTConfig& Conf) const
{
	BMatr M = GetCollMatr(Conf);
	if (pSpinRend != nullptr)
	{
		MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
		if (pAxis != nullptr)
			if (pAxis->IsSpinning())
			{
				BBox B = pSpinRend->GetGabar();
				B.Transform(M);
				return B;
			}
	}
	BBox B = Stl.GetGabar();
	B.Transform(M);
	return B;
}

const BMatr MTPart::GetMatr(const MTConfig& Conf) const
{
	MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
	if (pAxis == nullptr)
		 return BMatr();
	return pAxis->GetMatr();
}
BMatr MTPart::GetCollMatr(const MTConfig& Conf, const float*& Triangles, size_t& SRVersion)
{

	Triangles = nullptr;
	MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
	if (pAxis == nullptr)
		return BMatr();

	Triangles = GetTriangles(Conf, SRVersion);

	if (pAxis->IsSpinning())
	{
		const MTSpinAxis* pSpinAxis = GetSpinAncestor(Conf);
		if (pSpinAxis == nullptr)
			return BMatr();
		return pSpinAxis->GetMatr() * BMatr().rotg(90., pSpinAxis->GetBasePoint(), pSpinAxis->GetVectorY());
	}
	return pAxis->GetMatr();
}
BMatr MTPart::GetCollMatr(const MTConfig& Conf) const
{
	MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
	if (pAxis == nullptr)
		return BMatr();

	if (pAxis->IsSpinning())
	{
		const MTSpinAxis* pSpinAxis = GetSpinAncestor(Conf);
		if (pSpinAxis == nullptr)
			return BMatr();
		return pSpinAxis->GetMatr() * BMatr().rotg(90., pSpinAxis->GetBasePoint(), pSpinAxis->GetVectorY());
	}
	return pAxis->GetMatr();
}
void MTPart::SetColor(const NColor & Col)
{
	Color = Col;
}

NColor &MTPart::GetColor()
{
	return Color;
}

const GLfloat * MTPart::GetTriangles(const MTConfig& Conf, size_t& SRVersion)
{
	const MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
	if (pAxis == nullptr)
			return nullptr;
	if (pAxis->IsSpinning())
	{
		const MTSpinAxis* pSpinAxis = GetSpinAncestor(Conf);
		if (pSpinAxis == nullptr)
			return nullptr;
		ReCreateSpinRand(pAxis, pSpinAxis);
		SRVersion = SpinRendVersion;
		if (pSpinRend == nullptr)
			return nullptr;
		else
			return pSpinRend->GetTriangles();
	}
	SRVersion = 0;// STL
	return Stl.GetTriangles();
}

const BPoint MTPart::RayCasting(const MTConfig& Conf, const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	if(!Visible)
		return BPoint(0., 0., 0., -1.);
	BMatrPair M = GetCollMatr(Conf);
	BPoint R;
	if (pSpinRend != nullptr)
	{
		MTPartAxis* pAxis = static_cast<MTPartAxis*>(Conf.GetAxis(AxisName));
		if (pAxis != nullptr)
			if (pAxis->IsSpinning())
			{
				R = pSpinRend->RayCasting(P * M.Inv(), V * M.Inv(), MinW, N);
				if (R.GetH() < 0)
					return R;
				return R * M.Or();
			}
	}
	R = Stl.RayCasting(P * M.Inv(), V * M.Inv(), MinW, N);
	if(R.GetH() < 0)
		return R;
	return R * M.Or();
}

bool MTPart::FindSegmentsWithStl(const MTConfig& Conf, SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr)
{
    if(!IsVisible())
		return false;
	const auto Res = Stl.FindSegments(oSegments);
	if (Res)
	{
		const auto Rest = oSegments.size() - oMatrsInvr.size();
		BMatr MatrInvr = GetMatr(Conf).invr();
		oMatrsInvr.reserve(oMatrsInvr.size() + Rest);
		for (int i = 0; i < Rest; ++i)
			oMatrsInvr.push_back(MatrInvr);
	}
	return Res;
}

bool MTPart::IsActualSpinRend(bool CanMoveWhileSpin, const BMatr& RelMatr) const
{
	if (pSpinRend == nullptr)
		return false;
	if (!CanMoveWhileSpin)
		return true;
	return pSpinRend->CheckPrevState(RelMatr);
}

bool MTPart::ReCreateSpinRand(const MTPartAxis* pAxis, const MTSpinAxis* pSpinAxis)
{
	BMatr Rot;
	Rot.e0cong(pSpinAxis->GetVector(), BPoint(0., 0., 1., 0.));
	BMatr Tran;
	Tran.Trans(pSpinAxis->GetBasePoint(), BPoint(0., 0., 0., 1.));
	BMatr RelMatr(pAxis->GetMatr() * Tran * Rot);
	if (IsActualSpinRend(pAxis->GetCanMoveWhileSpin(), RelMatr))
		return false;
	delete pSpinRend;
	pSpinRend = new RSpinning;
	++SpinRendVersion;
	if (Stl.MakeSpinContours(RelMatr, pSpinRend->GetContours()))
	{
		pSpinRend->CreateRenders();
		pSpinRend->StorePrevState(RelMatr);
	}
	else
	{
		delete pSpinRend;
		pSpinRend = nullptr;
	}
	return true;
}

PropList * MTPart::Create(PropList * List)
{
	List->Clear();
	// Visibility
	PropSingle * h = new PropSingle();
	h->ID=1;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART;
	h->Group=IDS_PROP_MACH_PART;
	h->ST_Value = Visible ? IDS_TRUE : IDS_FALSE;
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	h->Color = 1;
	List->Add(h);
	// Edges
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_EDGE;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART_EDGE;
	h->Group=IDS_PROP_MACH_PART;
	h->ST_Value = (Stl.GetSharpEdges() ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	h->Color = 1;
	List->Add(h);
	// Smooth
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_SMOOTHING;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART_SMOOTH;
	h->Group=IDS_PROP_MACH_PART;
	h->ST_Value = (Stl.GetSmoothSurf() ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	h->Color = 1;
	List->Add(h);
	// Render mode
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_DISPLAY_MODE;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART_RENDER;
	h->Group=IDS_PROP_MACH_PART;
	h->ST_Value = Stl.GetRMode();
	h->For_Enum.Add(IDS_RM_SHADE);
	h->For_Enum.Add(IDS_RM_GLITTER);
	h->For_Enum.Add(IDS_RM_TRANSLUCENT);
	h->Color = 1;
	List->Add(h);
	// wire frame
	h = new PropSingle();
	h->ID=11;
	h->Name=IDS_WIRE_FRAME;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_PART_WIRE;
	h->Group=IDS_PROP_MACH_PART;
	h->ST_Value= (WFrame ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	List->Add(h);



	return List;
}

PropList * MTPart::Change(PropList *List, PropSingle *Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;
	switch(id)
	{
	case 1: // IDS_VISIBILITY
		Visible = (Single->ST_Value == IDS_TRUE);
		break;
	case 2: // Edges
		Stl.SetSharpEdges(Single->ST_Value == IDS_TRUE);
		break;
	case 3: // Smooth
		Stl.SetSmoothSurf(Single->ST_Value == IDS_TRUE);
		break;
	case 4: // Render mode
		Stl.SetRMode(RenderMode(Single->ST_Value));
		break;
	case 5: // Color
		{
			NColor Col;
			Col.SetRGB(Single->Color);
			SetColor(Col);
			break;
		}
	case 11: // wire frame
		WFrame = (Single->ST_Value == IDS_TRUE);
		break;
	}
	return 	Create(List);
}

MTPartVisProp *MTPart::GetPartProp()
{
	MTPartVisProp *pVisProp = new MTPartVisProp;

	pVisProp->Visible = Visible;
	pVisProp->WFrame = WFrame;
	pVisProp->SharpEdges = Stl.GetSharpEdges();
	pVisProp->SmoothSurf = Stl.GetSmoothSurf();
	pVisProp->RMode = Stl.GetRMode();
	return pVisProp;
}

void MTPart::SetPartProp(MTPartVisProp *pVisProp)
{
	Visible = pVisProp->Visible;
	WFrame = pVisProp->WFrame;
	Stl.SetSharpEdges(pVisProp->SharpEdges);
	Stl.SetSmoothSurf(pVisProp->SmoothSurf);
	Stl.SetRMode(pVisProp->RMode);
}
