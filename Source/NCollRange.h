#pragma once

#include "CadrID.h"

#include "WM_APP_messages.h"

class NCollRange
{
public:
	NCollRange(CNCMDoc* pD);
	void Set(cadrID StartHist, cadrID EndHist, bool iIgnoreWrongAxis) { StartHIDAbs = StartHist; EndHIDAbs = EndHist; IgnoreWrongAxis = iIgnoreWrongAxis; }
	~NCollRange(void);
	CWinThread* StartChecking(class NMachUnitPair* pMUPair);
	void Abort(void) { EndProcessing = true;}
	bool IsAborted() const { return EndProcessing; }
	void ResetAbort() { EndProcessing = false; }
	void SetMUPair(class NMachUnitPair* pMU) { pMUPair = pMU; }
	static cadrID CheckingProc( NCollRange &CollRange, bool StopIfStock);
	void SetStartHIDAbs(const cadrID& Val) { StartHIDAbs = Val; }
	void SetEndHIDAbs(const cadrID& Val) { EndHIDAbs = Val; }
	cadrID GetStartHIDAbs() const { return StartHIDAbs; }
	cadrID GetEndHIDAbs() const { return EndHIDAbs; }
protected:
	cadrID StartHIDAbs;
	cadrID EndHIDAbs;
	class NMachUnitPair* pMUPair;
	bool EndProcessing;
	bool IgnoreWrongAxis;
protected:
	static UINT CheckingThreadProc( LPVOID pParam );
	CNCMDoc* pGDoc;
};
