#pragma once
#include "BStl.h"
#include "ntoolturnholder.h"
class AFX_EXT_CLASS NToolTurnHolderSTL :
	public NToolTurnHolder
{
public:
	NToolTurnHolderSTL(void);
	virtual ~NToolTurnHolderSTL(void);
	NToolTurnHolderSTL(const CString &Name, const CString &FileName
		, double MountAngle, double ShiftX, double ShiftY, double ShiftZ);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	void Draw(RenderMode GlobRMode, int step) override;
	virtual NToolTurnHolderSTL* CreateCopyTool();
	int GenInit(void) override;
	const CString & GetFileName();
	const GLfloat* GetTriangles(void) const;
	bool FindSegments(SnapRopes& oSegments);
	DECLARE_SERIAL(NToolTurnHolderSTL)
protected:
	BStl Stl;
	double Y;
};

