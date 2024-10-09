#pragma once
#include <vector>
#include "SMParameter.h"

class SMDefiniton
{
	friend class SMDialog;
	friend class SMPropGridCtrl;
public:
	SMDefiniton();
	~SMDefiniton();
	void Clear();
	bool Read(CStdioFile& f, bool oldVersion);
	bool ReadAttr(CStdioFile& f, bool oldVersion);
	bool ReadParameters(CStdioFile& f);
	bool ReadParametersV8(CStdioFile& f);
	bool ProcMacro(const CString& src, CString& res, bool valueOnly);
	void Apply(CString& res);
	CString GetSpecData(const CString& res, CString& keyWord);
	bool AllParametersDefined() const;
	void WriteXML(class TiXmlElement* parent) const;
	void ReadXML(class TiXmlElement* parent);
	void RecalcFunctions();
	bool haveFunctions() const;
	void ResetEvaluated();
	void SetStartValues();
	void SetDefaultValues();
	const CString& GetPicture() const { return picture_; }
	const CString& GetAttachment() const { return attachment_; }
protected:
	std::vector<SMParameter> params_;
	CString attachment_;
	int groupID_;
	bool hideFormulas_;
	CString name_;
	int paramCount_;
	CString picture_;
	CString text_;
protected:
	bool favourite_;
};

