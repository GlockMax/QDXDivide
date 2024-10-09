// NToolPunch.cpp: implementation of the NToolPunch class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NToolPunch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NToolPunch::NToolPunch():NToolMill(CString("Punch"))
{
	SetType(30);// Punch tool
	Gabar.Expand(30,30,20);
	Gabar.Expand(-30,-30,0);
}

NToolPunch::~NToolPunch()
{

}

void NToolPunch::Draw(RenderMode GlobRMode, int step)
{
//	glNewList(DisplList,GL_COMPILE);
//	glEndList();
}
