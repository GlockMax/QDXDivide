#pragma once
#include <vector>
#include "tinyxml.h"
#include "resource.h"

using namespace std;

struct FullDescr
{
	CString picName;
	CString text;
};
/// ������������ ������
enum ParamType
{
	P_B,
	P_I,
	P_D,
	P_S,
	P_SE,
	P_EE,
	P_EMPTY
};

/// �������� ����� ����� �������� ������ �� ��������� �����
class Param
{
public:
	bool b;
	int i;
	double d;
	CString s;
	//vector <CString> e;

	Param(){type = P_EMPTY;};
	Param(bool a){b = a; type = P_B;};
	Param(int a){i = a; type = P_I;};
	Param(double a){d = a; type = P_D;};
	Param(CString a){s = a; type = P_S;};
	Param(CString a, bool strict){s = a; if(strict) type = P_SE; else type = P_EE;};
	ParamType GetType(){return type;};
	void SetType(ParamType newType){type = newType;};
	CString Show()
	{
		switch (type)
		{
		case P_B:
			if(b)
			{
				CString str; str.LoadString(IDS_BOOL_YES);
				return str;
			}//if
			else
			{
				CString str; str.LoadString(IDS_BOOL_NO);
				return str;
			}//else
			break;
		case P_I:
			{
				CString str;
				str.Format(_T("%i"),i);
				return str;
			}

			break;
		case P_D:
			{
				CString str;
				str.Format(_T("%lf"),d);
				return str;
			}
			break;
		case P_S:
			return s;
			break;
		case P_SE:
			return s;
			break;
		case P_EE:
			return s;
			break;
		case P_EMPTY:
			{
				CString str; str.LoadString(IDS_EMPTY);
				return str;
			}
			break;
		}//switch
		return CString(_T(""));
	};//CString Show()
protected:
	ParamType type;
};

class Def
{
public:
	CString defWord;
	Param param;
	Param defParam;
	CString group;
	bool presence;
	bool known;
	CString briefDescr;
	FullDescr fullDescr;
	vector <CString> enumerate;
};
class WRep
{
public:
	CString srcString;
	CString destString;
};

enum DefType
{
	D_COMMON,
	D_WREPL,
	D_EMPTY
};

class Definition
{
public:
	Def def;
	WRep wRep;
	
	Definition(){type = D_EMPTY;};
	Definition(Def & a){def = a; type = D_COMMON;};
	Definition(WRep & a){wRep = a; type = D_WREPL;};

	DefType GetType(){return type;};
	void SetType(DefType newType){type = newType;};
	bool operator >(const Definition &D)
	{
		if(type != D_COMMON)
			return false;
		if(def.defWord > D.def.defWord)
			return true;
		else return false;
	};
	bool operator <(const Definition &D)
	{
		if(type != D_COMMON)
			return false;
		if(def.defWord < D.def.defWord)
			return true;
		else return false;
	};
protected:
	DefType type;
};//class Definition

struct Group
{
	CString name;
	CString briefDescr;
	FullDescr fullDescr;
};//struct Group

/// ������ ������ ������� �� �����(� ����� ��� ������), ������ ����������� � ��������
struct Section
{
	CString name;
	vector <Definition> defList;
	CString briefDescr;
	FullDescr fullDescr;
	vector<Group> groupList;
};//struct Section

/// ������ ������������ ������
/**
\todo ������ ��������� ������, ������� �������� ��� ��������� ���������, �� ������� �� ������,
������������, �������� � ������� ���� �������� ������, ������������, �� ��������� � �����-���������, ��
�������������� � ����� �������� ������.
*/
class CNCUnitDef
{
public:
	CNCUnitDef(void);
	~CNCUnitDef(void);
	/// ������� ������ ��� ������������ �����-���������
	void Clear();
	/// ����� ������. ��������� ����������� � �������� �� ���������, ����� ������ presence, ��������
	/// ���� Word Replacement
	void Reset();
	/// �������� ��������� �� XML
	bool LoadXML(TiXmlDocument *doc);
	/// �������� ������������ �� ������, � ������� �������� ���������� �������� �����
	/**
	\todo ���������� �� ����� ��������� ������
	*/
	bool SetString(CString setString);
	/// ��������� ������, ������� ����� �������� � ���� ������������
	CString GetString();

	/// ���� wrepl �� ������, -1
	/// ���� ������, �� ����� wrepl � ������ ������ Word Replacement
	unsigned int FindWRepl(CString srcString, CString dstString);
	/// ����������� �������� Word Replacement
	bool MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString);

	bool GetGroup(CString sectName, CString groupName, Group &group);
private:
	/// ���������� ������ � �������� ������ (�������� ������ ���������� ��� �������������� � ������).
	/** ������ � �����������, �������������� � ���������, ���������� ��� �������������� � ������.
	���� ������ ��� ����������� � ��������� ���, �� ��� ���������. */	
	bool AddSection(Section section);
public:
	vector <Section> sectionList;
};

/**
data Section = Section { name :: String,
					     defList :: [Definition]
						 briefDescr :: String,
						 fullDescr :: FullDescr,
						 groupList :: [Group]}

data Group = Group { name :: String,
					 briefDescr :: String,
					 fullDescr :: FullDescr }

data Definition = Def { defword :: String,
                        param :: Param,
						defaultParam :: Param,
						group :: String,
						presence :: Bool,
						known :: Bool,
						briefDescr :: String,
						fullDescr :: FullDescr, 
						enumerate :: [String] }
				| WRep { srcString :: String,
				         dstString :: String }

data Param = B { b :: Bool }
           | I { i :: Int }
		   | D { d :: Double }
		   | S { s :: String }
		   | SE { e :: [String],
		          s :: String }
		   | EE { e :: [String],
		          s :: String }

data FullDescr = FullDescr { picName :: String,
							 text :: String }
*/
