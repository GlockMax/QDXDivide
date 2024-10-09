#pragma once

class AFX_EXT_CLASS BDebugState 
{
public:
	BDebugState();
	~BDebugState();
	bool GetDebugMode(void)const{ return DebugMode;}
	void SetDebugMode(bool in);
	CStdioFile & GetLogFile(void){ return LogFile;}
protected:
	bool DebugMode;
public:
	bool MultiThread;
	bool DrawFaces;
	double CellSize;
	int DXNumber;
	double Transp;
	unsigned int Step;//!!!
	double Maxl;//!!!
	double Dich;//!!!
	CStdioFile LogFile;
	bool UseSlicedTree;
	bool SurfelVis;
	bool MarchCubes;
	bool Async;
	static BDebugState & Get(void);
};
