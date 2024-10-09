#pragma once
#include "NToolTurnHolder.h"

class AFX_EXT_CLASS NToolTurnHolderBar :
	public NToolTurnHolder
{
public:
	NToolTurnHolderBar(void);
	NToolTurnHolderBar(const CString Name, double iDx, double iDy, double iDz, double iY
		, double MountAngle, double ShiftX, double ShiftY, double ShiftZ);
	~NToolTurnHolderBar(void);
	void FillPage(CDialog *pPage) override;
	void FillFromPage(CDialog *pPage) override;
	void Serialize(CArchive &ar) override;
	void Draw(RenderMode GlobRMode, int step) override;
	float* GetCompTriangles() const;
	NToolTurnHolderBar* CreateCopyTool() override;
	int GenInit(void) override;
	DECLARE_SERIAL(NToolTurnHolderBar)
protected:
	double Dx;
	double Dy;
	double Dz;
	double Y;
};
