// NToolShaped.cpp: implementation of the NToolShaped class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseRender.h"
#include "RRevSurf.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolShaped.h"
#include "NToolsCrDiskSheet.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const float d = 1.f/3.f;

IMPLEMENT_SERIAL(NToolShaped, NToolMill, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


NToolShaped::NToolShaped():NToolMill()
{
	h = 0.;
	SetType(70);
}
NToolShaped::NToolShaped(const CString & Name) 
	: NToolMill(Name)
{
	h = 0.;
	SetType(70);
}

NToolShaped::NToolShaped(const CString & Name, const NContour& iCont, bool CutCont) 
	: NToolMill(Name), Cont(iCont)
{
	h = 0.;
	if(Cont.num > 0 && CutCont)
		h = Cont.lns.GetAt(0).GetZ() - Cont.lns.GetAt(Cont.lns.GetSize() - 1).GetZ();

	SetType(70);
	Cutting = CutCont;
}

NToolShaped::~NToolShaped()
{
}

void NToolShaped::Serialize(CArchive &ar)
{
	NToolMill::Serialize(ar);
	SerializeElements(ar, &h, 1);
	Cont.Serialize(ar);
}

void NToolShaped::GenRender()
{
//Create NURBS control points
	int LinesNumber = Cont.num; // Lines Number in a contour
	int PointsNumber = LinesNumber * 3 + 1; // Points Number in a contour
	GLfloat *ctlarray = new GLfloat [4 * PointsNumber * 4];
	for(int i = 0; i < LinesNumber; ++i)
	{
		ctlarray[i * 12 + 0] = float(Cont.lns[i * 3].GetX());
		ctlarray[i * 12 + 1] = float(Cont.lns[i * 3].GetY());
		ctlarray[i * 12 + 2] = float(Cont.lns[i * 3].GetZ());
		ctlarray[i * 12 + 3] = float(Cont.lns[i * 3].GetH());

		ctlarray[i * 12 + 12] = float(Cont.lns[i * 3 + 3].GetX());
		ctlarray[i * 12 + 13] = float(Cont.lns[i * 3 + 3].GetY());
		ctlarray[i * 12 + 14] = float(Cont.lns[i * 3 + 3].GetZ());
		ctlarray[i * 12 + 15] = float(Cont.lns[i * 3 + 3].GetH());

		ctlarray[i * 12 + 4] = ctlarray[i * 12 + 0]*d + 2.f*d*float(Cont.lns[i * 3 + 1].GetX());
		ctlarray[i * 12 + 5] = ctlarray[i * 12 + 1]*d + 2.f*d*float(Cont.lns[i * 3 + 1].GetY());
		ctlarray[i * 12 + 6] = ctlarray[i * 12 + 2]*d + 2.f*d*float(Cont.lns[i * 3 + 1].GetZ());
		ctlarray[i * 12 + 7] = ctlarray[i * 12 + 3]*d + 2.f*d*float(Cont.lns[i * 3 + 1].GetH());

		ctlarray[i * 12 + 8] = ctlarray[i * 12 + 12]*d + 2.f*d*float(Cont.lns[i * 3 + 2].GetX());
		ctlarray[i * 12 + 9] = ctlarray[i * 12 + 13]*d + 2.f*d*float(Cont.lns[i * 3 + 2].GetY());
		ctlarray[i * 12 +10] = ctlarray[i * 12 + 14]*d + 2.f*d*float(Cont.lns[i * 3 + 2].GetZ());
		ctlarray[i * 12 +11] = ctlarray[i * 12 + 15]*d + 2.f*d*float(Cont.lns[i * 3 + 2].GetH());
	}
// -------------------------	
	GLfloat *uknot = new GLfloat [PointsNumber + 4];
	int k = 0;
	for (int i = 0; i <= LinesNumber; ++i)
	{
		int j = k;
		for (; j < k + 3; ++j)
			uknot[j] = (float)i;
		k += 3;
		if (i == 0 || i == LinesNumber)
		{	uknot[j] = (float)i;	k++;	}
		
	}
	GLfloat	vknot[8]={0.f,0.f,0.f,0.f,1.f,1.f,1.f,1.f};

//////////////////// не активная часть инструмента	/////////////////////////
	RenderP.Create(PointsNumber, ctlarray);
	CreateSkeleton(PointsNumber, ctlarray);

	delete[] ctlarray;
	delete[] uknot;
}

void NToolShaped::Draw(RenderMode GlobRMode, int step)
{
	if (!IsVisible())
		return;

	glPushMatrix();
	glTranslated(0., 0., Zdisp);
	//////////////////// не активная часть инструмента	/////////////////////////
	double Height = h;
	GLdouble equation[4] = { 0., 0., 1., -(Height - 0.01) };
	glClipPlane(GL_CLIP_PLANE0, equation);
	glEnable(GL_CLIP_PLANE0);

	ColorPassive.A() = float(Transparency);
	Color.A() = float(Transparency);
	glColor4fv(ColorPassive.array);

	RenderMode LocRMode = GetRMode();

	if (!RenderP.Draw(GlobRMode, LocRMode))
	{
		GenRender();
		RenderP.Draw(GlobRMode, LocRMode);
	}
	///////////////// активная часть инструмента ///////////////////////////
	equation[2] = -1.;
	equation[3] = (Height - 0.01);
	glClipPlane(GL_CLIP_PLANE0, equation);

	if (IsCutting())
		glColor4fv(Color.array);
	else
		glColor4fv(ColorPassive.array);

	RenderP.Draw(GlobRMode, LocRMode);

	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
}

void NToolShaped::FillSheet(CPropertySheet *pDial)
{
	NToolMill::FillSheet(pDial);
}

const NContour & NToolShaped::GetCont(void) const
{
	return Cont;
}

int NToolShaped::GenInit(void)
{
	NToolMill::GenInit();
	double buf[16] = {
		-1., 0., 0., 0., 
		0., -1., 0., 0.,
		0., 0., 1., 0.,
		0., 0., 0., 1. };
	BMatr InvXY(buf);
	for(int i = 0; i < Cont.lns.GetSize(); ++i)
	{
		BPoint P = Cont.lns.GetAt(i);
		P.SetY(P.GetX());
		Gabar.Expand(P);
		Gabar.Expand(P * InvXY);
	}

	SetType(70);

	return 0;
}
