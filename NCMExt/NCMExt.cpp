// NCMExt.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NCMVersion.h"
#include "process.h"
#include "ResLocal.h"
#include "JobKeyWords.h"
#include "RemoteCommands.h"
#include "NCMExtJob.h"
#include "NCMExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//using namespace std;

NCMExtJob Job;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	_tprintf(_T("Fatal Error: MFC initialization failed\n"));
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
	}

	return nRetCode;
}

double CalcRotAngle(enum PosAngle Pos)
{
	switch(Pos)
	{
	case PA_0:
		return 0.;
		break;
	case PA_90:
		return 90.;
		break;
	case PA_180:
		return 180.;
		break;
	case PA_270:
		return 270.;
		break;
	}
	return 0.;
}

bool WriteJobFile(const CString &FileName)
{
	// Open output file
	CStdioFile f;
	if (!f.Open(FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		return false;

	Job.WriteFile(f);

	f.Close();
	return true;
}

NCMECode SendCommand(RemoteCommands Command)
{
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;

	COPYDATASTRUCT Data2Send;
	Data2Send.cbData = 0;
	Data2Send.dwData = Command;

	LRESULT Res = ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send);

	if (!Res)
		return NCME_BAD_RETURN_CODE;
	return NCME_OK;

}

NCMECode SendTextC(RemoteCommands Command, CString &textC)
{
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;
	COPYDATASTRUCT Data2Send;
	Data2Send.cbData = textC.GetLength();
	Data2Send.dwData = Command;
	Data2Send.lpData = textC.GetBuffer();

	LRESULT Res = ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send);

	if (!Res)
		return NCME_BAD_RETURN_CODE;
	return NCME_OK;
}

NCMECode SendText(RemoteCommands Command, const char * text)
{
	CString textC(static_cast<const char *>(text), (int)strlen(text));
	return SendTextC(Command, textC);
}

NCMECode PressButton(UINT CommandID)
{
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;

	COPYDATASTRUCT Data2Send;
	Data2Send.cbData = sizeof(CommandID);
	Data2Send.dwData = RC_PressButton;
	Data2Send.lpData = &CommandID;

	LRESULT Res = ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send);

	if (!Res)
		return NCME_BAD_RETURN_CODE;
	return NCME_OK;
}
NCMECode NCMSend()
{
	CString regPath = NCMExtJob::GetRegPath();
	CString FileName = NCMExtJob::GetDataPath(regPath) + _T("NCMEfile.ncmj");
	if (!WriteJobFile(FileName))
		return NCME_CANT_WRITE;
	// Run NCManager
	CString CmdPrt;
	if (!Job.PrtName.IsEmpty())
		CmdPrt = Job.PrtName;
	CString MainPath = NCMExtJob::GetMainPath(regPath);
	CString CmdName = MainPath + _T("NCM30.exe");
	intptr_t Res = _spawnl(_P_NOWAIT, CmdName, _T("\"") + CmdName + _T("\""), _T("-prt: "), _T("\"") + CmdPrt + _T("\""), _T("\"") + FileName + _T("\""), NULL);

	if (Res == -1)
		return NCME_NOT_INSTALLED;

	return NCME_OK;
}

NCMECode NCMSendHost()
{
	CString DataPath = NCMExtJob::GetDataPath(NCMExtJob::GetRegPath());
	CString FileName = DataPath + _T("NCMEfile.ncmj");
	if (!WriteJobFile(FileName))
		return NCME_CANT_WRITE;

	COPYDATASTRUCT Data2Send;
	Data2Send.dwData = RC_JobFile;
	Data2Send.cbData = FileName.GetLength();
	Data2Send.lpData = ((Data2Send.cbData == 0) ? nullptr : FileName.GetBuffer());
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;

	LRESULT Res = ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send);

	if (!Res)
		return NCME_BAD_RETURN_CODE;

	return NCME_OK;
}

NCMECode NCMClear()
{
	Job.Reset();
	return NCME_OK;
}

NCMECode NCMAddProgram(const char *FileName)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;

	Job.ProgramNames.Add(CString(FileName));
	return NCME_OK;
}

NCMECode NCMAddSubProgram(const char *FileName)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;

	Job.SubProgramNames.Add(CString(FileName));
	return NCME_OK;
}

NCMECode NCMRemoveProgram(const char *FileName)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;
	int Ind = Job.FindProg(CString(FileName));
	if(Ind < 0)
		return NCME_PROG_NOT_FOUND;
	Job.ProgramNames.RemoveAt(Ind);
	return NCME_OK;
}

NCMEXT_API NCMECode NCMRemoveAllPrograms(void)
{
	Job.RemoveAllPrograms();
	return NCME_OK;
}

NCMECode NCMReplaceProgram(const char *OldFileName, const char *NewFileName)
{
	if(OldFileName == NULL)
		return NCME_BAD_STRING;
	if(NewFileName == NULL)
		return NCME_BAD_STRING;
	int Ind = Job.FindProg(CString(OldFileName));
	if(Ind < 0)
		return NCME_PROG_NOT_FOUND;
	Job.ProgramNames[Ind] = CString(NewFileName);
	return NCME_OK;
}

NCMECode NCMSetToolColors( COLORREF CutColor, COLORREF NonCutColor, int Position, int Turret/* = 0*/)
{
	CString Pos;
	Pos.Format(_T(" %d %d "), Position, Turret);
	CString Col;
	Col.Format(_T(" %d %d"), CutColor, NonCutColor);
	CString Descr = ComTool + _T(" ") + ComToolColors + Pos + Col;
	Job.AddToolDescr(Position, Turret, Descr);
	return NCME_OK;
}

NCMECode NCMAddMillCutCont(const char *Contour, int Position, int Turret/* = 0*/, int Axis/* = 0*/)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;
	CString Pos;
	Pos.Format(_T(" %d %d %d "), Position, Turret, Axis);
	CString Descr = ComTool + _T(" ") + Mill + _T(" ") + CuttCont + _T(" StC") + Pos + CString(Contour);
	Job.AddToolDescr(Position, Turret, Descr);
	return NCME_OK;
}

NCMECode NCMAddTurnDrillCutCont(const char *Contour, int Position, int Turret/* = 0*/, int Axis/* = 0*/)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;
	CString Pos;
	Pos.Format(_T(" %d %d %d "), Position, Turret, Axis);
	CString Descr = ComTool + _T(" ") + Turn + _T(" ") + CuttCont + _T(" StC") + Pos + CString(Contour);
	Job.AddToolDescr(Position, Turret, Descr);
	return NCME_OK;
}

NCMECode NCMAddTurnDrillHolderCont(const char *Contour, int Position, int Turret/* = 0*/, int Axis/* = 0*/)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;
	CString Pos;
	Pos.Format(_T(" %d %d %d "), Position, Turret, Axis);
	CString Descr = ComTool + _T(" ") + Turn + _T(" ") + HoldCont + _T(" StC") + Pos + CString(Contour);
	Job.AddToolDescr(Position, Turret, Descr);
	return NCME_OK;
}


NCMECode NCMAddMillHolderCont(const char *Contour, int Position, int Turret/* = 0*/, int Axis/* = 0*/)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;
	CString Pos;
	Pos.Format(_T(" %d %d %d "), Position, Turret, Axis);
	CString Descr = ComTool + _T(" ") + Mill + _T(" ") + HoldCont + _T(" StC") + Pos + CString(Contour);
	Job.AddToolDescr(Position, Turret, Descr);
	return NCME_OK;
}

NCMECode NCMAddTurnInsertCont(const char *Contour, int Position, int Turret/* = 0*/, int Axis/* = 0*/, double Thickness/* = 1.*/, double Angle/* = 0.*/)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;
	CString Pos;
	Pos.Format(_T(" %d %d %d "), Position, Turret, Axis);
	CString Dim;
	Dim.Format(_T(" %lf %lf"), Thickness, Angle);
	CString Descr = ComTool + _T(" ") + Insert + _T(" ") + CuttCont + _T(" StC") + Pos + CString(Contour) + Dim;
	Job.AddToolDescr(Position, Turret, Descr);
	return NCME_OK;
}

NCMECode NCMRemoveTool(int Position, int Turret/* = 0*/)
{
	if(Job.RemoveToolDescr(Position, Turret))
		return NCME_OK;
	return 	NCME_TOOL_NOT_FOUND;
}

NCMECode NCMStartToolNumber(int Position, int Turret/* = 0*/)
{
	Job.CurToolN.Set(Position, Turret);
	return NCME_OK;
}

NCMECode NCMStartToolPosition(double X/* = 0.*/, double Y/* = 0.*/, double Z/* = 0.*/
							  , double A/* = 0.*/, double B/* = 0.*/, double C/* = 0.*/)
{
	Job.MashX = X;
	Job.MashY = Y;
	Job.MashZ = Z;
	Job.MashA = A;
	Job.MashB = B;
	Job.MashC = C;
	return NCME_OK;
}

NCMECode NCMAddHCompensation(int Number, double Value)
{
	Job.HComp[Number] = Value;
	return NCME_OK;
}

NCMECode NCMAddDCompensation(int Number, double Value)
{
	Job.DComp[Number] = Value;
	return NCME_OK;
}

NCMECode NCMAddFixOffset(int Number, double X/* = 0.*/, double Y/* = 0.*/, double Z/* = 0.*/
						 , double A/* = 0.*/, double B/* = 0.*/, double C/* = 0.*/)
{
	Job.UCSs[Number].x0 = X;
	Job.UCSs[Number].y0 = Y;
	Job.UCSs[Number].z0 = Z;
	Job.UCSs[Number].a0 = A;
	Job.UCSs[Number].b0 = B;
	Job.UCSs[Number].c0 = C;
	return NCME_OK;
}

NCMECode NCMSetMillProject(void)
{
	Job.AppType = AT_MILL;
	return NCME_OK;
}

NCMECode NCMSetMillTurnProject(void)
{
	Job.AppType = AT_MILL_TURN;
	return NCME_OK;
}

NCMECode NCMSetTurnProject(void)
{
	Job.AppType = AT_TURN;
	return NCME_OK;
}

NCMECode NCMSetWeldProject(void)
{
	Job.AppType = AT_WELD;
	return NCME_OK;
}


NCMECode NCMSetNCControl(const char *Name)
{
	Job.UnitName = CString(Name);
	return NCME_OK;
}

NCMECode NCMSetNCMachine(const char *Name)
{
	Job.MachineName = CString(Name);
	return NCME_OK;
}

NCMECode NCMSetMillStockBox(double X0, double Y0, double Z0, double X1, double Y1, double Z1)
{
	CString Str;
	Str.Format(_T(" %lf %lf %lf %lf %lf %lf"), X0, Y0, Z0, X1, Y1, Z1);
	Job.StockDescriptions.Add(StockBox + Str);
	return NCME_OK;
}
NCMECode NCMSetMillStockCyl(double X, double Y, double Z, double H, double D, int Axis)
{
	double X1 = X, Y1 = Y, Z1 = Z;
	switch (Axis)
	{
	case 0:
		X1 += H;
		break;
	case 1:
		Y1 += H;
		break;
	case 2:
	default:
		Z1 += H;
		break;
	}
	CString Str;
	Str.Format(_T(" %lf %lf %lf %lf %lf %lf %lf"), X, Y, Z, X1, Y1, Z1, D);
	Job.StockDescriptions.Add(StockCylinder + Str);
	return NCME_OK;
}
NCMECode NCMSetMillStockStl(const char *FileName)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;

	Job.StockDescriptions.Add(StockStl + _T(" ") + CString(FileName));
	return NCME_OK;
}

NCMECode NCMSetMillStockExtrude(const char *FileName, double Zmin, double Zmax, PosAngle RotX, PosAngle RotY, PosAngle RotZ)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;

	CString Str;
	Str.Format(_T(" %lf %lf %lf %lf %lf"), Zmin, Zmax, CalcRotAngle(RotX), CalcRotAngle(RotY), CalcRotAngle(RotZ));
	Job.StockDescriptions.Add(StockExtrude + _T(" FIL ") + CString(FileName) + Str);
	return NCME_OK;
}

NCMECode NCMSetMillStockExtrudeStC(const char *Contour, double Zmin, double Zmax, PosAngle RotX, PosAngle RotY, PosAngle RotZ)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;

	CString Str;
	Str.Format(_T(" %lf %lf %lf %lf %lf"), Zmin, Zmax, CalcRotAngle(RotX), CalcRotAngle(RotY), CalcRotAngle(RotZ));
	Job.StockDescriptions.Add(StockExtrude + _T(" StC ") + CString(Contour) + Str);
	return NCME_OK;
}

NCMECode NCMRemoveStock()
{
	Job.StockDescriptions.RemoveAll();
	return NCME_OK;
}

NCMECode NCMSetTurnStockContour(const char *FileName)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;

	Job.StockDescriptions.Add(StockContour + _T(" FIL ") + CString(FileName));
	return NCME_OK;
}

NCMECode NCMSetTurnStockContourStC(const char *Contour)
{
	if(Contour == NULL)
		return NCME_BAD_STRING;

	Job.StockDescriptions.Add(StockContour + _T(" StC ") + CString(Contour));
	return NCME_OK;
}

NCMECode NCMSetPrtFileName(const char *FileName)
{
	if(FileName == NULL)
		return NCME_BAD_STRING;

	Job.PrtName = CString(FileName);
	return NCME_OK;
}

//Function to start a process and return the process handle
HANDLE StartProcess(const CString &Path, CString &Args, unsigned int MaxWaitTime)
{
	HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEPOSITION | STARTF_USESIZE;
	startupInfo.cb = sizeof(startupInfo);
	if (::CreateProcess(Path, (LPSTR)Args.GetString(),
		NULL,  // process security
		NULL,  // thread security
		FALSE, // no inheritance
		0,     // no startup flags
		NULL,  // no special environment
		NULL,  // default startup directory
		&startupInfo,
		&processInfo))
	{ /* success */
		hProcess = processInfo.hProcess;
		if (WaitForInputIdle(processInfo.hProcess, MaxWaitTime) != 0)
			hProcess = 0;
	} /* success */
	return hProcess;
}

NCMEXT_API NCMECode NCMStopHost(void)
{
	COPYDATASTRUCT Data2Send;
	Data2Send.dwData = RC_StopHost;
	Data2Send.cbData = 0;
	Data2Send.lpData = NULL;

	if (TRUE != ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send))
	{
		TerminateProcess(Job.HostHandle, 0);	//kill the process using handle
		WaitForSingleObject(Job.HostHandle, 5000);
	}
	CloseHandle(Job.HostHandle);
	return NCME_OK;
}

NCMEXT_API NCMECode NCMStartHost(HWND ParentWnd, unsigned int MaxWaitTime)
{
	if (!::IsWindow(ParentWnd))
		return NCME_BAD_WINDOW;
	CRect ParentRect;
	::GetClientRect(ParentWnd, ParentRect);

	// Find Path
	CString RegPath = NCMExtJob::GetRegPath();
	CString MainPath = NCMExtJob::GetMainPath(RegPath);
	CString Path = MainPath + _T("NCM30.exe");

	// Start process
	CString Args = _T(" -host");
	Job.HostHandle = StartProcess(Path, Args, MaxWaitTime);
	if (Job.HostHandle == NULL)
		return NCME_NOT_INSTALLED;

	CString WindowClassName = NCMExtJob::GetWindowClass(RegPath);
	if (WindowClassName.IsEmpty())
		return NCME_NOT_INSTALLED;

	Job.HostAppHWnd = ::FindWindow(WindowClassName, NULL);
	if (Job.HostAppHWnd != NULL)//check for window handle
	{
		HWND Res = ::SetParent(Job.HostAppHWnd, ParentWnd);
		auto Style = ::GetWindowLongPtr(Job.HostAppHWnd, GWL_STYLE);
		Style |= WS_CHILD;
		Style &= ~WS_POPUP;
		::SetWindowLongPtr(Job.HostAppHWnd, GWL_STYLE, Style);
		::SetWindowLongPtr(Job.HostAppHWnd, GWL_STYLE, WS_VISIBLE);//erase title
		NCMSizeHost(ParentRect);
		SetForegroundWindow(Job.HostAppHWnd);
	}
	else//no window for our process 
		return NCME_NOT_INSTALLED;

	return NCME_OK;
}

NCMECode NCMSizeHost(const CRect& Rect)
{
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;
	return ::SetWindowPos(Job.HostAppHWnd, NULL, Rect.left, Rect.top, Rect.right, Rect.bottom, SWP_NOZORDER) ?
		NCME_OK : NCME_NOT_INSTALLED;
}

NCMECode NCMSubscribeToProgramChanges(HWND MainWnd, ULONG_PTR Pass)
{
	if (!IsWindow(MainWnd))
		return NCME_BAD_WINDOW;
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;

	SubscribeStruct SS;
	SS.Pass = Pass;
	SS.Wnd = MainWnd;
	COPYDATASTRUCT Data2Send;
	Data2Send.cbData = sizeof(SS);
	Data2Send.lpData = &SS;
	Data2Send.dwData = RC_SubscribeProgChange;

	LRESULT Res = ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send);

	if (!Res)
		return NCME_BAD_RETURN_CODE;
	return NCME_OK;
}

NCMECode NCMUnsubscribeFromProgramChanges()
{
	return SendCommand(RC_UnsubscribeProgChange);
}

NCMECode NCMSaveAllPrograms()
{
	return SendCommand(RC_SavePrograms);
}

NCMECode NCMBlankAllPrograms()
{
	return PressButton(ID_PA_PROG_ALLUNV);
}

NCMECode NCMUnblankAllPrograms()
{
	return PressButton(ID_PA_PROG_ALLVIS);
}

NCMECode NCMShowProgramText(const char *program_name)
{
	return SendText(RC_ShowProgramText, program_name);
}

NCMECode NCMReset()
{
	return PressButton(ID_RESET);
}

NCMECode NCMStartExecution()
{
	return PressButton(ID_RUN_TO_BREAKPOINT);
}

NCMECode NCMStopExecution()
{
	return PressButton(ID_STEP_STOP);
}

NCMECode NCMSetModeCut()
{
	return PressButton(ID_CUT_MODE);
}

NCMECode NCMSetModeDontCut()
{
	return PressButton(ID_DONTCUT_MODE);
}

NCMECode NCMSetModeFastCut()
{
	return PressButton(ID_FAST_CUT_MODE5X);
}

NCMECode NCMSetTMSmoothly()
{
	return PressButton(ID_SMOTHNESS_MODE);
}

NCMECode NCMSetTMFeedrate()
{
	return PressButton(ID_SMOTHNESS_MODE_F);
}

NCMECode NCMSetTMResult()
{
	return PressButton(ID_RESULT_MODE);
}

NCMECode NCMViewZoomAll()
{
	 return PressButton(ID_TB_ZOOM_ALL);
}

NCMECode NCMViewScale()
{
	 return PressButton(ID_TB_ZOOM_SC);
}

NCMECode NCMViewZoomW()
{
	return PressButton(ID_TB_ZOOM_W);
}

NCMECode NCMViewPan()
{
	return PressButton(ID_TB_ZOOM_P);
}

NCMECode NCMViewSetView(double eyex, double eyey, double eyez, double upx, double upy, double upz)
{
	CString Str;
	Str.Format(_T("%f %f %f %f %f %f"), eyex, eyey, eyez, upx, upy, upz);
	return SendTextC(RC_SetView, Str);
}

NCMECode NCMRemoveAllSubprograms(void)
{
	Job.RemoveAllSubprograms();
	return NCME_OK;
}

NCMECode NCMRemoveSubprogram(const char *FileName)
{
	if (FileName == NULL)
		return NCME_BAD_STRING;
	int Ind = Job.FindSubprog(CString(FileName));
	if (Ind < 0)
		return NCME_PROG_NOT_FOUND;
	Job.SubProgramNames.RemoveAt(Ind);
	return NCME_OK;
}

NCMECode NCMSaveProgram(const char *program_name, const char *NewFileName /* = nullptr*/)
{
	CString str(program_name);
	if (NewFileName != nullptr)
		str = str + _T(";") + CString(NewFileName);
	return SendTextC(RC_SavePrograms, str);
}

NCMECode NCMViewRotateA(int Axis)
{
	switch (Axis)
	{
	case 0:
		return PressButton(ID_TB_ZOOM_X);
	case 1:
		return PressButton(ID_TB_ZOOM_Y);
	case 2:
		return PressButton(ID_TB_ZOOM_Z);
	}
	return NCME_BAD_ARGUMENT;
}

NCMECode NCMSetStockQuality(int Val)
{
	CString str;
	str.Format(_T("  0%d"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetStockColour(COLORREF Val)
{
	CString str;
	str.Format(_T("  1%u"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetStockVisibility(BOOL Val)
{
	CString str;
	str.Format(_T("  2%d"), Val ? 1 : 0);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetStockEdges(BOOL Val)
{
	CString str;
	str.Format(_T("  3%d"), Val ? 1 : 0);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetStockEdgesThickness(double Val)
{
	CString str;
	str.Format(_T("  4%f"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetProgramsShow(enum TrajPart Val)
{
	CString str;
	str.Format(_T("  5%u"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetToolsVisibility(BOOL Val)
{
	CString str;
	str.Format(_T("  6%d"), Val ? 1 : 0);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetProgramThickness(const char *FileName, double Val)
{
	if (FileName == NULL)
		return NCME_BAD_STRING;

	CString Name(FileName);
	CString str;
	str.Format(_T("  7%f %s"), Val, Name);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetToolToolpath(int Position, int Turret, BOOL Val)
{
	NToolID ToolID;
	ToolID.Set(Position, Turret);
	CString str;
	str.Format(_T("  8%d %s"), Val ? 1 : 0, ToolID);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetProgramVisibility(const char *FileName, BOOL Val)
{
	if (FileName == NULL)
		return NCME_BAD_STRING;

	CString Name(FileName);
	CString str;
	str.Format(_T("  9%d %s"), Val ? 1 : 0, Name);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSubscribeToColl(HWND MainWnd, ULONG_PTR Pass)
{
	if (!IsWindow(MainWnd))
		return NCME_BAD_WINDOW;
	if (Job.HostAppHWnd == NULL)
		return NCME_NOT_INSTALLED;

	SubscribeStruct SS;
	SS.Pass = Pass;
	SS.Wnd = MainWnd;
	COPYDATASTRUCT Data2Send;
	Data2Send.cbData = sizeof(SS);
	Data2Send.lpData = &SS;
	Data2Send.dwData = RC_SubscribeToColl;

	LRESULT Res = ::SendMessage(Job.HostAppHWnd, WM_COPYDATA, NULL, (LPARAM)&Data2Send);

	if (!Res)
		return NCME_BAD_RETURN_CODE;
	return NCME_OK;
}

NCMECode NCMSetToolsCollCheck(int Val)
{
	CString str;
	str.Format(_T(" 10%d"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetToolsCollIgnoreUp(BOOL Val)
{
	CString str;
	str.Format(_T(" 11%d"), Val ? 1 : 0);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetToolsCollMarginD(double Val)
{
	CString str;
	str.Format(_T(" 12%f"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMSetToolsCollMarginL(double Val)
{
	CString str;
	str.Format(_T(" 13%f"), Val);
	return SendText(RC_SetProperty, str);
}

NCMECode NCMGetMaxStockQuality(int &Val)
{
	Val = 1000000;
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex); 
	
	if (statex.ullTotalPhys < 1000000000)
		return NCME_OK;
	if (statex.ullTotalPhys < 2000000000)
	{
		Val = 1500000;
		return NCME_OK;
	}
	if (statex.ullTotalPhys < 3000000000)
	{
		Val = 2000000;
		return NCME_OK;
	}
#ifdef WIN32
	Val = 3000000;
	return NCME_OK;
#else
	if (statex.ullTotalPhys < 8000000000)
	{
		Val = 15000000;
		return NCME_OK;
	}
	Val = 30000000;
	return NCME_OK;
#endif
}

NCMECode NCM_BPProgramStop()
{
	return PressButton(ID_BP1);
}
NCMECode NCM_BPOptionalStop()
{
	return PressButton(ID_BP2);
}
NCMECode NCM_BPToolChange()
{
	return PressButton(ID_BP3);
}
NCMECode NCM_BPBookmark()
{
	return PressButton(ID_BP4);
}

NCMECode NCM_ShowMainMenu(BOOL Show)
{
	return SendTextC(RC_ShowMainMenu, Show ? CString(_T("1")) : CString(_T("0")));
}

NCMECode NCM_SetFixedCenter(BOOL Set)
{
	return SendTextC(RC_ShowMainMenu, Set ? CString(_T("1")) : CString(_T("0")));
}
