#include "..\Common\NToolTurnHolderSTL.h"
#include "..\Common\NToolTurnHolderSTL.h"
#include "..\Common\NToolTurnHolderSTL.h"
#include "stdafx.h"
#include "NCMProject.h"
#include "NToolsCrTurnHoldSTLDlg.h"
#include "NToolTurnHolderSTL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(NToolTurnHolderSTL, NToolTurnHolder, 0)
NToolTurnHolderSTL::NToolTurnHolderSTL(void) : NToolTurnHolder()
{
	SetType(62);
}
NToolTurnHolderSTL::NToolTurnHolderSTL(const CString &iName, const CString &FileName
		, double MountAngle, double ShiftX, double ShiftY, double ShiftZ) : NToolTurnHolder()
{
	SetType(62);
	Name = iName;
	MainAngle = MountAngle;
	Displacement.Set(ShiftX, ShiftZ, ShiftY, 0.);
	Stl.SetFileName(FileName);
}

NToolTurnHolderSTL::~NToolTurnHolderSTL(void)
{
}

void NToolTurnHolderSTL::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTurnHoldSTLDlg)))
		return;// error

	NToolTurnHolder::FillPage(pPage);

	NToolsCrTurnHoldSTLDlg *pP = (NToolsCrTurnHoldSTLDlg *)pPage;
	pP->FileName = Stl.GetFileName();
	Y = Displacement.GetZ();
	pP->m_Y = Y;
	pP->m_DX = float(Stl.GetGabar().dX());
	pP->m_DY = float(Stl.GetGabar().dY());
	pP->m_DZ = float(Stl.GetGabar().dZ());
}

void NToolTurnHolderSTL::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTurnHoldSTLDlg)))
		return;// error

	NToolTurnHolder::FillFromPage(pPage);

	NToolsCrTurnHoldSTLDlg *pP = (NToolsCrTurnHoldSTLDlg *)pPage;
	Stl.SetFileName(pP->FileName);
	Y = pP->m_Y;
	Displacement.SetZ(Y);
}
// создание копии инструмента
NToolTurnHolderSTL* NToolTurnHolderSTL::CreateCopyTool()
{
	NToolTurnHolderSTL *pCreatedTool = new NToolTurnHolderSTL;

	NToolTurnHolder::CreateCopyTo(pCreatedTool);

	pCreatedTool->Stl.SetFileName(Stl.GetFileName());
	pCreatedTool->Y = Y;
	pCreatedTool->GenInit();

	return pCreatedTool;
}
void NToolTurnHolderSTL::Serialize(CArchive& ar)
{
	NToolTurnHolder::Serialize(ar);

	Stl.Serialize(ar, NCM_PROJECT.GetInFileVersion());
	SerializeElements(ar, &Y, 1);
	if (ar.IsLoading())
		GenInit();
}

int NToolTurnHolderSTL::GenInit(void)
{
	bool IsCreated = Stl.Create();
	if(IsCreated)
	{
		BMatr M = BMatr().RotZ(0., 0., 0., MainAngle) * BMatr().Trans(-Displacement.GetX(), -Displacement.GetY(), -Displacement.GetZ()); 
		Stl.Transform(M);
	}
	Gabar = Stl.GetGabar();
	Gabar.Transform(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.));
	return 0;
}
void NToolTurnHolderSTL::Draw(RenderMode GlobRMode, int step)
{
	if(!IsVisible())
		return;
	ColorPassive.A() = float(Transparency);
	glColor4fv(ColorPassive.array);
	glPushMatrix();
	glRotated(-90., 1., 0., 0.);// NT
	glRotated(-90., 0., 0., 1.);// NT
	//glTranslated(Displacement.GetX(), Displacement.GetY(), Displacement.GetZ());
	//glRotated(MainAngle, 0.,0.,1.);
	Stl.DrawOriginal(RM_UNDEF, RM_UNDEF, WFrame == TRUE, TRUE, true, FALSE);
	glPopMatrix();
}

const CString & NToolTurnHolderSTL::GetFileName()
{
	return Stl.GetFileName();
}

const GLfloat* NToolTurnHolderSTL::GetTriangles(void) const
{
	return Stl.GetTriangles();
}

bool NToolTurnHolderSTL::FindSegments(SnapRopes& oSegments)
{
	return Stl.FindSegments(oSegments);
}
