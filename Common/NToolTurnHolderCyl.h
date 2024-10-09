#pragma once
#include "NToolTurnHolder.h"

class AFX_EXT_CLASS NToolTurnHolderCyl :
	public NToolTurnHolder
{
public:
	NToolTurnHolderCyl(void);
	NToolTurnHolderCyl(const CString Name, double iDx, double iY
		, double iD1, double iD2, int iFH
		, double MountAngle, double ShiftX, double ShiftY, double ShiftZ);
	~NToolTurnHolderCyl(void);
	void FillPage(CDialog *pPage) override;
	void FillFromPage(CDialog *pPage) override;
	void Serialize(CArchive &ar) override;
	void Draw(RenderMode GlobRMode, int step) override;
	float* GetCompTriangles() const;
	NToolTurnHolderCyl* CreateCopyTool() override;
	int GenInit(void) override;
	DECLARE_SERIAL(NToolTurnHolderCyl)
protected:
	double Dx;
	double Y;
	double D1;
	double D2;
	int FH;
};
