#pragma once
#include "ntoolturnblade.h"

class AFX_EXT_CLASS NToolTurnBladeISO :
	public NToolTurnBlade
{
public:
	NToolTurnBladeISO(void);
	NToolTurnBladeISO(const CString& Name, CString Form, double L1, double L2, double NoseR,
		double AngleC, double Height, double MountAngle, double ShiftX, double ShiftZ, bool RB1, int Edge);
	~NToolTurnBladeISO(void);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBladeISO* CreateCopyTool();
	DECLARE_SERIAL(NToolTurnBladeISO)
protected:
	double m_L1;
	double m_L2;
	int m_Edge;
	BOOL m_RB1;
	CString m_Form;
	CString m_BackAngle;
	CString m_SThickness;
	CString m_SRad;
public:
	int GenInit(void) override;
	void Regenerate(void) override;
	bool GetStdParams(double& Diameter, double& CornerRadius, double& ELength, double& FLength, double& BottomAngle, double& SideAngle, double& Height) override;
	int GenCirc(double L1);
	int GenRegPoly(int Num, double Length, double R);
	static double GetVal(const CString & str);
	int GenRhomb(double Angle, double Length, double R);
	int GenRect(double L1, double L2, double R);
	int GenParall(double Angle, double L1, double L2, double R);
	int GenTrigon(double L1, double R);
};
