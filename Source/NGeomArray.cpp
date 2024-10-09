// NGeomArray.cpp: implementation of the NGeomArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCMProject.h"
#include "ncunit.h"
#include "GL\gl.h"
#include "NCadrGeom.h"
#include "NGeomArray.h"
#include "NOpenGLWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NGeomArray::NGeomArray() : BGeomArray()
{
}

NGeomArray::~NGeomArray()
{
}

void NGeomArray::Draw(int FirstHCadr, int LastHCadr, int BoldCadr, double t)
{
// Draw wireframe image of array
// from the FirstHCadr to the LastHCadr
// if LastHCadr < 0 draw whole array

	glFinish();

	NColor *c, Color, FastMovCol;
	if(NCM_PROJECT.Colors.Lookup("Öèêë", c))
		Color = *c;
	else
		Color.SetRGB(RGB(255,255,255));
	if(NCM_PROJECT.Colors.Lookup("ÕîëÕîä", c))
		FastMovCol = *c;
	else
		FastMovCol.SetRGB(RGB(255,0,0));
	auto n = GetSize();
	if(LastHCadr < n && LastHCadr >=0)
		n = LastHCadr;
	int b = 0;
	if(FirstHCadr > 0)
		b = FirstHCadr;
	NCadrGeom *pGeom;

	int i, k;
// k - index of current step excluding steps in the cycles
// i - index of current step

	// Skip cadrs before StartHCadr
	for( i = 0, k = 0; k < b && i < GetSize(); i++)
	{
		if(!GetAt(i)->IsCycle())
			k++;
	}

	for(; k < n && i < GetSize(); ++i)
	{
		pGeom = GetAt(i);
		if(pGeom->IsCycle())
		{// Process cycle
			int CycleStart = i;
			for(++i; i < GetSize(); ++i)
			{
				pGeom = GetAt(i);
				if(pGeom->IsCycle())
					continue;
				// First non cycle cadr found
				int CycleEnd = i;
				++k;
				//Draw cycle approach
				bool IsLast = (k == n);
				NDrawCadr(i, 0, pGeom, IsLast ? t : -1., k == BoldCadr, Color, FastMovCol);				
				if(!IsLast || t < 0.)
				{//Draw cycle body
					for(int l = CycleStart; l < CycleEnd; ++l)
						NDrawCadr(i, l, GetAt(l), -1., false, Color, FastMovCol);
				}
				break;
			}
		}
		else
		{
			++k;
			bool IsLast = (k == n);
			NDrawCadr(i, 0, pGeom, IsLast ? t : -1., k == BoldCadr, Color, FastMovCol);				
		}
	}

	glFinish();

}

void NGeomArray::NDrawCadr(int iName, int CycleName, NCadrGeom *pGeom, double t, bool IsBoldCadr, const NColor &Color, const NColor &FastMovCol)
{
		glPushName(iName);
		if(CycleName > 0)
			glPushName(CycleName);
		switch(pGeom->GetAttr())
		{
		case 0:
			break;
		case 1:// Cycle attrib
		case 6:// Cycle attrib
		case 7:// Cycle attrib
			glPushAttrib(GL_CURRENT_BIT);
			glColor4fv(Color.array);
			break;
		case 2:// Spindle off
			glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT );
			glEnable(GL_LINE_STIPPLE);
			glLineStipple( 1/*factor*/, 0xcccc /* pattern */  );
			glColor4fv(FastMovCol.array);
			break;
		case 3:
			glPushAttrib(GL_CURRENT_BIT);
			glColor3f(1.,1.,0.);
			break;
		case 4:
			glPushAttrib(GL_CURRENT_BIT);
			glColor3f(0.,0.,1.);
			break;
		default:
			glPushAttrib(GL_CURRENT_BIT);
			break;
		}
		if(IsBoldCadr)
		{
			glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
			GLdouble curcol[4];
			glGetDoublev(GL_CURRENT_COLOR, curcol);
			curcol[0] = 1. - curcol[0];
			curcol[1] = 1. - curcol[1];
			curcol[2] = 1. - curcol[2];
			glColor3dv(curcol);
			glLineWidth(3.);
		}
		NOpenGLWnd::DrawCadrGeom(*pGeom, NCM_PROJECT.GetDouble("TolLineApp", 0.01), t);
		if(IsBoldCadr)
			glPopAttrib();

		if(pGeom->GetAttr() != 0)
			glPopAttrib();

		glPopName();
		if(CycleName > 0)
			glPopName();
}
void NGeomArray::DrawBoldCadr(int BoldCadr, const NColor &Color)
{
// Draw wireframe image of array
// from the FirstHCadr to the LastHCadr
// if LastHCadr < 0 draw whole array

//	glFinish();
	auto n = GetSize();
	NCadrGeom *pGeom;
	for(int i = 0, k = 0; k <= BoldCadr && i < n; i++)
	{
// k - index of current step excluding steps in the cycles
// i - index of current step


		pGeom = GetAt(i);
		if(!pGeom->IsCycle())// Cycle attrib
			k++;

		if(k == BoldCadr)
		{
			NColor BoldColor = Color;
			glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
			glColor4fv(BoldColor.Inv().array);
			glLineWidth(3.);
			NOpenGLWnd::DrawCadrGeom(*pGeom, NCM_PROJECT.GetDouble("TolLineApp", 0.01));
			glPopAttrib();
//			glLineWidth(1.);
			break;// to prevent cycle draw
		}
	}

	glFinish();

}
