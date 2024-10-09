// NToolPunch.h: interface for the NToolPunch class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NToolMill.h"

class AFX_EXT_CLASS NToolPunch : public NToolMill  
{
public:
	void Draw(RenderMode GlobRMode, int step) override;
	NToolPunch();
	virtual ~NToolPunch();
	virtual NCMApplicationType GetClass() const { return AT_PUNCH;}

};
