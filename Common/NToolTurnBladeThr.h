#pragma once
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeThr :
	public NToolTurnBlade
{
public:
	NToolTurnBladeThr(void);
	NToolTurnBladeThr(const CString& TNamedouble, double ThreadAngle
	, double CutWhide, double CutLength, double HalfT, double R, double AngleC, double Height, double MountAngle, double ShiftX, double ShiftZ, bool RB1);
	~NToolTurnBladeThr(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeThr* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeThr)
protected:
	int m_ThType;
	double m_R;
	int m_PointPos;
	BOOL m_RB1;
	double m_Thickness;
	double m_ThreadAngle;
	double m_CutWhide;
	double m_CutLength;
	double m_HalfT;
	double m_AngleP;


public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
};
