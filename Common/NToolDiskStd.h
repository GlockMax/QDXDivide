// NToolDiskStd.h: interface for the NToolDiskStd class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NToolMill.h"

class AFX_EXT_CLASS NToolDiskStd : public NToolMill
{
public:
	NToolDiskStd(const CString &Name, double diam, double b, double alf, double beta,
		double r1, double r2, double r3, double r4,
		double l1, double l2, double hm1, double hm2);
	void FillSheet(CPropertySheet *pDial);
	void GenRender() override;
	void Serialize(CArchive &ar);
	virtual int GenInit();
	void GetParam(double &diam, double &b, double &alf, double &beta,
		double &r1, double &r2, double &r3, double &r4,
		double &l1, double &l2, double &hm1, double &hm2);
	bool CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f);
	bool CreateToolContour(GLfloat *pts,int *type, BOOL f);
	bool IsDiskTool() const override { return true; }

	bool IsNameAuto;
	NToolDiskStd();
	virtual ~NToolDiskStd();
	DECLARE_SERIAL(NToolDiskStd);

protected:
	double D;	// Diameter
	double B;	// Width
	double Alf;	// Angle left Faset 
	double BAlf;// Angle right Faset 
	double L2;	// left Faset 
	double L1;	// right Faset 
	double R4;	// radius
	double R3;	// radius
	double R2;	// radius
	double R1;	// radius
	double h1; // величина режущей кромки - правая сторона
	double h2; // величина режущей кромки - левая сторона
protected:
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height);
};
