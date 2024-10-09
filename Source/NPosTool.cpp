#include "stdafx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "BaseRender.h"
#include "NToolStd.h"
#include "NPosTool.h"


NPosTool::NPosTool(void)
{
	PosToolX.Set(BPoint(0., 0., 0., 1.), BPoint(1., 0., 0., 0.), 100.);
	PosToolY.Set(BPoint(0., 0., 0., 1.), BPoint(0., 1., 0., 0.), 100.);
	PosToolZ.Set(BPoint(0., 0., 0., 1.), BPoint(0., 0., 1., 0.), 100.);
	PosToolX.SetConeColor(1., 0., 0.);
	PosToolY.SetConeColor(0., 1., 0.);
	PosToolZ.SetConeColor(0., 0., 1.);
	Active = false;
}


NPosTool::~NPosTool(void)
{
}

void NPosTool::Draw(RenderMode GlobRMode)
{
	if(!Active)
		return;
	glPushMatrix();
	glMultMatrixd(ReToolMatr.GetArray());
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(3.f);
	BaseRender::DrawBox(ReStockGabar, 1);
	glPopAttrib();
	glPopMatrix();
	glPushMatrix();
	glMultMatrixd(PosMatr.GetArray());
	PosToolX.Draw(true, false);
	PosToolY.Draw(true, false);
	PosToolZ.Draw(true);
	glPopMatrix();
}

void NPosTool::Activate(const BMatr &M)
{
	Active = true;
	PosMatr = M;
}

void NPosTool::SetStockGabar(const BBox &Gabar, const BMatr &ToolMatr)
{
	ReStockGabar = Gabar;
	ReToolMatr = ToolMatr;
}
