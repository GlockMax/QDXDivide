#pragma once
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeOut :
	public NToolTurnBlade
{
public:
	NToolTurnBladeOut(void);
	NToolTurnBladeOut(const CString& TName, double AngleA, double AngleB, double L1, double L2, double NoseR,
		double AngleC, double Height, double MountAngle, double ShiftX, double ShiftZ, bool RB1);
	~NToolTurnBladeOut(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeOut* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeOut)
protected:
	double m_AngleA;
	double m_AngleB;
//	double m_AngleP;
	double m_L1;
	double m_L2;
	double m_R;
	int m_PointPos;
public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
};
