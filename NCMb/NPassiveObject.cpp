#include "stdafx.h"
#include "NTiParams.h"
#include "BObjectManager.h"
#include "NPassiveObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NPassiveObject::NPassiveObject()
{
}

NPassiveObject::~NPassiveObject(void)
{
	Deleted();
}

void NPassiveObject::SetGroup(PObjectGroup Gr)
{
	Group = Gr;
	SetPassiveGroup(Gr);
}

void NPassiveObject::SetEdgesAttrs(const BEdgesAttrs & Attr)
{
	EdgesAttrs = Attr;
}

void NPassiveObject::SetPassiveGroup(PObjectGroup Gr)
{
	P_OBJECT_MANAGER->SetGroup((NPassiveObject *)this, Gr);
}

void NPassiveObject::Changed(void)
{
	P_OBJECT_MANAGER->ChangedPassiveObject(this);
}
void NPassiveObject::Deleted(void)
{
	P_OBJECT_MANAGER->DeletePassiveObject(this);
}
