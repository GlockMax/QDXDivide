#include "stdafx.h"
#include "QDivideBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

QDivideBase::QDivideBase()
{
	IsActive = false;
	CorrMatr.SetE();
	mode = REMOVE_NONE;
}

QDivideBase::~QDivideBase()
{
}

void QDivideBase::ChangeCuttingMode(bool flag)
{
	mode = (flag) ? REMOVE_OTHERS : REMOVE_CURRENT;
}

