#pragma once
#include "Macro_API.h"
#include <vector>

class Macro_API SGenMacro
{
	friend class SMDialog;
public:
	SGenMacro();
	~SGenMacro();
	const CString& Generate();
	bool ImportMacroDefFile(bool oldVersion);
	void ImportGroups(CStdioFile& f);
	CString GetGroupName(int ind) const;
	const CString& GetResult() const { return result_; }
	void ApplyMacro() const;
	void InserCadrNums(const CString& NumSym, int Num);
	bool WriteXML(class TiXmlDocument* doc) const;
	bool ReadXML(class TiXmlDocument* doc);
	bool SaveXML(const CString& path) const;
	bool SaveXML() const;
	bool LoadXML(const CString& path);
	bool LoadXML();
	const CString& GetFileName() const { return fileName_; }
protected:
	CString result_;
	CString fileName_;
	std::vector<class SMDefiniton> allMacros_;
	std::vector<class SMGroup> groups_;
	class SMDialog* dlg_;
	int cadrNumStep;
public:
	const CString currentPath_;
	const CString origPath_;
	const CString picturesPath_;
	const CString descriptionsPath_;
	const CString picturesCimPath_;
	const CString descriptionsCimPath_;
	CString fileExt_;
};

