// NControlPanel.h: interface for the NStateElem class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class NStateNote;

class NStateElem 
{
public:
	NStateElem();
	virtual CString GetNoteString();
	virtual CString GetValueString() = 0;
	virtual CString GetValue4Time() { return "";}
	NStateElem(NStateNote* pN);
	virtual ~NStateElem();
protected:
	NStateNote* pNote;
};

class NStateNote  
{
public:
	virtual CString GetString() = 0;
	NStateNote();
	virtual ~NStateNote();

};


class NStateDouble : public NStateElem
{
public:
	NStateDouble(double* pD, NStateNote* pN);
	CString GetValueString() override;
	virtual ~NStateDouble();

protected:
	double* pValue;
};

class NStateAddCoord : public NStateDouble
{
public:
	NStateAddCoord(const class SMachState &Coords, int Ind, NStateNote* pN);
	CString GetValueString() override;
	virtual ~NStateAddCoord();

protected:
	const class SMachState &Coords;
	int Ind;
};


class NStateBoolNote : public NStateNote  
{
public:
	CString GetString() override;
	NStateBoolNote(BOOL* pB, UINT idTrueString, UINT idFalseString);
	virtual ~NStateBoolNote();
protected:
	CString FalseString;
	CString TrueString;
	BOOL* pValue;
};


class NStateNoNote : public NStateNote  
{
public:
	CString GetString() override;
	NStateNoNote();
	virtual ~NStateNoNote();

};


class NStateString : public NStateElem  
{
public:
	CString GetValueString() override;
	NStateString(const CString &S, NStateNote* pN);
	virtual ~NStateString();
protected:
	const CString &String;
};


class NStateInt : public NStateElem  
{
public:
	CString GetValueString() override;
	NStateInt(int* pInt, NStateNote* pN);
	virtual ~NStateInt();

protected:
	int* pValue;
};


class NStateTime : public NStateElem  
{
public:
	CString GetValueString() override;
	CString GetValue4Time() override;
	NStateTime(double* pD, NStateNote* pN);
	virtual ~NStateTime();

protected:
	double* pValue;
};


class NStateBool : public NStateElem
{
public:
	CString GetNoteString();
	CString GetValueString() override;
	NStateBool(BOOL* pB, UINT idTrueString, UINT idFalseString, NStateNote* pN);
	virtual ~NStateBool();

protected:
	CString FalseString;
	CString TrueString;
	BOOL* pValue;
};

class NStatebool : public NStateElem
{
public:
	CString GetNoteString();
	virtual CString GetValueString();
	NStatebool(bool* pB, UINT idTrueString, UINT idFalseString, NStateNote* pN);
	virtual ~NStatebool();

protected:
	CString FalseString;
	CString TrueString;
	bool* pValue;
};


class NStateCodeNote : public NStateNote  
{
public:
	CString GetString() override;
	NStateCodeNote(CString WKey, CString FKey);
	virtual ~NStateCodeNote();

protected:
	CString FuncKey;
	CString WordKey;
};


class NStateEnumNote : public NStateNote  
{
public:
	CString GetString() override;
	NStateEnumNote(UINT* pV);
	virtual ~NStateEnumNote();

protected:
	UINT* pValue;
};


class NStatePercent : public NStateElem  
{
public:
	CString GetValueString() override;
	NStatePercent(double* pD, NStateNote* pN);
	virtual ~NStatePercent();

protected:
	double* pValue;
};


class NStatePercentNote : public NStateNote  
{
public:
	CString GetString() override;
	NStatePercentNote(double* pV, double* pP);
	virtual ~NStatePercentNote();

protected:
	double* pPercent;
	double* pValue;
};


class NStateEnum : public NStateElem
{
public:
	CString MakeNoteString(CString &buf) const;
	CString GetNoteString() override;
	CString GetValueString() override;
	NStateEnum(UINT* pV, NStateNote* pN);
	virtual ~NStateEnum();

protected:
	UINT* pValue;
};

class NStateEnumUCS : public NStateEnum
{
public:
	CString GetNoteString() override;
	CString GetValueString() override;
	NStateEnumUCS(UINT* pV, NStateNote* pN);
	virtual ~NStateEnumUCS();
};

class NStateStrNote : public NStateNote
{
public:
	CString GetString() override;
	NStateStrNote(const CString &Str);
	NStateStrNote(UINT idString);
	virtual ~NStateStrNote();
protected:
	CString String;
};
