#include "stdafx.h"
#include "BObjectManager.h"

BObjectManager::BObjectManager(void)
{
}

BObjectManager::~BObjectManager(void)
{
}
void BObjectManager::Serialize(CArchive &ar)
{
	SerializeElements(ar, this, 1);
}
