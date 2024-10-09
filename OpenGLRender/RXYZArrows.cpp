#include "stdafx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "BaseRender.h"
#include "NColor.h"
#include "RXYZArrows.h"

RXYZArrows::RXYZArrows()
{
	ArrowX.Set(BPoint(0., 0., 0., 1.), BPoint(1., 0., 0., 0.), 100.);
	ArrowY.Set(BPoint(0., 0., 0., 1.), BPoint(0., 1., 0., 0.), 100.);
	ArrowZ.Set(BPoint(0., 0., 0., 1.), BPoint(0., 0., 1., 0.), 100.);
	ArrowX.SetConeColor(1., 0., 0.);
	ArrowY.SetConeColor(0., 1., 0.);
	ArrowZ.SetConeColor(0., 0., 1.);
	Active = true;
}

RXYZArrows::~RXYZArrows()
{
}

void RXYZArrows::Draw(const NColor &Col)
{
	if (!Active)
		return;
	ArrowX.SetCylColor(Col.R(), Col.G(), Col.B());
	ArrowY.SetCylColor(Col.R(), Col.G(), Col.B());
	ArrowZ.SetCylColor(Col.R(), Col.G(), Col.B());
	ArrowZ.SetBaseColor(Col.R(), Col.G(), Col.B());
	glPushMatrix();
	glMultMatrixd(PosMatr.GetArray());
	ArrowX.Draw(true, false);
	ArrowY.Draw(true, false);
	ArrowZ.Draw(true);
	glPopMatrix();
}
