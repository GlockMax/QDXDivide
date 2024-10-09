#pragma once
#include "afxtempl.h"
#include "NRepReport.h"
class NEvalExp;


class NRepReportsArray
{
public:
	NRepReportsArray(void);
	~NRepReportsArray(void);
protected:
	NEvalExp *Eval;
	CArray<NRepReport,NRepReport>RepArray;
public:
	bool Init(CString FPath);
	int FindReport(CString Name);
	NRepReport& operator[](int i);
	bool SaveReportsDefinition(CString FPath);
	void RemoveExtraReps(void);
	void RemoveAll(void);
	int GetSize(void);
	void RemoveAt(int i);
	void AddReport(NRepReport nrep);
	void Evaluate();
	void SetUnit(class NCUnit * NUnit);
};
