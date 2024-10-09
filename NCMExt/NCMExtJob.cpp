#include "StdAfx.h"
#include "atlbase.h"
#include "JobKeyWords.h"
#include "NCMExtJob.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCMExtJob::NCMExtJob(void)
{
	Reset();
	HostAppHWnd = NULL;
	ThisAppHWnd = NULL;
	HostHandle = NULL;
	CopyDatawParam = 0;
}

NCMExtJob::~NCMExtJob(void)
{
}

void NCMExtJob::Reset(void)
{
	AppType = AT_MILL;
	AxesNum = 3;
	UnitName.Empty();
	MachineName.Empty();
	StockDescriptions.RemoveAll();
	RemoveAllPrograms();
	SubProgramNames.RemoveAll();
	RemoveAllTools();
	MashX = 0.;
	MashY = 0.;
	MashZ = 0.;
	MashA = 0.;
	MashB = 0.;
	MashC = 0.;
	for(int i = 0; i < 6; ++i)
	{
		UCSs[i].x0 = 0.;
		UCSs[i].y0 = 0.;
		UCSs[i].z0 = 0.;
		UCSs[i].a0 = 0.;
		UCSs[i].b0 = 0.;
		UCSs[i].c0 = 0.;
	}
	PrtName.Empty();
}

void NCMExtJob::RemoveAllTools(void)
{
	for(POSITION Pos = ToolsMap.GetStartPosition(); Pos != NULL;)
	{
		ToolDescriptions *pTDescr = NULL;
		NToolID ToolID;
		ToolsMap.GetNextAssoc(Pos, ToolID, pTDescr);
		if(pTDescr)
			pTDescr->RemoveAll();
	}
	ToolsMap.RemoveAll();
}

void NCMExtJob::RemoveAllPrograms(void)
{
	ProgramNames.RemoveAll();
}

void NCMExtJob::RemoveAllSubprograms(void)
{
	SubProgramNames.RemoveAll();
}

int NCMExtJob::FindProg(const CString &ProgName) const
{
	for(int i = 0; i < ProgramNames.GetSize(); ++i)
		if(ProgName.CompareNoCase(ProgramNames[i]) == 0)
			return i;
	return -1;
}

int NCMExtJob::FindSubprog(const CString &ProgName) const
{
	for (int i = 0; i < SubProgramNames.GetSize(); ++i)
		if (ProgName.CompareNoCase(SubProgramNames[i]) == 0)
			return i;
	return -1;
}

int NCMExtJob::WriteFile(CStdioFile &f)
{
	// AppType
	CString Delim(_T(" "));
	CString EndStr(_T("\n"));
	CString AppTypeText(Milling);
	switch(AppType)
	{
	case AT_MILL:
		AppTypeText = Milling;
		break;
	case AT_TURN:
		AppTypeText = Turning;
		break;
	case AT_MILL_TURN:
		AppTypeText = MillTurn;
		break;
	case AT_WELD:
		AppTypeText = Welding;
		break;
	}
	f.WriteString(Operation + Delim + AppTypeText + EndStr);
	// END:AppType

	// Controller file
	if(!UnitName.IsEmpty())
		f.WriteString(ControllerFile + Delim + UnitName + EndStr);
	// END:Controller file

	// Machine file
	if(!MachineName.IsEmpty())
		f.WriteString(MachineFile + Delim + MachineName + EndStr);
	// END:Machine file

	// Program files
	for(int i = 0; i < ProgramNames.GetSize(); ++i)
		f.WriteString(Gcodefile + Delim + ProgramNames[i] + EndStr);
	// END:Program files

	// SubProgram files
	for(int i = 0; i < SubProgramNames.GetSize(); ++i)
		f.WriteString(Gcodefilesub + Delim + SubProgramNames[i] + EndStr);
	// END:SubProgram files

	// Tools
	for(POSITION Pos = ToolsMap.GetStartPosition(); Pos != NULL;)
	{
		ToolDescriptions *pTDescr = NULL;
		NToolID ToolID;
		ToolsMap.GetNextAssoc(Pos, ToolID, pTDescr);
		if(pTDescr)
		{
			for(int i = 0; i < pTDescr->GetSize(); ++i)
				f.WriteString((*pTDescr)[i] + EndStr);
		}
	}
	// END:Tools

	// Start tool number
	CString ToolPos;
	ToolPos.Format(_T(" %d %d"), CurToolN.GetToolNum(), CurToolN.GetTurretNum());
	f.WriteString(StartToolNumber + ToolPos + EndStr); 
	// END:Start tool number

	// Start tool position
	CString Coords;
	Coords.Format(_T(" %lf %lf %lf %lf %lf %lf"), MashX, MashY, MashZ
		, MashA, MashB, MashC);
	f.WriteString(StartToolPosition + Coords + EndStr); 
	// END:Start tool position

	// Stock
	for (int i = 0; i < StockDescriptions.GetSize(); ++i)
		f.WriteString(StockDescriptions[i] + EndStr);
	// END:Stock

	// H and D compensation
	int Num = 0;
	double Val = 0;
	for(POSITION Pos = HComp.GetStartPosition(); Pos != NULL; )
	{
		HComp.GetNextAssoc(Pos, Num, Val);
		CString Str;
		Str.Format(_T(" %d %lf"), Num, Val);
		f.WriteString(Hcode + Str + EndStr);
	}
	for(POSITION Pos = DComp.GetStartPosition(); Pos != NULL; )
	{
		DComp.GetNextAssoc(Pos, Num, Val);
		CString Str;
		Str.Format(_T(" %d %lf"), Num, Val);
		f.WriteString(Dcode + Str + EndStr);
	}
	// END:H and D compensation

	// G54 - G59
	CString Names[6] = {G54, G55, G56, G57, G58, G59};
	for(int i = 0; i < 6; ++i)
	{
		if(UCSs[i].x0 != 0.
			|| UCSs[i].y0 != 0.
			|| UCSs[i].z0 != 0.
			|| UCSs[i].a0 != 0.
			|| UCSs[i].b0 != 0.
			|| UCSs[i].c0 != 0.)
		{
			CString Str;
			Str.Format(_T(" %lf %lf %lf %lf %lf %lf")
				, UCSs[i].x0
				, UCSs[i].y0
				, UCSs[i].z0
				, UCSs[i].a0
				, UCSs[i].b0
				, UCSs[i].c0);
			f.WriteString(FixOffset + _T(" ") + Names[i] + Str + EndStr);
		}
	}
	// END: G54 - G59
	return 0;
}

void NCMExtJob::AddToolDescr(int Position, int Turret, const CString &Description)
{
	NToolID ToolID;
	ToolID.Set(Position, Turret);
	ToolDescriptions *pTDescr = NULL;
	if(!ToolsMap.Lookup(ToolID, pTDescr))
		pTDescr = new ToolDescriptions;
	pTDescr->Add(Description);
	ToolsMap[ToolID] = pTDescr;
}

bool NCMExtJob::RemoveToolDescr(int Position, int Turret)
{
	NToolID ToolID;
	ToolID.Set(Position, Turret);
	ToolDescriptions *pTDescr = NULL;
	if(ToolsMap.Lookup(ToolID, pTDescr))
	{
		CString str;
		delete pTDescr;
		ToolsMap.RemoveKey(ToolID);
		return true;
	}
	return false;
}

static CString __stdcall PreparePath(LPCTSTR lpszPath)
{
	ENSURE(lpszPath != NULL);

	CString strPath = lpszPath;

	int iPathLen = strPath.GetLength();
	if (iPathLen > 0 && strPath [iPathLen - 1] == _T('\\'))
	{
		strPath = strPath.Left(iPathLen - 1);
	}

	return strPath;
}

CString NCMExtJob::GetWindowClass(const CString &regPath)
{
	CString WindowClassName;
	HKEY hkResult = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hkResult) == ERROR_SUCCESS)
	{
		CRegKey m_reg;
		m_reg.m_hKey = HKEY_CURRENT_USER;
		if (m_reg.Open(m_reg.m_hKey, regPath,
			(KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY)) == ERROR_SUCCESS)
		{
			DWORD dwCount = 0;
			if (m_reg.QueryStringValue(_T("WindowClassName"), NULL, &dwCount) == ERROR_SUCCESS)
			{
				for (int i = 0; i < 20 && dwCount < 2; ++i)
				{
					Sleep(500);
					m_reg.QueryStringValue(_T("WindowClassName"), NULL, &dwCount);
				}
				if (dwCount >= 2)
				{
					LPTSTR szValue = new TCHAR[dwCount + 1];

					if (m_reg.QueryStringValue(_T("WindowClassName"), szValue, &dwCount) == ERROR_SUCCESS)
						WindowClassName = szValue;
					delete[] szValue;
				}

			}
			m_reg.Close();
		}
		RegCloseKey(hkResult);
	}
	return WindowClassName;
}

CString NCMExtJob::GetMainPath(const CString &regPath)
{
	return GetPath(_T("MainPath"), regPath);
}

CString NCMExtJob::GetDataPath(const CString &regPath)
{
	return GetPath(_T("DataPath"), regPath);
}

CString NCMExtJob::GetPath(const CString &Name, const CString &regPath)
{
	// Find NCManager MainPath
	CString MainPath;
	LONG res;
	HKEY hkResult = NULL;
	res = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hkResult);
	if (res != ERROR_SUCCESS)
		return MainPath;



	CRegKey m_reg;
	m_reg.m_hKey = HKEY_CURRENT_USER;
	if (m_reg.Open(m_reg.m_hKey, PreparePath(regPath),
		(KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY)) == ERROR_SUCCESS)
	{
		MainPath.Empty();

		bool OK = true;
		DWORD dwCount = 0;
		if (m_reg.QueryStringValue(Name, NULL, &dwCount) != ERROR_SUCCESS)
		{
			return MainPath;
		}

		if (dwCount == 0)
		{
			return MainPath;
		}

		LPTSTR szValue = new TCHAR[dwCount + 1];

		BOOL bRes = m_reg.QueryStringValue(Name, szValue, &dwCount) == ERROR_SUCCESS;
		if (bRes)
		{
			MainPath = szValue;
		}

		delete[] szValue;
		m_reg.Close();
	}
	RegCloseKey(hkResult);
	return MainPath;
}

CString NCMExtJob::GetRegPath(void)
{
	// Try to find LastVersion
	CString LastVersion = _T("3.8");// Version 3.8 does not support LastVersion registry key
	CString regPath = _T("Software\\Inteks\\NCManager\\");
	HKEY hkResult = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hkResult) == ERROR_SUCCESS)
	{
		CRegKey m_reg;
		m_reg.m_hKey = HKEY_CURRENT_USER;
		if (m_reg.Open(m_reg.m_hKey, PreparePath(regPath),
			(KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY)) == ERROR_SUCCESS)
		{
			DWORD dwCount = 0;
			if (m_reg.QueryStringValue(_T("LastVersion"), NULL, &dwCount) == ERROR_SUCCESS)
			{
				if (dwCount != 0)
				{
					LPTSTR szValue = new TCHAR[dwCount + 1];

					if (m_reg.QueryStringValue(_T("LastVersion"), szValue, &dwCount) == ERROR_SUCCESS)
						LastVersion = szValue;
					delete[] szValue;
				}

			}
			m_reg.Close();
		}
		RegCloseKey(hkResult);
	}
	regPath += LastVersion;
	return regPath;
}
