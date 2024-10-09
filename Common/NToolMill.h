// NToolMill.h: interface for the NToolMill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NCEnums.h"
#include "BBox.h"
#include "NColor.h"
#include "ToolMash.h"
#include "RRevSurf.h"
#include "NTool.h"

class AFX_EXT_CLASS NToolMill : public NTool  
{
public:
	virtual bool CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f) {return false;}
	virtual bool CreateToolContour(GLfloat *pts,int *type, BOOL f) {return false;}
	virtual NToolMill* CreateCopyTool() override;
	virtual void SetZdisp(double z);
	NToolMill();
	virtual void FillDialog(CDialog *pDial);
	virtual void FillSheet(CPropertySheet *pSheet);
	void Serialize(CArchive &ar);
	NToolMill(const CString &n);
	void SetVertTool(bool val) { VertTool = val; }
	void Draw(RenderMode GlobRMode, int step) override;
	size_t CreateSkeleton(int PointsNumber, GLfloat* ctlarray);
	virtual ~NToolMill();
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height){ return false;}
	virtual NCMApplicationType GetClass() const { return AT_MILL;}
	virtual bool IsBallNose() const override { return false;}
	virtual bool IsCylWBallNose() const override { return false;}
	virtual bool IsFlatNose() const override { return false;}
	virtual void GenRender() {};
	virtual bool IsVertTool() const override { return VertTool; }

	DECLARE_SERIAL(NToolMill)

protected:
	virtual int GenInit();
	RRevSurf RenderP;
	RRevSurf RenderA;
	bool VertTool;

};
