#pragma once

class NRepReport
{
public:
	NRepReport(void);
	~NRepReport(void);
protected:
	CString m_Condition;
	CString m_Description;
public:
	bool Init(CString def, const CString& FPath);
protected:
	CString m_Name;
	bool m_Init;
	CStringArray Tabs;
	CString m_Path;
	int m_Mode;//0 - неактивен 1 - активен 2 - пауза
public:
	bool SetMode(int m);
	int GetMode(void);
	CString GetCondition(void);
	CString GetName(void);
	CString GetPath(void);
	void GetTabs(CStringArray& StrArr);
	NRepReport(NRepReport& rep);
	NRepReport& operator=(const NRepReport& rep);
	CString GetDescription(void);
	void SetName(const CString& Name);
	void SetCondition(const CString& Cond);
	void SetDescription(const CString& Descr);
	void SetTabs(CStringArray& str);
	void AddNote(class NEvalExp* Eval);
};
