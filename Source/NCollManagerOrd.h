#pragma once
#include "NCollManager.h"
#include "NCollisionLog.h"
#include "NCollTimeLine.h"

class NCollManagerOrd :
	public NCollManager
{
public:
	NCollManagerOrd(CNCMDoc* pD);
	virtual ~NCollManagerOrd();
	bool AskSettings();
	void ClearLogs(void);
	void AbortBack(void);
	void BeginRestart();
	void StopRestart();
	void RestartTimeoutElapsed(class MTMachineTool& MT);
	bool IsRestarting() { return RestartTimerID > 0; }
	bool IsBackCompleted() { return BackCheckCompleted; }
	bool IsBackType(void) const { return CheckType == 0; }
	bool IsForeType(void) const { return CheckType == 1; }
	bool IsBackMode(void) const { return BackMode; }
	void SetBackMode(bool Val) { BackMode = Val; }
	void FlipBackMode() { BackMode = !BackMode; CheckType = (CheckType == 2) ? 2 : (BackMode ? 0 : 1); }
	int StartBack(cadrID& StartHIDAbs, cadrID& EndHIDAbs, class NMachUnitPair* pMUPair);
	NCollisionLog* GetActiveLog();
	void CollcheckStopped(bool Aborted);
	bool IsBackCheckInProgress(void);
	bool IsCollisionFound(void);
	bool Disable(int ExcludeInd = -1);
	void ClearCollided();
	void Prepare(bool Fore) override;
	bool ProcWrongAxis(int WrongAxis, class MTMachineTool* pMach, const ProgTPoint& PTPoint);
	void SetMainWnd(CWnd* pWnd);
	CWnd* GetMyMainWnd();
	PObjectStatus OnStatusChanged(int ObjectInd, PObjectStatus NewStatus) override;
	bool ChangeCheckType(class MTMachineTool& MT);
	bool SetCheckType(int NewCheckType, const class MTMachineTool& MT);
	bool ResetCheckType();
	bool IsActive() const { return CheckType != 2; }
	bool CheckCurPosition(class MTMachineTool* pMach
		, const class NToolCombined* pActiveTool, const ProgTPoint& CadrAbs, bool IgnoreLimits, bool DontFixStock, bool IsFast5X);
	auto* GetForeLog() { return &ForeLog; }
	const auto* GetForeLog() const { return &ForeLog; }
	bool IsToolStockActive() const { return ToolStockActive; }
	void SetToolStockActive(bool Val);
	void SetToolStockMode(CollisWithStock Val);
	int HaveStockCollisions() const;// returns 1 - have non stock collision, 2 - have stock collision, 3 - both
	void AddCutCollision(const class BRangePosColl& PosColl);
	void PrepTimeLine(MainTime LastTime);
	const NCollisionID* GetCollID(MainTime intStart);
	const NCollisionID* GetCollID(MainTime intStart, MainTime intEnd);
	bool GetLogIsStale() const { return LogIsStale; }
	void SetLogIsStale() { LogIsStale = true; }
	auto& GetCollTimeLine() { return CollTimeLine; }
	void StopCreating();
	void SetCuttingDisabled(bool Val) { CuttingDisabled = Val; }
	void AddCollisionMark(NCollisionID& CollID);
	static int CollideDef(void* client_data, void* obj1, void* obj2, const void* coll_data);
protected:
	int MakeCollisionScene(void) override;
	bool Add2Log(const NCollisionID& CollID);
	static void CALLBACK lpfnTimer(HWND, UINT, UINT_PTR, DWORD);
protected:
	NCollisionLog BackLog;
	NCollisionLog ForeLog;
	int CheckType;
	bool BackMode;
	CWinThread* pCollCheckThread;
	int RestartTimerID;
	bool BackCheckCompleted;
	class NCollRange* pCollRange;
	static const int RestartTimeout = 5000;
	static CCriticalSection CritSection;
	CWnd* pMainWnd;
	enum CollisWithStock ToolStockMode;
	bool ToolStockActive;
	NCollTimeLine CollTimeLine;
	bool LogIsStale;
	bool CuttingDisabled;
};

