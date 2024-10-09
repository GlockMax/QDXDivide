#pragma once
class SMGroup
{
public:
	SMGroup();
	~SMGroup();
	void SetAuto(int k);
	void Set(int k, const CString& name);
	const CString& GetName() const;
	void WriteXML(class TiXmlElement* parent) const;
	void ReadXML(class TiXmlElement* parent);
protected:
	CString name_;
	int index_;
};

