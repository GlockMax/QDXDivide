#pragma once

enum RemoteCommands
{
	RC_Empty = 0,
	RC_Connect = 1,
	RC_Stop = 2,
	RC_Reset = 3,
	RC_Disconnect,
	RC_MakePlaceString,
	RC_WaitPoint,
	RC_Pick,
	RC_JobFile,
	RC_StopHost,
	RC_SubscribeProgChange,
	RC_SubscribeToColl,
	RC_UnsubscribeProgChange,
	RC_SavePrograms,
	RC_ShowProgramText,
	RC_PressButton,
	RC_SetProperty,
	RC_SetView,
	RC_ShowMainMenu,
	RC_SetFixedCenter
};

struct SubscribeStruct
{
	HWND Wnd;
	ULONG_PTR Pass;
};

enum ObjType
{
	OT_UNIT,
	OT_MACHINE,
	OT_STOCKS,
	OT_STOCK,
	OT_PROGRAMS,
	OT_PROGRAM,
	OT_TOOLS,
	OT_TOOL,
	OT_PARTS,
	OT_PART,
	OT_EXOBJS,
	OT_EX_OBJ
};