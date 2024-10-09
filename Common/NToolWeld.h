#pragma once
#include "BPoint.h"
#include "ntool.h"

class NToolWeld :
	public NTool
{
	friend class NToolsCrWeldSheet;
	friend class NToolsCrWeldPage;
public:
	NToolWeld(void);
	virtual ~NToolWeld(void);
	NToolWeld* CreateCopyTool(void);
	void Draw(RenderMode GlobRMode, int step) override;
	void SetLength(double Length){ ConeLength = Length;}
	const BPoint &GetDirection(void) { return ConeDir;}
	virtual void FillFromPage(CDialog *pPage);
	virtual void FillPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	DECLARE_SERIAL(NToolWeld)
	virtual NCMApplicationType GetClass() const { return AT_WELD;}
protected:
	virtual int GenInit();

protected:
	double ConeRad;
	double ConeLength;
	BPoint ConeDir;
	GLUquadricObj *pQObj;
};
