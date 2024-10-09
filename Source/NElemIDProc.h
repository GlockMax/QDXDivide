#pragma once
#include "melemidproc.h"
#include "NCMDoc.h"


class NElemIDProc :
	public MElemIDProc
{
protected:
	NElemIDProc(void);
	CNCMDoc *pDoc;

public:
	NElemIDProc(CNCMDoc *pDocum);
	virtual ~NElemIDProc(void);
	PackedColor GetColor(ElemID id) override;
	bool GetStockEdgesS(void) override;
	bool GetStockEdgesDX(void) override;
};
