// NToolBRound.h: interface for the NToolBRound class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NToolShaped.h"

class AFX_EXT_CLASS NToolBRound : public NToolShaped
{
public:
	NToolBRound(const CString &Name, double &diam, double &b, double &l, double &r,
		double &hm);
	void FillSheet(CPropertySheet *pDial);
	void Serialize(CArchive &ar);
	virtual int GenInit();
	void GetParam(double &diam, double &b, double &l, double &r,
		double &hm);
	bool CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f);
	bool CreateToolContour(GLfloat *pts,int *type, BOOL f);
	bool NeedAppr(void) const override;
		
	bool IsNameAuto;
	NToolBRound();
	virtual ~NToolBRound();
	DECLARE_SERIAL(NToolBRound);

protected:
	double D;	// Diameter
	double B;	// Height
	double L;	// Arc center height 
	double R;	// Arc radius 
protected:
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height) override;
};
