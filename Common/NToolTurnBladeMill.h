#pragma once
#include "ntoolmill.h"
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeMill :
	public NToolTurnBlade
{
public:
	NToolTurnBladeMill(void);
	~NToolTurnBladeMill(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeMill* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeMill)
protected:
	NToolMill *pMillTool;
public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
	void SetMillTool(NToolMill *pT);
	NToolMill * GetMillTool(void);
	void Draw(RenderMode GlobRMode, int step) override;
};
