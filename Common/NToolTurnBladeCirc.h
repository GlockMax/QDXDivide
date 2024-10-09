#pragma once
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeCirc :
	public NToolTurnBlade
{
public:
	NToolTurnBladeCirc(void);
	NToolTurnBladeCirc(const CString& TName, double R, double TSL
		, double ShiftX, double ShiftZ, bool RB1);
	~NToolTurnBladeCirc(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeCirc* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeCirc)
protected:
	double m_R;
public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
};
