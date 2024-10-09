#include "pch.h"
#include <tinyxml.h>
#include "SMParameter.h"
#include "SMGroup.h"

SMGroup::SMGroup()
{
	index_ = 0;
}

SMGroup::~SMGroup()
{
}

void SMGroup::SetAuto(int k)
{
	index_ = k;
	name_.Format(_T("Group %d"), k);
}

void SMGroup::Set(int k, const CString& name)
{
	index_ = k;
	name_ = name;
}

const CString& SMGroup::GetName() const
{
	return name_;
}

void SMGroup::WriteXML(TiXmlElement* parent) const
{
	if (parent == nullptr)
		return;

	TiXmlElement* e_gr = new TiXmlElement("group");
	SMParameter::SetParameter(e_gr, _T("name"), name_);
	SMParameter::SetParameter(e_gr, _T("index"), index_);

	parent->LinkEndChild(e_gr);
}

void SMGroup::ReadXML(TiXmlElement* parent)
{
	SMParameter::GetParameter(parent, _T("name"), name_);
	SMParameter::GetParameter(parent, _T("index"), index_);
}
