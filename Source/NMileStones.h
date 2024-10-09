#pragma once
#include "NBreakpointDlg.h"

class NMileStone;
class NCUnit;

class NMileStones : public CTypedPtrArray <CPtrArray, NMileStone *>
{
	friend class NMileStone;
protected:
	const class NCUnitStatePrev *pPrevState;
	class NCUnit *pUnit;
	bool Active;
public:
	void Clear(void);
	NMileStones(void);
	virtual ~NMileStones(void);
	const NMileStone * IsMileStone(class NCUnit *pU, const NCUnitStatePrev *pState);
	bool Remove(NMileStone * pElem);
	bool Serve(enum StoneNames Name, enum MiFunctions Func);
	void Activate(void);
	void DeActivate(void);
	bool IsActive(void);
	void FillFromDlg(const NBreakpointDlg &Dlg);
	bool HaveToolChangeOnly(void);
};
