#include "stdafx.h"
#include "BaseRender.h"
#include "NToolsCrWeldPage.h"
#include "NToolWeld.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolWeld, NTool, 0)

NToolWeld::NToolWeld(void)
{
	SetType(80);
	ConeRad = 5.;
	ConeDir.Set(0., 0., -1., 0.);
	ConeLength = 200.;
}

NToolWeld::~NToolWeld(void)
{
}
NToolWeld* NToolWeld::CreateCopyTool()
{
	NToolWeld *pCreatedTool = new NToolWeld;

	NTool::CreateCopyTo(pCreatedTool);
	
	pCreatedTool->ConeDir = ConeDir;
	pCreatedTool->ConeRad = ConeRad;
	pCreatedTool->ConeLength = ConeLength;
	pCreatedTool->Zdisp = Zdisp;
	pCreatedTool->Name = Name;

	return pCreatedTool;
}
void NToolWeld::Draw(RenderMode GlobRMode, int step)
{

	pQObj = gluNewQuadric();
	gluQuadricOrientation(pQObj, GLU_OUTSIDE);
	if(WFrame)
		BaseRender::SetWFMode();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
	Color.A() = float(Transparency);
	glColor4fv(Color.array);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glPushMatrix();
	BMatr M, T;
	T.Trans(GetDirection() * (/*-ConeLength*/-Zdisp), BPoint(0., 0., 0., 0.)); 
	M.e0cong(BPoint(0., 0., 1., 0), GetDirection());
	glMultMatrixd((T * M).GetArray());
	gluCylinder(pQObj, ConeRad, 0.1, ConeLength, 16, 1);
	glPopMatrix();
	glPopAttrib();
	if(WFrame)
		BaseRender::RemoveWFMode();
	gluDeleteQuadric(pQObj);
}
int NToolWeld::GenInit()
{
	NTool::GenInit();
//	Zdisp = 0.;
	SetType(80);
	Gabar.Expand(-ConeRad, -ConeRad, 0 + Zdisp);
	Gabar.Expand(ConeRad, ConeRad, - ConeLength + Zdisp);
	return 0;
}
void NToolWeld::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrWeldPage)))
		return;// error

	NTool::FillFromPage(pPage);


	NToolsCrWeldPage *pP = (NToolsCrWeldPage *)pPage;
	Zdisp = pP->m_displacement;
	ConeLength = pP->m_focus;
	ConeRad = pP->m_diam * 0.5;
	Name = pP->m_name;
	//m_AngleB = pP->m_AngleB;
	//m_R = pP->m_R;
	//m_R1 = pP->m_R1;
	//m_Edge = pP->m_Edge;
	//Thickness = pP->m_Thickness ;
	//Slope = pP->m_AngleP;
	//m_Whidth = pP->m_Whidth;
	//m_Length = pP->m_Length;
}
void NToolWeld::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrWeldPage)))
		return;// error

	NTool::FillPage(pPage);

	NToolsCrWeldPage *pP = (NToolsCrWeldPage *)pPage;
	pP->m_displacement = Zdisp;
	pP->m_focus = ConeLength;
	pP->m_diam = ConeRad * 2.;
	pP->m_name = Name;
}

void NToolWeld::Serialize(CArchive &ar)
{
	NTool::Serialize(ar);
	SerializeElements(ar, &ConeRad, 1);
	SerializeElements(ar, &ConeLength, 1);
	SerializeElements(ar, &ConeDir, 1);
	if(ar.IsLoading())
		GenInit();
}

