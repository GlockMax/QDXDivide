#pragma once
#include "BPoint.h"
#include "ntoolturn.h"

class AFX_EXT_CLASS NToolTurnHolder :
	public NToolTurn
{
public:
	NToolTurnHolder(void);
	virtual ~NToolTurnHolder(void);
	void Draw(RenderMode GlobRMode, int step) override;
	virtual const TBladeCont *GetBladeCont() const {return NULL;}
	virtual NToolTurnHolder * CreateCopyTool();
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
};
