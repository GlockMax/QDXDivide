#pragma once
#include <vector>

class SMParameter
{
	friend class SMDialog;
	friend class SMDefiniton;
	friend class SMProperty;
	friend class SMEdit;
public:
	typedef std::pair<CString, CString> LabelVal;
	typedef std::vector<LabelVal> ComboStrings;
public:
	SMParameter();
	~SMParameter();
	void Clear();
	bool Read(CStdioFile& f, int lastAttrInd);
	static bool ReadAttrBool(const CString& line, bool& val);
	static bool ReadAttrStr(const CString& line, CString& val);
	static bool ReadAttrDouble(const CString& line, double& val);
	static bool ReadAttrInt(const CString& line, int& val);
	bool ReadAttr(CStdioFile& f, int lastAttrInd);
	static bool ReadAttrV8(CStdioFile& f, std::vector<SMParameter>& params);
	void GetStrVal(CString& res) const;
	void SetStrVal(const class CMFCPropertyGridProperty* pProp);
	bool IsRequired() const { return required_; }
	bool IsMinMaxValid() const;
	bool IsMinMaxValid(const CString& val) const;
	void WriteXML(class TiXmlElement* parent) const;
	void ReadXML(class TiXmlElement* e_param);
	static void SetParameter(class TiXmlElement* parent, const CString& name, bool val);
	static void SetParameter(TiXmlElement* parent, const CString& name, int val);
	static void SetParameter(class TiXmlElement* parent, const CString& name, const CString& val);
	static const char* GetAttribute(class TiXmlElement* parent, const CString& name, bool& val);
	static const char* GetAttribute(class TiXmlElement* parent, const CString& name, CString& val);
	static const char* GetAttribute(class TiXmlElement* parent, const CString& name, int& val);
	static const char* GetParameter(class TiXmlElement* parent, const CString& name, bool& val);
	static const char* GetParameter(class TiXmlElement* parent, const CString& name, CString& val);
	static const char* GetParameter(class TiXmlElement* parent, const CString& name, int& val);
	bool IsEvaluated() const { return evaluated_; }
	void SetEvaluated(bool val) { evaluated_ = val; }
	bool GetIsFunc() const { return isFunc_; }

protected:
	// attributes of parameter
	bool allowAlpha_;// done
	bool allowNeg_;// done
	CString comboBoxCFG_;// done
	bool comboBoxEnabled_;// done
	int decimals_;// done
	CString default_;// done
	CString desc_;// done
	int digits_;// done
	int inputorder_;
	bool isFunc_; // done
	CString lastVal_;// done partial
	CString max_;// done
	CString min_;// done
	bool noEmptyLine_;
	CString paramPicture_;// done
	CString prefix_;// done
	bool remember_;// done
	bool required_;// done
	CString suffix_;// done
	bool upperCase_;// done
	bool useSign_;// done
	CString help_;// done
protected:
	CString value_;
	bool evaluated_;// for isFunc only
	bool attrsFilled_;// for import only
};

