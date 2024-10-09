#pragma once
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeCut :
	public NToolTurnBlade
{
public:
	NToolTurnBladeCut(void);
	NToolTurnBladeCut(const CString& TName, double NoseR, double Width, double Length
		, double MountAngle, double TSL, double NoseA, double NoseR1, double AngleC
		, double Height, double ShiftX, double ShiftZ, bool RB1, int Edge);
	~NToolTurnBladeCut(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeCut* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeCut)
protected:
	double m_AngleB;
//	double m_AngleP;
	double m_R;
	double m_Whidth;
	double m_Length;
	double m_R1;
	int m_Edge;
public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
};
