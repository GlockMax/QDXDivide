#include "NToolTurnHolderBar.h"
#include "stdafx.h"
#include "ConstDef.h"
#include "BaseRender.h"
#include "NToolsCrTHBar.h"
#include "NToolTurnHolderBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnHolderBar, NToolTurnHolder, 0)
NToolTurnHolderBar::NToolTurnHolderBar(void) : NToolTurnHolder()
	, Dx(0.)
	, Dy(0.)
	, Dz(0.)
	, Y(0.)
{
	SetType(60);
}
NToolTurnHolderBar::NToolTurnHolderBar(const CString iName, double iDx
		, double iDy, double iDz,  double iY
		, double MountAngle, double ShiftX, double ShiftY, double ShiftZ) : NToolTurnHolder()
{
	SetType(60);
	Name = iName;
	Dx = iDx;
	Y = iY;
	Dy = iDy;
	Dz = iDz;
	MainAngle = MountAngle;
	Displacement.Set(ShiftX, ShiftZ, ShiftY, 0.);
}

NToolTurnHolderBar::~NToolTurnHolderBar(void)
{
}

void NToolTurnHolderBar::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTHBar)))
		return;// error

	NToolTurnHolder::FillPage(pPage);

	NToolsCrTHBar *pP = (NToolsCrTHBar *)pPage;
	pP->m_Dx = Dx;
	pP->m_Dy = Dy;
	pP->m_Dz = Dz;
	pP->m_Y = Y;
}

void NToolTurnHolderBar::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTHBar)))
		return;// error

	NToolTurnHolder::FillFromPage(pPage);

	NToolsCrTHBar *pP = (NToolsCrTHBar *)pPage;
	Dx = pP->m_Dx;
	Dy = pP->m_Dy;
	Dz = pP->m_Dz;
	Y = pP->m_Y;
}
// создание копии инструмента
NToolTurnHolderBar* NToolTurnHolderBar::CreateCopyTool()
{
	NToolTurnHolderBar *pCreatedTool = new NToolTurnHolderBar;

	NToolTurnHolder::CreateCopyTo(pCreatedTool);

	pCreatedTool->Dx = Dx;
	pCreatedTool->Dy = Dy;
	pCreatedTool->Dz = Dz;
	pCreatedTool->Y = Y;

	return pCreatedTool;
}
void NToolTurnHolderBar::Serialize(CArchive &ar)
{
	NToolTurnHolder::Serialize(ar);

	SerializeElements(ar, &Dx, 1);
	SerializeElements(ar, &Dy, 1);
	SerializeElements(ar, &Dz, 1);
	SerializeElements(ar, &Y, 1);
	if(ar.IsLoading())
		GenInit();
}

int NToolTurnHolderBar::GenInit(void)
{
//параметр в диалоге; координата в СК инструмента; имя параметра
//
//
//I. Осн параметры
//1. +Z  (dX)
//2. +X  (dY)
//3. +Y  (dZ)
//
//II. Offset
//
//1.  +Z  (Displacement.X)
//2.  +X  (Displacement.Y)
//3.  -Y  (Y)
//
//III. Main angle
//
//Поворот вокруг оси Y на (-)MainAngle (от Z к X), смещенной на offset,
// т.е. сначала создан параллелепипед по размерам п. I, затем повернут вокруг Y на -MainAngle(п.III), 
// затем перемещен по п.II

	Gabar.Init(); 
	Gabar.Expand(BPoint(0., 0., 0., 1.));
	Gabar.Expand(BPoint(Dx, Dy, Dz, 1.));
	Gabar.Transform(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.));
	Gabar.Transform(GetPosMatr() * BMatr().Trans(0., -Y, 0.));
	return 0;
}
void NToolTurnHolderBar::Draw(RenderMode GlobRMode, int step)
{
	if(!IsVisible())
		return;
	ColorPassive.A() = float(Transparency);
	glColor4fv(ColorPassive.array);
	glPushMatrix();
	glRotated(-90., 1., 0., 0.);// NT
	glRotated(-90., 0., 0., 1.);// NT
	glTranslated(-Displacement.GetX(), -Displacement.GetY(), -Displacement.GetZ());
	glRotated(MainAngle, 0., 0., 1.);
	auto LocRMode = GetRMode();
	if (GlobRMode == RM_TRANSLUCENT && LocRMode == RM_TRANSLUCENT || LocRMode != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT)
	{
		if (!WFrame)
		{
			glPushAttrib(GL_ENABLE_BIT);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1., 1.);
			glBegin(GL_QUADS);
			glNormal3d(0., 0., 1.);
			glVertex3d(0., 0., 0. - Y);
			glVertex3d(Dx, 0., 0. - Y);
			glVertex3d(Dx, Dy, 0. - Y);
			glVertex3d(0., Dy, 0. - Y);
			glNormal3d(0., 0., 1.);
			glVertex3d(0., 0., Dz - Y);
			glVertex3d(Dx, 0., Dz - Y);
			glVertex3d(Dx, Dy, Dz - Y);
			glVertex3d(0., Dy, Dz - Y);

			glNormal3d(0., -1., 0.);
			glVertex3d(0., 0., 0. - Y);
			glVertex3d(Dx, 0., 0. - Y);
			glVertex3d(Dx, 0., Dz - Y);
			glVertex3d(0., 0., Dz - Y);
			glNormal3d(0., -1., 0.);
			glVertex3d(0., Dy, 0. - Y);
			glVertex3d(Dx, Dy, 0. - Y);
			glVertex3d(Dx, Dy, Dz - Y);
			glVertex3d(0., Dy, Dz - Y);

			glNormal3d(1., 0., 0.);
			glVertex3d(0., 0., 0. - Y);
			glVertex3d(0., Dy, 0. - Y);
			glVertex3d(0., Dy, Dz - Y);
			glVertex3d(0., 0., Dz - Y);
			glNormal3d(1., 0., 0.);
			glVertex3d(Dx, 0., 0. - Y);
			glVertex3d(Dx, Dy, 0. - Y);
			glVertex3d(Dx, Dy, Dz - Y);
			glVertex3d(Dx, 0., Dz - Y);
			glEnd();
			glPopAttrib();
		}
	}
	// Draw edges
	if (GlobRMode != RM_TRANSLUCENT)
	{
		BaseRender::SetWFMode();
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
		glLineWidth(2.);
		glColor3f(0., 0., 0.);

		glBegin(GL_LINE_STRIP);
		glVertex3d(0., 0., 0. - Y);
		glVertex3d(Dx, 0., 0. - Y);
		glVertex3d(Dx, 0., Dz - Y);
		glVertex3d(0., 0., Dz - Y);
		glVertex3d(0., 0., 0. - Y);
		glVertex3d(0., Dy, 0. - Y);
		glVertex3d(Dx, Dy, 0. - Y);
		glVertex3d(Dx, Dy, Dz - Y);
		glVertex3d(0., Dy, Dz - Y);
		glVertex3d(0., Dy, 0. - Y);
		glEnd();
		glBegin(GL_LINES);
		glVertex3d(Dx, 0., 0. - Y);
		glVertex3d(Dx, Dy, 0. - Y);
		glVertex3d(Dx, 0., Dz - Y);
		glVertex3d(Dx, Dy, Dz - Y);
		glVertex3d(0., 0., Dz - Y);
		glVertex3d(0., Dy, Dz - Y);
		glEnd();

		glPopAttrib();
		BaseRender::RemoveWFMode();
	}
	glPopMatrix();
}

float* NToolTurnHolderBar::GetCompTriangles() const
{
	BMatr M(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.)
		* GetPosMatr() * BMatr().Trans(0., -Y, 0.));
	std::vector<BPoint> Vert(8);
	Vert[0].Set(0., 0., 0., 1.);
	Vert[1].Set(Dx, 0., 0, 1.);
	Vert[2].Set(Dx, Dy, 0., 1.);
	Vert[3].Set(0., Dy, 0., 1.);
	Vert[4].Set(0., 0., Dz, 1.);
	Vert[5].Set(Dx, 0., Dz, 1.);
	Vert[6].Set(Dx, Dy, Dz, 1.);
	Vert[7].Set(0., Dy, Dz, 1.);
	for(int k = 0; k < Vert.size(); ++k)
		Vert[k] *= M;
	std::vector<int> Ind(12 * 3);
	Ind[0] = 0; Ind[1] = 1; Ind[2] = 2;
	Ind[3] = 0; Ind[4] = 2; Ind[5] = 3;

	Ind[6] = 4; Ind[7] = 6; Ind[8] = 5;
	Ind[9] = 4; Ind[10] = 7; Ind[11] = 6;

	Ind[12] = 1; Ind[13] = 0; Ind[14] = 4;
	Ind[15] = 0; Ind[16] = 5; Ind[17] = 1;

	Ind[18] = 2; Ind[19] = 1; Ind[20] = 5;
	Ind[21] = 1; Ind[22] = 6; Ind[23] = 2;

	Ind[24] = 3; Ind[25] = 2; Ind[26] = 6;
	Ind[27] = 2; Ind[28] = 3; Ind[29] = 7;

	Ind[30] = 0; Ind[31] = 3; Ind[32] = 7;
	Ind[33] = 3; Ind[34] = 0; Ind[35] = 4;

	float* pTriangles = new float[12 * 3 * 6 + 1];
	pTriangles[0] = 12;
	int k = 1;
	for (int i = 0; i < 12 * 3; ++i)
	{
		pTriangles[k++] = 0.;
		pTriangles[k++] = 0.;
		pTriangles[k++] = 0.;
		pTriangles[k++] = float(Vert[Ind[i]].GetX());
		pTriangles[k++] = float(Vert[Ind[i]].GetY());
		pTriangles[k++] = float(Vert[Ind[i]].GetZ());
	}
	return pTriangles;
}
