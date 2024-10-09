#include "NToolTurnHolderCyl.h"
#include "stdafx.h"
#include "ConstDef.h"
#include "BaseRender.h"
#include "NToolsCrTHCyl.h"
#include "NToolTurnHolderCyl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurnHolderCyl, NToolTurnHolder, 0)
NToolTurnHolderCyl::NToolTurnHolderCyl(void) : NToolTurnHolder()
	, Dx(0.)
	, D1(0.)
	, D2(0.)
	, Y(0.)
	, FH(0)
{
	SetType(61);
}
NToolTurnHolderCyl::NToolTurnHolderCyl(const CString iName, double iDx, double iY
		, double iD1, double iD2, int iFH
		, double MountAngle, double ShiftX, double ShiftY, double ShiftZ) : NToolTurnHolder()
{
	SetType(61);
	Name = iName;
	Dx = iDx;
	Y = iY;
	D1 = iD1;
	D2 = iD2;
	FH = iFH;
	MainAngle = MountAngle;
	Displacement.Set(ShiftX, ShiftZ, ShiftY, 0.);
}
NToolTurnHolderCyl::~NToolTurnHolderCyl(void)
{
}

void NToolTurnHolderCyl::FillPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTHCyl)))
		return;// error

	NToolTurnHolder::FillPage(pPage);

	NToolsCrTHCyl *pP = (NToolsCrTHCyl *)pPage;
	pP->m_Dx = Dx;
	pP->m_Y = Y;
	pP->m_D1 = D1;
	pP->m_D2 = D2;
	pP->m_FH = FH;
}

void NToolTurnHolderCyl::FillFromPage(CDialog *pPage)
{
	if(!pPage->IsKindOf(RUNTIME_CLASS(NToolsCrTHCyl)))
		return;// error

	NToolTurnHolder::FillFromPage(pPage);

	NToolsCrTHCyl *pP = (NToolsCrTHCyl *)pPage;
	Dx = pP->m_Dx;
	Y = pP->m_Y;
	D1 = pP->m_D1;
	D2 = pP->m_D2;
	FH = pP->m_FH;
}
// создание копии инструмента
NToolTurnHolderCyl* NToolTurnHolderCyl::CreateCopyTool()
{
	NToolTurnHolderCyl *pCreatedTool = new NToolTurnHolderCyl;

	NToolTurnHolder::CreateCopyTo(pCreatedTool);

	pCreatedTool->Dx = Dx;
	pCreatedTool->Y = Y;
	pCreatedTool->D1 = D1;
	pCreatedTool->D2 = D2;
	pCreatedTool->FH = FH;

	return pCreatedTool;
}
void NToolTurnHolderCyl::Serialize(CArchive &ar)
{
	NToolTurnHolder::Serialize(ar);

	SerializeElements(ar, &Dx, 1);
	SerializeElements(ar, &Y, 1);
	SerializeElements(ar, &D1, 1);
	SerializeElements(ar, &D2, 1);
	SerializeElements(ar, &FH, 1);
	if(ar.IsLoading())
		GenInit();
}

int NToolTurnHolderCyl::GenInit(void)
{
//параметр в диалоге; координата в —  инструмента; им€ параметра
//
//
//I. ќсн параметры
//1. +Z  (dX)
//2. +XY  (D1)
//3. +XY  (D2)
//
//II. Offset
//
//1.  +Z  (Displacement.X)
//2.  +X  (Displacement.Y)
//3.  -Y  (Y)
//
//III. Main angle
//
//ѕоворот вокруг оси Y на (-)MainAngle (от Z к X), смещенной на offset,
// т.е. сначала создан параллелепипед по размерам п. I, затем повернут вокруг Y на -MainAngle(п.III), 
// затем перемещен по п.II

	Gabar.Init(); 
	double D = max(D1, D2);
	Gabar.Expand(BPoint(-D * 0.5, -D * 0.5, 0.));
	Gabar.Expand(BPoint(D * 0.5, (FH == 0) ? D * 0.5 : 0., Dx));// FH == 0 - full cone, otherwise (== 1) - half cone
	Gabar.Transform(GetPosMatr() * BMatr().Trans(0., -Y, 0.));
	return 0;
}
void NToolTurnHolderCyl::Draw(RenderMode GlobRMode, int step)
{
	if(!IsVisible())
		return;

	const int bStepsNum = 40;// Must be even
	double AngleStep = 360. / bStepsNum;
	BPoint Me[3][bStepsNum + 1];
	int StepsNum = FH == 0 ? bStepsNum : bStepsNum / 2;

	BPoint N(0.,1.,0.,0.);
	BPoint P0(0., D1 / 2., 0., 1.);
	BPoint P1(Dx, D2 / 2., 0., 1.);
	BPoint C0(0., 0., 0., 1.);
	BPoint C1(Dx, 0., 0., 1.);

	BMatr Rot;
	Rot.RotX(BPoint(0.,0.,0.,1.), -AngleStep);

	Me[0][0] = N;
	Me[1][0] = P0;
	Me[2][0] = P1;
	for(int i = 1; i <= StepsNum; ++i)
	{
		Me[0][i] = Me[0][i - 1] * Rot;
		Me[1][i] = Me[1][i - 1] * Rot;
		Me[2][i] = Me[2][i - 1] * Rot;
	}
	glPushMatrix();
	glRotated(-90., 1., 0., 0.);// NT
	glRotated(-90., 0., 0., 1.);// NT
	glTranslated(-Displacement.GetX(), -Displacement.GetY(), -Displacement.GetZ() - Y);
	glRotated(MainAngle, 0., 0., 1.);

	auto LocRMode = GetRMode();
	if (!WFrame && (GlobRMode == RM_TRANSLUCENT && LocRMode == RM_TRANSLUCENT || LocRMode != RM_TRANSLUCENT && GlobRMode != RM_TRANSLUCENT))
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
		Color.A() = float(Transparency);
		glColor4fv(Color.array);
		glBegin(GL_QUAD_STRIP);// Ѕокова€ поверхность
		for (int i = 0; i <= StepsNum; ++i)
		{
			glNormal3dv(Me[0][i].GetArray());
			glVertex3dv(Me[1][i].GetArray());
			glVertex3dv(Me[2][i].GetArray());
		}
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glNormal3d(-1., 0., 0.);
		glVertex3dv(C0.GetArray());
		for (int k = 0; k <= StepsNum; ++k)
			glVertex3dv(Me[1][k].GetArray());
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glNormal3d(1., 0., 0.);
		glVertex3dv(C1.GetArray());
		for (int l = StepsNum; l >= 0; --l)
			glVertex3dv(Me[2][l].GetArray());
		glEnd();
		if (FH == 1)
		{// Half of cone
			glBegin(GL_QUADS);
			glNormal3d(0., 0., 1.);
			glVertex3d(0., -D1 / 2., 0.);
			glVertex3d(Dx, -D2 / 2., 0.);
			glVertex3d(Dx, D2 / 2., 0.);
			glVertex3d(0., D1 / 2., 0.);
			glEnd();
		}
		glPopAttrib();
	}
	// Draw edges
	if (GlobRMode != RM_TRANSLUCENT)
	{
		BaseRender::SetWFMode();
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
		glLineWidth(2.);
		glColor3f(0., 0., 0.);

		glBegin(GL_LINE_STRIP);
		for (int k = 0; k <= StepsNum; ++k)
			glVertex3dv(Me[1][k].GetArray());
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (int l = StepsNum; l >= 0; --l)
			glVertex3dv(Me[2][l].GetArray());
		glEnd();
		if (FH == 1)
		{// Half of cone
			glBegin(GL_QUADS);
			glVertex3d(0., -D1 / 2., 0.);
			glVertex3d(Dx, -D2 / 2., 0.);
			glVertex3d(Dx, D2 / 2., 0.);
			glVertex3d(0., D1 / 2., 0.);
			glEnd();
		}
		glPopAttrib();
		BaseRender::RemoveWFMode();
	}

	glPopMatrix();
}

float* NToolTurnHolderCyl::GetCompTriangles() const
{
	BMatr M(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.)
		* GetPosMatr() * BMatr().Trans(0., -Y, 0.));

	const int bStepsNum = 40;// Must be even
	const double AngleStep = 360. / bStepsNum;
	BPoint Me[2][bStepsNum + 1];
	const int StepsNum = FH == 0 ? bStepsNum : bStepsNum / 2;

	BPoint P0(0., D1 / 2., 0., 1.);
	BPoint P1(Dx, D2 / 2., 0., 1.);

	BMatr Rot;
	Rot.RotX(BPoint(0., 0., 0., 1.), -AngleStep);

	Me[0][0] = P0;
	Me[1][0] = P1;
	for (int i = 1; i <= StepsNum; ++i)
	{
		Me[0][i] = Me[0][i - 1] * Rot;
		Me[1][i] = Me[1][i - 1] * Rot;
	}
	for (int i = 0; i <= StepsNum; ++i)
	{
		Me[0][i] *= M;
		Me[1][i] *= M;
	}
	int TriNum = StepsNum * 2 /* side surf */ + (StepsNum - 1) * 2 /* faces */ + ((FH == 1) ? 2 : 0) /* half rectangle */;
	std::vector<std::pair<int, int>> Ind;
	Ind.reserve(TriNum * 3);
	// Ѕокова€ поверхность
	for (int i = 0; i < StepsNum; ++i)
	{
		Ind.push_back(std::pair < int, int>(0, i));
		Ind.push_back(std::pair < int, int>(1, i));
		Ind.push_back(std::pair < int, int>(0, i + 1));
		Ind.push_back(std::pair < int, int>(0, i));
		Ind.push_back(std::pair < int, int>(1, i + 1));
		Ind.push_back(std::pair < int, int>(0, i + 1));
	}
	// faces
	for (int i = 1; i < StepsNum; ++i)
	{
		Ind.push_back(std::pair < int, int>(0, 0));
		Ind.push_back(std::pair < int, int>(0, i + 1));
		Ind.push_back(std::pair < int, int>(0, i));
		Ind.push_back(std::pair < int, int>(1, 0));
		Ind.push_back(std::pair < int, int>(1, i));
		Ind.push_back(std::pair < int, int>(1, i + 1));
	}
	if (FH == 1)
	{// Half of cone
		Ind.push_back(std::pair < int, int>(0, 0));
		Ind.push_back(std::pair < int, int>(0, StepsNum));
		Ind.push_back(std::pair < int, int>(1, 0));
		Ind.push_back(std::pair < int, int>(1, 0));
		Ind.push_back(std::pair < int, int>(0, StepsNum));
		Ind.push_back(std::pair < int, int>(1, StepsNum));
	}
	float* pTriangles = new float[TriNum * 3 * 6 + 1];
	pTriangles[0] = float(TriNum);
	int k = 1;
	for (int i = 0; i < TriNum * 3; ++i)
	{
		pTriangles[k++] = 0.;
		pTriangles[k++] = 0.;
		pTriangles[k++] = 0.;
		pTriangles[k++] = float(Me[Ind[i].first][Ind[i].second].GetX());
		pTriangles[k++] = float(Me[Ind[i].first][Ind[i].second].GetY());
		pTriangles[k++] = float(Me[Ind[i].first][Ind[i].second].GetZ());
	}
	return pTriangles;
}
