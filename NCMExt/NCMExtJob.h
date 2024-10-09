#pragma once
#include "NToolID.h"
#include "NCMComData.h"

typedef CMap<int,int,double,double> CMapIntToDouble;

class NCBaseT  
{
public:
	double x0;
	double y0;
	double z0;
	double a0;
	double b0;
	double c0;
	NCBaseT(){};
	~NCBaseT(){};
};

class NCMExtJob
{
public:
	NCMExtJob(void);
	~NCMExtJob(void);
	int WriteFile(CStdioFile &f);
	int FindProg(const CString &ProgName) const;
	int FindSubprog(const CString &ProgName) const;
	void Reset();
	void RemoveAllTools(void);
	void RemoveAllPrograms(void);
	void RemoveAllSubprograms(void);
	void AddToolDescr(int Position, int Turret, const CString &Description);
	bool RemoveToolDescr(int Position, int Turret);
	static CString GetMainPath(const CString &RegPath);
	static CString GetDataPath(const CString &RegPath);
	static CString GetPath(const CString &Name, const CString &RegPath);
	static CString GetWindowClass(const CString &RegPath);
	static CString GetRegPath(void);

	NCMApplicationType AppType;
	CString UnitName;
	CString MachineName;
	CArray <CString, const CString &> ProgramNames;
	CArray <CString, const CString &> SubProgramNames;

	typedef CArray <CString, const CString &> ToolDescriptions;
	CTypedPtrMap<CMapStringToOb, CString, ToolDescriptions *> ToolsMap;

	NToolID CurToolN;
	double MashX, MashY, MashZ, MashA, MashB, MashC;
	NCBaseT UCSs[6];
	CMapIntToDouble DComp;
	CMapIntToDouble HComp;
	int AxesNum;

	CArray <CString, const CString &> StockDescriptions;
	CString PrtName;

	HANDLE HostHandle;//process handle
	HWND HostAppHWnd;//window handle
	HWND ThisAppHWnd;//this app main window handle
	UINT CopyDatawParam;
	
};
