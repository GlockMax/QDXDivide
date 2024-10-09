// NTool.cpp: implementation of the NTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NFastColorMode.h"
#include "BaseRender.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "NContour.h"
#include "MElemIDProc.h"
#include "NToolsCrDial.h"
#include "NToolsCrDiskSheet.h"
#include "math.h"
#include "BPoint.h"
#include "BMatr.h"
#include "Gl\GLu.h"
#include "NToolShaped.h"
#include "RenderMode.h"
#include "NCollidableObject.h"
#include "NTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NTool, CObject, 0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Initialize the static members
BOOL NTool::ShapeVisibleTool = TRUE;
BOOL NTool::ShapeVisibleHolder = TRUE;
NFastColorMode NTool::FastColorMode = FCM_ALL;

CollisWithStock NTool::ToolStockCollision = CWS_OFF;
bool NTool::IgnoreUpToolStockColl = true;
RenderMode NTool::RMTool = RM_SHADE;
RenderMode NTool::RMHolder = RM_SHADE;
double NTool::Transparency = 0.5;
double NTool::TurnOffset = 0.;
double NTool::XYOffset = 0.5;
double NTool::ZOffset = 0.;
double NTool::XYOffsetSh = 0.;
double NTool::ZOffsetSh = 0.;
NTool::NTool()
{
  	Name = "EmptyTool";
	Cutting = true;
	Shank = false;
	Probe = false;

  	GenInit();
}

NTool::~NTool()
{
}

void NTool::Draw(RenderMode GlobRMode, int step)
{
	Shader::ShaderOFF();
	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBegin(GL_POINTS);
			glVertex3d(0.,0.,0.);
		glEnd();
	glPopAttrib();
}

NTool::NTool(const CString &n)
{
	Name = n;
	Cutting = true;
	Shank = false;
	Probe = false;

	GenInit();
}

int NTool::GenInit()
{
	Color.SetRGB(RGB(0,0,255));
	ColorPassive.SetRGB(RGB(0,255,255));
	NColor *pCol;
	if(NCM_PROJECT.Colors.Lookup("»нструментЌе–аб„астьѕо”молчанию",pCol))
		ColorPassive = *pCol;
	Visible = TRUE;
	WFrame = FALSE;
	return 0;
}


void NTool::Serialize(CArchive &ar)
{
	Color.Serialize(ar);
	SerializeElements(ar,&Name,1);
	SerializeElements(ar,&Visible,1);
	SerializeElements(ar,&WFrame,1);

	SerializeElements(ar,&Zdisp,1);
	ColorPassive.Serialize(ar);
	SerializeElements(ar,&ToolType,1);
	if(ar.IsLoading())
		SetType(ToolType);// To set proper Cutting value
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.8")))
	{
		SerializeElements(ar, &Probe, 1);
		SerializeElements(ar, &Cutting, 1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.0")))
	{
		SerializeElements(ar, &Shank, 1);
	}
}

void NTool::FillDialog(CDialog *pDial)
{
}

void NTool::FillSheet(CPropertySheet *pSheet)
{
}
void NTool::FillPage(CDialog *pPage)
{
}

void NTool::FillFromPage(CDialog *pPage)
{
}

const CString &NTool::GetName() const
{
	return Name;
}

// создание копии инструмента
NTool* NTool::CreateCopyTool()
{
	return NULL;
}

int NTool::GetType()
{
	return ToolType;
}

int NTool::GetToolStep(ToolMash m /* = UNDEF */)
{
	int step = 15;//step = 15;
	int *pInt,Int = m;
	pInt = &Int;
	if(*pInt == UNDEF)
		NCM_PROJECT.IntParams.Lookup("ToolMash", pInt);
	switch(*pInt)
	{
	case Thick:
		step = 16;//step = 50; // step = 3;
		break;
	case AVERAGE:
		step = 8;//step = 15; //step = 35;
		break;
	case RARE:
		step = 4;//step = 5; //step = 60;
		break;
	default:
		break;
	}
	return step;
}

void NTool::FlipShapeDraw()
{
	ShapeVisibleTool = !ShapeVisibleTool;
	ShapeVisibleHolder = !ShapeVisibleHolder;
}

BOOL NTool::IsShapeVisible()
{
	return ShapeVisibleTool;
}

BOOL NTool::IsShapeVisibleHolder()
{
	return ShapeVisibleHolder;
}

double NTool::RoundP(double in)
{
	return 0.001 * round(in * 1000.);
}

void NTool::CreateCopyTo(NTool *pCreatedTool)
{
	pCreatedTool->Name = Name;
	pCreatedTool->Visible = Visible;
	pCreatedTool->WFrame = WFrame;
	pCreatedTool->ToolType = ToolType;
	pCreatedTool->Cutting = Cutting;
	pCreatedTool->Shank = Shank;
	pCreatedTool->Color = Color;
	pCreatedTool->ColorPassive = ColorPassive;
	pCreatedTool->Gabar = Gabar;
	pCreatedTool->Probe = Probe;
}

void NTool::Regenerate(void)
{
}

void NTool::SetType(int type)
{
	ToolType = type;
	if(	(ToolType >= 40 && ToolType <= 49) ||
		(ToolType >= 60 && ToolType <= 69)
		)

		Cutting = false;
}

void NTool::SetProbe(bool Val)
{
	if (Val)
	{
		Cutting = false;
		Shank = false;
	}
	Probe = Val;
}

BOOL NTool::CreateSimGeneratrix(NTool *pTool, NContour &tmpContour)
{
	// в одном контуре содержатс€ нерабоча€ и рабоча€ части
	if (!pTool) 
		return FALSE;
	
	BOOL res = FALSE;
	int type = pTool->GetType(), index, j, k;
	BPoint pt0, pt1, pt2, pt3;

	if ((10 <= type && type <= 14) || (40 <= type && type <= 43))
	{
		double Zdisp = ((NToolMill *)pTool)->GetZdisp();
		res = TRUE;
		NToolStd *pToolStd;
		double dtop,dbot,h,alf,beta,r,hm;
		int LinesNumber; // Lines Number in a contour
		int PointsNumber;
		TypeLineInContour *types;
		GLfloat *ctlarray;

		pToolStd = (NToolStd*)pTool;
		pToolStd->GetParam(dtop,dbot,h,alf,beta,r,hm);

		// Top line
		pt0.Set(0., 0., h+Zdisp, 1.);

		pt3.Set(dtop/2.f, 0., h+Zdisp, 1.);

		pt1 = (pt0 + pt3) * 0.5;

		tmpContour.AddFirst(pt0, pt1, pt1, pt3);
		tmpContour.SetTypeLine(0,HORIZONTAL);

		k = 0;
		index = 0;
		LinesNumber = 3;
//		if ( r == 0 || (r == 0 && beta == 0)	// нет скруглени€ или фаски
//			|| fabs(r - dbot*0.5/(tan((PI/2.-alf-beta)/2.)*cos(beta))) < MIND )// нет нижнего участка (Bottom line)
//			LinesNumber--;
		PointsNumber = LinesNumber*3+1; // Points Number in a contour
		ctlarray = new GLfloat [4*(PointsNumber)];
		types = new TypeLineInContour[LinesNumber];
		// построение контура
		pToolStd->CreateToolContour(ctlarray, types, LinesNumber, TRUE);
		int LNDelta = 0;// LinesNumber change
		for (j=0, k=0; j<LinesNumber; j++, k += 3)
		{
			pt0.Set(ctlarray[k*4+0], 
				ctlarray[k*4+1], 
				ctlarray[k*4+2]+Zdisp, 
				ctlarray[k*4+3]);

			pt3.Set(ctlarray[(k+3)*4+0], 
				ctlarray[(k+3)*4+1], 
				ctlarray[(k+3)*4+2]+Zdisp, 
				ctlarray[(k+3)*4+3]);
			if((pt0 - pt3).D2() < MIND*MIND)
			{
				LNDelta += 1;
				// replace last point by pt3
				tmpContour.lns.SetAt(tmpContour.lns.GetSize()-1, pt3);
			}
			else
			{
				pt1.Set(ctlarray[(k+1)*4+0], 
					ctlarray[(k+1)*4+1], 
					ctlarray[(k+1)*4+2]+Zdisp, 
					ctlarray[(k+1)*4+3]);

				pt2.Set(ctlarray[(k+2)*4+0], 
					ctlarray[(k+2)*4+1], 
					ctlarray[(k+2)*4+2]+Zdisp, 
					ctlarray[(k+2)*4+3]);
				// вычисление H
				if (types[j] == SPHERE || types[j] == TORAHS )
				{
					pt1 = pt1*(sqrt((pt3-pt0).D2()/(4*(pt1-pt0).D2())));
					pt2 = pt1;
				}
				
				tmpContour.AddFollow(pt1, pt2, pt3);
				tmpContour.SetTypeLine(index+(j+1)-LNDelta, types[j]);
			}
		}
		delete []ctlarray;
		delete []types;
	}

	if (20 <= type && type <= 24)
	{
		double Zdisp = ((NToolMill *)pTool)->GetZdisp();
		res = TRUE;
		NToolDiskStd *pToolDiskStd;
		double diam, b, alf, beta,
			r1, r2, r3, r4,
			l1, l2, hm1, hm2;
		int LinesNumber; // Lines Number in a contour
		int PointsNumber;
		TypeLineInContour *types;
		GLfloat *ctlarray;

		pToolDiskStd = (NToolDiskStd*)pTool;
		pToolDiskStd->GetParam(diam, b, alf, beta,
			r1, r2, r3, r4,
			l1, l2, hm1, hm2);
		k = 0;
		index = 0;
		double e, f, g;
		(l1 != 0. && alf !=0.) ? e = l1 + r3*tan((PI/2.-alf)/2.) 
			: e = r1;
		(l2 != 0. && beta != 0.) ? g = l2 + r4*tan((PI/2.-beta)/2.)
			: g = r2;
		f = b - e - g;
		LinesNumber = 9; // Lines Number in a contour
		if ( r1 == 0.)				// нет скруглений на инструменте
			LinesNumber--;
		if ( r2 == 0.)
			LinesNumber--;
		if ( r3 == 0.)
			LinesNumber--;
		if ( r4 == 0. )
			LinesNumber--;
		if (l1 == 0 && alf == 0.)	// нет фаски c правой стороны
			LinesNumber--;
		if (l2 == 0 && beta == 0.) 	// нет фаски с левой стороны
			LinesNumber--;
		if (f == 0. )//|| (R2 != 0. && R3 != 0.))
			LinesNumber--;


		PointsNumber = LinesNumber*3+1; // Points Number in a contour
		ctlarray = new GLfloat [4*PointsNumber];
		types = new TypeLineInContour[LinesNumber];
		// построение контура
		pToolDiskStd->CreateToolContour(ctlarray, types, LinesNumber, TRUE);
		int LNDelta = 0;// LinesNumber change
		for (j=0, k=0; j<LinesNumber; j++, k += 3)
		{
			pt0.Set(ctlarray[k*4+0], 
				ctlarray[k*4+1], 
				ctlarray[k*4+2]+Zdisp, 
				ctlarray[k*4+3]);

			pt3.Set(ctlarray[(k+3)*4+0], 
				ctlarray[(k+3)*4+1], 
				ctlarray[(k+3)*4+2]+Zdisp, 
				ctlarray[(k+3)*4+3]);
			if((pt0 - pt3).D2() < MIND*MIND)
			{
				if(j - LNDelta != 0)
				{
					// replace last point by pt3
					tmpContour.lns.SetAt(tmpContour.lns.GetSize()-1, pt3);
				}
				LNDelta += 1;
			}
			else
			{
				pt1.Set(ctlarray[(k+1)*4+0], 
					ctlarray[(k+1)*4+1], 
					ctlarray[(k+1)*4+2]+Zdisp, 
					ctlarray[(k+1)*4+3]);

				pt2.Set(ctlarray[(k+2)*4+0], 
					ctlarray[(k+2)*4+1], 
					ctlarray[(k+2)*4+2]+Zdisp, 
					ctlarray[(k+2)*4+3]);
				// вычисление H
				if (types[j] == SPHERE || types[j] == TORAHS )
				{
					pt1 = pt1*(sqrt((pt3-pt0).D2()/(4*(pt1-pt0).D2())));
					pt2 = pt1;
				}
				if (j-LNDelta == 0)
					tmpContour.AddFirst(pt0, pt1, pt2, pt3);
				else
					tmpContour.AddFollow(pt1, pt2, pt3);
				tmpContour.SetTypeLine(index+j-LNDelta, types[j]);
			}
		}
		delete []ctlarray;
		delete []types;
	}
	if (70 <= type && type <= 72)
	{
		NToolShaped *pToolShaped = (NToolShaped *)pTool;
		tmpContour = pToolShaped->GetCont() * BMatr().Trans(BPoint(0., 0., pToolShaped->GetZdisp(), 0.));
		res = TRUE;

	}

	return res;
}

void NTool::SetColor(const NColor & Col)
{
	MElemIDProc::ReplacePC(Color, Col);
	Color = Col;
}
void NTool::SetColorPassive(const NColor & Col)
{
	MElemIDProc::ReplacePC(ColorPassive, Col);
	ColorPassive = Col;
}
void NTool::SetColors(const NColor & Col, const NColor & ColP)
{
	SetColor(Col);
	SetColorPassive(ColP);
}

void NTool::SetTempColors(const NColor& Col, const NColor& ColP)
{
	Color = Col;
	ColorPassive = Col;
}

const NColor & NTool::GetColor(void) const
{
	return Color;
}

const NColor & NTool::GetColorPassive(void) const
{
	return ColorPassive;
}

bool NTool::IsConcave(void) const
{
	return false;
}
bool NTool::NeedAppr(void) const
{
	return false;
}
bool NTool::IsStdCC(void) const
{
	return false;
}
bool NTool::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	return false;
}
bool NTool::CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f)
{
	return false;
}
bool NTool::CreateToolContour(GLfloat *pts,int *type, BOOL f)
{
	return false;
}

void NTool::SetOpaque(void)
{
	Color.A() = 1.;
	ColorPassive.A() = 1.;
}

bool NTool::IsTranslucent()
{
	return RMTool == RM_TRANSLUCENT || RMHolder == RM_TRANSLUCENT;
}

bool NTool::IsToolTranslucent()
{
	return RMTool == RM_TRANSLUCENT;
}

bool NTool::IsVisible()
{
	return (IsHolder() ? NTool::ShapeVisibleHolder : NTool::ShapeVisibleTool) == TRUE;
}

RenderMode NTool::GetRMode(void)
{
	return 	IsHolder() ? NTool::RMHolder : NTool::RMTool;
}

NFastColorMode NTool::GetFastColorMode()
{
	return FastColorMode; 
}

void NTool::SetFastColorMode(NFastColorMode Val)
{
	FastColorMode = Val; 
}

enum CollisWithStock NTool::GetToolStockCollision(void)
{
	return ToolStockCollision;
}

void NTool::SetToolStockCollision(CollisWithStock val)
{
	ToolStockCollision = val;
}

void NTool::SetTurnOffset(double val)
{
	TurnOffset = val;
}

void NTool::SetXYOffset(double val)
{
	XYOffset = val;
}

void NTool::SetXYOffsetSh(double val)
{
	XYOffsetSh = val;
}

bool NTool::GetIgnoreUpToolStockColl(void)
{
	return IgnoreUpToolStockColl;
}

void NTool::SetIgnoreUpToolStockColl(bool val)
{
	IgnoreUpToolStockColl = val;
}

void NTool::SetZOffset(double val)
{
	ZOffset = val;
}

void NTool::SetZOffsetSh(double val)
{
	ZOffsetSh = val;
}

double NTool::GetXYOffset(void)
{
	return XYOffset;
}

double NTool::GetTurnOffset(void)
{
	return TurnOffset;
}

double NTool::GetXYOffsetSh(void)
{
	return XYOffsetSh;
}

double NTool::GetZOffset(void)
{
	return ZOffset;
}

double NTool::GetZOffsetSh(void)
{
	return ZOffsetSh;
}

void NTool::SetShapeVisibleTool(bool Val) 
{ 
	ShapeVisibleTool = Val; 
}

bool NTool::NeedDraw(enum RenderMode GlobRMode)
{
	if (!IsVisible())
		return false;
	if (GetRMode() != RM_TRANSLUCENT && GlobRMode == RM_TRANSLUCENT)
		return false;
	BaseRender::SetShader(GetRMode());
	return true;
}

NTool::NColorExchanger::NColorExchanger()
{
	pTool = nullptr;
}

void NTool::NColorExchanger::Store(NTool* pT)
{
	pTool = pT;
	if (pTool == nullptr)
		return;
	Col = pTool->GetColor();
	ColPass = pTool->GetColorPassive();
}

void NTool::NColorExchanger::Restore()
{
	if (pTool == nullptr)
		return;
	pTool->SetTempColors(Col, ColPass);
	pTool = nullptr;
}
