#include "stdafx.h"
#include "ConstDef.h"
#include "TFascia.h"
#include "NToolsCrTurn.h"
#include "math.h"
#include "ntoolturnblade.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBlade, NToolTurn, 0)

NToolTurnBlade::NToolTurnBlade(void)
{
	Thickness = (MM_INCH == M_MM) ? 4.76 : 0.188;
	Slope = 5.;
	Angle1 = 105.;
	Angle2 = 45.;
	Length1 = (MM_INCH == M_MM) ? 2. : 0.1;
	Length2 = (MM_INCH == M_MM) ? 1. : 0.05;
	SetType(50);// Turn tool
}
NToolTurnBlade::NToolTurnBlade(const CString &name) : NToolTurn(name)
{
	Thickness = (MM_INCH == M_MM) ? 4.76 : 0.188;
	Slope = 5.;
	SetType(50);// Turn tool
}

NToolTurnBlade::~NToolTurnBlade(void)
{
}

void NToolTurnBlade::Draw(RenderMode GlobRMode, int step)
{
	if(!IsVisible())
		return;

	Color.A() = float(Transparency);
	ColorPassive.A() = float(Transparency);
	glColor4fv(Color.array);
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
	glLineWidth(1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);
	glPushMatrix();
	glRotated(-90., 1., 0., 0.);// NT
	glRotated(-90., 0., 0., 1.);// NT
	TBladeCont *pFirstCont = new TBladeCont;
	pFirstCont->MakeCopy(BCont);// To remove short edges
	glColor3d(0.,0.,0.);
	pFirstCont->DrawLines();
	TFascia Fas;
	TBladeCont *pAddCont = Fas.Create(pFirstCont, Thickness / fabs(cos((180. - Slope) * PI / 180.)), 180. - Slope);
	if(pAddCont)
	{
		glColor3d(0.,0.,0.);
		if (GlobRMode != RM_TRANSLUCENT)
			Fas.DrawLines();
		if (!WFrame && (GlobRMode == RM_TRANSLUCENT && GetRMode() == RM_TRANSLUCENT || GetRMode() != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT))
			Fas.Draw(Color, ColorPassive);
		if (GlobRMode != RM_TRANSLUCENT)
			pAddCont->DrawLines();
		glColor4fv(ColorPassive.array);
		if (!WFrame && (GlobRMode == RM_TRANSLUCENT && GetRMode() == RM_TRANSLUCENT || GetRMode() != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT))
			pAddCont->Draw();
		delete pAddCont;
	}
	pAddCont = Fas.Create(pFirstCont, Length1, Angle1);
	if(pAddCont)
	{
		glColor3d(0.,0.,0.);
		if (GlobRMode != RM_TRANSLUCENT)
			Fas.DrawLines();
		if (!WFrame && (GlobRMode == RM_TRANSLUCENT && GetRMode() == RM_TRANSLUCENT || GetRMode() != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT))
			Fas.Draw(Color, ColorPassive);
		pAddCont->RemoveShortEdges();
		TBladeCont *pBuf = pAddCont;
		if (GlobRMode != RM_TRANSLUCENT)
			pAddCont->DrawLines();
		pAddCont = Fas.Create(pAddCont, Length2, Angle2);
		if(pAddCont)
		{
			glColor3d(0.,0.,0.);
			if (GlobRMode != RM_TRANSLUCENT)
				Fas.DrawLines();
			if (!WFrame && (GlobRMode == RM_TRANSLUCENT && GetRMode() == RM_TRANSLUCENT || GetRMode() != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT))
				Fas.Draw(ColorPassive, ColorPassive);
			if (GlobRMode != RM_TRANSLUCENT)
				pAddCont->DrawLines();
			glColor4fv(ColorPassive.array);
			if (!WFrame && (GlobRMode == RM_TRANSLUCENT && GetRMode() == RM_TRANSLUCENT || GetRMode() != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT))
					pAddCont->Draw();
			delete pAddCont;
			delete pBuf;
		}
	}
	glPopAttrib();
	glPopMatrix();
	delete pFirstCont;
}
void NToolTurnBlade::MakeBladeMesh(MeshArr& RMeshArr, bool IsToolCutting, bool IsShank) const
{
	int StartMeshN = RMeshArr.GetSize();
	TBladeCont* pFirstCont = new TBladeCont;
	pFirstCont->MakeCopy(BCont);// To remove short edges
	TFascia Fas;
	TBladeCont* pAddCont = Fas.Create(pFirstCont, Thickness / fabs(cos((180. - Slope) * PI / 180.)), 180. - Slope);
	if (pAddCont)
	{
		Fas.MakeMesh(RMeshArr);
		pAddCont->MakeMesh(RMeshArr);
		delete pAddCont;
	}
	pAddCont = Fas.Create(pFirstCont, Length1, Angle1);
	if (pAddCont)
	{
		Fas.MakeMesh(RMeshArr);
		pAddCont->RemoveShortEdges();
		TBladeCont* pBuf = pAddCont;
		pAddCont = Fas.Create(pAddCont, Length2, Angle2);
		if (pAddCont)
		{
			Fas.MakeMesh(RMeshArr);
			pAddCont->MakeMesh(RMeshArr);
			delete pAddCont;
			delete pBuf;
		}
	}
	delete pFirstCont;
	for (int i = StartMeshN; i < RMeshArr.GetSize(); ++i)
	{
		auto pMesh = RMeshArr.GetAt(i);
		pMesh->CuttingTool = IsToolCutting;
		pMesh->Shank = IsShank;
	}
}
const TBladeCont *NToolTurnBlade::GetBladeCont() const
{
	if(!Cutting)
		return NULL;
	return &BCont;
}
void NToolTurnBlade::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTurn)))
		return;// error

	NToolTurn::FillPage(pPage);

	NToolsCrTurn *pLoc = (NToolsCrTurn *)pPage;

	pLoc->m_name = Name;
	pLoc->m_Xdisp = Displacement.GetX();
	pLoc->m_Zdisp = Displacement.GetY();
	pLoc->m_MainAngle = MainAngle;
}
void NToolTurnBlade::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTurn)))
		return;// error

	NToolTurn::FillFromPage(pPage);

	NToolsCrTurn *pLoc = (NToolsCrTurn *)pPage;

	Name = pLoc->m_name;
	Displacement.Set(pLoc->m_Xdisp, pLoc->m_Zdisp, 0., 0.);
	MainAngle = pLoc->m_MainAngle;
}
NToolTurnBlade* NToolTurnBlade::CreateCopyTool()
{
	return NULL;
}

void NToolTurnBlade::CreateCopyTo(NToolTurnBlade *pCreatedTool)
{
	NToolTurn::CreateCopyTo(pCreatedTool);
	pCreatedTool->BCont.MakeCopy(BCont);
	pCreatedTool->Thickness = Thickness ;
	pCreatedTool->Slope = Slope;
}
void NToolTurnBlade::Serialize(CArchive &ar)
{
	NToolTurn::Serialize(ar);

	SerializeElements(ar, &Angle1, 1);
	SerializeElements(ar, &Angle2, 1);
	SerializeElements(ar, &Length1, 1);
	SerializeElements(ar, &Length2, 1);
	SerializeElements(ar, &Slope, 1);
	SerializeElements(ar, &Thickness, 1);
	BCont.Serialize(ar);
}


void NToolTurnBlade::FillGabar(void)
{// Контур уже пересчитан с учетом MainAngle и Displacement
	BMatr M;
	M.Set(	0., 0., 1., 0.,
			1., 0., 0., 0.,
			0., 1., 0., 0.,
			0., 0., 0., 1.);
	Gabar.Init();
	for(TBladeEdge *pCur = BCont.GetStart(); pCur != NULL; pCur = BCont.GetNext(pCur))
	{
		Gabar.Expand(pCur->GetPoint() * M);
		Gabar.Expand((pCur->GetPoint() - BPoint(0.,0.,1.,0.) * Thickness) * M);
	}
}

void NToolTurnBlade::MakeBContSkeleton()
{
	ToolSkel.RemoveAll();
	
	TBladeEdge* pPrev = BCont.GetStart()->GetPrev();// last point
	for (TBladeEdge* pCur = BCont.GetStart(); pCur != nullptr; pCur = BCont.GetNext(pCur))
	{
		NCadrGeom* pGeom = new NCadrGeom();
		if (pPrev->GetCircCenter().IsPoint())
		{// arc
			BPoint I = pPrev->GetCircCenter() - pPrev->GetPoint();
			BPoint V = pCur->GetPoint() - pPrev->GetPoint();

			NCadrGeom::Curve ArcType = (I % V).GetZ() > 0 ? cwarc : ccwarc;
			const BPoint& B = pPrev->GetPoint();
			// find last arc point
			for (; pCur != nullptr; pCur = BCont.GetNext(pCur))// for all sequential arcs with the same center
			{
				if ((pCur->GetCircCenter() - pPrev->GetCircCenter()).D2() > MIND * MIND)
					break;
			}
			const BPoint& E = pCur == nullptr ? BCont.GetStart()->GetPoint() : pCur->GetPoint();
			if (pCur == nullptr)
				pCur = BCont.GetStart()->GetPrev(); // to finsh loop on the next iteration
			pGeom->Set(ArcType, B.GetX(), B.GetY(), B.GetZ(), E.GetX(), E.GetY(), E.GetZ()
				, I.GetX(), I.GetY(), I.GetZ(), XY);
		}
		else
		{// line
			pGeom->Set(line, pPrev->GetPoint(), pCur->GetPoint());
		}
		ToolSkel.Add(pGeom);
		pPrev = pCur;
	}
}

