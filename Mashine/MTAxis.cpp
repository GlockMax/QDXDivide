// MTAxis.cpp: implementation of the MTAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "constdef.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "MTAxis.h"
#include "BMatr.h"
#include "math.h"
#include "mtaxis.h"
#include "BaseRender.h"
#include "BBox.h"
#include "RArrow.h"
#include "MTSpinAxis.h"
#include "MTExpression.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTAxis::MTAxis()
{
	XYZB.SetE();
	XYZBOr.SetE();
	LocalDisplVec.Set(0, 0, 0, 0);
	InitVectorY(BPoint(0., 1., 0., 0.));

	Length = 10.;
	Parent = _T("");
	Marked = false;
	Visible = false;

	Movement = 0.;
	MoveMin = 0.;
	MoveMax = 0.;

	IsCS = false;

	CanMoveWhileSpin = false;
	CanBeSpinned = false;
	Spinning = false;

	pExpr = nullptr;
}

MTAxis::~MTAxis()
{
	delete pExpr;
	pExpr = nullptr;
}

void MTAxis::SetName(const CString& NewName)
{
	Name = NewName;	
}

const CString & MTAxis::GetName() const
{
	return Name;
}

void MTAxis::SetParent(const CString& NewParent)
{
	Parent = NewParent;
}

const CString & MTAxis::GetParent() const
{
	return Parent;
}

void MTAxis::InitBasePoint(const BPoint& point)
{
	XYZBOr.SetRow(3, point);
	XYZB.SetRow(3, point);
}

const BPoint MTAxis::GetBasePoint() const
{
	ASSERT(XYZB.GetRow(3).GetH() == 1.);
	return XYZB.GetRow(3);
}

const BPoint MTAxis::GetBasePointOr() const
{
	ASSERT(XYZBOr.GetRow(3).GetH() == 1.);
	return XYZBOr.GetRow(3);
}

void MTAxis::SetColor(NColor NewC)
{
	Color = NewC;
}

NColor MTAxis::GetColor() const
{
	return Color;
}

void MTAxis::SetVector(const BPoint& vector)
{
	XYZB.SetRow(0, vector * (1. / sqrt(vector.D2())));
}

void MTAxis::InitVector(const BPoint& vector)
{
	SetVector(vector);
	XYZBOr.SetRow(0, vector);
}

void MTAxis::SetVectorY(const BPoint& vector)
{
	XYZB.SetRow(1, vector * (1. / sqrt(vector.D2())));
}

void MTAxis::InitVectorY(const BPoint& vector)
{
	SetVectorY(vector);
	XYZBOr.SetRow(1, vector);
}

const BPoint MTAxis::GetVector() const
{
	return XYZB.GetRow(0);
}

const BPoint MTAxis::GetVectorOr() const
{
	return XYZBOr.GetRow(0);
}

void MTAxis::GoTo(const BMatr &MoveMatrix)
{
	XYZB = XYZBOr * MoveMatrix;
}

void MTAxis::Move(const BMatr &MoveMatrix)
{
	XYZB = XYZB * MoveMatrix;
}

BOOL MTAxis::CreateMoveMatrix(BMatr &MoveMatrix, double param)
{
	Movement = Movement + param;

	if (GetType() == AT_SPIN)
		Spinning = (Movement != 0.);

	if (IsCS)
		MoveMatrix.Trans(BPoint(0., 0., 0., 1.), GetBasePoint());
	else
		MoveMatrix.SetE();

	// check new value
	if ((MoveMax == 0 && MoveMin == 0))
		return TRUE;
	if ((Movement > MoveMax) || (Movement < MoveMin))
		return FALSE;
	return TRUE;
}

void MTAxis::Draw(bool selected)
{
	if (!Visible)
		return;
	DrawArrow(GetVector(), NColor(1., 0., 0., 1.));
	DrawArrow(GetVectorY(), NColor(0., 1., 0., 1.));
	DrawArrow(GetVectorZ(), NColor(0., 0., 1., 1.));

	if (selected)
	{
		DrawGabar(GetVector());
		DrawGabar(GetVectorY());
		DrawGabar(GetVectorZ());
	}
}

MTAxis::MTAxis(const MTAxis &Ax)
{
	*this = Ax;
	pExpr = nullptr;
}

const MTAxis& MTAxis::operator=(const MTAxis& Ax)
{
	XYZB = Ax.XYZBOr;
	XYZBOr = Ax.XYZBOr;
	LocalDisplVec = (Ax.GetLocalDisplVec());

	Length = Ax.Length;
	SetName(Ax.GetName());
	SetParent(Ax.GetParent());
	pExpr = nullptr;
	SetExpr(Ax.GetExpr());
	SetColor(Ax.GetColor());
	Marked = Ax.Marked;
	Visible = Ax.Visible;

	Movement = Ax.GetMovement();
	MoveMax = Ax.GetMax();
	MoveMin = Ax.GetMin();

	SetIsCS(Ax.GetIsCS());
	return *this;
}

MTAxis::AXIS_TYPE MTAxis::GetType() const
{
	return AT_EMPTY;
}

void MTAxis::InitBasePoint(double x, double y, double z)
{
	InitBasePoint(BPoint(x, y, z, 1));
}

void MTAxis::SetColor(COLORREF Col)
{
	Color.SetRGB(Col);
}

const BPoint MTAxis::GetVectorY() const
{
	return XYZB.GetRow(1);
}

const BPoint MTAxis::GetVectorYOr() const
{
	return XYZBOr.GetRow(1);
}

const BPoint MTAxis::GetVectorZ() const
{
	return XYZB.GetRow(2);
}

const BPoint MTAxis::GetVectorZOr() const
{
	return XYZBOr.GetRow(2);
}

void MTAxis::MakeVectorZ()
{
	BPoint p(GetVector() % GetVectorY());
	double D = sqrt(p.D2());
	if (D < MIND)
		XYZB.SetRow(2, GetVector());
	else
		XYZB.SetRow(2, p * (1. / D));
}

void MTAxis::MakeVectorZOr()
{
	BPoint p(GetVectorOr() % GetVectorYOr());
	double D = sqrt(p.D2());
	if (D < MIND)
		XYZBOr.SetRow(2, GetVector());
	else
		XYZBOr.SetRow(2, p * (1. / D));
}

const BPoint & MTAxis::GetLocalDisplVec() const
{
	return 	LocalDisplVec;
}

void MTAxis::SetLocalDisplVec(const BPoint& p)
{
	LocalDisplVec = p;
	return;
}

double MTAxis::GetMin() const
{
	return MoveMin;
}

double MTAxis::GetMax() const
{
	return MoveMax;
}

void MTAxis::SetMin(double dmin)
{
	MoveMin = dmin;
}

void MTAxis::SetMax(double dmax)
{
	MoveMax = dmax;
}

double MTAxis::GetMovement() const
{
	return Movement;
}

void MTAxis::SetMovement(double move)
{
	Movement = move;
}
bool MTAxis::HaveExpr() const
{
	return !Expression.IsEmpty();
}
const CString & MTAxis::GetExpr() const
{
	return Expression;
}

void MTAxis::SetExpr(const CString & NewExpr)
{
	Expression = NewExpr;
	delete pExpr;
	pExpr = nullptr;
}

const std::vector<int>& MTAxis::GetExprIndexes() const
{
	return pExpr->GetIndexes();
}

double MTAxis::CalcExprValue(const std::vector<double>& VarValues)
{
	return pExpr->CalcValue(VarValues);
}

bool MTAxis::IsMarked(void) const
{
	return Marked;
}
void MTAxis::SetMarked(bool Mark)
{
	Marked = Mark;
}

void MTAxis::PropParentSpinStatus(const MTAxis* pParent)
{
	if (pParent == nullptr)
	{
		CanBeSpinned = false;
		CanMoveWhileSpin = false;
		Spinning = false;
		return;
	}

	const MTSpinAxis* pSpinAx = dynamic_cast<const MTSpinAxis*>(pParent);
	if (pSpinAx != nullptr || pParent->CanBeSpinned)
		CanBeSpinned = true;
	else
		CanBeSpinned = false;

	CanMoveWhileSpin = pParent->Spinning;
	if (CanBeSpinned)
	{
		const auto Type = pParent->GetType();
		if (Type == MTAxis::AT_FOR || Type == MTAxis::AT_ROT || Type == MTAxis::AT_SCHAIN)
			CanMoveWhileSpin = true;
	}

	Spinning = pParent->Spinning;
}

BOOL MTAxis::CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double param) const
{
	return 0;
}

BOOL MTAxis::CreateDerivMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const
{
	return 0;
}

BOOL MTAxis::CreateInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param) const
{
	return 0;
}

BOOL MTAxis::CreateDerivInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const
{
	return 0;
}

void MTAxis::DrawArrow(const BPoint &Dir, NColor & HeadColor)
{
	RArrow Arrow;
	Arrow.Set(GetBasePoint(), Dir, Length);
	Arrow.SetColor(Color.R(), Color.G(), Color.B());
	Arrow.SetConeColor(HeadColor.R(), HeadColor.G(), HeadColor.B());
	Arrow.Draw();
}

void MTAxis::DrawGabar(const BPoint &Dir)
{
	RArrow Arrow;
	Arrow.Set(GetBasePoint(), Dir, Length);
	BBox box = Arrow.GetGabar();
	BMatr M;
	BPoint P;
	P.Set(0., 0., 1., 0.);
	M.e0cong(P,Dir);
	glPushMatrix();
	glTranslated(GetBasePoint().GetX(), GetBasePoint().GetY(), GetBasePoint().GetZ());
	glMultMatrixd(M.GetArray());
	glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
	glLineWidth(3.f);
	glColor3i(0,0,0);
	BaseRender::DrawBox(box); 
	glPopAttrib();
	glPopMatrix();
}

bool MTAxis::CompileExpr(const std::string& exprText, const std::vector<std::string>& varNames, const std::vector<int>& varIndexes)
{
	delete pExpr;
	pExpr = new MTExpression();
	pExpr->Define(exprText, varNames, varIndexes);
	return true;
}

void MTAxis::Reset()
{
	XYZB = XYZBOr;
	Movement = 0;
	Spinning = false;
}

const BMatr& MTAxis::GetMatr() const
{
	return XYZB;
}

const BMatr& MTAxis::GetMatrOr() const
{
	return XYZBOr;
}
