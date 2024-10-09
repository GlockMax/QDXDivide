#include "NToolCombined.h"
#include "..\Common\NToolCombined.h"
#include "..\Common\NToolCombined.h"
// NToolCombined.cpp: implementation of the NToolCombined class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseRender.h"
#include "math.h"
#include "Constdef.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "NLine.h"
#include "NToolCombined.h"
#include "NToolShaped.h"
#include "NToolStd.h"
#include "NToolStdCC.h"
#include "NToolsConCrMillPage.h"
#include "NToolsConCrTurnPage.h"
#include "NToolStd.h"
#include "ntoolcombined.h"
#include "PropList.h"
#include "NCadrGeom.h"
#include "RenderMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolCombined, NPassiveObject, 0)

NToolPalette NToolCombined::Palet;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NToolCombined::NToolCombined() : 
	CompShift(0., 0., 0., 0.)
{
//	m_pTools = NULL;
	Name = _T("No name");
	num_tools = 0;
	PointSize = 5;
	Turret = 0;
	Attach = 0;
	Position = 1;
	LifeTime = 0.;
	IgnoreUpToolStockColl = false;
	double* pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("ToolMarker", pDouble))
		PointSize = int(*pDouble);

	GenInit();
}

NToolCombined::NToolCombined(const CString &name) : 
	CompShift(0., 0., 0., 0.)
{
//	m_pTools = NULL;
	Turret = 0;
	Attach = 0;
	Position = 1;
	Name = name;
	num_tools = 0;
	PointSize = 5;
	IsUsed = false;
	LifeTime = 0.;
	double *pDouble;
	if(NCM_PROJECT.DoubleParams.Lookup("ToolMarker", pDouble))
		PointSize = int(*pDouble);

	GenInit();
}

void NToolCombined::GenInit()
{
	Color.SetRGB(RGB(0,0,255));
	ColorPassive.SetRGB(RGB(0,255,255));
	ColorHolder = ColorPassive;
	NColor *pCol;
	if(NCM_PROJECT.Colors.Lookup("»нструментќправкаѕо”молчанию",pCol))
		ColorHolder = *pCol;
	if(NCM_PROJECT.Colors.Lookup("»нструментЌе–аб„астьѕо”молчанию",pCol))
		ColorPassive = *pCol;
	Visible = TRUE;
	WFrame = FALSE;
	Zdisp = 0.;
	IsNameAuto = true;
}

NToolCombined::~NToolCombined()
{
	ClearAll();
}

int compare_double(const void *a, const void *b)
{
	if(*(double*)a>*(double*)b)
		return -1;
	return (*(double*)a==*(double*)b) ? 0 : 1;
}

void NToolCombined::Serialize(CArchive &ar)
{
	pTools.Serialize(ar);
	Color.Serialize(ar);
	ColorPassive.Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.91")))
	{
		SerializeElements(ar, &IsNameAuto, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		ColorHolder.Serialize(ar);
	}
	SerializeElements(ar, &num_tools,1);
	SerializeElements(ar, &Name,1);
	SerializeElements(ar, &PointSize,1);
	SerializeElements(ar, &Visible,1);
	SerializeElements(ar, &WFrame,1);
	SerializeElements(ar, &Zdisp,1);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.3")))
	{
		if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.4")))
		{
			SerializeElements(ar, &Attach,1);
		}
		SerializeElements(ar, &Turret,1);
		SerializeElements(ar, &Position,1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.7")))
	{
		SerializeElements(ar, &CompShift, 1);
		SerializeElements(ar, &CompShiftAuto, 1);
	}
	else
	{
		CompShiftAuto = false;
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.5")))
		SerializeElements(ar, &LifeTime, 1);
	else
		LifeTime = 0;
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.9")))
		SerializeElements(ar, &IgnoreUpToolStockColl, 1);
}

// добавление инструмента к составному
int NToolCombined::AddTool(NTool *pTool)
{
	pTools.Add(pTool);
	num_tools = GetToolsNum();
	return num_tools;
}

bool NToolCombined::AddToolSafe(NTool *pTool)
{
	if(pTool->GetClass() != GetClass())
		return false;
	// place pTool above all existing tools
	double Zdisp = 0.;
	for (int i = 0; i < num_tools; i++)
	{
		const NTool* pLocTool = GetTool(i);
		const double d = pLocTool->GetZdisp() + pLocTool->GetGabar().dZ();
		if (d > Zdisp)
			Zdisp = d;
	}
	pTool->SetZdisp(Zdisp);
	pTools.Add(pTool);
	num_tools = GetToolsNum();
	return true;
}

// удаление инструмента из составного
bool NToolCombined::RemoveTool(int index)
{
	if (index < 0 || index >= num_tools || num_tools == 0)
		return false;
	if (num_tools == 1)
	{
		ClearAll();
		return true;
	}

	delete pTools[index];
	pTools.RemoveAt(index);
	num_tools = GetToolsNum();
	
	return true;
}

// удаление ссылки на инструмент 
// (сам инструмент остаЄтс€)
bool NToolCombined::RemoveReference(int index)// TO DELETE
{
	if (index < 0 || index >= num_tools || num_tools == 0)
		return false;
	pTools.RemoveAt(index);
	num_tools = GetToolsNum();
	return true;
}

// удаление всех инструментов из составного
void NToolCombined::ClearAll()
{
	Skeleton.SetSize(0);// pointers are deleted in ToolSkel
	for (int i = 0; i < num_tools; i++)
		delete pTools[i];
	pTools.RemoveAll();
	num_tools = GetToolsNum();
 }

void NToolCombined::Draw(RenderMode GlobRMode) const
{
	if (num_tools == LB_ERR || num_tools == 0 || (!NTool::IsShapeVisible() && !NTool::IsShapeVisibleHolder()))
	{
		BaseRender::SetWFMode();
		glColor4fv(Color.array);
		glPointSize((float)PointSize);
		glBegin(GL_POINTS);
			glVertex3d(0., 0., 0.);
		glEnd();
		BaseRender::RemoveWFMode();
	}
	else
	{
		GLenum errCode;
		if ((errCode = glGetError()) != GL_NO_ERROR)
		{
			AfxMessageBox("Draw 3:OpenGL error " + CString(gluErrorString(errCode)));
		}
		//		BaseRender::PhShader.ShaderON();
		int Step = pTools[0]->GetToolStep();
		for (int i = 0; i < num_tools; ++i)
		{
			NTool* pT = pTools.GetAt(i);
			if(!pT->NeedDraw(GlobRMode))
				continue;
			NTool::NColorExchanger ColEx;
			NColor buf;
			NColor bufPassive;
			if (IsCollided())
			{
				ColEx.Store(pT);
				pT->SetTempColors(GetCollidedColor(), GetCollidedColor());
			}

			pT->Draw(GlobRMode, Step);

			if (IsCollided())
				ColEx.Restore();
		}
	}
}

const CString &NToolCombined::GetName() const
{
	return Name;
}

BBox NToolCombined::GetGabar() const
{
	BBox ToolGabar;
	for (int i=0; i<num_tools; i++)
		ToolGabar.Expand(pTools[i]->Gabar);

	if(num_tools == 0 || !ToolGabar.IsDefined())
	{
		ToolGabar.Expand(0.,0.,0.);
		ToolGabar.Expand(1.,1.,1.);
	}
	return ToolGabar;
}

void NToolCombined::SetName(const CString& pName)
{
	Name = pName;
}

void NToolCombined::FillDialog(CDialog *pDial) const
{
	NToolsConCrPage *pD = (NToolsConCrPage *)pDial;
	
	pD->m_name = Name;
	pD->m_PointSize = PointSize;
	pD->m_IsNameAuto = IsNameAuto;
	pD->m_pos = Position;
	pD->m_pos_turret = Turret;
	pD->m_pos_attach = Attach;
	pD->m_pos_cshift_x = CompShift.GetX();
	pD->m_pos_cshift_y = CompShift.GetY();
	pD->m_pos_cshift_z = CompShift.GetZ();
	pD->m_pos_cshift_auto = CompShiftAuto;
	pD->m_LifeTime = LifeTime;
	pD->m_IgnoreUpToolStockColl = IgnoreUpToolStockColl;
}

// создание копии инструмента
NToolCombined* NToolCombined::CreateCopyTool() const
{
	return NULL;
}

InfoTool & NToolCombined::GetInfo()
{
	return Info;
}

BOOL NToolCombined::IsVisible()
{
	return Visible;
}

NTool * NToolCombined::GetTool(int n) const
{
	if(n < 0 || n >= num_tools)
		return NULL;
	return pTools[n];
}

NTool * NToolCombined::GetLowermostTool() const
{
	if(num_tools < 1)
		return NULL;
	NTool *pResTool = pTools[num_tools - 1];// Lowermost tool is the last tool in most cases
	for(int i = 0; i < num_tools - 1; ++i)
	{
		NTool *pCurTool = pTools[i];
		if(pCurTool->GetZdisp() < pResTool->GetZdisp())
			pResTool = pCurTool;
	}
	return pResTool;
}



void NToolCombined::Regenerate(void)
{
	for(int i = 0; i < num_tools; ++i)
		pTools[i]->Regenerate();
	Changed();
}

bool NToolCombined::NeedAppr(void) const
{
	for (int i = 0; i < num_tools; ++i)
	{
		if (pTools[i]->NeedAppr())
			return true;
	}
	return false;
}

bool NToolCombined::MakeApprTool()
{
	// Divide concave tool into convex parts
	for(int i = 0; i < num_tools; ++i)
	{
		if(pTools[i]->NeedAppr())
		{
			NToolShaped *pToolShaped = (NToolShaped *)pTools[i];
			NContour AppCont(pToolShaped->GetCont());
			delete pToolShaped;
			pTools.RemoveAt(i);
			CArray <BPoint, const BPoint &> *pCenters = new CArray <BPoint, const BPoint &>;
			AppCont.Approximate(pCenters);
			for(int k = 0, v = 0; k < AppCont.num; ++k)
			{
				NLine Line;
				AppCont.GetLine(k, Line);
				double h = Line.p[0].GetZ() - Line.p[3].GetZ();
				if(h < MIND)
					continue;
				double alf = atan((Line.p[0].GetX() - Line.p[3].GetX())/ h);
				alf = alf *180./PI;
				BPoint Center = (*pCenters)[k];
				double sign = ((Line.p[0] - Line.p[3]) % (Line.p[0] - Center)).GetY() < 0. ? -1. : 1.;// < 0 if concave
				NToolStd *pNewTool = NULL;
				if(Center.GetH() < 0.)
					pNewTool = new NToolStd(CString("AT"), 2. * Line.p[3].GetX(),h,alf,0.,0.,0.,0.,h);
				else
					pNewTool = new NToolStdCC(CString("AT"), 2. * Line.p[3].GetX(),h,alf,0.,0.,0.,0.,h
						, sign * sqrt(((*pCenters)[k] - Line.p[0]).D2()));
				pNewTool->SetType(10);
				pNewTool->SetZdisp(Line.p[3].GetZ());
				pNewTool->GenInit();
				pTools.InsertAt(i, pNewTool);
				++i;
			}
			delete pCenters;
			num_tools = GetToolsNum();
		}
	}
	return false;
}

double NToolCombined::GetProbeDiam()
{
	double Diam = 3.;
	if (num_tools < 1)
		return Diam;
	for (int i = 0; i < num_tools; ++i)
	{
		NTool *pTool = GetTool(i);
		if (!pTool)
			return Diam;
		if (pTool->IsProbe())
		{
			double Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height;
			pTool->GetStdParams
				(Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height);
			Diam = fmin(Diam, Diameter);
		}
	}
	Diam = fmax(Diam, 0.5);
	return Diam;
}

bool NToolCombined::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	if(num_tools < 1)
		return false;
	NTool *pTool = GetTool(0);
	for(int i = 1; i < num_tools; ++i)
	{
		NTool *pTTool = GetTool(i);
		if(pTTool->GetZdisp() < pTool->GetZdisp())
			pTool = pTTool;
	}
	if(!pTool)
		return false;

	return pTool->GetStdParams
		(Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height);
}

BMatr NToolCombined::GetCompMatr() const
{
	BPoint Vector(GetCompVect());
	return BMatr().Trans(Vector.GetX(), Vector.GetY(), Vector.GetZ());
}

BPoint NToolCombined::GetCompVect(void) const
{
	if(GetClass() == AT_MILL)
		return CompShift;

	BPoint tmp = CompShift * BMatr().RotY(0., 0., 0., 90.);
	return BPoint(0., 0., 0., 0.);
}

void NToolCombined::CalcCompShift(void)
{
	if(!CompShiftAuto)
		return;

	if(GetClass() == AT_MILL)
	{
		CompShift.Set(0., 0., -GetGabar().dZ(), 0.);
	}
}

void NToolCombined::SetChildrenColors(void)
{
	for(int i = 0; i < num_tools; ++i)
	{
		for (int i = 0; i < num_tools; ++i)
		{
			if (GetTool(i)->IsShank())
			{
				GetTool(i)->SetColor(ColorPassive);
				GetTool(i)->SetColorPassive(ColorPassive);
			}
			else if (GetTool(i)->IsCutting())
			{
				GetTool(i)->SetColor(Color);
				GetTool(i)->SetColorPassive(ColorPassive);
			}
			if (GetTool(i)->IsHolder())
			{
				GetTool(i)->SetColor(ColorHolder);
				GetTool(i)->SetColorPassive(ColorHolder);
			}
		}
	}

}
PropList * NToolCombined::Create(PropList * List)
{
	List->Clear();
	// Visibility tool
	PropSingle *h = new PropSingle();
	h->ID=10;
	h->Name=IDS_PROG_COMP;
	h->Type=1;
	h->Comment=IDS_COMM_VISIBILITY_COMTOOL_PATH;
	h->Group=IDS_PROP_ONETOOLS;
	h->ST_Value= (Visible ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	List->Add(h);
	// Wire frame
	h = new PropSingle();
	h->ID=13;
	h->Name=IDS_WIRE_FRAME;
	h->Type=1;
	h->Comment=IDS_COMM_COMTOOL_WIRE_FRAME;
	h->Group=IDS_PROP_ONETOOLS;
	h->ST_Value= (WFrame ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	List->Add(h);
	// Color 
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_COLOR_CUT_TOOL;
	h->Type=3;
	h->Comment=IDS_COMM_COLOR_CUT_TOOL;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value = Color.GetRGB();
	h->Color = 1;
	List->Add(h);
	// Color passive 
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_COLOR_NOTCUT_TOOL;
	h->Type=3;
	h->Comment=IDS_COMM_COLOR_NOTCUT_TOOL;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value = ColorPassive.GetRGB();
	h->Color = 1;
	List->Add(h);
	// Color holder
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_COLOR_HOLDER;
	h->Type=3;
	h->Comment=IDS_COMM_COLOR_HOLDER;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value = ColorHolder.GetRGB();
	h->Color = 1;
	List->Add(h);
	// Position
	h = new PropSingle();
	h->ID=6;
	h->Name=IDS_STATE_NAME_CurToolN;
	h->Type=5;
	h->Comment=IDS_COMM_PROP_POSITION;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value=Position;
	List->Add(h);
	// Attach
	h = new PropSingle();
	h->ID=7;
	h->Name=IDS_PROP_AXIS;
	h->Type=5;
	h->Comment=IDS_COMM_PROP_AXIS;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value=Attach;
	List->Add(h);
	// Turret
	h = new PropSingle();
	h->ID=8;
	h->Name=IDS_PROP_TURRET;
	h->Type=5;
	h->Comment=IDS_COMM_PROP_TURRET;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value=Turret;
	List->Add(h);
	// PointSize
	h = new PropSingle();
	h->ID=9;
	h->Name=IDS_PROP_MARKER;
	h->Type=5;
	h->Comment=IDS_COMM_PROP_MARKER;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value=PointSize;
	List->Add(h);
	// Time
	h = new PropSingle();
	h->ID=11;
	h->Name=IDS_PROG_TIME;
	h->Type=6;
	h->Comment=IDS_COMM_TIME_TOOL;
	h->Group=IDS_PROP_ONETOOLS;
	h->Time = GetInfo().Time;
	h->Color = 0;
	List->Add(h);
	// FastLength
	h = new PropSingle();
	h->ID=12;
	h->Name=IDS_PROP_LENGTH_FASTTOOL;
	h->Type=4;
	h->Comment=IDS_COMM_PROP_LENGTH_FASTTOOL;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value = GetInfo().FastLength;
	h->Color = 0;
	List->Add(h);
	// WorkLength
	h = new PropSingle();
	h->ID=12;
	h->Name=IDS_PROP_LENGTH_CUTTOOL;
	h->Type=4;
	h->Comment=IDS_COMM_PROP_LENGTH_CUTTOOL;
	h->Group=IDS_PROP_ONETOOLS;
	h->V_Value = GetInfo().WorkLength;
	h->Color = 0;
	List->Add(h);

	return List;
}

PropList * NToolCombined::Change(PropList * List,PropSingle * Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;

	switch(id)
	{
	case 3: // Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetColor(Col);
			AdditionalModes = N_SETTINGS;// N_SETTINGS required for text view regeneration with new tool color
		}
		break;
	case 4: // Color passive
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetColorPassive(Col);
			AdditionalModes = N_SETTINGS;
		}
		break;
	case 5: // Color holder
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetColorHolder(Col);
		}
		break;
	case 6: // Position
		Position = int(Single->V_Value);
		break;
	case 7: // Attach
		Attach = int(Single->V_Value);
		break;
	case 8: // Turret
		Turret = int(Single->V_Value);
		break;
	case 9: // Marker
		PointSize = int(Single->V_Value);
		break;
	case 10: // IDS_VISIBILITY
		Visible = (Single->ST_Value == IDS_TRUE);
		break;
	case 13: // Wire frame
		{
			WFrame = (Single->ST_Value == IDS_TRUE);
			for (int i = 0; i < num_tools; ++i)
				GetTool(i)->WFrame = WFrame;
		}
		break;
	}
	return 	Create(List);
}

void NToolCombined::SetColor(const NColor &Col)
{
	float Alf = Color.A();
	Color = Col;
	Color.A() = Alf;
	for (int i = 0; i < num_tools; ++i)
	{
		if (GetTool(i)->IsCutting())
			GetTool(i)->SetColor(Color);
	}
}
void NToolCombined::SetColorPassive(const NColor &Col)
{
	float Alf = ColorPassive.A();
	ColorPassive = Col;
	ColorPassive.A() = Alf;
	for (int i = 0; i < num_tools; ++i)
	{
		if (GetTool(i)->IsShank())
		{
			GetTool(i)->SetColor(ColorPassive);
			GetTool(i)->SetColorPassive(ColorPassive);
		}
		else if (GetTool(i)->IsCutting())
		{
			GetTool(i)->SetColorPassive(ColorPassive);
		}
	}
}
void NToolCombined::SetColorHolder(const NColor &Col)
{
	float Alf = ColorHolder.A();
	ColorHolder = Col;
	ColorHolder.A() = Alf;
	for (int i = 0; i < num_tools; ++i)
	{
		if (GetTool(i)->IsHolder())
		{
			GetTool(i)->SetColor(ColorHolder);
			GetTool(i)->SetColorPassive(ColorHolder);
		}
	}
}

void NToolCombined::SetIsUsed(bool In)
{
	IsUsed = In;
}

bool NToolCombined::GetIsUsed(void) const
{
	return IsUsed;
}

NToolCombined * NToolCombined::CreateOffsetTool(bool NonCuttingOnly) const
{
	NToolCombined *pOffsetTool = CreateCopyTool();
	for(int i = 0; i < num_tools; ++i)
	{
		NTool *pNewTool = pTools[i]->CreateOffsetTool(NonCuttingOnly);
		delete pOffsetTool->pTools[i];
		pOffsetTool->pTools[i] = pNewTool;
	}
	return pOffsetTool;
}

NToolCombined* NToolCombined::CreateTool4Cutting() const
{
	NToolCombined* pNewTool = nullptr;
	bool OffsetApplyed = false;
	if (NTool::GetToolStockCollision() == CWS_ALL)
	{
		pNewTool = CreateOffsetTool(true);
		OffsetApplyed = true;
	}
	if (NeedAppr())
	{
		if (!OffsetApplyed)
			pNewTool = CreateCopyTool();
		pNewTool->MakeApprTool();
	}
	return pNewTool;
}

void NToolCombined::RemoveEndBall(void)
{
	NToolStd *pTool = static_cast<NToolStd *>(GetLowermostTool());
	if (!pTool)
		return;
	if (pTool->IsBallNose())
		pTool->RemoveEndBall();
}

void NToolCombined::RestoreEndBall(void)
{
	NToolStd *pTool = static_cast<NToolStd *>(GetLowermostTool());
	if (!pTool)
		return;
	pTool->RestoreEndBall();
}

bool NToolCombined::IsLifeTimeDifferent(const NToolCombined *pOld) const
{
	bool Res = (GetLifeTime() != 0.);
	if (pOld == nullptr)
		return Res;
	if (pOld->GetLifeTime() != GetLifeTime())
		return true;
	if (!Res) // Both are equal to 0
		return false;
	return (Turret != pOld->Turret) || (Position != pOld->Position);
}

bool NToolCombined::IsVertComTool() const
{
	for (int i = 0; i < num_tools; ++i)
	{
		if (pTools[i]->IsVertTool())
			return true;
	}
	return false;
}

bool NToolCombined::IsConcave() const
{
	for (int i = 0; i < num_tools; ++i)
	{
		if (pTools[i]->IsConcave())
			return true;
	}
	return false;
}

BBox NToolCombined::GetProbeGabar() const
{
	BBox ToolGabar;
	for (int i = 0; i < num_tools; i++)
	{
		auto pTool = pTools[i];
		if(pTool->IsProbe())
			ToolGabar.Expand(pTool->Gabar);
	}
	return ToolGabar;
}

bool NToolCombined::IsIgnoreUpToolStockColl() const
{
	return NTool::GetIgnoreUpToolStockColl() || IgnoreUpToolStockColl;
}

void NToolCombined::DrawGabar(RenderMode GlobRMode) const
{
	if (GlobRMode == RM_TRANSLUCENT)
		return;
	glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
	auto& Attrs = GetSelectAttrs();
	glLineWidth(GLfloat(Attrs.GetBoxWidth()));
	if (Attrs.GetBox().GetA() == 0.)
		glColor3fv(GetColor().array);
	else
		glColor3fv(Attrs.GetBox().array);
	BaseRender::DrawBox(GetGabar());
	glPopAttrib();
}

const BGeomArray& NToolCombined::GetSkeleton()
{
	return Skeleton;
}

void NToolCombined::RemakeSkeleton()
{
	Skeleton.SetSize(0);// pointers are deleted in ToolSkel
	for (int i = 0; i < num_tools; ++i)
	{
		NTool* pTool = pTools[i];
		if (pTool == nullptr)
			continue;
		const BGeomArray& ToolSkel = pTool->GetToolSkel();
		Skeleton.Append(ToolSkel);
	}
}

const NToolCombined* NToolCombined::GetTestTool() const
{
	return nullptr;
}
