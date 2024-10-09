#pragma once
#include "WM_APP_messages.h"
#include "NCMComData.h"
#include "ppltasks.h"
#include "NMachUnitPair.h"

enum CutResult
{
	CUT_EMPTY = 0,
	CUT_INTERRUPTED = 1,
	CUT_MEMORY_EX = 2,
	CUT_OK = 4,
	CUT_PROJ_END = 8
};


class BProgram;
class FRange;

class FCutting
{
protected:
	CTypedPtrArray <CPtrArray, FRange *> RangesArray;
	bool Stopped;// true - main cycle in CuttingThreadProc is over. Nothing will be added to the queue; false - CuttingThreadProc is running
	const BProgram *pLastProg;
	int NLastProg;
	int LastHCadr;
	NMachUnitPair MUPair;
	class NChannel* pChannel;
public:
	bool Started;
public:
	FCutting(class NChannel* pChan);
	~FCutting(void);
	const BProgram *GetLastProg(void) const { return pLastProg;}
	int GetNLastProg(void) const { return NLastProg;}
	int GetLastHCadr(void) const { return LastHCadr;}
	cadrID GetLastHCadrID(void) const { return cadrID(NLastProg, LastHCadr);}
	INT_PTR AddRange(FRange *pRange);
	bool IsStopped(void) const;
	bool IsStoppedFull(void) const;
	UINT CuttingThreadProc( LPVOID pParam, CWnd* pMainWnd);
	int StoreProgFragment(const class NSpeedPar* pSpeedPar, int ProgNum, const BProgram * pProg, int StartHCadr, int EndHCadr);
	void CutSynchr(void);
	void Init();
	bool HasRanges(void) const;
	NCMApplicationType GetNextState(void) const;
	const FRange * GetFirstRange(void) const;
	const FRange * GetLastRange(void) const;
	bool HasCutting(void) const;
	void Clear(void);
//	bool ProcTransformRange(BMatr &Matr);
};
