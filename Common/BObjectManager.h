#pragma once
#include "NObjectManager.h"
class AFX_EXT_CLASS BObjectManager :
	public NObjectManager<256, 256, 4>
{
public:
	BObjectManager();
	~BObjectManager();
	void Serialize(CArchive &ar);
};
