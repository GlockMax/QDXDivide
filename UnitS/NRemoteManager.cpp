#include "stdafx.h"
#include "SMachState.h"
#include "SCoordsDef.h"
#include "NCUnitState.h"
#include "RemoteCommands.h"
#include "NRemoteManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NRemoteManager::NRemoteManager(void)
	: PickP(0., 0., 0., -1.), PickN(0., 0., 1., 0.)
{
	pMachCoordsDef = NULL;
	pMachState = NULL;
	pBufState = NULL;
	WaitPoint = false;
	RCWnd = NULL;
	EditorVis = false;
	MainPC = 0;
}

NRemoteManager::~NRemoteManager(void)
{
	delete pMachState;
	delete pMachCoordsDef;
}

void NRemoteManager::Store(const NCUnitState &StartState, const CString &Prog, int MPC)
{
	pBufState = new NCUnitState(StartState);
	BProgName = Prog;
	MainPC = MPC;
}
void NRemoteManager::Restore(NCUnitState &StartState, CString &Prog, int &MPC)
{
	StartState = *pBufState;
	delete pBufState;
	Prog = BProgName;
	RCWnd = NULL;
	MPC = MainPC;
}

void NRemoteManager::SetPoint(const BPoint & InP) const
{

//	P = InP;
}
void NRemoteManager::SendPick()
{
	if(!RCWnd)
		return;
	COPYDATASTRUCT Data2Send;
	Data2Send.dwData = RC_Pick;
	Data2Send.cbData = 0;
	Data2Send.lpData = NULL;

	::SendMessage(RCWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&Data2Send);
}

void NRemoteManager::SetWaitPoint(bool Wait)
{
	WaitPoint = Wait;
	PickP.Set(0., 0., 0., -1.);
	PickArrow.Clear();
}

void NRemoteManager::SetPickedRay(const BPoint &P, const BPoint &N)
{
	PickP = P;
	PickN = N;
	PickArrow.Set(P, N, 100);
}
