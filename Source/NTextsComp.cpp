#include "stdafx.h"
#include "NCMProject.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "MainFrm.h"
#include "NTextView.h"
#include "NTextsComp.h"

const CString NTextsComp::Txt1(_T("Text1.txt"));
const CString NTextsComp::Txt2(_T("Text2.txt"));
const CString NTextsComp::Exe(_T("WinMergeU.exe"));
const CString NTextsComp::IniFile(_T("WinMerge.ini"));

NTextsComp::NTextsComp(const CNCMDoc& doc) : doc_(doc)
{
	constexpr int size = 5;
	patterns_.resize(size);
	replacements_.resize(size);
	enable_.resize(size);
	patternsKey_.resize(size);
	replacementsKey_.resize(size);
	enableKey_.resize(size);
	const auto pUnit = doc_.GetUnit();
	if (pUnit != nullptr)
	{
		pUnit->GetOtherRegExp(_T("IgnoreLineNumber"), patterns_.at(0), replacements_.at(0));
		enable_.at(0) = NCM_PROJECT.Defaults.GetBool(_T("Defaults/IgnoreLineNumber@Enable"), true);
		pUnit->GetOtherRegExp(_T("IgnoreComment"), patterns_.at(0), replacements_.at(1));
		enable_.at(0) = NCM_PROJECT.Defaults.GetBool(_T("Defaults/IgnoreComment@Enable"), true);
		pUnit->GetOtherRegExp(_T("IgnoreBlanks"), patterns_.at(0), replacements_.at(2));
		enable_.at(0) = NCM_PROJECT.Defaults.GetBool(_T("Defaults/IgnoreBlanks@Enable"), true);
		pUnit->GetOtherRegExp(_T("IgnoreCase"), patterns_.at(0), replacements_.at(3));
		enable_.at(0) = NCM_PROJECT.Defaults.GetBool(_T("Defaults/IgnoreCase@Enable"), true);
		pUnit->GetOtherRegExp(_T("IgnoreNumFormat"), patterns_.at(0), replacements_.at(4));
		enable_.at(0) = NCM_PROJECT.Defaults.GetBool(_T("Defaults/IgnoreNumFormat@Enable"), true);
	}
	for (int i = 0; i < size; ++i)
	{
		CString iStr;
		iStr.Format(_T("%02d="), i);
		patternsKey_.at(i) = _T("SubstitutionFilters/Pattern") + iStr;
		replacementsKey_.at(i) = _T("SubstitutionFilters/Replacement") + iStr;
		enableKey_.at(i) = _T("SubstitutionFilters/Enabled") + iStr;
	}
	prefix_.LoadString(IDS_ACTIVE);
}

NTextsComp::~NTextsComp()
{
}

bool NTextsComp::DoComp() const
{
	CString IniPath = NCM_PROJECT.GetComparePath() + IniFile;
	SaveIniFile(IniPath);

	TCHAR szPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(nullptr, szPath, _MAX_PATH));
	CString Name = szPath;
	int nIndex = Name.ReverseFind(_T('\\'));
	if (nIndex > 0)
		Name = Name.Left(nIndex + 1);
	else
		Name.Empty();
	Name = Name + Exe;
	CString LeftTitle(_T("-dl \"") + title1_ + _T("\""));
	CString RightTitle(_T("-dr \"") + title2_ + _T("\""));

	auto res = _spawnl(_P_NOWAIT, Name, _T("\"") + Name + _T("\"")
		, _T("\"") + NCM_PROJECT.GetTempPath() + Txt1 + _T("\""), _T("\"") + NCM_PROJECT.GetTempPath() + Txt2 + _T("\"")
		, LeftTitle, RightTitle
		, _T("-inifile \"") + IniPath + _T("\""), nullptr);

	return res >= 0;
}

void NTextsComp::SaveIniFile(const CString& path) const
{
	CStdioFile f;
	if (!f.Open(path, CFile::typeText | CFile::modeRead))
		return;
	char buf[100000];
	auto count = f.Read(buf, sizeof(buf) - 1);
	buf[count] = '\0';
	f.Close();

	CString text(buf);
	SetParamsToIni(text);

	if (!f.Open(path, CFile::typeText | CFile::modeWrite))
		return;
	f.Write(text, text.GetLength());
	f.Close();
}

void NTextsComp::SetParamsToIni(CString& iniText) const
{
	for (int i = 0; i < enable_.size(); ++i)
	{
		SetValue(iniText, patternsKey_.at(i), patterns_.at(i));
		SetValue(iniText, replacementsKey_.at(i), replacements_.at(i));
		SetValue(iniText, enableKey_.at(i), enable_.at(i) ? _T("1") : _T("0"));
	}
}

void NTextsComp::SetValue(CString& text, const CString& key, const CString& val)
{
	const int startPos = text.Find(key);
	const int nextPos = startPos + key.GetLength();
	const int strEndPos = text.Find('\n', nextPos);
	if (startPos < 0 || strEndPos < 0)
		return;
	text.Delete(nextPos, strEndPos - nextPos);
	text.Insert(nextPos, val);
}

bool NTextsComp::MakeArg1(int progInd)
{
	bool res = MakeArg(Txt1, progInd);
	title1_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArg2(int progInd)
{
	bool res = MakeArg(Txt2, progInd);
	title2_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArg(const CString& stdFile, const CString& sourceFile)
{
	titleTemp_ = sourceFile;
	return 0 != CopyFile(sourceFile, NCM_PROJECT.GetTempPath() + stdFile, FALSE);
}

bool NTextsComp::MakeArgOrig(const CString& stdFile, int progInd)
{
	CString fileName;
	CObject* pProg;
	if(!doc_.NGetProgramList()->GetAt(progInd, fileName, pProg))
		return false;
	titleTemp_ = fileName;
	return MakeArg(stdFile, fileName);
}

bool NTextsComp::MakeArgOrig(const CString& stdFile)
{
	NTextView* pTextView = (NTextView*)((CMainFrame*)AfxGetMainWnd())->GetTextView();
	if (pTextView == nullptr)
		return false;
	int loadedProgInd = pTextView->GetLoadedProgInd();
	if (loadedProgInd < 0)
		return false;
	return MakeArgOrig(stdFile, loadedProgInd);
}

bool NTextsComp::MakeArg(const CString& stdFile)
{
	NTextView* pTextView = (NTextView*)((CMainFrame*)AfxGetMainWnd())->GetTextView();
	if (pTextView == nullptr)
		return false;
	CString fullName = pTextView->GetLoadedProg();
	const int index = fullName.ReverseFind('\\');
	titleTemp_ = prefix_ + fullName.Mid(index + 1);
	return 	((NTextView*)((CMainFrame*)AfxGetMainWnd())->GetTextView())->SaveCopy(NCM_PROJECT.GetTempPath() + Txt1);
}

bool NTextsComp::MakeArg(const CString& stdFile, int progInd)
{
	if (progInd < 0)
		return false;
	CString fullName;
	NProgram* pProg;
	doc_.NGetProgramList()->GetAt(progInd, fullName, (CObject*&)pProg);
	if (pProg == nullptr)
		return false;
	const int index = fullName.ReverseFind('\\');
	titleTemp_ = fullName.Mid(index + 1);
	doc_.SaveProgText(NCM_PROJECT.GetTempPath() + stdFile, pProg);
	return true;
}

bool NTextsComp::MakeArg1(const CString& sourceFile)
{
	bool res = MakeArg(Txt1, sourceFile);
	title1_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArg2(const CString& sourceFile)
{
	bool res = MakeArg(Txt2, sourceFile);
	title2_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArgOrig1(int progInd)
{
	bool res = MakeArgOrig(Txt1, progInd);
	title1_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArgOrig2(int progInd)
{
	bool res = MakeArgOrig(Txt2, progInd);
	title2_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArgOrig1()
{
	bool res = MakeArgOrig(Txt1);
	title1_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArgOrig2()
{
	bool res = MakeArgOrig(Txt2);
	title2_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArg1()
{
	bool res = MakeArg(Txt1);
	title1_ = titleTemp_;
	return res;
}

bool NTextsComp::MakeArg2()
{
	bool res = MakeArg(Txt2);
	title2_ = titleTemp_;
	return res;
}
