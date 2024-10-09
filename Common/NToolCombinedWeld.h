#pragma once
#include "BPoint.h"
#include "ntoolcombined.h"

class AFX_EXT_CLASS NToolCombinedWeld :
	public NToolCombined
{
public:
	NToolCombinedWeld(void);
	NToolCombinedWeld(const CString &Name);
	~NToolCombinedWeld(void);
	virtual NCMApplicationType GetClass() const { return AT_WELD;}
	virtual NToolCombinedWeld* CreateCopyTool() const;
	virtual void FillDialog(CDialog *pDial) const;
	void SetConeLength(double Length);
	const BPoint GetDirection(void) const;
	virtual BPoint GetCompVect(void) const;
	DECLARE_SERIAL(NToolCombinedWeld)
protected:
};
