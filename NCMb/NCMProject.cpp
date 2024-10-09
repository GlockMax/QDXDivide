#include "..\Common\NCMProject.h"
#include "stdafx.h"
#include "BObjectManager.h"
#include "NCMVersion.h"
#include "NFont.h"
#include "BConst.h"
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include "NSurfSettings.h"
#include "NCMProject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCMProject Project;
NCMProject &NCMProject::ActiveProject = Project;
NCMProject::NCMProject(void) :
CommonPaths(NCMVersion) , Defaults(*GenNTiParams())
{
	
	pObjectManager = new BObjectManager;
	InFileVersion = 1.;
	FloatingLicense = false;
	PostUserID = 0;
	pPExport = NULL;
	UseQuadTreeParallel = true;
	SetDefaultSettings();
}

NCMProject::~NCMProject(void)
{
	delete pObjectManager;

	POSITION pos;
	CString key;
	// Delete all colors from Colors map
	NColor *c;
	for(pos = Colors.GetStartPosition(); pos != NULL;)
	{
		Colors.GetNextAssoc(pos, key, c);
		delete c;
	}
	Colors.RemoveAll();
	// Delete doubles
	double *pDouble;
	for(pos = DoubleParams.GetStartPosition(); pos != NULL;)
	{
		DoubleParams.GetNextAssoc(pos, key, pDouble);
		delete pDouble;
	}
	DoubleParams.RemoveAll();
	// Delete ints
	int *pInt;
	for(pos = IntParams.GetStartPosition(); pos != NULL;)
	{
		IntParams.GetNextAssoc(pos, key, pInt);
		delete pInt;
	}
	IntParams.RemoveAll();
	Defaults.Clear();
}
int NCMProject::StartNewProject(void)
{
	return 0;
}
int NCMProject::CloseProject(void)
{
	return 0;
}
void NCMProject::Serialize(CArchive &ar)
{
	CStringArray UnitContFiles;
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.31")))
	{
		if (ar.IsStoring())
		{
			INT_PTR Count = UnitConfs.GetSize();
			SerializeElements(ar, &Count, 1);
			for (int i = 0; i < Count; ++i)
				UnitConfs.GetAt(i).Serialize(ar);
		}
		else
		{
			INT_PTR Count = 0;
			SerializeElements(ar, &Count, 1);
			UnitConfs.SetSize(Count);
			for (int i = 0; i < Count; ++i)
				UnitConfs.GetAt(i).Serialize(ar);
		}
	}
	else if (NCM_PROJECT.GetInFileVersion() >= atof("5.1"))
	{
		UnitContFiles.Serialize(ar);//done
	}
	else
	{
		CString tmp;
		SerializeElements(ar, &tmp, 1);
		if (!tmp.IsEmpty())
		{
			UnitContFiles.SetSize(1);
			UnitContFiles[0] = tmp;
		}
	}
	if (!UnitContFiles.IsEmpty())// before 5.31
	{
		UnitConfs.SetSize(UnitContFiles.GetSize());
		for (int i = 0; i < UnitContFiles.GetSize(); ++i)
		{
			CString Name;
			if (i == 0)
				Name = _T("TempUnit.sm3");
			else
				Name.Format(_T("TempUnit%d.sm3"), i);
			UnitConfs.GetAt(i).SetAt(Name, UnitContFiles.GetAt(i));
		}
		UnitContFiles.RemoveAll();
	}
	SerializeElements(ar, &DFNFile, 1);
	SerializeElements(ar, &DefaultsFile, 1);
	StockFile.Serialize(ar);
	MachineFile.Serialize(ar);
	PartsFile.Serialize(ar);
	ExObjsFile.Serialize(ar);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.3")))
		ToolFile.Serialize(ar);
}

void NCMProject::PostSerialize(CArchive& ar, const CString& DocPath)
{
	if (ar.IsLoading())
	{
		if (!MachineFile.IsEmpty())
		{
			CString TempName;
			MakeTempName(MachineFile.GetAt(0), TempName);
			MachineFile.SetAt(0, TempName);
			CString NewPath = GetMachinesPath() + TempName + _T("\\");
			MakeTempMachDir(TempName);
			SaveStr2File(NewPath + _T("MAIN.xml"), MachineFile[1], true);
			for (INT_PTR i = 2; i < MachineFile.GetSize(); i += 2)
			{
				SaveStr2File(NewPath + MachineFile[i], MachineFile[i + 1], true);
			}
		}
		for (INT_PTR i = 0; i < StockFile.GetSize(); i += 2)
		{
			if (StockFile[i].IsEmpty())
				continue;
			SaveStr2File(DocPath + StockFile[i], StockFile[i + 1], false);
		}
		for (INT_PTR i = 0; i < ToolFile.GetSize(); i += 2)
		{
			if (ToolFile[i].IsEmpty())
				continue;
			SaveStr2File(DocPath + ToolFile[i], ToolFile[i + 1], false);
		}
		for (INT_PTR i = 0; i < PartsFile.GetSize(); i += 2)
		{
			SaveStr2File(DocPath + PartsFile[i] + _T(".stl"), PartsFile[i + 1], false);
		}
		for (INT_PTR i = 0; i < ExObjsFile.GetSize(); i += 2)
		{
			SaveStr2File(DocPath + ExObjsFile[i] + _T(".stl"), ExObjsFile[i + 1], false);
		}
		if (!DFNFile.IsEmpty())
		{
			CString buf = DFNFile.SpanExcluding("\n");
			DFNFile.Delete(0, buf.GetLength() + 1);
			if (!DFNFile.IsEmpty())
			{
				SaveStr2File(GetSettingsPath() + _T("TempSettings.dfn"), DFNFile, true);
				DFNFile = _T("TempSettings.dfn");
			}
			else
				DFNFile = buf;
		}
	}
	else
	{
		DFNFile.Empty();
		DefaultsFile.Empty();
		MachineFile.RemoveAll();
	}
	PartsFile.RemoveAll();
	ExObjsFile.RemoveAll();
	StockFile.RemoveAll();
	ToolFile.RemoveAll();
}

NCMProject &NCMProject::GetProject(void)
{
	return ActiveProject;
}
bool NCMProject::LockLicense(int i)
{
	if(i < 0 || i >= MODULES_NUM)
		return false;

	if(!FloatingLicense)
		return true;

	return Modules[i].Lock(i);
}
bool NCMProject::UnlockLicense(int i)
{
	if(i < 0 || i >= MODULES_NUM)
		return false;

	if(!FloatingLicense)
		return true;

	return Modules[i].Unlock();
}
void NCMProject::ReleaseLicenses()
{
	for(int i = 0; i < MODULES_NUM; ++i)
		Modules[i].Clear();
}

bool NCMProject::LoadDefaults(const CString& FileName)
{
	SetDefaultColorsFonts();
	CString name = GetSettingsPath() + FileName;
	bool loadOkay = Defaults.LoadFile(name.GetBuffer());
	if (loadOkay)
	{
		Defaults.GetMiskDouble((MM_INCH == M_MM) ? _T("Defaults/MiskParams/Doubles/mm") : _T("Defaults/MiskParams/Doubles/inch"), DoubleParams);
		Defaults.GetMiskInt(_T("Defaults/MiskParams/Ints"), IntParams);
		Defaults.GetMiskColors(_T("Defaults/MiskColors"), Colors);
		double* pDouble = nullptr;
		if (DoubleParams.Lookup("ModTolerance", pDouble))// Added 16.12.06
			NSurfSettings::SetTolerance(*pDouble);
	}
	return loadOkay;
}

unsigned int NCMProject::GetPostUserID()
{
	return PostUserID;
}

bool NCMProject::IsPostUserMaster()
{
#ifdef NC_DEMO
	return true;
#endif
	return (PostUserID == ALL_POST_USER_ID);
}

void NCMProject::SetDefaultSettings()
{
	// Doubles
	double *pDouble;
	pDouble = new double(3.);
	DoubleParams["CircleTolerance"] = pDouble;

	pDouble = new double(5.);
	DoubleParams["SelectionRegion"] = pDouble;

	pDouble = new double(5.);
	DoubleParams["ToolMarker"] = pDouble;

	pDouble = new double(10.);
	DoubleParams["SnapMarker"] = pDouble;

	pDouble = new double(4.);
	NCM_PROJECT.DoubleParams["Axes1Size"] = pDouble;

	pDouble = new double(4.);
	NCM_PROJECT.DoubleParams["Axes2Size"] = pDouble;

	pDouble = new double(4.);
	NCM_PROJECT.DoubleParams["Axes3Size"] = pDouble;

	pDouble = new double(0.1);
	NCM_PROJECT.DoubleParams["WeldBeamR"] = pDouble;

	pDouble = new double(0.1);
	DoubleParams["ModTolerance"] = pDouble;

	pDouble = new double(0.1);
	DoubleParams["ModToleranceTurn"] = pDouble;

	pDouble = new double(0.1);
	DoubleParams["ModToleranceMillTurn"] = pDouble;

	pDouble = new double(0.1);
	DoubleParams["ModToleranceWeld"] = pDouble;

	pDouble = new double(0.001);
	DoubleParams["ContTolerance"] = pDouble;

	pDouble = new double(0.01);
	DoubleParams["TolLineApp"] = pDouble;

	pDouble = new double(0.0002);
	DoubleParams["DevArcSpan"] = pDouble;

	pDouble = new double(0.05);
	DoubleParams["MinRadArc"] = pDouble;

	pDouble = new double(30.);
	DoubleParams["ProgToolAxesLength"] = pDouble;

	pDouble = new double(5.);
	DoubleParams["ProgNodesSize"] = pDouble;

	pDouble = new double(1.);
	DoubleParams["ProgToolAxesThickness"] = pDouble;

	pDouble = new double(0.2);
	DoubleParams["ConverterAcc"] = pDouble;

	pDouble = new double(1.2);
	DoubleParams["ConverterAng"] = pDouble;

	// Ints
	int *pInt;
	pInt = new int(8);
	IntParams["ToolMash"] = pInt;
	pInt = new int(500);
	IntParams["ViewChangeTime"] = pInt;
	pInt = new int(1);
	IntParams["GradientBack"] = pInt;
	pInt = new int(0);
	IntParams["CutHelix"] = pInt;
}
CString NCMProject::GetMachinesPath(void) const
{
	return CommonPaths.GetPath(CF_MACHINES);
}

CString NCMProject::GetUnitsPath(void) const
{
	return CommonPaths.GetPath(CF_UNITS);
}

CString NCMProject::GetSettingsPath(void) const
{
	return CommonPaths.GetPath(CF_SETTINGS);
}

CString NCMProject::GetPrototypesPath(void) const
{
	return CommonPaths.GetPath(CF_PROTOTYPES);
}

CString NCMProject::GetToolsPath(void) const
{
	return CommonPaths.GetPath(CF_TOOLS);
}

CString NCMProject::GetDemoPath(void) const
{
	return CommonPaths.GetPath(CF_DEMO);
}

CString NCMProject::GetComparePath(void) const
{
	return CommonPaths.GetPath(CF_COMP);
}

CString NCMProject::GetMacroPath(void) const
{
	return CommonPaths.GetPath(CF_MACRO);
}

CString NCMProject::GetTempPath(void) const
{
	return CommonPaths.GetPath(CF_TEMP);
}

int NCMProject::LoadFile2Str(const CString &Path, CString &Str)
{
	CStdioFile f;
	if(!f.Open(Path, CFile::typeBinary | CFile::modeRead ))
		return 0;
	int n = __min(int(f.GetLength()), INT_MAX);
	n = f.Read(Str.GetBuffer(n), n);
	Str.ReleaseBuffer(n);
	f.Close();
	return n;
}

int NCMProject::SaveStr2File(const CString& Path, const CString& Str, bool AllowRewrite)
{
	if (Str.IsEmpty())
		return 0;
	CStdioFile f;
	if (!AllowRewrite && f.Open(Path, CFile::modeRead | CFile::typeBinary)) // to prevent rewriting existing file
		return 0;
	if (!f.Open(Path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return 0;
	f.Write(Str, Str.GetLength());
	return Str.GetLength();
}

BOOL NCMProject::IsDots(const TCHAR* str)
{
    if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
    return TRUE;
}

BOOL NCMProject::DeleteDirectory(const TCHAR* sPath, bool KeepFolder/* = false*/)
{
    HANDLE hFind;  // file handle

    WIN32_FIND_DATA FindFileData;

    TCHAR DirPath[MAX_PATH];
    TCHAR FileName[MAX_PATH];

    _tcscpy(DirPath,sPath);
    _tcscat(DirPath,"\\*");    // searching all files

    _tcscpy(FileName,sPath);
    _tcscat(FileName,"\\");

    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
    _tcscpy(DirPath,FileName);
        
    bool bSearch = true;
    while(bSearch) { // until we finds an entry
        if(FindNextFile(hFind,&FindFileData)) {
            if(IsDots(FindFileData.cFileName)) continue;
            _tcscat(FileName,FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                // we have found a directory, recurse
                if(!DeleteDirectory(FileName)) { 
                    FindClose(hFind); 
                    return FALSE; // directory couldn't be deleted
                }
                RemoveDirectory(FileName); // remove the empty directory
                _tcscpy(FileName,DirPath);
            }
            else {
                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    _chmod(FileName, _S_IWRITE); // change read-only file mode
                if(!DeleteFile(FileName)) {  // delete the file
                    FindClose(hFind); 
                    return FALSE; 
                }                 
                _tcscpy(FileName,DirPath);
            }
        }
        else {
            if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
            bSearch = false;
            else {
                // some error occured, close the handle and return FALSE
                FindClose(hFind); 
                return FALSE;
            }

        }

    }
    FindClose(hFind);  // closing file handle
 
    return KeepFolder ? TRUE : RemoveDirectory(sPath); // remove the empty directory

}

void NCMProject::MakeTempMachDir(const CString& Name)
{
	CString NewPath = GetMachinesPath() + Name;
	int Res = _mkdir(NewPath);
	if(errno == EEXIST)
	{
		DeleteDirectory(NewPath, true);
//		Res = _mkdir(NewPath);
	}//if
}

bool NCMProject::ReadDirFiles(const CString &Path, const CString &Filter, CArray<CString> &Names)
{
	Names.RemoveAll();
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(Path + Filter, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	for (BOOL Stop = FALSE; !Stop; Stop = !FindNextFile(hFind, &findFileData))
	{
		if (findFileData.cFileName[0] != _T('.'))
			Names.Add(findFileData.cFileName);
	}
	FindClose(hFind);  // closing file handle

	return true;

}

double NCMProject::GetDouble(const CString &Name, double Default) const
{
	double *pDouble;
	if (DoubleParams.Lookup(Name, pDouble))
		return *pDouble;
	return Default;
}

void NCMProject::MakeTempName(const CString& FileName, CString& TempName)
{
	static const CString Suffix = _T("temp");
	TempName = FileName + Suffix;
}

void NCMProject::StoreDIParams2XML() const
{
	// Doubles
	CString key = (MM_INCH == M_MM) ? _T("Defaults/MiskParams/Doubles/mm@") : _T("Defaults/MiskParams/Doubles/inch@");
	for (auto pos = NCM_PROJECT.DoubleParams.GetStartPosition(); pos != NULL;)
	{
		double* pDouble = nullptr;;
		CString str;
		NCM_PROJECT.DoubleParams.GetNextAssoc(pos, str, pDouble);
		NCM_PROJECT.Defaults.SetDouble(key + str, *pDouble);
	}
	// Ints
	key = _T("Defaults/MiskParams/Ints@");
	for (auto pos = NCM_PROJECT.IntParams.GetStartPosition(); pos != NULL;)
	{
		int* pInt = nullptr;
		CString str;
		NCM_PROJECT.IntParams.GetNextAssoc(pos, str, pInt);
		NCM_PROJECT.Defaults.SetDouble(key + str, *pInt);
	}
}

void NCMProject::StoreMColors2XML() const
{
	const CString Sect = _T("Defaults/MiskColors/");
	const CString NameFormat = _T("Color%d");
	// Colors
	for (POSITION pos = Colors.GetStartPosition(); pos != NULL;)
	{
		CString key;
		NColor* pCol = nullptr;
		Colors.GetNextAssoc(pos, key, pCol);
		int ID = 0;
		if (!TrBaseID.Lookup(key, ID))
			continue;
		CString Name;
		Name.Format(NameFormat, ID);
//		Defaults.AddElement(Sect, Name);
		Defaults.SetColor((Sect + Name).GetBuffer(), *pCol);
	}
}

void NCMProject::AddColor(const CString& BaseStr, NColor* pcol, int CurLangStrID)
{
	CString t;
	if (!t.LoadString(CurLangStrID))
		return;
	NColor* c = nullptr;
	if (Colors.Lookup(BaseStr, c))
	{
		*c = *pcol;
		delete pcol;
	}
	else
		Colors[BaseStr] = pcol;
	TrBaseCur[BaseStr] = t;
	TrBaseID[BaseStr] = CurLangStrID;
	TrCurBase[t] = BaseStr;
}

void NCMProject::SetDefaultColorsFonts()
{
	// Colors
	NColor* pcol;
	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.168627f; pcol->array[3] = 1.f;
	AddColor("‘онѕанелиќбъектов", pcol, IDS_ColKeyOPB);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("“екстѕанелиќбъектов", pcol, IDS_ColKeyOPT);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.168627f; pcol->array[3] = 1.f;
	AddColor("‘онѕанели онтрол€", pcol, IDS_ColKeyCPB);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.168627f; pcol->array[3] = 1.f;
	AddColor("¬ектор»нструмента", pcol, IDS_ColKeyTV);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("“екстѕанели онтрол€", pcol, IDS_ColKeyCPT);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("“екстѕанелиќшибки", pcol, IDS_ColKeyEPT);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("‘онѕанелиќшибки", pcol, IDS_ColKeyEPB);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("“екстѕанели—войств", pcol, IDS_ColKeyPPT);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("‘онѕанели—войств", pcol, IDS_ColKeyPPB);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.168627f; pcol->array[3] = 1.f;
	AddColor("‘он“екстовогоќкна", pcol, IDS_ColKeyTWB);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("“екстѕрограммы", pcol, IDS_ColKeyPT);

	pcol = new NColor;
	pcol->array[0] = 240.f / 255; pcol->array[1] = 240.f / 255; pcol->array[2] = 240.f / 255; pcol->array[3] = 1.f;
	AddColor("‘онƒл€Ќомеров—трок", pcol, IDS_ColKeyFFSN);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("“екстЌомеров—трок", pcol, IDS_ColKeyTSN);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("—трелкајктивного адра", pcol, IDS_ColKeyAAL);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.5f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("—трелка”казанного адра", pcol, IDS_ColKeyASL);

	pcol = new NColor;
	pcol->array[0] = 0.043137f; pcol->array[1] = 0.f; pcol->array[2] = 0.082353f; pcol->array[3] = 1.f;
	AddColor("‘он√рафическогоќкна", pcol, IDS_ColKeyGWB);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.082353f; pcol->array[3] = 1.f;
	AddColor("‘он√рафическогоќкнаЋев¬ерх", pcol, IDS_ColKeyGWBLT);

	pcol = new NColor;
	pcol->array[0] = 0.043137f; pcol->array[1] = 0.f; pcol->array[2] = 0.082353f; pcol->array[3] = 1.f;
	AddColor("‘он√рафическогоќкнаЋевЌижн", pcol, IDS_ColKeyGWBLB);

	pcol = new NColor;
	pcol->array[0] = 0.043137f; pcol->array[1] = 0.f; pcol->array[2] = 0.082353f; pcol->array[3] = 1.f;
	AddColor("‘он√рафическогоќкнаѕрав¬ерх", pcol, IDS_ColKeyGWBRT);

	pcol = new NColor;
	pcol->array[0] = 0.043137f; pcol->array[1] = 0.f; pcol->array[2] = 0.082353f; pcol->array[3] = 1.f;
	AddColor("‘он√рафическогоќкнаѕравЌижн", pcol, IDS_ColKeyGWBRB);

	pcol = new NColor;
	pcol->array[0] = 0.5f; pcol->array[1] = 0.1f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("—ечение", pcol, IDS_ColKeyS);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.250980f; pcol->array[3] = 1.f;
	AddColor("ѕрограмма", pcol, IDS_ColKeyP);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 1.f; pcol->array[2] = 0.250980f; pcol->array[3] = 1.f;
	AddColor("÷икл", pcol, IDS_ColKeyC);

	pcol = new NColor;
	pcol->array[0] = 0.784314f; pcol->array[1] = 0.701961f; pcol->array[2] = 0.117647f; pcol->array[3] = 0.5f;
	AddColor("»нструментќправкаѕо”молчанию", pcol, IDS_ColKeyHTWP);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.1f; pcol->array[2] = 1.f; pcol->array[3] = 0.5f;
	AddColor("»нструментЌе–аб„астьѕо”молчанию", pcol, IDS_ColKeyTNWP);

	pcol = new NColor;
	pcol->array[0] = 0.5f; pcol->array[1] = 0.5f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor("«аготовкаѕо”молчанию", pcol, IDS_ColKeySt);

	pcol = new NColor;
	pcol->array[0] = 0.5f; pcol->array[1] = 0.5f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("«аготовкаќбработанна€ѕо”молчанию", pcol, IDS_ColKeyStCut);

	pcol = new NColor;
	pcol->array[0] = 0.5f; pcol->array[1] = 1.0f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("ƒетальЌомер1", pcol, IDS_ColKeyPt1);

	pcol = new NColor;
	pcol->array[0] = 0.5f; pcol->array[1] = 1.0f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("ѕриспособлениеѕо”молчанию", pcol, IDS_ColKeyFx);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor("ѕрив€зка", pcol, IDS_ColKeySn);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("’ол’од", pcol, IDS_ColKeyFM);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("ƒвижениеѕоќтрезку", pcol, IDS_ColKeyLM);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor("ƒвижениеѕ„—", pcol, IDS_ColKeyCW);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("ƒвижениеѕр„—", pcol, IDS_ColKeyCCW);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.5f; pcol->array[3] = 1.f;
	AddColor("ѕоворот", pcol, IDS_ColKeyRot);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("Ѕезќхлаждени€", pcol, IDS_ColKeyCO);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("ќхлаждение¬оздухом", pcol, IDS_ColKeyCM);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor("ќхлаждение∆идкостью", pcol, IDS_ColKeyCF);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor(" оррекци€¬верх", pcol, IDS_ColKeyUC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor(" оррекци€¬низ", pcol, IDS_ColKeyDC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor(" оррекци€Ќаƒлину¬ыкл", pcol, IDS_ColKeyLCF);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor(" оррекци€¬лево", pcol, IDS_ColKeyLHC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor(" оррекци€¬право", pcol, IDS_ColKeyRHC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor(" онтурна€ оррекци€¬ыкл", pcol, IDS_ColKeyCCF);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("÷икл—верлени€", pcol, IDS_ColKeyDRC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("÷икл–езьбы", pcol, IDS_ColKeyTHC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f; pcol->array[3] = 1.f;
	AddColor("÷икл–асточки", pcol, IDS_ColKeyBC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("Ќераспознанные—имволы", pcol, IDS_COLTXT);

	pcol = new NColor;
	pcol->array[0] = 250.0f / 255.0f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("G оды", pcol, IDS_COLGCOD);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 250.0f / 255.0f; pcol->array[3] = 1.f;
	AddColor("M оды", pcol, IDS_COLMCOD);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 150.0f / 255.0f; pcol->array[2] = 250.0f / 255.0f; pcol->array[3] = 1.f;
	AddColor("–егистр", pcol, IDS_COLREG);

	pcol = new NColor;
	pcol->array[0] = 150.0f / 255.0f; pcol->array[1] = 150.0f / 255.0f; pcol->array[2] = 250.f / 255.f; pcol->array[3] = 1.f;
	AddColor("ќбозначени€", pcol, IDS_COLOTHER);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("ѕростые„исла", pcol, IDS_COLSDIG);

	pcol = new NColor;
	pcol->array[0] = 100.0f / 255.0f; pcol->array[1] = 0.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor("Ќомер оманды", pcol, IDS_COLCMDNUM);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f; pcol->array[3] = 1.f;
	AddColor(" омментарии", pcol, IDS_COLCOMM);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f;
	AddColor("ћаксимальна€ѕодача", pcol, IDS_COLFEEDMAX);

	pcol = new NColor;
	pcol->array[0] = 1.f; pcol->array[1] = 0.f; pcol->array[2] = 0.f;
	AddColor("’олостой’од", pcol, IDS_COLFEEDRAPID);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 1.f; pcol->array[2] = 0.f;
	AddColor("ѕодача1", pcol, IDS_COLFEEDFIRST);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("ѕодача2", pcol, IDS_COLFEEDSECOND);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("ЌомерЋинии", pcol, IDS_COLLINENUM);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("ѕодача", pcol, IDS_COLFEED);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("—корость", pcol, IDS_COLSPEED);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("»нструмет", pcol, IDS_COLTOOL);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("Ћинейные оординаты", pcol, IDS_COLLINEAR);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("”гловые оординаты", pcol, IDS_COLANGLE);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor(" ругова€»нтерпол€ци€", pcol, IDS_COLARC);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor(" оррекци€", pcol, IDS_COLOFFSET);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("—тартѕрограммы", pcol, IDS_COLPROGSE);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("Ќачалоѕрограммы", pcol, IDS_COLPROGBE);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("ќсевое—ечение«аготовок", pcol, IDS_COLSTOCKSECT);

	pcol = new NColor;
	pcol->array[0] = 0.f; pcol->array[1] = 0.f; pcol->array[2] = 1.f;
	AddColor("ќсевое—ечениеƒеталей", pcol, IDS_COLPARTSECT);

	// Fonts
	LOGFONT lf;
	AFX_GLOBAL_DATA* pGlobData = GetGlobalData();
	pGlobData->fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	pGlobData->GetNonClientMetrics(info);
	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	CString FontName(_T("Arial"));

	memcpy_s(lf.lfFaceName, 31, FontName.GetBuffer(), FontName.GetLength());
	lf.lfFaceName[min(FontName.GetLength(), 31)] = '\x0';

	NCM_PROJECT.Defaults.SetFont(_T("Defaults/PaneObjects/Font"), lf);
	NCM_PROJECT.Defaults.SetFont(_T("Defaults/PaneParameters/Font"), lf);
	NCM_PROJECT.Defaults.SetFont(_T("Defaults/PaneErrors/Font"), lf);
	NCM_PROJECT.Defaults.SetFont(_T("Defaults/PaneProperties/Font"), lf);
	NCM_PROJECT.Defaults.SetFont(_T("Defaults/PaneText/Font"), lf);
}

