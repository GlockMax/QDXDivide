#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NToolsCrTBISO.h"
#include "NToolTurnBladeISO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnBladeISO, NToolTurnBlade, 0)
NToolTurnBladeISO::NToolTurnBladeISO(void) : NToolTurnBlade()
	, m_L1(0.)
	, m_L2(0.)
{
	Angle1 = 115.;
	Angle2 = 45.;
	Length1 = (MM_INCH == M_MM) ? 2. : 0.1;
	Length2 = (MM_INCH == M_MM) ? 1. : 0.05;
	SetType(53);
}
NToolTurnBladeISO::NToolTurnBladeISO(const CString& TName, CString Form, double L1, double L2, double NoseR,
		double AngleC, double Height, double MountAngle, double ShiftX, double ShiftZ, bool RB1, int Edge) : NToolTurnBlade()
{
	CMapStringToString ISO_Values;
	ISO_Values["C"] = "C=80";
	ISO_Values["D"] = "D=55";
	ISO_Values["E"] = "E=75";
	ISO_Values["F"] = "F=50";
	ISO_Values["M"] = "M=86";
	ISO_Values["V"] = "V=35";
	ISO_Values["A"] = "A=85";
	ISO_Values["B"] = "B=82";
	ISO_Values["K"] = "K=55";
	SetType(53);
	Name = TName;
	m_Form = Form;
	ISO_Values.Lookup(Form, m_Form);
	m_L1 = L1;
	m_L2 = L2;
	m_SRad.Format("%lf", NoseR);
	m_RB1 = RB1;
	m_BackAngle.Format("%lf", AngleC);
	Displacement.Set(ShiftX, ShiftZ, 0., 0.);
	MainAngle = MountAngle;
	Slope = GetVal(m_BackAngle);
	if(Height != 0.)
	{
		m_SThickness.Format("%lf", Height);
		Thickness = GetVal(m_SThickness);
	}
	m_Edge = Edge;
}

NToolTurnBladeISO::~NToolTurnBladeISO(void)
{
}

void NToolTurnBladeISO::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBISO)))
		return;// error

	NToolTurnBlade::FillPage(pPage);

	NToolsCrTBISO *pP = (NToolsCrTBISO *)pPage;
	pP->m_L1 = m_L1;
	pP->m_L2 = m_L2;
	pP->m_Form = m_Form; 
	pP->m_Edge = m_Edge; 
	pP->m_BackAngle = m_BackAngle;
	pP->m_SThickness = m_SThickness;
	pP->m_SRad = m_SRad;
}

void NToolTurnBladeISO::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTBISO)))
		return;// error

	NToolTurnBlade::FillFromPage(pPage);

	NToolsCrTBISO *pP = (NToolsCrTBISO *)pPage;
	m_L1 = pP->m_L1;
	m_L2 = pP->m_L2;
	m_BackAngle = pP->m_BackAngle;
	Slope = GetVal(m_BackAngle);
	m_Form = pP->m_Form; 
	m_Edge = pP->m_Edge; 
	m_SThickness = pP->m_SThickness;
	Thickness = GetVal(m_SThickness);
	m_SRad = pP->m_SRad;
}
// создание копии инструмента
NToolTurnBladeISO* NToolTurnBladeISO::CreateCopyTool()
{
	NToolTurnBladeISO *pCreatedTool = new NToolTurnBladeISO;

	NToolTurnBlade::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->m_L1 = m_L1;
	pCreatedTool->m_L2 = m_L2;
	pCreatedTool->m_Form = m_Form; 
	pCreatedTool->m_Edge = m_Edge; 
	pCreatedTool->m_BackAngle = m_BackAngle;
	pCreatedTool->m_SThickness = m_SThickness;
	pCreatedTool->m_SRad = m_SRad;
	return pCreatedTool;
}
void NToolTurnBladeISO::Serialize(CArchive &ar)
{
	NToolTurnBlade::Serialize(ar);

	SerializeElements(ar, &m_L1, 1);
	SerializeElements(ar, &m_L2, 1);
	SerializeElements(ar, &m_Edge, 1);
	SerializeElements(ar, &m_Form, 1);
	SerializeElements(ar, &m_BackAngle, 1);
	SerializeElements(ar, &m_SThickness, 1);
	SerializeElements(ar, &m_SRad, 1);

	if(ar.IsLoading())
		GenInit();
}

int NToolTurnBladeISO::GenInit(void)
{
	if(m_Form.GetLength() < 1)
		return -1;
	if(m_L1 <= 0.)
		return -2;
	double R = GetVal(m_SRad);
	if(R < 0.)
		return -3;
	R -= TOOL_OFFSET;
	if(R < 0.)
		R = 0.;


	switch( m_Form[0] )
	{
	case 'H':// 6
		GenRegPoly(6, m_L1, R);
		break;
	case 'O':// 8
		GenRegPoly(8, m_L1, R);
		break;
	case 'P':// 5
		GenRegPoly(5, m_L1, R);
		break;
	case 'S':// 4
		GenRegPoly(4, m_L1, R);
		break;
	case 'T':// 3 
		GenRegPoly(3, m_L1, R);
		break;
	case 'C':// 80 Ромб
	case 'D':// 55
	case 'E':// 75
	case 'F':// 50
	case 'M':// 86
	case 'V':// 35
		GenRhomb(GetVal(m_Form), m_L1, R);
		break;
	case 'A':// 85 Параллелограмм
	case 'B':// 82
	case 'K':// 55
		GenParall(GetVal(m_Form), m_L1, m_L2, R);
		break;
	case 'W':
		GenTrigon(m_L1, R);
		break;
	case 'L':// Rectangle
		if(m_L2 <= 0.)
			return -2;
		GenRect(m_L1, m_L2, R);
		break;
	case 'R':
		GenCirc(m_L1);
		break;
	default:
		return -3;
		break;
	}
	BCont.Trans(BMatr().RotZ(BPoint(0.,0.,0.,1.), MainAngle)
		* BMatr().Trans(BPoint(0.,0.,0.,1.), Displacement*(-1.)));

	FillGabar();
	MakeBContSkeleton();
	return 0;
}

void NToolTurnBladeISO::Regenerate(void)
{
	GenInit();
}

int NToolTurnBladeISO::GenCirc(double L1)
{
// Contour must lie in XY plane
	NCadrGeom Geom[4];
	Geom[0].Set(line, L1, 0., 0., 0., L1, 0.);
	Geom[1].Set(line, 0., L1, 0., - L1, 0., 0.);
	Geom[2].Set(line, - L1, 0., 0., 0., - L1, 0.);
	Geom[3].Set(line, 0., - L1, 0., L1, 0., 0.);
	BCont.MakeContour(Geom, 4);
	for(TBladeEdge *pCur = BCont.GetStart(); pCur != NULL; pCur = BCont.GetNext(pCur))
	{
		pCur->SetCircCenter(BPoint(0.,0.,0.,1.));
		pCur->SetAttr(BE_CUT);
	}
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	// Define Length1 Length2
	if(L1 < Length1 + Length2)
	{
		Length1 = 2./3. * L1;
		Length2 = 1./3. * L1;
	}
	return 0;
}

int NToolTurnBladeISO::GenRegPoly(int Num, double Length, double Rs)
{// Generate regular polygon with Num edges. Edge length = Length
	if(Num < 3)
		return -1;
	double Alf = 2. * PI / Num;
	double R = Length / (2. * sin( Alf * 0.5 ));// circumcircle radii
	Alf *= 180. / PI;

	BCont.Clear();
	for(int i = 0; i < Num; ++i)
	{
		TBladeEdge *pEdge = new TBladeEdge;
		pEdge->SetPoint(BPoint(0., 0., 0., 1.)
			* BMatr().RotZ(BPoint(R, 0., 0., 1.), Alf * i));
		BCont.AddEdge(pEdge);
	}

	if(Rs > 0.)
	{
		TBladeEdge *pEdge = BCont.GetStart();
		for(int i = 0; i < Num; ++i)
		{
			pEdge->InsertTangRad(Rs);
			pEdge = pEdge->GetNext();
			pEdge = pEdge->GetNext();
		}
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), BCont.GetStart()->GetCircCenter() * (-1.)));
	}
	BCont.GetStart()->SetAttr(BE_CUT);
	BCont.GetStart()->GetPrev()->SetAttr(BE_CUT);
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	// Define Length1 Length2
	if(R * 0.5 < Length1 + Length2)
	{
		Length1 = 2./3. * (0.5 * R);
		Length2 = 1./3. * (0.5 * R);
	}

	return 0;
}

double NToolTurnBladeISO::GetVal(const CString & str)
{
	const char Delim = '=';
	double Val = -1.;
	int ind = str.Find(Delim);
	if(ind < 0)
		if(sscanf(str, "%lf", &Val) < 1)
			return -1.;
		else
			return Val;

	CString b = str.Right(str.GetLength() - ind - 1);
	if(sscanf(b, "%lf", &Val) < 1)
		return -1.;
	else
		return Val;
	return 0;
}

int NToolTurnBladeISO::GenRhomb(double Angle, double Length, double R)
{
	if(Angle < 0)
		return -1;

	double AngleR = Angle * PI / 180.;

	double dx = Length * cos(AngleR * 0.5);
	double dy = Length * sin(AngleR * 0.5);

	BCont.Clear();

	TBladeEdge *pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(dx, dy, 0., 1.));
	pEdge->SetAttr(BE_CUT);
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(0., 0., 0., 1.));
	pEdge->SetAttr(BE_CUT);
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(dx, -dy, 0., 1.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(2. * dx, 0., 0., 1.));
	BCont.AddEdge(pEdge);
	

	if(R > 0.)
	{
		BCont.GetStart()->GetNext()->InsertTangRad(R);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->GetPrev()->InsertTangRad(R);
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), BCont.GetStart()->GetNext()->GetCircCenter() * (-1.)));
		BCont.BreakArcs();
		BCont.RemoveShortEdges();
	}
	// Define Length1 Length2
	if(dy < Length1 + Length2)
	{
		Length1 = 2./3. * dy;
		Length2 = 1./3. * dy;
	}

	return 0;
}

int NToolTurnBladeISO::GenRect(double L1, double L2, double R)
{
// Contour must lie in XY plane
 	NCadrGeom Geom[4];
	Geom[0].Set(line, L1, -L2, 0., L1, 0., 0.);
	Geom[1].Set(line, 0., 0., 0., 0., 0., 0.);
	Geom[1].SetB(Geom[0].GetE());
	Geom[2].Set(line, 0., 0., 0., 0., - L2, 0.);
	Geom[3].Set(line, 0., - L2, 0., 0., 0., 0.);
	Geom[3].SetE(Geom[0].GetB());
	BCont.MakeContour(Geom, 4);
	BCont.Trans(BMatr().RotZ(BPoint(0.,0.,0.,1.), - 270.));

	BCont.GetStart()->SetAttr(BE_CUT);
	BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
	BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
	BPoint ActCenter(0.,0.,0.,1.);
	BPoint ActCenter1(0.,0.,0.,1.);
	if(R > 0)
	{
		BCont.GetStart()->GetNext()->GetNext()->InsertTangRad(R);
		BCont.GetStart()->GetNext()->GetNext()->GetNext()->SetAttr(BE_CUT);
		ActCenter = BCont.GetStart()->GetNext()->GetNext()->GetCircCenter();
		BCont.GetStart()->GetNext()->InsertTangRad(R);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		ActCenter1 = BCont.GetStart()->GetNext()->GetCircCenter();
		BCont.BreakArcs();
		BCont.RemoveShortEdges();
	}
	else
	{
		ActCenter = BCont.GetStart()->GetNext()->GetNext()->GetPoint();
		ActCenter1 = BCont.GetStart()->GetNext()->GetPoint();
	}
	if(m_Edge == 1)
		BCont.Trans(BMatr().Trans(ActCenter1, BPoint(0.,0.,0.,1.)));
	else
		BCont.Trans(BMatr().Trans(ActCenter, BPoint(0.,0.,0.,1.)));
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	// Define Length1 Length2
	double B = min(L1, L2) * 0.5;
	if(B < Length1 + Length2)
	{
		Length1 = 2./3. * B;
		Length2 = 1./3. * B;
	}

	return 0;
}

int NToolTurnBladeISO::GenParall(double Angle, double L1, double L2, double R)
{
	if(Angle < 0)
		return -1;

	double AngleR = Angle * PI / 180.;

	double dx = L1 * cos(AngleR);
	double dy = L1 * sin(AngleR);

	BCont.Clear();

	TBladeEdge *pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(dx, dy, 0., 1.));
	pEdge->SetAttr(BE_CUT);
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(0., 0., 0., 1.));
	pEdge->SetAttr(BE_CUT);
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(L2, 0., 0., 1.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(L2 + dx, dy, 0., 1.));
	pEdge->SetAttr(BE_CUT);
	BCont.AddEdge(pEdge);
	
	BPoint ActCenter(0.,0.,0.,1.);
	BPoint ActCenter1(0.,0.,0.,1.);
	if(R > 0.)
	{

		BCont.GetStart()->GetNext()->InsertTangRad(R);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		ActCenter = BCont.GetStart()->GetNext()->GetCircCenter();
		BCont.GetStart()->InsertTangRad(R);
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		ActCenter1 = BCont.GetStart()->GetCircCenter();
		BCont.BreakArcs();
		BCont.RemoveShortEdges();
	}
	else
	{
		ActCenter = BCont.GetStart()->GetNext()->GetPoint();
		ActCenter1 = BCont.GetStart()->GetPoint();
	}
	if(m_Edge == 1)
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), ActCenter1 * (-1.)));
	else
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), ActCenter * (-1.)));
	// Define Length1 Length2
	if(dy * 0.5 < Length1 + Length2)
	{
		Length1 = 2./3. * dy * 0.5;
		Length2 = 1./3. * dy * 0.5;
	}

	return 0;
}

int NToolTurnBladeISO::GenTrigon(double L1, double R)
{
	const double Angle = 80.;

	double AngleR = Angle * PI / 180.;

	double dx = L1 * cos(AngleR * 0.5);
	double dy = L1 * sin(AngleR * 0.5);
	double dxc = dx + dy * tan(PI/6.);

	BCont.Clear();

	TBladeEdge *pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(dx, dy, 0., 1.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(0., 0., 0., 1.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(dx, -dy, 0., 1.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(0., 0., 0., 1.) * BMatr().RotZ(BPoint(dxc, 0., 0., 1.), 120.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(dx, -dy, 0., 1.) * BMatr().RotZ(BPoint(dxc, 0., 0., 1.), 120.));
	BCont.AddEdge(pEdge);

	pEdge = new TBladeEdge;
	pEdge->SetPoint(BPoint(0., 0., 0., 1.) * BMatr().RotZ(BPoint(dxc, 0., 0., 1.), 240.));
	BCont.AddEdge(pEdge);

	if(R > 0.)
	{
		TBladeEdge *pEdge = BCont.GetStart();
		for(int i = 0; i < 6; ++i)
		{
			pEdge->InsertTangRad(R);
			pEdge = pEdge->GetNext();
			pEdge = pEdge->GetNext();
		}
		BCont.GetStart()->GetNext()->GetNext()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->GetNext()->GetNext()->SetAttr(BE_CUT);
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
		BCont.Trans(BMatr().Trans(BPoint(0.,0.,0.,-1.), BCont.GetStart()->GetNext()->GetNext()->GetCircCenter() * (-1.)));
	}
	else
	{
		BCont.GetStart()->SetAttr(BE_CUT);
		BCont.GetStart()->GetNext()->SetAttr(BE_CUT);
	}
	BCont.BreakArcs();
	BCont.RemoveShortEdges();
	// Define Length1 Length2
	if(L1 * 0.5 < Length1 + Length2)
	{
		Length1 = 2./3. * L1 * 0.5;
		Length2 = 1./3. * L1 * 0.5;
	}
	return 0;
}
bool NToolTurnBladeISO::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = 	GetVal(m_SRad) * 2.;
	CornerRadius = 0.;
    ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = 0.;
	Height = 0.;
	return true;
}