#include "stdafx.h"
#include "NActiveObject.h"
#include "BObjectManager.h"
#include "NOMScaner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NOMScaner::NOMScaner(BObjectManager *pOM, NActiveObject *pObj)
{
	pPassiveObject = NULL;
	PassiveGroup = OG_NO_GROUP;
	PassiveInd = -1;
	Status = ST_EMPTY;
	pObjectManager = pOM;
	if(!pObjectManager)
		return;
	pObjectManager->MakeScaner(this, pObj);
}

NOMScaner::~NOMScaner(void)
{
}
void NOMScaner::Next(void)
{
	pObjectManager->MakeNextScaner(this);
}
bool NOMScaner::IsEnded(void)
{
	return (Status == ST_EMPTY);
}
