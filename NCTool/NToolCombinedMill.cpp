#include "stdafx.h"
#include "math.h"
#include "NLine.h"
#include "ResLocal.h"
#include "NToolsConCrMillPage.h"
#include "NToolMill.h"
#include "NToolStd.h"
#include "ntoolcombinedTurn.h"
#include "NToolTurnBladeMill.h"
#include "NToolShaped.h"
#include "NToolVert.h"
#include "ntoolcombinedmill.h"

IMPLEMENT_SERIAL(NToolCombinedMill, NToolCombined, 0)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NToolCombinedMill::NToolCombinedMill(void)
{
	CompShiftAuto = true;
}
NToolCombinedMill::NToolCombinedMill(const CString &Name)
 : NToolCombined(Name)
{
	CompShiftAuto = true;
}

NToolCombinedMill::~NToolCombinedMill(void)
{
}

NToolMill * NToolCombinedMill::GetTool(int n) const
{
	return (NToolMill *)NToolCombined::GetTool(n);
}
NToolCombinedMill* NToolCombinedMill::CreateCopyTool() const
{

	NToolCombinedMill *pCreatedTool = new NToolCombinedMill(Name);

	pCreatedTool->CompShift = CompShift;
	pCreatedTool->CompShiftAuto = CompShiftAuto;

	pCreatedTool->Zdisp = Zdisp;
	pCreatedTool->Color = Color;
	pCreatedTool->Visible = Visible;
	pCreatedTool->PointSize = PointSize;
	pCreatedTool->IsNameAuto = IsNameAuto;
	pCreatedTool->ColorPassive = ColorPassive;
	pCreatedTool->Position = Position;
	pCreatedTool->Turret = Turret;
	pCreatedTool->Attach = Attach;
	pCreatedTool->LifeTime = LifeTime;
	pCreatedTool->IgnoreUpToolStockColl = IgnoreUpToolStockColl;

	for (int i = 0; i < num_tools; ++i)
		pCreatedTool->AddTool(GetTool(i)->CreateCopyTool());

	return pCreatedTool;
}

void NToolCombinedMill::FillDialog(CDialog *pDial) const
{
//	ASSERT(pDial->IsKindOf(RUNTIME_CLASS(NToolsCrDial)));
	NToolCombined::FillDialog(pDial);

	NToolsConCrMillPage *pD = (NToolsConCrMillPage *)pDial;
	pD->SetToolCombined(this);
	
	pD->m_displacement = Zdisp;
}

void NToolCombinedMill::SetChildrenDispl(void)
{
}

void NToolCombinedMill::RestChildrenDispl(void)
{
}

BPoint NToolCombinedMill::GetCompVect(void) const
{
	return CompShift;
}

NToolCombinedTurn *NToolCombinedMill::Convert2TurnTool(void)
{
	NToolCombinedTurn *pToolComTurn = new NToolCombinedTurn(GetName());
	pToolComTurn->CompShift = CompShift;
	pToolComTurn->CompShiftAuto = CompShiftAuto;

	pToolComTurn->Zdisp = Zdisp;
	pToolComTurn->Color = Color;
	pToolComTurn->Visible = Visible;
	pToolComTurn->PointSize = PointSize;
	pToolComTurn->IsNameAuto = IsNameAuto;
	pToolComTurn->ColorPassive = ColorPassive;
	pToolComTurn->Position = Position;
	pToolComTurn->Turret = Turret;
	pToolComTurn->Attach = Attach;
	pToolComTurn->LifeTime = LifeTime;
	pToolComTurn->IgnoreUpToolStockColl = IgnoreUpToolStockColl;
	for(int i = 0; i < num_tools; ++i)
	{
		NToolTurnBladeMill *pToolTurnMill = new NToolTurnBladeMill;
		NToolMill *pToolMill = GetTool(i);
		pToolTurnMill->SetMillTool(pToolMill);
		pToolTurnMill->SetColors(pToolMill->GetColor(), pToolMill->GetColorPassive()); 
		if(pToolTurnMill->GenInit() != 0)
		{
			delete pToolTurnMill;
			continue;
		}
		pToolComTurn->AddTool(pToolTurnMill);
	}
	pTools.RemoveAll();
	num_tools = 0;
	return pToolComTurn;
}

bool NToolCombinedMill::MakeVertTool(NContour &ToolCont, bool CutCont)
{
	NToolVert *pVert = NToolVert::MakeTool(_T("VertTool"), "", ToolCont, CutCont);
	if (pVert != nullptr)
	{
		AddTool(pVert);
		return true;
	}
	return false;
}

bool NToolCombinedMill::MakeComplTool(const NToolCombinedMill* pTool)
{
	if (pTool == nullptr)
		return false;
	CompShift = pTool->CompShift;
	CompShiftAuto = pTool->CompShiftAuto;

	Zdisp = pTool->Zdisp;
	Color = pTool->Color;
	Visible = pTool->Visible;
	PointSize = pTool->PointSize;
	IsNameAuto = false;
	Name = _T("ComplTool");
	ColorPassive = pTool->ColorPassive;
	Position = pTool->Position;
	Turret = pTool->Turret;
	Attach = pTool->Attach;
	LifeTime = pTool->LifeTime;
	IgnoreUpToolStockColl = pTool->IgnoreUpToolStockColl;

	

	for (int i = 0; i < pTool->num_tools; ++i)
	{
		NTool *pSimTool = pTool->GetTool(i);
		if (pSimTool->IsKindOf(RUNTIME_CLASS(NToolVert)))
		{
			const NContour &ToolCont = static_cast<NToolVert *>(pSimTool)->GetCont();
			// Input contour is up -> down oriented
			// Divide ToolCont into convex parts
			NContour ConvCont;
			bool ContStart = true;
			for (int ip = 0; ip < ToolCont.num; ++ip)
			{
				NLine Line;
				ToolCont.GetLine(ip, Line);
				if (Line.p[0].GetZ() <= Line.p[3].GetZ())
					continue;

				// Make new tool
				NLine NewLine = Line;
				NewLine.p[3] = NewLine.p[0];
				NewLine.p[0].SetX(0.);
				ConvCont.Add(NewLine);
				ConvCont.SetTypeLine(0, HORIZONTAL);
				ConvCont.Add(Line);
				ConvCont.SetTypeLine(1, ToolCont.GetType(ip));
				NewLine = Line;
				NewLine.p[0] = NewLine.p[3];
				NewLine.p[3].SetX(0.);
				ConvCont.Add(NewLine);
				ConvCont.SetTypeLine(2, HORIZONTAL);
				ConvCont.SetMiddlePoints();
				NToolShaped *pShaped = new NToolShaped(_T("SHTool"), ConvCont, false);
				pShaped->GenInit();
				AddTool(pShaped);
				ConvCont.Clear();
			}
		}
		else
		{
			AddTool(pSimTool->CreateCopyTool());
			if (pSimTool->IsVertTool())
			{
				int NewInd = num_tools - 1;
				GetTool(NewInd)->SetCutting(false);
			}
		}
	}
	return true;
}

NTool* NToolCombinedMill::MakeConvTool(const NContour& iCont, bool CutCont)
{
	double Alf, H, DBot, R, BAlf, ELength, FLength;
	const bool IsStdTool = iCont.GetApt7(Alf, H, DBot, R, BAlf, ELength, FLength);
	if (IsStdTool && CutCont)
	{
		NToolStd* pToolStd = new NToolStd(DBot, H, Alf, R, BAlf, ELength, FLength, H);
		pToolStd->SetType(0);
		pToolStd->SetCutting(CutCont);
		pToolStd->SetZdisp(iCont.lns[0].GetZ() - H);
		pToolStd->GenToolName();
		if (pToolStd->GenInit() == 0)
			return pToolStd;
		else
			delete pToolStd;
	}
	NToolShaped* pShaped = new NToolShaped(_T("SHTool"), iCont, CutCont);
	pShaped->GenInit();
	return pShaped;
}

bool NToolCombinedMill::MakeShapedTools(NContour &ToolCont, bool CutCont)
{
	// Input contour is down -> up oriented
	if (ToolCont.num <= 0)
		return false;
	// Check last point
	BPoint P = ToolCont.lns.GetAt(ToolCont.lns.GetSize() - 1);
	if (fabs(P.GetX()) > 1.e-4)
	{
		P.SetX(0.);
		ToolCont.AddFollow(P, P, P);
		ToolCont.Types[ToolCont.num - 1] = HORIZONTAL;
	}
	// Check first line
	if (ToolCont.Types[0] != HORIZONTAL && ToolCont.Types[0] != SPHERE)
	{
		ToolCont.Types.InsertAt(0, HORIZONTAL);
		BPoint Sp = ToolCont.lns.GetAt(0);
		Sp.SetX(0.01);
		ToolCont.lns.InsertAt(3, Sp);
		ToolCont.lns.InsertAt(3, Sp);
		ToolCont.lns.InsertAt(3, Sp);
		++ToolCont.num;
	}
	ToolCont.SetMiddlePoints();
	// Divide ToolCont into convex parts
	NContour ConvCont;
	bool ContStart = true;
	for (int ip = 0; ip < ToolCont.num; ++ip)
	{
		NLine Line;
		ToolCont.GetLine(ip, Line);
		if (ContStart)
		{
			ConvCont.AddFirst(Line);
			ConvCont.SetTypeLine(ConvCont.num - 1, ToolCont.Types[ip]);
			ContStart = false;
		}
		else
		{ // Check rotation direction
			NLine PrevLine;
			ConvCont.GetLine(ConvCont.num - 1, PrevLine);
			BPoint Vp = PrevLine.GetDir(1) % Line.GetDir(0);
			if (Vp.GetY() <= 0.)
			{// Convex angle
				ConvCont.AddFollow(Line);
				ConvCont.SetTypeLine(ConvCont.num - 1, ToolCont.Types[ip]);
			}
			else
			{// Concave angle
			 // Make new tool
				NLine LastLine;
				ConvCont.GetLine(ConvCont.num - 1, LastLine);
				BPoint LastPoint = LastLine.p[3];
				BPoint AddPoint(LastPoint);
				AddPoint.SetX(0.);
				if (ConvCont.Types[ConvCont.Types.GetSize() - 1] == HORIZONTAL)
				{
					ConvCont.lns.SetAt(ConvCont.lns.GetSize() - 1, AddPoint);
				}
				else
				{
					ConvCont.AddFollow(AddPoint, AddPoint, AddPoint);
					ConvCont.SetTypeLine(ConvCont.num - 1, HORIZONTAL);
				}
				// Create tool!!!
				ConvCont.Invert();
				NTool* pNewTool = MakeConvTool(ConvCont, CutCont);
				if(pNewTool != nullptr)
					AddTool(pNewTool);
				ConvCont.Clear();
				if (ToolCont.Types[ip] == HORIZONTAL)
				{
					Line.p[0] = AddPoint;
					ConvCont.AddFirst(Line);
					ConvCont.SetTypeLine(ConvCont.num - 1, ToolCont.Types[ip]);
				}
				else
				{
					ConvCont.AddFirst(AddPoint, LastPoint, LastPoint, LastPoint);
					ConvCont.SetTypeLine(ConvCont.num - 1, HORIZONTAL);
					ConvCont.AddFollow(Line);
					ConvCont.SetTypeLine(ConvCont.num - 1, ToolCont.Types[ip]);
				}
			}
			if (ip == ToolCont.num - 1)// Very last line
			{
				ConvCont.Invert();
				NTool* pNewTool = MakeConvTool(ConvCont, CutCont);
				if (pNewTool != nullptr)
					AddTool(pNewTool);
			}

		}
	}
	return true;
}

