#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NToolCombined.h"
#include "BSpacePos.h"
#include "BProgram.h"
#include "NCUnit.h"
#include "NMachUnitPair.h"
#include "GQuadRenderMill.h"
#include "GAnimateCadrMill.h"
#include "NCollisionID.h"
#include "NCollTimeLine.h"
#include "GAnimateCadr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GAnimateCadr::GAnimateCadr(GBodyRender *piStart, const NSpeedPar* pPar)
	: GDynamicCadr(piStart), pSpeedPar(pPar)
{
	LargeArc = false;
	Surplus = 0.;
	StepsNum = 0;
	CurStep = 0;
	CurTVal = 0.;
	CurTime = 0;
	LineType = true;
	pColl = nullptr;
}

GAnimateCadr::~GAnimateCadr(void)
{
}

void GAnimateCadr::Restart(void)
{
	if(!pEnd)
		return;
	CurStep = 0;
	CurTVal = 0.;
	CurTime = 0;
	pStart->TransModifArrays(*pEnd);
	pEnd = NULL;
}
void GAnimateCadr::RestartFrom(GAnimateCadr& In)
{
	// pStart list contains ALL renders
	// In.pEnd list contains ALL rot OR ALL mill renders
	if (In.pEnd == nullptr || pStart == nullptr)
		return;
	pStart->TransModifArrays(*In.pEnd);
	In.pEnd = nullptr;
}
void GAnimateCadr::Recalc(void)
{
	if(StepsNum <= 0)
		return;
	double LocStep = pSpeedPar->IsTrackFeed() ? pSpeedPar->GetTimeStep() * pSpeedPar->GetTrackFeedScale() : GetCurLengthStep();
	double Length = pSpeedPar->IsTrackFeed() ? Geom.GetWholeTimeS() * 1000 : GetCadrLength(Geom);//pSpeedPar->GetTimeStep() in milliseconds
	const int NewStepsNum = max( 1, int(Length / LocStep));
	const double k = double(StepsNum) / NewStepsNum;
	CurStep = (CurStep * NewStepsNum) / StepsNum + 1;
	RecalcCurTVal();
	StepsNum = NewStepsNum;
	if(Geom.IsRotate())
	{
		CalcPlane4Rotate();
		return;
	}
	if(LineType)
	{
		BPoint e(TrGeom.GetE() - TrGeom.GetB());
		plane[3] = -(e * (TrGeom.GetB() * (1. - CurTVal) + TrGeom.GetE() * CurTVal));

	}
	else
	{
		BMatr Z90;
		Z90.rotg(90, BPoint(0., 0., 0., 1), TrGeom.GetN());
		BPoint e((TrGeom.GetI() * Z90) * ((TrGeom.IsCWArc()) ? 1 : -1));
		double ax, ay, angle;
		Geom.GetArcA(&ax, &ay, &angle); // We use Geom instead of TrGeom because GetArcA can't work with inclined plane
		if (fabs(angle) < MINAD)
			angle = 360.;
		BMatr Rot;
		Rot.rotg(angle * CurTVal, BPoint(0., 0., 0., 1.), TrGeom.GetN());
		e = (e * Rot);
		plane[0] = e.GetX();
		plane[1] = e.GetY();
		plane[2] = e.GetZ();
		plane[3] = - (e * TrGeom.GetC());
	}
}

void GAnimateCadr::PlanesRenderOne(GBodyRender* pLocStart, GBodyRender* pLocEnd, RenderMode GlobRMode, int Parm)
{
	if (pLocEnd == nullptr || pLocStart == nullptr)
		return;
	pLocEnd->SetCollided(pLocStart->IsCollided());

	glPushAttrib(GL_ENABLE_BIT);
	if (LargeArc)
	{
		glEnable(GL_CLIP_PLANE1);
		GLdouble bplane[4];
		if (CurTVal > 0.5)
		{
			bplane[0] = -plane1[0];
			bplane[1] = -plane1[1];
			bplane[2] = -plane1[2];
			bplane[3] = -plane1[3];
			glClipPlane(GL_CLIP_PLANE1, bplane);
			pLocEnd->RenderCh(GlobRMode, Parm, true);
		}
		else
		{
			bplane[0] = plane1[0];
			bplane[1] = plane1[1];
			bplane[2] = plane1[2];
			bplane[3] = plane1[3];
			glClipPlane(GL_CLIP_PLANE1, bplane);
			pLocStart->RenderWMaskCh(*pLocEnd, Parm, false);
		}
		bplane[0] *= -1.;
		bplane[1] *= -1.;
		bplane[2] *= -1.;
		bplane[3] *= -1.;
		glClipPlane(GL_CLIP_PLANE1, bplane);
	}
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);
	pLocStart->RenderWMaskCh(*pLocEnd, Parm, false);
	plane[0] *= -1.;
	plane[1] *= -1.;
	plane[2] *= -1.;
	plane[3] *= -1.;
	glClipPlane(GL_CLIP_PLANE0, plane);
	pLocEnd->RenderCh(GlobRMode, Parm, true);
	plane[0] *= -1.;
	plane[1] *= -1.;
	plane[2] *= -1.;
	plane[3] *= -1.;
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glPopAttrib();
}

void GAnimateCadr::RenderWAllData(const std::vector<class GAnimateCadrMill*>& DynBSPCadrs, const std::vector<GQuadRenderMill*>& MillBSPRenders
	, const std::vector<std::vector<GQuadRenderMill*>>& EndsForAllCadrs, enum RenderMode GlobRMode, int Type)
{
	for (int i = 0; i < MillBSPRenders.size(); ++i)
	{
		GQuadRenderMill* pLocStart = MillBSPRenders.at(i);
		if (pLocStart == nullptr)
			continue;

		// Set appropriate matrix
		const BStockGrID StGr = pLocStart->GetStockPos();
		glPushMatrix();
		BMatr M = MUPair.GetMach().GetStockMovedMatrSGr(StGr, true) * MUPair.GetMach().GetStockMovedMatr(0, true).invr();
		glMultMatrixd(M.GetArray());
		//END: Set appropriate matrix
		if (DynBSPCadrs.empty())
		{
			pLocStart->RenderCh(GlobRMode, Type, true);
		}
		else
		{
			GQuadRenderMill* pCommonMask = GQuadRenderMill::MakeCommonRender(i, EndsForAllCadrs);
			for (int l = 0; l < DynBSPCadrs.size(); ++l)
			{
				GAnimateCadrMill& ActCadr = *DynBSPCadrs.at(l);
				if (MUPair.GetMach().Conv2StPos(StGr) != ActCadr.Geom.GetStockPos())
				{// draw original state
					pLocStart->RenderCh(GlobRMode, Type, true);
				}
				else
				{
					if (pCommonMask == nullptr)// draw original except mask
					{
						pLocStart->RenderCh(GlobRMode, Type, true);
					}
					else
					{// draw original except mask
						pLocStart->RenderWMaskCh(*pCommonMask, Type, true);
						GQuadRenderMill* pLocEnd = EndsForAllCadrs.at(l).at(i);
						if (pLocEnd != nullptr)
						{
							if (ActCadr.CurTVal == 1.)
							{
								// draw final state
								pLocEnd->SetCollided(pStart->IsCollided());
								pLocEnd->RenderCh(GlobRMode, Type, true);
							}
							else
							{// Draw common part
								ActCadr.PlanesRenderOne(pLocStart, pLocEnd, GlobRMode, Type);
							}
						}
					}
				}
			}
			if (pCommonMask != nullptr)
			{
				pCommonMask->Clear();
				delete pCommonMask;
			}
		}

		glPopMatrix();
	}
}

GAnimateCadr::GAnimateCadr(const GAnimateCadr & In) : GDynamicCadr(nullptr)
{
	*this = In;
}

void GAnimateCadr::SetStart(GBodyRender *pS)
{
	pStart = pS;
}

void GAnimateCadr::CalcPlane4Rotate(void)
{
	if (StepsNum <= 0)
		return;
	// store state
	SCoordsDef CDef(0);
	SMachState MState(NULL);
	BMTLinkage Link;
	MUPair.GetMach().GetState(Link, MState, CDef);

	BMatr DirM;
	double t = GetGeomT();
	const SCoordsDef& FixedDef = MUPair.GetUnit().GetFixedDef();
	SMachState Begin(&FixedDef);
	Begin.SetCoord(0, Geom.GetB().GetX());
	Begin.SetCoord(1, Geom.GetB().GetY());
	Begin.SetCoord(2, Geom.GetB().GetZ());
	Begin.SetCoord(4, Geom.GetI().GetX());
	Begin.SetCoord(5, Geom.GetI().GetY());
	Begin.SetCoord(6, Geom.GetI().GetZ());
	SMachState End(&FixedDef);
	End.SetCoord(0, Geom.GetE().GetX());
	End.SetCoord(1, Geom.GetE().GetY());
	End.SetCoord(2, Geom.GetE().GetZ());
	End.SetCoord(4, Geom.GetN().GetX());
	End.SetCoord(5, Geom.GetN().GetY());
	End.SetCoord(6, Geom.GetN().GetZ());

	BPoint XYZ(0., 0., 0., 1.), ABC(0., 0., 0., 1.);
	CalcCurCoordDC(XYZ, ABC);
	BMatr MCS = MUPair.GetMach().GetMCS(&Geom, pLastTool, MUPair.GetUnit().GetFixedDef());
	XYZ = XYZ * MCS;
	MUPair.GetMach().SetState(Geom.GetMTLink(), SMachState(XYZ, ABC, &MUPair.GetUnit().GetFixedDef()));
	int Tur = 0;
	int Att = 0;
	if (pLastTool)
	{
		Tur = pLastTool->Turret;
		Att = pLastTool->Attach;
	}
	BMatr Stock2ToolMatr = MUPair.GetMach().GetStock2ToolMatr(Geom.GetStockPos(), Tur, Att);
	BPoint P = BPoint(0., 0., 0., 1.) * Stock2ToolMatr;
	MUPair.GetMach().SetState(Geom.GetMTLink(), SMachState(Geom.GetB(), Geom.GetI(), &MUPair.GetUnit().GetFixedDef()));
	MUPair.GetMach().TotalDerivative(DirM, Begin, End, 0., MUPair.GetUnit().GetCurStockPosition(), Tur, Att);
	BPoint e(P * DirM);
	plane[0] = e.GetX();
	plane[1] = e.GetY();
	plane[2] = e.GetZ();
	plane[3] = -(e * P);
	// restore state
	MUPair.GetMach().SetState(Link, MState);
}

void GAnimateCadr::RenderTop(RenderMode GlobRMode, int Type, const std::vector<GAnimateCadrMill*>& DynBSPCadrs, bool Collide)
{
	// make mill bsp renders vector and other renders vector
	std::vector<GQuadRenderMill*> MillBSPRenders;
	std::vector< GBodyRender*> OtherRenders;
	for (GBodyRender* pRender = pStart; pRender != nullptr; pRender = pRender->GetNextRender())
	{
		pRender->SetCollided(Collide);
		if (pRender->GetType() == AT_MILL && NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
			MillBSPRenders.push_back(static_cast<GQuadRenderMill*>(pRender));
		else
			OtherRenders.push_back(pRender);
	}
	for each (GBodyRender * pRender in OtherRenders)
	{
		const BStockGrID StPos = pRender->GetStockPos();
		glPushMatrix();
		BMatr M = MUPair.GetMach().GetStockMovedMatrSGr(StPos, true) * MUPair.GetMach().GetStockMovedMatr(0, true).invr();
		glMultMatrixd(M.GetArray());

		pRender->RenderCh(GlobRMode, Type, true);

		glPopMatrix();
	}
	if (!MillBSPRenders.empty())
	{
		// make convinient data structure
		std::vector<std::vector<GQuadRenderMill*>> EndsForAllCadrs;// first ind - cadr ind; All pEnd mill renders from all mill animate cadrs
		EndsForAllCadrs.resize(DynBSPCadrs.size());
		for(int i = 0; i < DynBSPCadrs.size(); ++i)
		{
			for (GBodyRender* pLocEnd = DynBSPCadrs.at(i)->pEnd; pLocEnd != nullptr; pLocEnd = pLocEnd->GetNextRender())
			{
				EndsForAllCadrs.at(i).push_back(static_cast<GQuadRenderMill*>(pLocEnd));
			}
			if (EndsForAllCadrs.at(i).size() != MillBSPRenders.size())
			{
				EndsForAllCadrs.at(i).resize(0);
				for(int k = 0; k < MillBSPRenders.size(); ++k)
					EndsForAllCadrs.at(i).push_back(nullptr);
			}
		}
		RenderWAllData(DynBSPCadrs, MillBSPRenders, EndsForAllCadrs, GlobRMode, Type);
	}
}

double GAnimateCadr::GetTimeS(void) const
{
	return Geom.GetStartTimeS() + Geom.GetWorkTimeS() * CurTVal;
}

double GAnimateCadr::GetGeomT() const
{
	if (!pSpeedPar->IsTrackFeed())
		return CurTVal;
	return Geom.RecalcT2Geom(CurTVal);
}

bool GAnimateCadr::CalcCurCoordDC(BPoint &XYZ, BPoint &ABC)
{
	if(StepsNum < 0)
		return false;
	double t = GetGeomT();
	if (Geom.IsRCP())
	{
		SMachState P(&MUPair.GetUnit().GetFixedDef());
		BPoint XYZ0, XYZ1, ABC0, ABC1;
		Geom.CalcCurCoord(0., XYZ0, ABC0);
		Geom.CalcCurCoord(1., XYZ1, ABC1);
		int Tur = 0;
		int Att = 0;
		if (pLastTool)
		{
			Tur = pLastTool->Turret;
			Att = pLastTool->Attach;
		}
		MUPair.GetMach().CalcIntState(Geom.GetRCPMask(), t, SMachState(XYZ0, ABC0, &MUPair.GetUnit().GetFixedDef())
			, SMachState(XYZ1, ABC1, &MUPair.GetUnit().GetFixedDef()), P, Geom.GetStockPos(), Tur, Att);
		XYZ.Set(P.GetCoord(0), P.GetCoord(1), P.GetCoord(2), 1.);
		ABC.Set(P.GetCoord(3), P.GetCoord(4), P.GetCoord(5), 1.);
	}
	else
		Geom.CalcCurCoord(t, XYZ, ABC);
	return true;
}

bool GAnimateCadr::CalcCurCoordMCS(BPoint & XYZ, BPoint & ABC)
{
	if (StepsNum < 0)
		return false;
	double t = GetGeomT();
	if (Geom.IsRCP())
	{
		SMachState P(&MUPair.GetUnit().GetFixedDef());
		BPoint XYZ0, XYZ1, ABC0, ABC1;
		Geom.CalcCurCoordMCS(0., MCS, XYZ0, ABC0);
		Geom.CalcCurCoordMCS(1., MCS, XYZ1, ABC1);
		int Tur = 0;
		int Att = 0;
		if (pLastTool)
		{
			Tur = pLastTool->Turret;
			Att = pLastTool->Attach;
		}
		MUPair.GetMach().CalcIntState(Geom.GetRCPMask(), t, SMachState(XYZ0, ABC0, &MUPair.GetUnit().GetFixedDef())
			, SMachState(XYZ1, ABC1, &MUPair.GetUnit().GetFixedDef()), P, Geom.GetStockPos(), Tur, Att);
		XYZ.Set(P.GetCoord(0), P.GetCoord(1), P.GetCoord(2), 1.);
		ABC.Set(P.GetCoord(3), P.GetCoord(4), P.GetCoord(5), 1.);
	}
	else
		Geom.CalcCurCoordMCS(t, MCS, XYZ, ABC);
	return true;
}

double GAnimateCadr::GetCurParam(void) noexcept
{
	if(Geom.IsCycle())
		return -1.;

	double t = GetGeomT();
	return 0.5 + tPart * (t - 0.5);
	// tPart may be +1 or -1 only for mill processing
}
void GAnimateCadr::CalcCurAddCoord(SMachState &AddCoord, double t)
{
	pProg->GetAddCoord(Geom.GetMSHistNum(), AddCoord);
	if(!(Geom.GetType().flags & CU_ADDC))
	{// Additional coordinates are not changing in this cadr
		return;
	}
	// Additional coordinates change in this cadr
	SMachState End(AddCoord);
	SMachState Start(AddCoord);
	pProg->GetAddCoordPrev(Geom.GetMSHistNum(), Start);
	AddCoord = Start * (1. - t) + End * t;
}

double GAnimateCadr::GetCadrLength(const NCadrGeom &InGeom) const
{
	if( InGeom.GetType().type == addcoord )
	{
		SMachState EndS(pCoordsDef);
		pProg->GetAddCoord(Geom.GetMSHistNum(), EndS);
		SMachState StartS(pCoordsDef);
		pProg->GetAddCoordPrev(Geom.GetMSHistNum(), StartS);
		double Length = InGeom.GetCadrLength(&StartS, &EndS);
		if(Length != 0.)
		{
			if(pSpeedPar->IsTrackFeed())
				Length = 1.;
		}
		return Length;
	}
	return InGeom.GetCadrLength(nullptr, nullptr);
}

void GAnimateCadr::IncrCurTVal()
{
//	pColl = nullptr;
	MainTime PrevTime = CurTime;// time of this step start
	++CurStep;
	RecalcCurTVal();
	// CurTime is the time of this step end now
	pColl = nullptr;
	const auto pCurColl = pTimeLine->GetCollID(PrevTime);
	if (pCurColl != nullptr)
	{
		MainTime NextTime = pCurColl->PTPoint.GetTime();// time of the first collision after PrevTime
		if (NextTime < CurTime)
		{
			--CurStep;
			CurTVal = Geom.GetWorkTime() == 0 ? 1. : double(NextTime - (Geom.GetStartTime() + Geom.GetWaitTime0())) / Geom.GetWorkTime();
			CurTime = NextTime + 1;// + 1 - to prevent looping
			pColl = pCurColl;
		}
	}
	if(pColl == nullptr)
		pColl = &pTimeLine->GetCollIDByTime(CurTime);
}

void GAnimateCadr::RecalcCurTVal()
{
	CurTVal = (StepsNum <= 0 || CurStep == StepsNum) ? 1. : (double(CurStep) / StepsNum);// ensure CurTVal is equal to 1 exactly when 
	CurTime = Geom.GetStartTime() + Geom.GetWaitTime0() + MainTime(Geom.GetWorkTime() * CurTVal);
}