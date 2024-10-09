#include "StdAfx.h"
#include "t3dxinit.h"

T3DInterface *T3DxInit::pInt = NULL;

T3DxInit::T3DxInit(void)
{
}

T3DxInit::~T3DxInit(void)
{
	delete pInt;
}

int T3DxInit::Init(void)
{
	pInt = new T3DInterface;
	return 0;
}
int T3DxInit::UnInit(void)
{
	delete pInt;
	pInt = NULL;
	return 0;
}
