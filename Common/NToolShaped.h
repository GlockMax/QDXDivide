// NToolShaped.h: interface for the NToolShaped class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NToolMill.h"
#include "NContour.h"

class AFX_EXT_CLASS NToolShaped : public NToolMill
{
public:
	virtual bool CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f);
	virtual bool CreateToolContour(GLfloat *pts,int *type, BOOL f);
	void Draw(RenderMode GlobRMode, int step) override;
	virtual int GenInit() override;
	void FillSheet(CPropertySheet *pDial) override;
	void Serialize(CArchive &ar);
	const NContour & GetCont(void) const;
		
	NToolShaped();
	NToolShaped(const CString &name);
	NToolShaped(const CString & Name, const NContour& Cont, bool CutCont); 
	double GetH() const { return h; }
	void SetH(double H) { h = H; }
	virtual ~NToolShaped();
	DECLARE_SERIAL(NToolShaped);
protected:
	NContour Cont;
	double h;	// Cut size height
	void GenRender() override;

protected:
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height){ return false;}
};
