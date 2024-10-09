#include "pch.h"
#include "SMDefiniton.h"
#include <tinyxml.h>
#include <SMEExpression.h>

SMDefiniton::SMDefiniton()
{
	Clear();
}

SMDefiniton::~SMDefiniton()
{
}

void SMDefiniton::Clear()
{
	params_.clear();
	attachment_.Empty();
	groupID_ = 0;
	bool hideFormulas_ = false;
	name_.Empty();
	int paramCount_ = 0;
	picture_.Empty();
	text_.Empty();
	favourite_ = false;
}

bool SMDefiniton::Read(CStdioFile& f, bool oldVersion)
{
	// find [MACRO???]
	CString line;
	bool endFile = false;
	for (; !endFile;)
	{
		if (!(endFile = !f.ReadString(line)))
		{
			if (line.GetLength() != 10)
				continue;
			if (line[9] != ']')
				continue;
			if (!isdigit(line[8]) || !isdigit(line[7]) || !isdigit(line[6]))
				continue;
			if (line.Left(6) == _T("[MACRO"))
				break;
		}
	}
	if (endFile)
		return false;
	if (oldVersion)
	{
		bool endMacro = false;
		while (!endMacro)
		{
			endMacro = !ReadAttr(f, oldVersion);
		}
		ReadParametersV8(f);
	}
	else
	{
		ReadParameters(f);
		bool endMacro = false;
		while (!endMacro)
		{
			endMacro = !ReadAttr(f, oldVersion);
		}
	}

	return true;
}

bool SMDefiniton::ReadAttr(CStdioFile& f, bool oldVersion)
{
	static const std::vector<CString> attrNames
		= { _T("ATTACHMENT"), _T("GROUPID"), _T("HIDEFORMULAS"), _T("NAME"), _T("PARAMCOUNT"), _T("PICTURE"), _T("TEXT"), };
	CString line;
	if (!f.ReadString(line))
		return false;
	CString attrName = line.SpanExcluding(_T("="));
	line.Delete(0, static_cast<int>(attrName.GetLength() + 2));// +2 - "= "
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
		SMParameter::ReadAttrStr(line, attachment_);
		break;
	case 1:
		SMParameter::ReadAttrInt(line, groupID_);
		break;
	case 2:
		SMParameter::ReadAttrBool(line, hideFormulas_);
		break;
	case 3:
		SMParameter::ReadAttrStr(line, name_);
		break;
	case 4:
		SMParameter::ReadAttrInt(line, paramCount_);
		if (oldVersion)
			return false;
		break;
	case 5:
		SMParameter::ReadAttrStr(line, picture_);
		break;
	case 6:
		SMParameter::ReadAttrStr(line, text_);
		text_.Replace('ÿ', '\n');
		text_.Replace(_T("PI"), _T("pi"));
		if(!oldVersion)
			return false;
		break;
	default:
		return false;
	}
	return true;
}

bool SMDefiniton::ReadParameters(CStdioFile& f)
{
	int lastAttrInd = -1;
	// read parameters
	bool endParams = false;
	while (!endParams)
	{
		params_.emplace_back();
		endParams = !params_.back().Read(f, lastAttrInd);
		++lastAttrInd;
	}
	if (!params_.back().attrsFilled_)
		params_.pop_back();
	return true;
}

bool SMDefiniton::ReadParametersV8(CStdioFile& f)
{
	if (paramCount_ == 0)
		return true;
	// read parameters
	params_.resize(paramCount_);
	while (SMParameter::ReadAttrV8(f, params_));
	return true;
}

bool SMDefiniton::ProcMacro(const CString& src, CString& res, bool valueOnly)
{
	// returns false if one of functional parameters is not evaluated
	res = src;
	// replace parameters + date-time functions
	for (bool stop = false; !stop;)
	{
		int ind = -1;
		for (int i = 0; i < res.GetLength() - 1; ++i)
			if (res[i] == '$' || res[i] == '@')
			{
				ind = i;
				break;
			}
		if (ind >= 0)
		{
			bool formula = (res[ind] == '@');
			CString num = res.Mid(ind + 1).SpanIncluding("0123456789");
			int parInd = atoi(num) - 1;// one based index
			if (parInd >= int(params_.size()))// int() prevents looping in case when parInd = -1 and it converts to 0xFFFFFFFF
				continue;
			int formLen = 0;
			CString val;
			if (parInd < 0)
			{
				if (!formula)// special data
					val = GetSpecData(res.Mid(ind + 1), num);
			}
			else
			{
				if (formula)
				{// remove {...}
					int s = ind + num.GetLength();
					int e = s - 1;
					if (res[s] == '{')
					{
						e = res.Find('}', s);
						if (e >= 0)
							formLen = e - s + 1;
					}
				}
				const SMParameter& par = params_.at(parInd);
				if (par.isFunc_ && !par.IsEvaluated())
				{
					res.Empty();
					return false;
				}
				val = par.value_;
				if (!val.IsEmpty() && !valueOnly)
					val = par.prefix_ + val + par.suffix_;
			}
			res = res.Left(ind) + val + res.Mid(ind + 1 + num.GetLength() + formLen);
		}
		else
			stop = true;
	}
	return true;
}

void SMDefiniton::Apply(CString& res)
{
	ProcMacro(text_, res, false);// false means using prefix and suffix
}

CString ExtractFunc(int ind, const CString& str)
{
	int posF = -1;
	for (int i = 0; i < str.GetLength(); ++i)
	{
		posF = str.Find('@', posF + 1);
		if (posF < 0)
			return CString();
		CString num = str.Mid(posF + 1).SpanIncluding("0123456789");
		posF += num.GetLength() + 1;
		if (posF + 2 >= str.GetLength())
			return CString();
		if (atoi(num) == ind + 1)// 1 based index
		{
			if (str[posF] != '{')
				continue;
			++posF;// first position inside {}
			int posE = str.Find('}', posF);
			if (posE < 0)
				return CString();
			return str.Mid(posF, posE - posF);
		}
	}
	return CString();
}

CString SMDefiniton::GetSpecData(const CString& res, CString& keyWord)
{
	static const std::vector<CString> names = { _T("DATETIME"), _T("DATE"), _T("TIME")};// must be sorted by decreasing of string length
	int dataInd = 0;
	for (; dataInd < names.size(); ++dataInd)
	{
		if (res.Left(names.at(dataInd).GetLength()) == names.at(dataInd))
		{
			keyWord = names.at(dataInd);
			break;
		}
	}
	switch (dataInd)
	{
	case 0:// DATETIME
	{
		COleDateTime time = COleDateTime::GetCurrentTime();
		return time.Format(_T("%d.%m.%g %H:%M"));
	}
	case 1:// DATE
	{
		COleDateTime time = COleDateTime::GetCurrentTime();
		return time.Format(_T("%d.%m.%g"));
	}
	case 2:// TIME
	{
		COleDateTime time = COleDateTime::GetCurrentTime();
		return time.Format(_T("%H:%M"));
	}
	default:
		return CString();
	}
	if (dataInd < names.size())
		return names.at(dataInd);
	return CString();
}

bool SMDefiniton::AllParametersDefined() const
{
	for each (const auto & el in params_)
	{
		if (el.required_ && el.value_.IsEmpty())
			return false;
	}
	return true;
}

void SMDefiniton::WriteXML(TiXmlElement* parent) const
{
		if (parent == nullptr)
			return;

		TiXmlElement* e_macro = new TiXmlElement("macro");
		SMParameter::SetParameter(e_macro, _T("attachment"), attachment_);
		SMParameter::SetParameter(e_macro, _T("name"), name_);
		SMParameter::SetParameter(e_macro, _T("paramCount"), paramCount_);
		SMParameter::SetParameter(e_macro, _T("groupID"), groupID_);
		SMParameter::SetParameter(e_macro, _T("hideFormulas"), hideFormulas_);
		SMParameter::SetParameter(e_macro, _T("picture"), picture_);
		SMParameter::SetParameter(e_macro, _T("text_"), text_);
		SMParameter::SetParameter(e_macro, _T("favourite"), favourite_);

		for each (auto param in params_)
			param.WriteXML(e_macro);

		parent->LinkEndChild(e_macro);
}

void SMDefiniton::ReadXML(TiXmlElement* parent)
{
	SMParameter::GetParameter(parent, _T("attachment"), attachment_);
	SMParameter::GetParameter(parent, _T("name"), name_);
	SMParameter::GetParameter(parent, _T("paramCount"), paramCount_);
	SMParameter::GetParameter(parent, _T("groupID"), groupID_);
	SMParameter::GetParameter(parent, _T("hideFormulas"), hideFormulas_);
	SMParameter::GetParameter(parent, _T("picture"), picture_);
	SMParameter::GetParameter(parent, _T("text_"), text_);
	SMParameter::GetParameter(parent, _T("favourite"), favourite_);

	params_.clear();
	for (TiXmlElement* p_el = parent->FirstChildElement(_T("parameter")); p_el != nullptr; p_el = p_el->NextSiblingElement())
	{
		params_.emplace_back().ReadXML(p_el);
	}
}

void SMDefiniton::RecalcFunctions()
{
	if (!haveFunctions())
		return;
	ResetEvaluated();
	bool finish = false;
	for (int k = 0; !finish && k < 16; ++k)// k is for looping prevention only
	{
		finish = true;
		for (int i = 0; i < params_.size(); ++i)
		{
			SMParameter& par = params_.at(i);
			if (par.isFunc_ && !par.IsEvaluated())
			{
				CString source = ExtractFunc(i, text_);
//				if (!source.IsEmpty())
				{
					CString exprText;
					bool needFunc = !ProcMacro(source, exprText, true);// true means using value only
					if (needFunc)
						finish = false;// there is unevaluated function in the expr
					else
					{
						par.value_.Empty();
						SMEExpression expr;
						if (expr.Define(std::string(exprText)))
						{
							double dval = expr.CalcValue();
							par.value_.Format(_T("%lf"), dval);
							par.SetEvaluated(true);
						}
					}
				}
			}
		}
	}
}

bool SMDefiniton::haveFunctions() const
{
	for each (auto& par in params_)
		if (par.isFunc_)
			return true;
	return false;
}

void SMDefiniton::ResetEvaluated()
{
	for (int i = 0; i < params_.size(); ++i)
	{
		auto& par = params_.at(i);
		if (par.isFunc_)
			par.SetEvaluated(false);
	}
}

void SMDefiniton::SetStartValues()
{
	for (int i = 0; i < params_.size(); ++i)
	{
		auto& par = params_[i];
		par.value_ = par.lastVal_.IsEmpty() ? par.default_ : par.lastVal_;
	}
}

void SMDefiniton::SetDefaultValues()
{
	for (int i = 0; i < params_.size(); ++i)
	{
		auto& par = params_[i];
		par.value_ = par.default_;
	}
}
