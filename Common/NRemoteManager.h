#pragma once
#include "RArrow.h"
#include "NMapStringToOb.h"

class AFX_EXT_CLASS NRemoteManager
{
public:
	NRemoteManager(void);
	virtual ~NRemoteManager(void);
	const SMachState & GetMachState(void) { return *pMachState;}
	const class NCUnitState &GetBufState(void) { return *pBufState;}
	bool IsWaitingPoint(void) const { return WaitPoint;}
	void SetWaitPoint(bool Wait);
	void SetPoint(const BPoint & InP) const;
	void SaveCommand(const CString &Comm) { Command = Comm;}
	const CString & GetCommand(void) const { return Command;}
	void SetWnd(HWND Wnd) { RCWnd = Wnd;}
	void SendPick();
	void SetEditorVis(bool Vis) {EditorVis = Vis;}
	bool GetEditorVis(void) const {return EditorVis;}
	void SetPickedRay(const BPoint &P, const BPoint &N);
	bool GetPickedRay(BPoint &P, BPoint &N) { P = PickP; N = PickN; return P.GetH() > 0.;}
	const RArrow &GetPickArrow(void) { return PickArrow;}
	void Store(const NCUnitState &StartState, const CString &Prog, int MPC);
	void Restore(NCUnitState &StartState, CString &Prog, int &MPC);
	BMapStringToNProgram &GetBufProgList(void) { return BufProgList;}

protected:
	BMapStringToNProgram BufProgList;
	class SMachState *pMachState;
	class SCoordsDef *pMachCoordsDef;
	class NCUnitState *pBufState;
	CString BProgName;
	int MainPC;
	bool WaitPoint;
	CString Command;
	HWND RCWnd;
	bool EditorVis;
	BPoint PickP;
	BPoint PickN;
	class RArrow PickArrow;

};
