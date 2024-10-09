#include "stdafx.h"
#include "BOpenGLWnd.h"
#include "BaseRender.h"
#include "BOpenGLCtrl.h"
#include "NCMComData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BOpenGLCtrl::BOpenGLCtrl(BOpenGLWnd *pP)
{
	pParent = pP;
	Active = false;
	SelectedInd = -1;
	GabarRect.SetRect(0, 0, 0, 0);
	Scale = 1.;
	Elems.push_back(new ElemMain(this, IDS_EMAIN));
	Elems.push_back(new ElemArrowZ(this, IDS_ELVIEW));
	Elems.push_back(new ElemArrowX(this, IDS_ELVIEW));
	Elems.push_back(new ElemArrowY(this, IDS_ELVIEW));
	Elems.push_back(new ElemArrow_Z(this, IDS_ELVIEW));
	Elems.push_back(new ElemArrow_X(this, IDS_ELVIEW));
	Elems.push_back(new ElemArrow_Y(this, IDS_ELVIEW));
	Elems.push_back(new ElemArrowRotZ(this, IDS_ELROT));
	Elems.push_back(new ElemArrowRot_Z(this, IDS_EL_ROT));
	Elems.push_back(new ElemArrowRotX(this, IDS_ELROT));
	Elems.push_back(new ElemArrowRot_X(this, IDS_EL_ROT));
	Elems.push_back(new ElemArrowRotY(this, IDS_ELROT));
	Elems.push_back(new ElemArrowRot_Y(this, IDS_EL_ROT));
	Elems.push_back(new ElemArrowIsoX(this, IDS_EISO));
	Elems.push_back(new ElemArrowIso_X(this, IDS_EISO));
	Elems.push_back(new ElemArrowIsoY(this, IDS_EISO));
	Elems.push_back(new ElemArrowIso_Y(this, IDS_EISO));
	Elems.push_back(new ElemArrowIsoZ(this, IDS_EISO));
	Elems.push_back(new ElemArrowIso_Z(this, IDS_EISO));
	Elems.push_back(new ElemArrowLetXYZ(this, 0));
}

BOpenGLCtrl::~BOpenGLCtrl()
{
	for each (BOGLCtrlElem *pElem in Elems)
		delete pElem;
}

bool BOpenGLCtrl::OnLButtonDown(UINT nFlags, const CPoint & point)
{
	return false;
}

bool BOpenGLCtrl::OnLButtonUp(UINT nFlags, const CPoint & point)
{
	
	if (SelectedInd >= 0 && SelectedInd < int(Elems.size()))
		Elems[SelectedInd]->DoAction();

	return false;
}

bool BOpenGLCtrl::OnMouseMove(UINT nFlags, const CPoint & point)
{
	CalcScale();
	bool NewActive = HitTest(point);
	if (NewActive != Active)
	{
		Active = NewActive;
		if (Active)
		{
			glDrawBuffer(GL_FRONT);
			Draw();
			glDrawBuffer(GL_BACK);
		}
	}
	int NewSelectedInd = -1;
	if (Active)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int px = min(point.x, viewport[2] - 1);
		int py = min(point.y, viewport[3] - 1);
		px = max(1, px);
		py = max(1, py);
		GLubyte StencilVal = 0;
		glReadPixels(px, viewport[3] - py, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &StencilVal);
		NewSelectedInd = (StencilVal >> 2) - 1;
	}
	if (NewSelectedInd != SelectedInd)
	{
		SelectedInd = NewSelectedInd;
		glDrawBuffer(GL_FRONT);
		Draw();
		glDrawBuffer(GL_BACK);
		UINT MessID = (SelectedInd == -1) ? AFX_IDS_IDLEMESSAGE : Elems[SelectedInd]->GetMessageID();
		if (MessID > 0)
			static_cast<CFrameWnd *>(AfxGetMainWnd())->SetMessageText(MessID);
	}

	return false;
}

void BOpenGLCtrl::Draw()
{
	CalcScale();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
	glLineWidth(5.);
	glScissor(0, 0, ZoneXSize, ZoneYSize);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xfc);
	glClearStencil(0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (unsigned int i = 0; i < Elems.size(); ++i)
	{
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, (i + 1) << 2, 0xfc);
		Elems[i]->Draw(SelectedInd == i);
	}
	glPopAttrib();
	glFinish();
}

bool BOpenGLCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (SelectedInd >= 0 && SelectedInd < int(Elems.size()))
		return Elems[SelectedInd]->DoWheelAction(zDelta);
	return false;
}

bool BOpenGLCtrl::HitTest(const CPoint & point)
{
	return (GabarRect.PtInRect(point) == TRUE);
}

void BOpenGLCtrl::CalcScale()
{
	GLint viewport[4];
	GLdouble projMatrix[16], modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject((int)(viewport[0] + ShiftX),
		(int)(viewport[1] + ShiftY),
		0.5,
		modelMatrix, projMatrix, viewport, &X0, &Y0, &Z0);
	double x, y, z;
	BaseRender::GetPixelDim(x, y, z);
	Scale = x;
	GabarRect.SetRect(0, viewport[3] - ZoneYSize, ZoneXSize, viewport[3]);
}

BOpenGLCtrl::BOGLCtrlElem::BOGLCtrlElem(BOpenGLCtrl *pParCtrl, UINT iMessageID) : Ctrl(*pParCtrl)
{
	MessageID = iMessageID;
}

void BOpenGLCtrl::BOGLCtrlElem::DrawLetter(char  Letter, const BPoint & P0, const BPoint & Ex, const BPoint & Ey)
{
	// P0 - center point, Ex - X axis half vector, Ey - Y axis half vector
	switch (Letter)
	{
	case 'X':
	{
		BPoint Plb = P0 - Ex - Ey;
		BPoint Prb = P0 + Ex - Ey;
		BPoint Plt = P0 - Ex + Ey;
		BPoint Prt = P0 + Ex + Ey;
		glBegin(GL_LINES);
		glVertex3d(Plb.GetX(), Plb.GetY(), Plb.GetZ());
		glVertex3d(Prt.GetX(), Prt.GetY(), Prt.GetZ());
		glVertex3d(Prb.GetX(), Prb.GetY(), Prb.GetZ());
		glVertex3d(Plt.GetX(), Plt.GetY(), Plt.GetZ());
		glEnd();
	}
		break;
	case 'Y':
	{
		BPoint Pmb = P0 - Ey;
		BPoint Plt = P0 - Ex + Ey;
		BPoint Prt = P0 + Ex + Ey;
		glBegin(GL_LINES);
		glVertex3d(Pmb.GetX(), Pmb.GetY(), Pmb.GetZ());
		glVertex3d(P0.GetX(), P0.GetY(), P0.GetZ());
		glVertex3d(P0.GetX(), P0.GetY(), P0.GetZ());
		glVertex3d(Prt.GetX(), Prt.GetY(), Prt.GetZ());
		glVertex3d(P0.GetX(), P0.GetY(), P0.GetZ());
		glVertex3d(Plt.GetX(), Plt.GetY(), Plt.GetZ());
		glEnd();
	}
		break;
	case 'Z':
	{
		BPoint Plt = P0 - Ex + Ey;
		BPoint Prt = P0 + Ex + Ey;
		BPoint Plb = P0 - Ex - Ey;
		BPoint Prb = P0 + Ex - Ey;
		glBegin(GL_LINES);
		glVertex3d(Plt.GetX(), Plt.GetY(), Plt.GetZ());
		glVertex3d(Prt.GetX(), Prt.GetY(), Prt.GetZ());
		glVertex3d(Prt.GetX(), Prt.GetY(), Prt.GetZ());
		glVertex3d(Plb.GetX(), Plb.GetY(), Plb.GetZ());
		glVertex3d(Plb.GetX(), Plb.GetY(), Plb.GetZ());
		glVertex3d(Prb.GetX(), Prb.GetY(), Prb.GetZ());
		glEnd();
	}
		break;
	}

}

void BOpenGLCtrl::ElemMain::DoAction()
{
	Ctrl.pParent->DoTbZoomAll();
}

void BOpenGLCtrl::ElemMain::Draw(bool Hi)
{



	//// XYZ letter
	//glPushAttrib(GL_ENABLE_BIT);
	//glDisable(GL_LIGHTING);
	//glLineWidth(2.);
	//glColor3d(0.929, 0.109, 0.14);
	//DrawLetter('X', BPoint(Ctrl.X0 + ShiftXY*Ctrl.Scale, Ctrl.Y0, Ctrl.Z0, 1.)
	//	, BPoint(5.*Ctrl.Scale, 0., 0., 0.)
	//	, BPoint(0., 8.*Ctrl.Scale, 0., 0.));
	//glColor3d(0.133, 0.694, 0.298);
	//DrawLetter('Y', BPoint(Ctrl.X0, Ctrl.Y0 + ShiftXY*Ctrl.Scale, Ctrl.Z0, 1.)
	//	, BPoint(5.*Ctrl.Scale, 0., 0., 0.)
	//	, BPoint(0., 8.*Ctrl.Scale, 0., 0.));
	//glColor3d(0.247, 0.282, 0.8);
	//DrawLetter('Z', BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 + ShiftXY*Ctrl.Scale, 1.)
	//	, BPoint(5.*Ctrl.Scale, 0., 0., 0.)
	//	, BPoint(0., 0., 8.*Ctrl.Scale, 0.));

	//glPopAttrib();

	Arrow.SetParams(BaseR, 7., 12., 60., 12.);
	if (!Ctrl.Active)
		return;

/*	glBegin(GL_LINES);
	glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0);
	glVertex3d(Ctrl.X0 + 4. * Ctrl.Scale, Ctrl.Y0, Ctrl.Z0);
	glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0);
	glVertex3d(Ctrl.X0, Ctrl.Y0 + 4. * Ctrl.Scale, Ctrl.Z0);
	glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0);
	glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 + 4. * Ctrl.Scale);
	glEnd();
	glPopAttrib();*/

//	if (Ctrl.Active)
//	{
		if (Hi)
			Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
		else
			Arrow.SetColor(ColorBaseR, ColorBaseG, ColorBaseB);
		Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 1., 0., 0));
		Arrow.Draw(true, true, false, false);
/*		glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0);
		glVertex3d(Ctrl.X0 - 4. * Ctrl.Scale, Ctrl.Y0, Ctrl.Z0);
		glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0);
		glVertex3d(Ctrl.X0, Ctrl.Y0 - 4. * Ctrl.Scale, Ctrl.Z0);
		glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0);
		glVertex3d(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 - 4. * Ctrl.Scale);*/

//		if (Hi)
//			Arrow.SetColor(1., 0., 0.);
//		else
//		Arrow.SetColor(0., 1., 0.);

//	}
}

bool BOpenGLCtrl::ElemMain::DoWheelAction(short zDelta) const
{
	short Shift = zDelta / WHEEL_DELTA;
	if (Shift > 0)
	{
		for (int i = 0; i < Shift; ++i)
			Ctrl.pParent->DoTbViewNext();
	}
	else
	{
		for (int i = 0; i < -Shift; ++i)
			Ctrl.pParent->DoTbViewPrev();
	}


	return true;
}

//Axis +Z View on Y-up X-right
BOpenGLCtrl::ElemArrowZ::ElemArrowZ(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowZ::DoAction()
{
	Ctrl.pParent->DoTbReset();
//	Ctrl.pParent->RotateAroundRotCenter('Z', 90);
}

void BOpenGLCtrl::ElemArrowZ::Draw(bool Hi)
{
	if (!Ctrl.Active)
	{
		// +X default 
		Arrow.SetColor(ColorAxisXR, ColorAxisXG, ColorAxisXB);
		Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(1., 0., 0., 0));
		Arrow.Draw(true, false, true, true);
		// +Y default 
		Arrow.SetColor(ColorAxisYR, ColorAxisYG, ColorAxisYB);
		Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 1., 0., 0));
		Arrow.Draw(true, false, true, true);
		// +Z default 
		Arrow.SetColor(ColorAxisZR, ColorAxisZG, ColorAxisZB);
		Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 0., 1., 0));
		Arrow.Draw(true, false, true, true);
		// base default 
		Arrow.SetColor(ColorBaseR, ColorBaseG, ColorBaseB);
		Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 0., 1., 0));
		Arrow.Draw(true, true, false, false);

		return;
	}
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisZR, ColorAxisZG, ColorAxisZB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 0., 1., 0));
	Arrow.Draw(true, false, false, true);
}

bool BOpenGLCtrl::ElemArrowZ::DoWheelAction(short zDelta) const
{
	GLdouble Shift = ShiftAxis * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->MoveAlongAxis('Z', Shift);

	return true;
}


//Axis -Z View on Y-up X-left
BOpenGLCtrl::ElemArrow_Z::ElemArrow_Z(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrow_Z::DoAction()
{
	Ctrl.pParent->DoTbViewD();
}

void BOpenGLCtrl::ElemArrow_Z::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_ZR, ColorAxis_ZG, ColorAxis_ZB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 0., -1., 0));
	Arrow.Draw(true, false, false, true);
}

bool BOpenGLCtrl::ElemArrow_Z::DoWheelAction(short zDelta) const
{
	GLdouble Shift = ShiftAxis * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->MoveAlongAxis('Z', Shift);

	return true;
}


//Axis +X View on Z-up Y-right
BOpenGLCtrl::ElemArrowX::ElemArrowX(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowX::DoAction()
{
	Ctrl.pParent->DoTbViewR();
}


void BOpenGLCtrl::ElemArrowX::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisXR, ColorAxisXG, ColorAxisXB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(1., 0., 0., 0));
	Arrow.Draw(true, false, false, true);
}

bool BOpenGLCtrl::ElemArrowX::DoWheelAction(short zDelta) const
{
	GLdouble Shift = ShiftAxis * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->MoveAlongAxis('X', Shift);

	return true;
}


//Axis -X View on Z-up Y-left
BOpenGLCtrl::ElemArrow_X::ElemArrow_X(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrow_X::DoAction()
{
	Ctrl.pParent->DoTbViewL();
}


void BOpenGLCtrl::ElemArrow_X::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_XR, ColorAxis_XG, ColorAxis_XB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(-1., 0., 0., 0));
	Arrow.Draw(true, false, false, true);
}

bool BOpenGLCtrl::ElemArrow_X::DoWheelAction(short zDelta) const
{
	GLdouble Shift = ShiftAxis * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->MoveAlongAxis('X', Shift);

	return true;
}


//Axis +Y View on Zup Xright
BOpenGLCtrl::ElemArrowY::ElemArrowY(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowY::DoAction()
{
	if (NCMComData::GetType() == AT_MILL)
		Ctrl.pParent->DoTbViewB();
	else
		Ctrl.pParent->DoTbViewZx();
}


void BOpenGLCtrl::ElemArrowY::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisYR, ColorAxisYG, ColorAxisYB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 1., 0., 0));
	Arrow.Draw(true, false, false, true);
}

bool BOpenGLCtrl::ElemArrowY::DoWheelAction(short zDelta) const
{
	GLdouble Shift = ShiftAxis * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->MoveAlongAxis('Y', Shift);

	return true;
}

//Axis -Y View on Z-up X-left
BOpenGLCtrl::ElemArrow_Y::ElemArrow_Y(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrow_Y::DoAction()
{
	if (NCMComData::GetType() == AT_MILL)
		Ctrl.pParent->DoTbViewF();
	else
		Ctrl.pParent->DoTbVewXz();

}


void BOpenGLCtrl::ElemArrow_Y::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_YR, ColorAxis_YG, ColorAxis_YB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., -1., 0., 0));
	Arrow.Draw(true, false, false, true);
}

bool BOpenGLCtrl::ElemArrow_Y::DoWheelAction(short zDelta) const
{
	GLdouble Shift = ShiftAxis * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->MoveAlongAxis('Y', Shift);

	return true;
}


//Rotate +Z 90
BOpenGLCtrl::ElemArrowRotZ::ElemArrowRotZ(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowRotZ::DoAction()
{

	Ctrl.pParent->RotateAroundRotCenter('Z', -90);
}

void BOpenGLCtrl::ElemArrowRotZ::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisZR, ColorAxisZG, ColorAxisZB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0), BPoint(0., 0., 1., 0));
	Arrow.Draw(true, false, true, false);
}

bool BOpenGLCtrl::ElemArrowRotZ::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Z', Angle);
	return true;
}

//Rotate -Z 90
BOpenGLCtrl::ElemArrowRot_Z::ElemArrowRot_Z(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowRot_Z::DoAction()
{

	Ctrl.pParent->RotateAroundRotCenter('Z', 90);
}

void BOpenGLCtrl::ElemArrowRot_Z::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_ZR, ColorAxis_ZG, ColorAxis_ZB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0), BPoint(0., 0., -1., 0));
	Arrow.Draw(true, false, true, false);
}

bool BOpenGLCtrl::ElemArrowRot_Z::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Z', Angle);
	return true;
}

//Rotate +X 90
BOpenGLCtrl::ElemArrowRotX::ElemArrowRotX(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowRotX::DoAction()
{

	Ctrl.pParent->RotateAroundRotCenter('X', -90);
}

void BOpenGLCtrl::ElemArrowRotX::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisXR, ColorAxisXG, ColorAxisXB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(1., 0., 0., 0));
	Arrow.Draw(true, false, true, false);
}

bool BOpenGLCtrl::ElemArrowRotX::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('X', Angle);
	return true;
}

//Rotate -X 90
BOpenGLCtrl::ElemArrowRot_X::ElemArrowRot_X(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowRot_X::DoAction()
{

	Ctrl.pParent->RotateAroundRotCenter('X', 90);
}

void BOpenGLCtrl::ElemArrowRot_X::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_XR, ColorAxis_XG, ColorAxis_XB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(-1., 0., 0., 0));
	Arrow.Draw(true, false, true, false);
}

bool BOpenGLCtrl::ElemArrowRot_X::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('X', Angle);
	return true;
}



//Rotate +Y 90
BOpenGLCtrl::ElemArrowRotY::ElemArrowRotY(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowRotY::DoAction()
{

	Ctrl.pParent->RotateAroundRotCenter('Y', -90);
}

void BOpenGLCtrl::ElemArrowRotY::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisYR, ColorAxisYG, ColorAxisYB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., 1., 0., 0));
	Arrow.Draw(true, false, true, false);
}

bool BOpenGLCtrl::ElemArrowRotY::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Y', Angle);
	return true;
}

//Rotate -Y 90
BOpenGLCtrl::ElemArrowRot_Y::ElemArrowRot_Y(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowRot_Y::DoAction()
{

	Ctrl.pParent->RotateAroundRotCenter('Y', 90);
}

void BOpenGLCtrl::ElemArrowRot_Y::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_YR, ColorAxis_YG, ColorAxis_YB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(0., -1., 0., 0));
	Arrow.Draw(true, false, true, false);
}

bool BOpenGLCtrl::ElemArrowRot_Y::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Y', Angle);
	return true;
}

//ISO +X
BOpenGLCtrl::ElemArrowIsoX::ElemArrowIsoX(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowIsoX::DoAction()
{
	Ctrl.pParent->DoTbViewRU();
}

void BOpenGLCtrl::ElemArrowIsoX::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisXR, ColorAxisXG, ColorAxisXB);
	Arrow.Set(BPoint(Ctrl.X0 + ShiftIso*Ctrl.Scale, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(1., 0., 0., 0));
	Arrow.Draw(true, true, false, false);
}

bool BOpenGLCtrl::ElemArrowIsoX::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('X', Angle);
	return true;
}

//ISO -X
BOpenGLCtrl::ElemArrowIso_X::ElemArrowIso_X(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowIso_X::DoAction()
{
	Ctrl.pParent->DoTbViewLD();
}

void BOpenGLCtrl::ElemArrowIso_X::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_XR, ColorAxis_XG, ColorAxis_XB);
	Arrow.Set(BPoint(Ctrl.X0 - ShiftIso*Ctrl.Scale, Ctrl.Y0, Ctrl.Z0, 1.), BPoint(-1., 0., 0., 0));
	Arrow.Draw(true, true, false, false);
}

bool BOpenGLCtrl::ElemArrowIso_X::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('X', Angle);
	return true;
}

//ISO +Y
BOpenGLCtrl::ElemArrowIsoY::ElemArrowIsoY(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowIsoY::DoAction()
{
	Ctrl.pParent->DoTbViewRD();
}

void BOpenGLCtrl::ElemArrowIsoY::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisYR, ColorAxisYG, ColorAxisYB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0 + ShiftIso*Ctrl.Scale, Ctrl.Z0, 1.), BPoint(0., 1., 0., 0));
	Arrow.Draw(true, true, false, false);
}

bool BOpenGLCtrl::ElemArrowIsoY::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Y', Angle);
	return true;
}

//ISO -Y
BOpenGLCtrl::ElemArrowIso_Y::ElemArrowIso_Y(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowIso_Y::DoAction()
{
	Ctrl.pParent->DoTbViewLU();
}

void BOpenGLCtrl::ElemArrowIso_Y::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_YR, ColorAxis_YG, ColorAxis_YB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0 - ShiftIso*Ctrl.Scale, Ctrl.Z0, 1.), BPoint(0., -1., 0., 0));
	Arrow.Draw(true, true, false, false);
}

bool BOpenGLCtrl::ElemArrowIso_Y::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Y', Angle);
	return true;
}

//ISO +Z
BOpenGLCtrl::ElemArrowIsoZ::ElemArrowIsoZ(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowIsoZ::DoAction()
{
	Ctrl.pParent->DoTbViewT1();
}

void BOpenGLCtrl::ElemArrowIsoZ::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxisZR, ColorAxisZG, ColorAxisZB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 + ShiftIso*Ctrl.Scale, 1.), BPoint(0., 0., 1., 0));
	Arrow.Draw(true, true, false, false);
}

bool BOpenGLCtrl::ElemArrowIsoZ::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Z', Angle);
	return true;
}

//ISO -Z
BOpenGLCtrl::ElemArrowIso_Z::ElemArrowIso_Z(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
	Arrow.SetParams(AxisBaseR, AxisCylR, AxisConeR, AxisCylLen, AxisConeLen);
}

void BOpenGLCtrl::ElemArrowIso_Z::DoAction()
{
	Ctrl.pParent->DoTbViewT5();
}

void BOpenGLCtrl::ElemArrowIso_Z::Draw(bool Hi)
{
	if (!Ctrl.Active)
		return;
	if (Hi)
		Arrow.SetColor(ColorHoverR, ColorHoverG, ColorHoverB);
	else
		Arrow.SetColor(ColorAxis_ZR, ColorAxis_ZG, ColorAxis_ZB);
	Arrow.Set(BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 - ShiftIso*Ctrl.Scale, 1.), BPoint(0., 0., 1., 0));
	Arrow.Draw(true, true, false, false);
}

bool BOpenGLCtrl::ElemArrowIso_Z::DoWheelAction(short zDelta) const
{
	GLdouble Angle = 10. * double(zDelta) / WHEEL_DELTA;
	Ctrl.pParent->RotateAroundRotCenter('Z', Angle);
	return true;
}




// XYZ letter
BOpenGLCtrl::ElemArrowLetXYZ::ElemArrowLetXYZ(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID)
{
}

void BOpenGLCtrl::ElemArrowLetXYZ::DoAction()
{
}

void BOpenGLCtrl::ElemArrowLetXYZ::Draw(bool Hi)
{
	// XYZ letter
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glLineWidth(2.);
	glColor3d(0.929, 0.109, 0.14);
	if (NCMComData::GetType() == AT_MILL)
		DrawLetter('X', BPoint(Ctrl.X0 + ShiftXY*Ctrl.Scale, Ctrl.Y0, Ctrl.Z0, 1.)
		, BPoint(5.*Ctrl.Scale, 0., 0., 0.)
		, BPoint(0., 8.*Ctrl.Scale, 0., 0.));
	else
		DrawLetter('X', BPoint(Ctrl.X0 + ShiftXY*Ctrl.Scale, Ctrl.Y0, Ctrl.Z0, 1.)
		, BPoint( 0., 0., 5.*Ctrl.Scale, 0.)
		, BPoint( 8.*Ctrl.Scale, 0., 0., 0.));
	glColor3d(0.133, 0.694, 0.298);
	DrawLetter('Y', BPoint(Ctrl.X0, Ctrl.Y0 + ShiftXY*Ctrl.Scale, Ctrl.Z0, 1.)
		, BPoint(5.*Ctrl.Scale, 0., 0., 0.)
		, BPoint(0., 8.*Ctrl.Scale, 0., 0.));
	glColor3d(0.247, 0.282, 0.8);
	if (NCMComData::GetType() == AT_MILL)
		DrawLetter('Z', BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 + ShiftXY*Ctrl.Scale, 1.)
		, BPoint(5.*Ctrl.Scale, 0., 0., 0.)
		, BPoint(0., 0., 8.*Ctrl.Scale, 0.));
	else
		DrawLetter('Z', BPoint(Ctrl.X0, Ctrl.Y0, Ctrl.Z0 + ShiftXY*Ctrl.Scale, 1.)
		, BPoint( 0., 0., 5.*Ctrl.Scale, 0.)
		, BPoint( 8.*Ctrl.Scale, 0., 0., 0.));

	glPopAttrib();
}

