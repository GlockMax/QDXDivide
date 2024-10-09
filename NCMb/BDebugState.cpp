#include "stdafx.h"
#include "BDebugState.h"

BDebugState DebugState;

BDebugState::BDebugState(void)
{
	DebugMode = false;
	SetDebugMode(false);
	DrawFaces = false;
	MultiThread = true;
	CellSize = 64.;
	Transp = 1.;
	Step = 8;//!!!
	Maxl = 1.0;//!!!
	Dich = 0.001;//!!!
	UseSlicedTree = true;
	DXNumber = 32;
	MarchCubes = true;
	Async = true;
	SurfelVis = false;
}

BDebugState::~BDebugState(void)
{
}
void BDebugState::SetDebugMode(bool in)
{
	if(DebugMode == in)
		return;
	DebugMode = in;
	if(DebugMode)
	{ 
		if(!LogFile.Open(_T("debug.log"),
			CFile::typeText | CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
		{
			DebugMode = false;
//			AfxMessageBox(IDS_LOGFILE_ERR);
		}
		
	}
	else
	{/*
		LogFile.Close();*/
	}
}

BDebugState & BDebugState::Get(void)
{
	return DebugState;
}
