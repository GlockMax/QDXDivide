#include "pch.h"
#include "SMProperty.h"
#include "SMParameter.h"
#include <tinyxml.h>

SMParameter::SMParameter()
{
	Clear();
}

SMParameter::~SMParameter()
{
}

void SMParameter::Clear()
{
	allowAlpha_ = false;
	allowNeg_ = false;
	std::vector<CString> comboBoxCFG_;
	comboBoxEnabled_ = false;
	decimals_ = 0;
	default_.Empty();
	desc_.Empty();
	digits_ = 0;
	inputorder_ = 0;
	isFunc_ = false;
	lastVal_.Empty();
	max_.Empty();
	min_.Empty();
	noEmptyLine_ = false;
	paramPicture_.Empty();
	prefix_.Empty();
	remember_ = false;
	required_ = false;
	suffix_.Empty();
	upperCase_ = false;
	useSign_ = false;

	evaluated_ = false;
	attrsFilled_ = false;
}

bool SMParameter::Read(CStdioFile& f, int lastAttrInd)
{
	// read all atributes for 1 parameter
	bool firstAttr = true;
	bool endParam = false;
	while (!endParam)
		endParam = !ReadAttr(f, lastAttrInd);
	CString line;
	if (!f.ReadString(line))
		return false;
	CString leadDig = line.SpanIncluding(_T("0123456789"));
	f.Seek(-line.GetLength() - 2, CFile::current);
	if (leadDig.GetLength() != 3)
		return false;
	return true;
}

bool SMParameter::ReadAttrBool(const CString& line, bool& val)
{
	if (line == _T("Yes"))
		val = true;
	else
		val = false;
	return true;
}

bool SMParameter::ReadAttrStr(const CString& line, CString& val)
{
	val = line;
	return true;
}

bool SMParameter::ReadAttrDouble(const CString& line, double& val)
{
	if (line.IsEmpty())
		val = 0.;
	else
		val = atof(line);
	return true;
}

bool SMParameter::ReadAttrInt(const CString& line, int& val)
{
	if (line.IsEmpty())
		val = 0;
	else
		val = atoi(line);
	return true;
}

bool SMParameter::ReadAttr(CStdioFile& f, int lastAttrInd)
{
	static const std::vector<CString> attrNames
		= { _T("ALLOWALPHA"), _T("ALLOWNEG"), _T("COMBOBOXCFG"), _T("COMBOBOXENABLED"), _T("DECIMALS")
		, _T("DEFAULT"), _T("DESC"), _T("DIGITS"), _T("INPUTORDER"), _T("ISFUNC")
		, _T("LASTVAL"), _T("MAX"), _T("MIN"), _T("NOEMPTYLINE"), _T("PARAMPICTURE") 
		, _T("PREFIX"), _T("REMEBER"), _T("REQUIRED"), _T("SUFFIX"), _T("UPPERCASE")
		, _T("USESIGN") };

	CString line;
	if (!f.ReadString(line))
		return false;
	CString attrName = line.SpanExcluding(_T("="));
	CString strInd = attrName.SpanIncluding(_T("0123456789"));
	int index = atoi(attrName.Left(3));
	if (index != lastAttrInd + 1 || strInd.GetLength() != 3)
	{
		f.Seek(-line.GetLength() - 2, CFile::current);
		return false;
	}
	line.Delete(0, attrName.GetLength() + 2);// +2 - "= "
	attrName.Delete(0, 3);// 3 leading digits
	attrName.Remove(' ');
	attrName.Remove('\t');
	int i = 0;
	for (; i < attrNames.size(); ++i)
	{
		if (attrName == attrNames.at(i))
			break;
	}
	if (i == attrNames.size())
		return false;

	switch (i)
	{
	case 0:
		SMParameter::ReadAttrBool(line, allowAlpha_);
		attrsFilled_ = true;
		break;
	case 1:
		SMParameter::ReadAttrBool(line, allowNeg_);
		attrsFilled_ = true;
		break;
	case 2:
		SMParameter::ReadAttrStr(line, comboBoxCFG_);
		attrsFilled_ = true;
		break;
	case 3:
		SMParameter::ReadAttrBool(line, comboBoxEnabled_);
		attrsFilled_ = true;
		break;
	case 4:
		SMParameter::ReadAttrInt(line, decimals_);
		attrsFilled_ = true;
		break;
	case 5:
		SMParameter::ReadAttrStr(line, default_);
		attrsFilled_ = true;
		break;
	case 6:
		SMParameter::ReadAttrStr(line, desc_);
		attrsFilled_ = true;
		break;
	case 7:
		SMParameter::ReadAttrInt(line, digits_);
		attrsFilled_ = true;
		break;
	case 8:
		SMParameter::ReadAttrInt(line, inputorder_);
		attrsFilled_ = true;
		break;
	case 9:
		SMParameter::ReadAttrBool(line, isFunc_);
		attrsFilled_ = true;
		break;
	case 10:
		SMParameter::ReadAttrStr(line, lastVal_);
		attrsFilled_ = true;
		break;
	case 11:
		SMParameter::ReadAttrStr(line, max_);
		attrsFilled_ = true;
		break;
	case 12:
		SMParameter::ReadAttrStr(line, min_);
		attrsFilled_ = true;
		break;
	case 13:
		SMParameter::ReadAttrBool(line, noEmptyLine_);
		attrsFilled_ = true;
		break;
	case 14:
		SMParameter::ReadAttrStr(line, paramPicture_);
		attrsFilled_ = true;
		break;
	case 15:
		SMParameter::ReadAttrStr(line, prefix_);
		attrsFilled_ = true;
		break;
	case 16:
		SMParameter::ReadAttrBool(line, remember_);
		attrsFilled_ = true;
		break;
	case 17:
		SMParameter::ReadAttrBool(line, required_);
		attrsFilled_ = true;
		break;
	case 18:
		SMParameter::ReadAttrStr(line, suffix_);
		attrsFilled_ = true;
		break;
	case 19:
		SMParameter::ReadAttrBool(line, upperCase_);
		attrsFilled_ = true;
		break;
	case 20:
		SMParameter::ReadAttrBool(line, useSign_);
		attrsFilled_ = true;
		break;
	default:
		break;
	}
	return true;
}

bool SMParameter::ReadAttrV8(CStdioFile& f, std::vector<SMParameter>& params)
{
	// returns index of parameter containing this attribute or -1
	static const std::vector<CString> attrNames
		= { _T("ALLOWALPHA"), _T("ALLOWNEG"), _T("COMBOBOXCFG"), _T("COMBOBOXENABLED"), _T("DECIMALS")
		, _T("DEFAULT"), _T("DESC"), _T("DIGITS"), _T("INPUTORDER"), _T("ISFUNC")
		, _T("LASTVAL"), _T("MAX"), _T("MIN"), _T("NOEMPTYLINE"), _T("PARAMPICTURE")
		, _T("PREFIX"), _T("REMEBER"), _T("REQUIRED"), _T("SUFFIX"), _T("UPPERCASE")
		, _T("USESIGN") };

	CString line;
	if (!f.ReadString(line))
		return false;
	CString attrName = line.SpanExcluding(_T("="));
	CString strInd = attrName.SpanIncluding(_T("0123456789"));
	if (strInd.GetLength() != 3)
	{
		f.Seek(-line.GetLength() - 2, CFile::current);
		return false;
	}
	line.Delete(0, attrName.GetLength() + 2);// +2 - "= "
	attrName.Delete(0, 3);// 3 leading digits
	attrName.Remove(' ');
	attrName.Remove('\t');
	int i = 0;
	for (; i < attrNames.size(); ++i)
	{
		if (attrName == attrNames.at(i))
			break;
	}
	if (i == attrNames.size())
		return false;

	int index = atoi(strInd);
	if (index < 0 || index >= params.size())
		return false;
	auto& curParam = params.at(index);

	switch (i)
	{
	case 0:
		SMParameter::ReadAttrBool(line, curParam.allowAlpha_);
		break;
	case 1:
		SMParameter::ReadAttrBool(line, curParam.allowNeg_);
		break;
	case 2:
		SMParameter::ReadAttrStr(line, curParam.comboBoxCFG_);
		break;
	case 3:
		SMParameter::ReadAttrBool(line, curParam.comboBoxEnabled_);
		break;
	case 4:
		SMParameter::ReadAttrInt(line, curParam.decimals_);
		break;
	case 5:
		SMParameter::ReadAttrStr(line, curParam.default_);
		break;
	case 6:
		SMParameter::ReadAttrStr(line, curParam.desc_);
		break;
	case 7:
		SMParameter::ReadAttrInt(line, curParam.digits_);
		break;
	case 8:
		SMParameter::ReadAttrInt(line, curParam.inputorder_);
		break;
	case 9:
		SMParameter::ReadAttrBool(line, curParam.isFunc_);
		break;
	case 10:
		SMParameter::ReadAttrStr(line, curParam.lastVal_);
		break;
	case 11:
		SMParameter::ReadAttrStr(line, curParam.max_);
		break;
	case 12:
		SMParameter::ReadAttrStr(line, curParam.min_);
		break;
	case 13:
		SMParameter::ReadAttrBool(line, curParam.noEmptyLine_);
		break;
	case 14:
		SMParameter::ReadAttrStr(line, curParam.paramPicture_);
		break;
	case 15:
		SMParameter::ReadAttrStr(line, curParam.prefix_);
		break;
	case 16:
		SMParameter::ReadAttrBool(line, curParam.remember_);
		break;
	case 17:
		SMParameter::ReadAttrBool(line, curParam.required_);
		break;
	case 18:
		SMParameter::ReadAttrStr(line, curParam.suffix_);
		break;
	case 19:
		SMParameter::ReadAttrBool(line, curParam.upperCase_);
		break;
	case 20:
		SMParameter::ReadAttrBool(line, curParam.useSign_);
		break;
	default:
		break;
	}
	return true;
}

void SMParameter::GetStrVal(CString& res) const
{
	res = value_;
}

void SMParameter::SetStrVal(const CMFCPropertyGridProperty* pProp)
{
	const SMProperty* pSMProp = dynamic_cast<const SMProperty*>(pProp);
	if (pSMProp == nullptr)
		return;
	if (comboBoxEnabled_)
	{
		value_.Empty();
		ComboStrings strs;
		pSMProp->ParseComboBoxCFG(strs);
		for each (auto el in strs)
		{
			if (el.first == pProp->GetValue().bstrVal)
			{
				value_ = el.second;
				break;
			}
		}
	}
	else
		value_ = pProp->GetValue().bstrVal;
	if (remember_)
		lastVal_ = value_;
}

bool SMParameter::IsMinMaxValid() const
{
	return IsMinMaxValid(value_);
}

bool SMParameter::IsMinMaxValid(const CString& val) const
{
	if (allowAlpha_ || comboBoxEnabled_)
		return true;
	if (val.IsEmpty())
		return true;
	if (decimals_ == 0)
	{
		if (!min_.IsEmpty() && atoi(val) < atoi(min_))
			return false;
		if (!max_.IsEmpty() && atoi(val) > atoi(max_))
			return false;
	}
	else
	{
		if (!min_.IsEmpty() && atof(val) < atof(min_))
			return false;
		if (!max_.IsEmpty() && atof(val) > atof(max_))
			return false;
	}
	return true;
}

void SMParameter::WriteXML(TiXmlElement* parent) const
{
	TiXmlElement* e_param = new TiXmlElement("parameter");

	SetParameter(e_param, _T("allowAlpha"), allowAlpha_);
	SetParameter(e_param, _T("allowNeg"), allowNeg_);
	SetParameter(e_param, _T("comboBoxEnabled"), comboBoxEnabled_);
	SetParameter(e_param, _T("isFunc"), isFunc_);
	SetParameter(e_param, _T("noEmptyLine"), noEmptyLine_);
	SetParameter(e_param, _T("remember"), remember_);
	SetParameter(e_param, _T("required"), required_);
	SetParameter(e_param, _T("upperCase"), upperCase_);
	SetParameter(e_param, _T("useSign"), useSign_);

	SetParameter(e_param, _T("comboBoxCFG"), comboBoxCFG_);
	SetParameter(e_param, _T("decimals"), decimals_);
	SetParameter(e_param, _T("default"), default_);
	SetParameter(e_param, _T("desc"), desc_);
	SetParameter(e_param, _T("digits"), digits_);
	SetParameter(e_param, _T("inputorder"), inputorder_);
	SetParameter(e_param, _T("lastVal"), lastVal_);
	SetParameter(e_param, _T("max"), max_);
	SetParameter(e_param, _T("min"), min_);
	SetParameter(e_param, _T("paramPicture"), paramPicture_);
	SetParameter(e_param, _T("prefix"), prefix_);
	SetParameter(e_param, _T("suffix"), suffix_);
	SetParameter(e_param, _T("help"), help_);

	parent->LinkEndChild(e_param);
}

void SMParameter::ReadXML(TiXmlElement* e_param)
{
	GetParameter(e_param, _T("allowAlpha"), allowAlpha_);
	GetParameter(e_param, _T("allowNeg"), allowNeg_);
	GetParameter(e_param, _T("comboBoxEnabled"), comboBoxEnabled_);
	GetParameter(e_param, _T("isFunc"), isFunc_);
	GetParameter(e_param, _T("noEmptyLine"), noEmptyLine_);
	GetParameter(e_param, _T("remember"), remember_);
	GetParameter(e_param, _T("required"), required_);
	GetParameter(e_param, _T("upperCase"), upperCase_);
	GetParameter(e_param, _T("useSign"), useSign_);

	GetParameter(e_param, _T("comboBoxCFG"), comboBoxCFG_);
	GetParameter(e_param, _T("decimals"), decimals_);
	GetParameter(e_param, _T("default"), default_);
	GetParameter(e_param, _T("desc"), desc_);
	GetParameter(e_param, _T("digits"), digits_);
	GetParameter(e_param, _T("inputorder"), inputorder_);
	GetParameter(e_param, _T("lastVal"), lastVal_);
	GetParameter(e_param, _T("max"), max_);
	GetParameter(e_param, _T("min"), min_);
	GetParameter(e_param, _T("paramPicture"), paramPicture_);
	GetParameter(e_param, ("prefix"), prefix_);
	GetParameter(e_param, _T("suffix"), suffix_);
	GetParameter(e_param, _T("help"), help_);
}

void SMParameter::SetParameter(TiXmlElement* parent, const CString& name, bool val)
{
	SetParameter(parent, name, val ? CString("yes") : CString("no"));
}

void SMParameter::SetParameter(TiXmlElement* parent, const CString& name, int val)
{
	CString sval;
	sval.Format(_T("%d"), val);
	SetParameter(parent, name, sval);
}

void SMParameter::SetParameter(TiXmlElement* parent, const CString& name, const CString& val)
{
	TiXmlElement* e_par = new TiXmlElement(name);
	TiXmlText* text = new TiXmlText(val);
	e_par->LinkEndChild(text);
	parent->LinkEndChild(e_par);
}

const char* SMParameter::GetAttribute(TiXmlElement* parent, const CString& name, bool& val)
{
	const char* res = parent->Attribute(name);
	if(res != nullptr)
		val = (CString(res) == _T("yes"));
	return res;
}

const char* SMParameter::GetAttribute(TiXmlElement* parent, const CString& name, CString& val)
{
	const char* res = parent->Attribute(name);
	if (res != nullptr)
		val = CString(res);
	return res;
}

const char* SMParameter::GetAttribute(TiXmlElement* parent, const CString& name, int& val)
{
	const char* res = parent->Attribute(name, &val);
	return res;
}

const char* SMParameter::GetParameter(TiXmlElement* parent, const CString& name, bool& val)
{
	CString str;
	const char* res = GetParameter(parent, name, str);
	val = (CString(res) == _T("yes"));
	return res;
}

const char* SMParameter::GetParameter(TiXmlElement* parent, const CString& name, CString& val)
{
	auto par = parent->FirstChildElement(name);
	if(par == nullptr)
		return nullptr;
	const char* res = par->GetText();
	if (res != nullptr)
		val = CString(res);
	return res;
}

const char* SMParameter::GetParameter(TiXmlElement* parent, const CString& name, int& val)
{
	CString str;
	const char* res = GetParameter(parent, name, str);
	val = atoi(str);
	return res;
}
