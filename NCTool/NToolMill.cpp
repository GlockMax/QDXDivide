// NToolMill.cpp: implementation of the NToolMill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCMProject.h"
#include "BaseRender.h"
#include "RRevSurf.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolMill.h"
#include "NToolBRound.h"
#include "NToolVert.h"
#include "NToolsCrDial.h"
#include "NToolsCrDiskSheet.h"
#include "NMendrelDial.h"
#include "math.h"
#include "BPoint.h"
#include "NCadrGeom.h"
#include "BMatr.h"
#include "Gl\GLu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolMill, NTool, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Initialize the static members

NToolMill::NToolMill()
{
  	Name = "EmptyTool";
	SetType(10);
 
  	GenInit();
}

NToolMill::~NToolMill()
{
}

NToolMill::NToolMill(const CString &n)
{
	Name = n;
	SetType(10);
	
	GenInit();
}

int NToolMill::GenInit()
{
	NTool::GenInit();
	Zdisp = 0.;
	SetType(0);
	VertTool = false;
	return 0;
}


void NToolMill::Serialize(CArchive &ar)
{
	NTool::Serialize(ar);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.7")))
		SerializeElements(ar, &VertTool, 1);
}

void NToolMill::FillDialog(CDialog *pDial)
{
	if (40 <= GetType() && GetType() <= 43)
	{
		NMendrelDial *pD = (NMendrelDial *)pDial;
		pD->m_displacement = RoundP( Zdisp );
		pD->m_check_shank = IsShank();
		if (IsShank())
		{
			auto pToolStd = dynamic_cast<NToolStd*>(this);
			if(pToolStd != nullptr)
			pD->m_dprevious = pToolStd->GetDTop();
		}
		return;
	}
	NToolsCrDial *pD = (NToolsCrDial *)pDial;
	pD->m_displacement = RoundP( Zdisp );
	pD->m_vert_tool = VertTool;
	pD->m_check_probe = IsProbe();
}

void NToolMill::FillSheet(CPropertySheet *pSheet)
{
	NToolsCrDiskSheet *pD = (NToolsCrDiskSheet *)pSheet;
	
	pD->m_fasetpage.m_displacement = RoundP( Zdisp );
	pD->m_roundpage.m_displacement = RoundP( Zdisp );
	pD->m_anglepage.m_displacement = RoundP( Zdisp );
	pD->m_anglespage.m_displacement = RoundP( Zdisp );
	pD->m_aptpage.m_displacement = RoundP( Zdisp );
	pD->m_shapedpage.m_displacement = RoundP( Zdisp );
	pD->m_broundpage.m_displacement = RoundP( Zdisp );
	pD->m_borepage.m_displacement = RoundP( Zdisp );
	pD->m_roundpage.m_check_probe = IsProbe();
}

void NToolMill::SetZdisp(double z)
{
	Gabar = Gabar.Move(0.,0.,z-Zdisp);
	Zdisp = z;
}

// создание копии инструмента
NToolMill* NToolMill::CreateCopyTool()
{
	bool fIsNameAuto;
	NToolMill *pCreatedTool = NULL;
	
	switch(GetType())
	{
	case 10: case 11: case 12: case 13: case 14: case 40: case 41: case 42: case 43:
		NToolStd *pToolStd;
		double dtop,dbot,h,alf,beta,r,hm;
		
		pToolStd = (NToolStd*)this;
		pToolStd->GetParam(dtop,dbot,h,alf,beta,r,hm);
		fIsNameAuto = pToolStd->IsNameAuto;
		alf = alf*180./PI;
		beta = beta*180./PI;
		pToolStd = new NToolStd(GetName(),
					dbot,h,alf,r,beta,0.,0.,hm);
		pToolStd->IsNameAuto = fIsNameAuto;
		if(GetType() == 10)
			pToolStd->SetType(10);
		else if (GetType() >= 40 && GetType() <= 43)
			pToolStd->SetType(GetType());

		if (pToolStd->GenInit() != 0)
			return NULL;
		pCreatedTool = pToolStd;
		break;
	case 20: case 21: case 22: case 23: case 24: 
		NToolDiskStd *pToolDiskStd;
		double diam, b,
				r1, r2, r3, r4,
				l1, l2, hm1, hm2;
				
		pToolDiskStd = (NToolDiskStd*)this;
		pToolDiskStd->GetParam(diam, b, alf, beta,
								r1, r2, r3, r4,
								l1, l2, hm1, hm2);
		fIsNameAuto = pToolDiskStd->IsNameAuto;
		alf = alf*180./PI;
		beta = beta*180./PI;
		pToolDiskStd = new NToolDiskStd(GetName(),
							diam, b, alf, beta,
							r1, r2, r3, r4,
							l1, l2, hm1, hm2);
		pToolDiskStd->IsNameAuto = fIsNameAuto;
		if (pToolDiskStd->GenInit() != 0)
			return NULL;
		pCreatedTool = pToolDiskStd;
		break;
	case 71: 
		NToolBRound *pToolBRound;
		double l;
				
		pToolBRound = (NToolBRound *)this;
		pToolBRound->GetParam(diam, b, l, r, hm);
		fIsNameAuto = pToolBRound->IsNameAuto;
		pToolBRound = new NToolBRound(GetName(),
							diam, b, l, r, hm);
		pToolBRound->IsNameAuto = fIsNameAuto;
		if (pToolBRound->GenInit() != 0)
			return NULL;
		pCreatedTool = pToolBRound;
		break;
	case 70: 
	{
		NToolShaped *pToolShaped = (NToolShaped *)this;
		double buf = pToolShaped->GetH();
		pToolShaped = new NToolShaped(pToolShaped->GetName(), pToolShaped->GetCont(), pToolShaped->IsCutting());
		pToolShaped->SetH(buf);
		if (pToolShaped->GenInit() != 0)
			return NULL;
		pCreatedTool = pToolShaped;
	}
		break;
	case 72:
	{
		NToolVert *pToolVert = (NToolVert *)this;
		double buf = pToolVert->GetH();
		pToolVert = (pToolVert->GetFileName().IsEmpty()) ?
			pToolVert->CreateCopy() :
			new NToolVert(pToolVert->GetName(), pToolVert->GetFileName(), pToolVert->GetCont(), pToolVert->IsCutting());
		pToolVert->SetH(buf);
		if (pToolVert->GenInit() != 0)
		{
			delete pToolVert;
			return NULL;
		}
		pCreatedTool = pToolVert;
	}
		break;
	}
	if(pCreatedTool)
	{
		pCreatedTool->SetZdisp(GetZdisp());
		pCreatedTool->SetType(GetType());
		pCreatedTool->Color = Color;
		pCreatedTool->ColorPassive = ColorPassive;
	}
	pCreatedTool->VertTool = VertTool;
	pCreatedTool->Cutting = Cutting;
	pCreatedTool->Shank = Shank;
	pCreatedTool->SetProbe(IsProbe());

	return pCreatedTool;
}

void NToolMill::Draw(RenderMode GlobRMode, int step)
{
	if (!IsVisible())
		return;
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		AfxMessageBox("Draw 2:OpenGL error " + CString(gluErrorString(errCode)));
	}
	if (WFrame)
		BaseRender::SetWFMode();

	RenderMode LocRMode = GetRMode();
	//////////////////// не активная часть инструмента	/////////////////////////
	ColorPassive.A() = float(Transparency);
	glColor4fv(ColorPassive.array);
	glPushMatrix();
	glTranslated(0., 0., Zdisp);
	if (!RenderP.Draw(GlobRMode, LocRMode))
	{
		GenRender();
		RenderP.Draw(GlobRMode, LocRMode);
	}
	///////////////// активная часть инструмента ///////////////////////////
	Color.A() = float(Transparency);
	glColor4fv(Color.array);

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		AfxMessageBox("Draw 0:OpenGL error " + CString(gluErrorString(errCode)));
	}
	if (!RenderA.Draw(GlobRMode, LocRMode))
	{
		GenRender();
		RenderA.Draw(GlobRMode, LocRMode);
	}
	glPopMatrix();
	if (WFrame)
		BaseRender::RemoveWFMode();

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		AfxMessageBox("Draw 1:OpenGL error " + CString(gluErrorString(errCode)));
	}
}

size_t NToolMill::CreateSkeleton(int PointsNumber, GLfloat* ctlarray)
{
	ToolSkel.RemoveAll();
	ToolSkel.SetSize(PointsNumber / 3 + 1);
	for (int i = 0, k = 0; i < PointsNumber / 3 + 1; ++i, k += 4 * 3)
	{
		NCadrGeom* pGeom = new NCadrGeom;
		pGeom->Set(ccwarc, ctlarray[k], 0., ctlarray[k + 2] + Zdisp, ctlarray[k], 0., ctlarray[k + 2] + Zdisp, -ctlarray[k], 0., 0., XY);
		ToolSkel[i] = pGeom;
	}
	return ToolSkel.GetSize();
}