#pragma once
#include "BPoint.h"
#include "NContour.h"
#include "ntoolturn.h"

class AFX_EXT_CLASS NToolTurnBlade :
	public NToolTurn
{
public:
	NToolTurnBlade(void);
	NToolTurnBlade(const CString &name);
	virtual ~NToolTurnBlade(void);
	void Draw(RenderMode GlobRMode, int step) override;
	void MakeBladeMesh(MeshArr& RMeshArr, bool IsToolCutting, bool IsShank) const;
	virtual const TBladeCont *GetBladeCont() const;
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurnBlade * CreateCopyTool();
	bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height) override { return false;}
	DECLARE_SERIAL(NToolTurnBlade)
protected:
	void CreateCopyTo(NToolTurnBlade *pTool);
	void FillGabar(void);
	void MakeBContSkeleton();
protected:
	TBladeCont BCont;
	double Thickness;
	double Slope;
	double Angle1;
	double Angle2;
	double Length1;
	double Length2;
};
