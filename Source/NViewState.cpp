// NViewState.cpp: implementation of the NViewState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NViewState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NViewState::NViewState()
{
	VMode = Shade;
}

NViewState::~NViewState()
{

}

void NViewState::SetBase(double x,double y,double z)
{
	BaseX = x;
	BaseY = y;
	BaseZ = z;
}
