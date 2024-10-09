// NToolStd.h: interface for the NToolStd class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NToolMill.h"

class AFX_EXT_CLASS NToolStd : public NToolMill
{
public:
	bool CreateToolContour(GLfloat *pts, TypeLineInContour *type, int size, BOOL f);
	bool CreateToolContour(GLfloat *pts, int *type, BOOL f);
	void GetParam(double &dtop, double &dbot, double &h, double &alf, double &beta, double &r,double &hm);
	void FillSheet(CPropertySheet *pDial);
	int AssignType();
	bool IsNameAuto;
	void FillDialog(CDialog *pDial);
	void Serialize(CArchive &ar);
	NToolStd(const CString &Name,double dbot,double hb,double alf,double r,double beta,double e,double f,double hm);
	NToolStd(double dbot,double hb,double alf,double r,double beta,double e,double f,double hm);
	NToolStd();
	virtual int GenInit();
	virtual ~NToolStd();
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height);
	virtual bool IsBallNose() const override;
	virtual bool IsCylWBallNose() const override;
	virtual bool IsFlatNose() const override;
	virtual bool IsFullBall() const override;
	virtual NTool * CreateOffsetTool(bool NonCuttingOnly);
	static double CalcEndRad(double BottomDiam, double Alf);
	void RemoveEndBall(void);
	void RestoreEndBall(void);
	double GetR() const { return R; }
	void SetR(double Val) { R = Val; }
	double GetDTop() const { return DTop; }
	void GenToolName(const CString& Prefix = "");
	DECLARE_SERIAL(NToolStd)
protected:
	void GenRender() override;

protected:
	double Alf;//Side/Beta angle
	double H;//Height
	double DTop;// ----
	double DBot;//Diameter
	double R;//Corner radius
	double BAlf;//Bottom/Alfa angle
	double ELength;//X-distance to corner rad center (unused)
	double FLength;//Y-distance to corner rad center (unused)
	double h; // величина режущей кромки
};
