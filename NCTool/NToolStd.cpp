// NToolStd.cpp: implementation of the NToolStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseRender.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "NToolStd.h"
#include "NToolsCrDial.h"
#include "NToolsCrDiskSheet.h"
#include "NMendrelDial.h"
#include "ConstDef.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolStd, NToolMill, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const float d = 1.f/3.f;

NToolStd::NToolStd():NToolMill()
{
	H = 0.;
	DTop = DBot = 0.; 
	R = 0.;
	BAlf = 0.;
	h = 0.;
//	GenInit();
}

NToolStd::~NToolStd()
{
}

NToolStd::NToolStd(const CString &Name,double dbot,double hb,double alf,double r,
				   double beta, double e, double f, double hm)
			:NToolMill(Name)
{
	H=hb;
	DBot = dbot;
	Alf = alf * PI / 180.;
	R = r;
	BAlf=beta*PI/180.;
	h = hm;
	ELength = e;
	FLength = f;
	if (R == 0. && ELength != 0. && FLength != 0.)
		R = (H - FLength - ELength * tan(BAlf)) * cos(BAlf);
//	GenInit();
}
NToolStd::NToolStd(double dbot,double hb,double alf,double r,
				   double beta, double e, double f, double hm)
			:NToolMill(_T("Tool"))
{
	H = hb;
	DBot = dbot;
	Alf = alf * PI / 180.;
	R = r;
	BAlf = beta * PI / 180.;
	h = hm;
	ELength = e;
	FLength = f;
	if (R == 0. && ELength != 0. && FLength != 0.)
		R = (H-FLength-ELength*tan(BAlf))*cos(BAlf);
//	GenInit();
}

double NToolStd::CalcEndRad(double BottomDiam, double Alf)
{
	return  BottomDiam / (2. * tan((PI / 2. - Alf) / 2.));
}

void NToolStd::Serialize(CArchive &ar)
{
	NToolMill::Serialize(ar);
	SerializeElements(ar,&DBot,1);
	SerializeElements(ar,&DTop,1);
	SerializeElements(ar,&H,1);
	SerializeElements(ar,&R,1);
	SerializeElements(ar,&Alf,1);
	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.1"))
	{
		if(fabs( DBot - DTop ) < TOOL_OFFSET)
		{
			if(fabs(R*2. - DBot) < TOOL_OFFSET)
				R = (DBot - TOOL_OFFSET)/2.;
			DBot -= TOOL_OFFSET;
			DTop -= TOOL_OFFSET;
		}
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("2.0")))
	{
		SerializeElements(ar,&BAlf,1);
		SerializeElements(ar,&h,1);
		SerializeElements(ar,&IsNameAuto,1);
	}
	else
		h = H;
	if(ar.IsLoading())
	{
		GenInit();
		double bz = Zdisp;
		Zdisp = 0;
		SetZdisp(bz);
	}
}
// проверка параметров инструмента
int NToolStd::GenInit()
{
	if (GetType() == 10)
		DTop = DBot + 2.*(H-(DBot/2.)*tan(BAlf))*tan(Alf);
	else
		DTop = DBot + 2.*H*tan(Alf);
// Calculate dimensions in object coord
	Gabar.Expand(-DTop/2.,-DTop/2.,0);
	Gabar.Expand(DTop/2.,DTop/2.,H);

	double x = R*tan((PI/2.-Alf-BAlf)/2);
	double y = x*cos(Alf);
	double c = DBot + 2.*(H-(DBot/2.)*tan(BAlf))*tan(Alf);

// проверка типа инструмента
	if (NToolMill::GetType() == 0)
		NToolMill::SetType(AssignType());
	int type = NToolMill::GetType();
// проверка геометрических размеров инструмента
	if (type != 42 && (PI / 2. - fabs(Alf)) < MIND)
		return 2; // недопустимый угол
	if (type != 42 && H * tan(Alf) + DBot * 0.5 < MIND)
		return 7; // двойной конус
	if (type != 43 && (BAlf < 0. || PI/2.-BAlf < MIND))
		return 2;	// недопустимый угол
	if (type == 10 && tan(Alf) > c/(2.*H))
		return 3; // невыпуклый инструмент ј–“ 7
	if (type == 10 && tan(BAlf) > 2*H/c)
		return 4; // невыпуклый инструмент ј–“ 7
	if (R != 0 && BAlf != 0 && type != 10) 
	{			  // проверка параметров фаски
		if ((R < 0. && type != 43) || R > DBot/2. && type != 43)
			return 3; // неверны параметры фаски
		y = fabs(R) * tan(BAlf);
		if (y > H)
			return 4; // слишком большой угол фаски
	}
	else
	{
		if (DBot/2.-x*cos(BAlf) < -MIND )
			return 1; // велик радиус
		if (H - y - (DBot/2.)*tan(BAlf) < -MIND)// || H - R*)
			return 1; // велик радиус
	}
	if (R == 0 && PI/2. - BAlf < MINAR)
		return 5; // слишком большой угол сверла
// проверка активной части инструмента
	if (type == 10)
	{
		y = R*tan((PI/2.-Alf-BAlf)/2);
		x = DBot/2.*tan(BAlf)+y*cos(Alf);
	}
	if (type == 11 || type == 43) // инструмент с фаской
		(BAlf != 0.) ? x = R * tan(BAlf) : x = 0.;
	if (type == 12) // инструмент со скруглени€ми
		x = R;
	if (type == 13) // угловой инструмент
		x = y;
	if (type == 14) // сверло
		(BAlf != 0.) ? x = DBot/2.*tan(BAlf) : x = 0;
	if (h > H)
		h = H;
	if ((x > h))
		return 6; // неправильно задана активна€ часть инструмента
	return 0;
}

void NToolStd::GenToolName(const CString& Prefix)
{
	CString d, h, r, a, buf;
	CString Format(_T("%1.6g"));
	d.Format("D" + Format, DBot);

	h.Format("H" + Format, H);

	if (fabs(R) > MIND)
		r.Format("R" + Format, R);

	if (fabs(Alf) > MINAR)
		a.Format("A" + Format, Alf);

	if(fabs(BAlf) > MINAR)
		r.Format("F" + Format, BAlf);

	Name = Prefix + d + h;
	if(Prefix != _T("Hold") && Prefix != _T("Shank"))
	Name += r + a;
	IsNameAuto = true;
}

void NToolStd::GenRender()
{
	//////////////////// не активна€ часть инструмента	/////////////////////////
	double cx = 0., cy = 0.;//учЄт смещени€ дл€ типа 10
	if (GetType() == 10)
	{
		cy = DBot / 2.*tan(BAlf);
		cx = cy * tan(Alf);
		DTop = DBot + 2.*(H - cy)*tan(Alf);
	}
	else
		DTop = DBot + 2.*H*tan(Alf);

	double y = h;
	double x = y*tan(Alf) - cx;
	//Create NURBS control points
	int LinesNumber = 2; // Lines Number in a contour
	int PointsNumber = LinesNumber * 3 + 1; // Points Number in a contour
	GLfloat *ctlarray = new GLfloat[4 * PointsNumber * 4];
	// определение крайних точек участков контура
	//Top line
	ctlarray[0 * 4] = 0;
	ctlarray[0 * 4 + 1] = 0;
	ctlarray[0 * 4 + 2] = (float)H;
	ctlarray[0 * 4 + 3] = 1;
	// Draft line
	ctlarray[3 * 4] = (float)DTop / 2.f;
	ctlarray[3 * 4 + 1] = 0;
	ctlarray[3 * 4 + 2] = (float)H;
	ctlarray[3 * 4 + 3] = 1;
	//
	ctlarray[6 * 4] = (float)(DBot / 2. + x);
	ctlarray[6 * 4 + 1] = 0;
	ctlarray[6 * 4 + 2] = (float)y;
	ctlarray[6 * 4 + 3] = 1;
	// -------------------------	
	GLfloat *uknot = new GLfloat[PointsNumber + 4];
	int k = 0;
	for (int i = 0; i <= LinesNumber; i++)
	{
		int j = k;
		for (; j < k + 3; j++)
			uknot[j] = (float)i;
		k += 3;
		if (i == 0 || i == LinesNumber)
		{
			uknot[j] = (float)i;	k++;
		}

	}
	GLfloat	vknot[8] = { 0.f,0.f,0.f,0.f,1.f,1.f,1.f,1.f };
	for (int i = 0; i<4; i++)
	{
		ctlarray[1 * 4 + i] = ctlarray[0 * 4 + i] * 2.f*d + d*ctlarray[3 * 4 + i];
		ctlarray[2 * 4 + i] = ctlarray[0 * 4 + i] * d + 2.f*d*ctlarray[3 * 4 + i];
		ctlarray[4 * 4 + i] = ctlarray[3 * 4 + i] * 2.f*d + d*ctlarray[6 * 4 + i];
		ctlarray[5 * 4 + i] = ctlarray[3 * 4 + i] * d + 2.f*d*ctlarray[6 * 4 + i];
	}
	RenderP.Create(PointsNumber, ctlarray);

	///////////////// активна€ часть инструмента ///////////////////////////

	//Create NURBS control points
	int ToolType = NToolMill::GetType(); // необходимо 
										 // дл€ различи€ инструмента с фаской и 
										 // и заданного ј–“ 7 
										 //(инструмент с фаской не описываетс€ ј–“ 7)
	LinesNumber = 3; // Lines Number in a contour
	if ((R == 0)	// нет скруглени€ и фаски 
		|| R + 0.0001 >= DBot*0.5 / (tan((PI / 2. - Alf - BAlf) / 2.)*cos(BAlf))// нет нижнего участка у тора (Bottom line)
		)
		LinesNumber--;
	PointsNumber = LinesNumber * 3 + 1; // Points Number in a contour
	delete[] ctlarray;
	ctlarray = new GLfloat[4 * PointsNumber * 4];
	// определение крайних точек участков контура
	k = 3;
	// Draft line
	ctlarray[0 * 4] = (float)(DBot / 2. + x);
	ctlarray[0 * 4 + 1] = 0;
	ctlarray[0 * 4 + 2] = (float)y;
	ctlarray[0 * 4 + 3] = 1;

	x = R*tan((PI / 2. - Alf - BAlf) / 2.);// EK
	y = x*sin(Alf);

	// Arc or Faset (If exists)
	if (R != 0.)
	{
		if ((BAlf == 0. && ToolType != 10) ||
			ToolType == 10)		// Arc
		{
			if (ToolType != 10)
			{
				ctlarray[k * 4] = (float)(DBot / 2. + y);
				ctlarray[k * 4 + 1] = 0;
				ctlarray[k * 4 + 2] = (float)(x*cos(Alf));
				ctlarray[k * 4 + 3] = 1;
			}
			if (ToolType == 10)	// APT 7
			{
				ctlarray[k * 4] = (float)(DBot / 2. + y);
				ctlarray[k * 4 + 1] = 0;
				ctlarray[k * 4 + 2] = (float)(cy + x*cos(Alf));
				ctlarray[k * 4 + 3] = 1;
			}

			ctlarray[(k + 2) * 4] = (float)(DBot / 2.);
			ctlarray[(k + 2) * 4 + 1] = 0;
			if (ToolType != 10)
				ctlarray[(k + 2) * 4 + 2] = 0;
			else
				ctlarray[(k + 2) * 4 + 2] = (float)cy;
			ctlarray[(k + 2) * 4 + 3] = 1;
		}
		if (BAlf != 0. && ToolType != 10)	// Faset
		{
			x = R;		// в переменной R содержитс€ значение Faset
			y = x*tan(BAlf);
			ctlarray[k * 4] = (float)DBot / 2.f;
			ctlarray[k * 4 + 1] = 0;
			ctlarray[k * 4 + 2] = (float)y;
			ctlarray[k * 4 + 3] = 1;
		}
		k += 3;
	}
	//Bottom line (If exists)
	if (R <= DBot*0.5 / (tan((PI / 2. - Alf - BAlf) / 2.)*cos(BAlf)))
	{
		if ((BAlf != 0. && R == 0) || ToolType == 10)
		{
			if (ToolType != 10)	// сверло
			{
				ctlarray[k * 4] = (float)(DBot / 2.);
				ctlarray[k * 4 + 1] = 0;
				ctlarray[k * 4 + 2] = (float)((DBot / 2.)*tan(BAlf));
			}
			if (ToolType == 10)	// APT 7
			{
				ctlarray[k * 4] = (float)(DBot / 2. - x*cos(BAlf));
				ctlarray[k * 4 + 1] = 0;
				ctlarray[k * 4 + 2] = (float)(cy - x*sin(BAlf));
			}
		}
		else			// не сверло и не APT 7
		{
			ctlarray[k * 4] = (float)(DBot / 2. - x);
			ctlarray[k * 4 + 1] = 0;
			ctlarray[k * 4 + 2] = 0;
		}
		ctlarray[k * 4 + 3] = 1;
		k += 3;
	}
	ctlarray[k * 4] = 0;
	ctlarray[k * 4 + 1] = 0;
	ctlarray[k * 4 + 2] = 0;
	ctlarray[k * 4 + 3] = 1;

	// -------------------------	
	delete[] uknot;
	uknot = new GLfloat[PointsNumber + 4];
	k = 0;
	for (int i = 0; i <= LinesNumber; i++)
	{
		int j = k;
		for (; j < k + 3; j++)
			uknot[j] = (float)i;
		k += 3;
		if (i == 0 || i == LinesNumber)
		{
			uknot[j] = (float)i;	k++;
		}

	}
	k = 4;
	for (int i = 0; i<4; i++)
	{
		ctlarray[1 * 4 + i] = ctlarray[0 * 4 + i] * 2.f*d + d*ctlarray[3 * 4 + i];
		ctlarray[2 * 4 + i] = ctlarray[0 * 4 + i] * d + 2.f*d*ctlarray[3 * 4 + i];
		if ((R != 0 && BAlf == 0 && ToolType != 10) ||
			(R != 0 && ToolType == 10))		// Arc 
		{
			ctlarray[k * 4 + i] = ctlarray[(k - 1) * 4 + i] * d +
				2.f*d*ctlarray[(k + 1) * 4 + i] * (float)(cos((PI / 2. - Alf - BAlf) / 2.));
			ctlarray[(k + 1) * 4 + i] = ctlarray[(k + 1) * 4 + i] *
				2.f*d*(float)(cos((PI / 2.f - Alf) / 2.f)) + d*ctlarray[(k + 2) * 4 + i];
			k = k + 3;
		}
		if (R != 0 && BAlf != 0 && ToolType != 10) // Faset
		{
			ctlarray[k * 4 + i] = ctlarray[(k - 1) * 4 + i] * 2.f*d + d*ctlarray[(k + 2) * 4 + i];
			ctlarray[(k + 1) * 4 + i] = ctlarray[(k - 1) * 4 + i] * d + 2.f*d*ctlarray[(k + 2) * 4 + i];
			k = k + 3;
		}
		if (R <= DBot*0.5 / (tan((PI / 2. - Alf - BAlf) / 2.)*cos(BAlf))) // Bottom line
		{
			ctlarray[k * 4 + i] = ctlarray[(k - 1) * 4 + i] * 2.f*d + d*ctlarray[(k + 2) * 4 + i];
			ctlarray[(k + 1) * 4 + i] = ctlarray[(k - 1) * 4 + i] * d + 2.f*d*ctlarray[(k + 2) * 4 + i];
		}
		k = 4;
	}

	RenderA.Create(PointsNumber, ctlarray);
	CreateSkeleton(PointsNumber, ctlarray);

	delete[] ctlarray;
	delete[] uknot;
}


void NToolStd::FillDialog(CDialog *pDial)
{
//	ASSERT(pDial->IsKindOf(RUNTIME_CLASS(NToolsCrDial)));
	NToolMill::FillDialog(pDial);
	if (40 <= NToolMill::GetType() && NToolMill::GetType() <= 43)
	{
		NMendrelDial *pD = (NMendrelDial *)pDial;
		pD->m_dtop = RoundP( DTop );
		pD->m_h = RoundP( H );
		pD->m_dbot = RoundP( DBot );
		if (BAlf != 0. && R != 0.)
		{
			pD->m_dbot = RoundP( DTop - 2.*R );
			pD->m_hm = RoundP(R * tan(BAlf));
		}
		else
			pD->m_hm = 0.;
		pD->m_name = Name;
		pD->IsNameAuto = IsNameAuto;
	}
	else
	{
		NToolsCrDial *pD = (NToolsCrDial *)pDial;
		pD->m_diam = RoundP( DBot );
		pD->m_h = RoundP( H );
		if (BAlf == 0.)
			pD->m_alf = RoundP( Alf*180./PI );
		else
			pD->m_alf = RoundP( 180 - 2*(BAlf*180./PI) );
		pD->m_beta = RoundP( BAlf*180./PI );
		if (BAlf != 0. && R != 0.) // если фаска
		{
			pD->m_alf = RoundP( 90-pD->m_beta );
			pD->m_faset = RoundP( R );
		}
		else
			pD->m_r = RoundP( R );
		pD->m_hm = RoundP( h );
		pD->m_name = Name;
		pD->IsNameAuto = IsNameAuto;	
	}
}
// определение типа инструмента дл€ APT 7
int NToolStd::AssignType()
{
	if (R != 0. && BAlf == 0.)
	{
		if (Alf != 0.)
			return 13;	// угловой инструмент со сферическим концом
		else
			return 12;	// инструмент со сферическим концом
	}
	else
	{
		if (Alf != 0. && BAlf == 0.)
			return 13;	// угловой инструмент 
		else
		{
			if (BAlf != 0. && R == 0)
				return 14;	// сверло
			else
				return 11;	// инструмент с фаской
		}
	}
}

void NToolStd::FillSheet(CPropertySheet *pDial)
{
	NToolMill::FillSheet(pDial);
	NToolsCrDiskSheet *pD = (NToolsCrDiskSheet *)pDial;
	
	if (NToolMill::GetType() == 10)
	{
		pD->tab_pos = 4;
		pD->m_aptpage.m_diam = RoundP( DBot );
		pD->m_aptpage.m_h = RoundP( H );
		pD->m_aptpage.m_alf = RoundP( Alf*180./PI );
		pD->m_aptpage.m_beta = RoundP( BAlf*180./PI );
		if (ELength == 0. && FLength == 0.)
		{
			pD->m_aptpage.m_r = RoundP( R );
			pD->m_aptpage.m_e = pD->m_aptpage.m_f = 0.;
		}
		else
		{
			pD->m_aptpage.m_r = 0.;
			pD->m_aptpage.m_e = RoundP( ELength );
			pD->m_aptpage.m_f = RoundP( FLength );
		}
		pD->m_aptpage.m_hm = RoundP( h );
		pD->m_aptpage.m_name = Name;
		pD->m_aptpage.IsNameAuto = IsNameAuto;
	}
}

// определение геометрических параметров инструмента
void NToolStd::GetParam(double &dtop, double &dbot, double &hr, double &alf, double &beta, double &r, double &hm)
{
	dtop = DTop;
	dbot = DBot;
	hr = H;
	alf = Alf;
	beta = BAlf;
	r = R;
	hm = h;
}

bool NToolStd::GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height)
{
	Diameter = this->DBot;
	CornerRadius = this->R;
    ELength = this->ELength;
	FLength = this->FLength;
	BottomAngle = this->BAlf;
	SideAngle = this->Alf;
	Height = this->H;
	return true;
}

bool NToolStd::IsBallNose() const
{ 
	double RightR = CalcEndRad(DBot, Alf);
	return fabs(R - RightR) < 0.001 * DBot;
}

bool NToolStd::IsCylWBallNose() const
{ 
	return fabs(DBot - DTop) < 0.001 * DBot && IsBallNose();
}

bool NToolStd::IsFullBall() const
{ 
	return (h <= R + 0.01) && IsBallNose();
}

bool NToolStd::IsFlatNose() const
{ 
	return (fabs(R) < 0.001 && fabs(DBot - DTop) < 0.001 * DBot && fabs(BAlf) < 0.001 );
}

NTool * NToolStd::CreateOffsetTool(bool NonCuttingOnly)
{
	const double XYOff = IsShank() ? NTool::GetXYOffsetSh() : NTool::GetXYOffset();
	const double ZOff = IsShank() ? NTool::GetZOffsetSh() : NTool::GetZOffset();
	NTool *pBufTool = CreateCopyTool();
	if(NonCuttingOnly && IsCutting())
		return pBufTool;
	NToolStd  * pToolStd = (NToolStd *)pBufTool;
	pToolStd->DTop += 2. * XYOff;
	pToolStd->DBot += 2. * XYOff;
	pToolStd->H += ZOff;
	pToolStd->Zdisp -= ZOff;
	if(pToolStd->GenInit() > 0)
	{
		delete pBufTool;
		return CreateCopyTool();
	}
	return pBufTool;
}

void NToolStd::RemoveEndBall(void)
{
	DBot = 2. * R * cos(Alf);
	double dZ = R * (1. - sin(Alf));
	Zdisp += dZ;
	H -= dZ;
	h -= dZ;
	R = 0;
}

void NToolStd::RestoreEndBall(void)
{
	R = DBot / (2. * cos(Alf));
	double dZ = R * (1. - sin(Alf));
	DBot -= 2. * dZ * tan(Alf);
	Zdisp -= dZ;
	h += dZ;
	H += dZ;
}
