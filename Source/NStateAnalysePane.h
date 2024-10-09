#pragma once
#include "NWeldParams.h"
#include "NStateParamsPane.h"

class NStateAnalysePane :
	public NStateParamsPane
{
public:
	NStateAnalysePane(void);
	virtual ~NStateAnalysePane(void);
	void OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint);
	void SetAnalyseState(class NAnalyseState *pState);
	void EnsureActual(void) { WeldParams.EnsureActual();}
	void ClearActual(void) { WeldParams.ClearActual();}

protected:
	void ResetElements(void);
protected:
	class NAnalyseState * pAnalyseState;
	NWeldParams WeldParams;
};
