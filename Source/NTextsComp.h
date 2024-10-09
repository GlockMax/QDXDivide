#pragma once
class NTextsComp
{
public:
	NTextsComp(const class CNCMDoc& doc);
	~NTextsComp();
	bool DoComp() const;
	void SaveIniFile(const CString& path) const;
	void SetParamsToIni(CString& path) const;
	static void SetValue(CString& text, const CString& key, const CString& val);
	const CString& GetTxt1() const { return Txt1; }
	const CString& GetTxt2() const { return Txt2; }
	bool MakeArg1(int progInd);// internal program text
	bool MakeArg2(int progInd);
	bool MakeArg1(const CString& sourceFile);// arbitrary file
	bool MakeArg2(const CString& sourceFile);
	bool MakeArg1();// text editor content
	bool MakeArg2();
	bool MakeArgOrig1(int progInd);// original fail for a given program
	bool MakeArgOrig2(int progInd);
	bool MakeArgOrig1();// original fail for a text editor
	bool MakeArgOrig2();
protected:
	bool MakeArg(const CString& stdFile, int progInd);
	bool MakeArg(const CString& stdFile, const CString& sourceFile);
	bool MakeArg(const CString& stdFile);
	bool MakeArgOrig(const CString& stdFile, int progInd);
	bool MakeArgOrig(const CString& stdFile);
protected:
	static const CString Txt1;
	static const CString Txt2;
	static const CString Exe;
	static const CString IniFile;
protected:
	CString prefix_;
	CString title1_;
	CString title2_;
	CString titleTemp_;
	std::vector<CString> patterns_;
	std::vector<CString> replacements_;
	std::vector<bool> enable_;
	std::vector<CString> patternsKey_;
	std::vector<CString> replacementsKey_;
	std::vector<CString> enableKey_;
	const class CNCMDoc& doc_;
};

