#pragma once
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeCont :
	public NToolTurnBlade
{
public:
	NToolTurnBladeCont(void);
	NToolTurnBladeCont(const CString& TName, const TBladeCont &ToolCont, bool CutCont, double Thick, double Angle);
	~NToolTurnBladeCont(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeCont* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeCont)
protected:
public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
};
