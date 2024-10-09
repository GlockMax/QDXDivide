#pragma once
//#include "stdafx.h"
#include "string.h"
#include "NCMB_API.h"
#include "tinyxml.h"
typedef CMap<CStringA, LPCSTR, int, int> CMapStringToInt;
typedef CTypedPtrMap<CMapStringToPtr, CString, int*> CMapStringToIntPtr;
typedef CTypedPtrMap<CMapStringToOb, CString, class NColor*> CMapStringToNColor;
typedef CTypedPtrMap<CMapStringToPtr, CString, double*> CMapStringToDoublePtr;
typedef CTypedPtrMap<CMapStringToOb, CString, class NFont*> CMapStringToNFont;
typedef CMap<CStringA, LPCSTR, LOGFONT, LOGFONT> CMapStringToLOGFONT;


class NCMB_API NTiParams 
	
{
public:
	virtual double GetDouble(const char * Key, double Default) = 0;
	virtual const char * GetString(const char * Key, const char * Default) = 0;
	virtual class NColor GetColor(const char* Key, const NColor& Default) = 0;
	virtual LOGFONT GetFont(const char* Key, const LOGFONT& Default) = 0;
	virtual class BPoint GetVector(const char* Key, const BPoint& Default) = 0;
	virtual void SetDouble(const char* Key, double Val) = 0;
	virtual void SetInt(const char* Key, int Val) = 0;
	virtual void SetString(const char* Key, const char* Val) = 0;
	virtual void SetColor(const char* Key, const class NColor& Col) = 0;
	virtual void SetFont(const char* Name, const LOGFONT& Font) = 0;
	virtual void SetVector(const char* Key, const class BPoint& Vector) = 0;
	virtual bool GetBool(const char * Key, bool Default) = 0;
	virtual bool LoadFile(const char *Name) = 0;
	virtual bool SaveFile(const char* Name) const = 0;
	virtual void Clear() = 0;
	virtual void GetMiskDouble(const char* Key, CMapStringToDoublePtr& Doubles) = 0;
	virtual void GetMiskInt(const char* Key, CMapStringToIntPtr& Ints) = 0;
	virtual void GetMiskColors(const char* Key, CMapStringToNColor& Colors) = 0;
	virtual void AddElement(const char* Key, const char* Name) = 0;
};


class NTiParamsInt :
	public NTiParams, TiXmlDocument
{
public:
	NTiParamsInt(void);
	virtual ~NTiParamsInt(void) override;
	void Clear() { delete this; }
	double GetDouble( const char * Key, double Default) override;
	const char * GetString( const char * Key, const char * Default) override;
	class NColor GetColor(const char* Key, const NColor& Default) override;
	LOGFONT GetFont(const char* Key, const LOGFONT& Default) override;
	class BPoint GetVector(const char* Key, const BPoint& Default) override;
	bool GetBool( const char * Key, bool Default) override;
	bool LoadFile(const char* Name) override;
	bool SaveFile(const char* Name) const override;
	void SetColor(const char* Name, const class NColor& Col) override;
	void SetFont(const char* Name, const LOGFONT& Font) override;
	void SetVector(const char* Key, const class BPoint& Vector) override;
	void SetDouble(const char* Key, double Val) override;
	void SetInt(const char* Key, int Val) override;
	void SetString(const char* Key, const char* Val) override;
	void GetMiskDouble(const char* Key, CMapStringToDoublePtr& Doubles) override;
	void GetMiskInt(const char* Key, CMapStringToIntPtr& Ints) override;
	void GetMiskColors(const char* Key, CMapStringToNColor& Colors) override;
	void AddElement(const char* Key, const char* Name) override;
protected:
	TiXmlElement * GetElement(const char * Key, size_t Length);
};

extern "C" NCMB_API NTiParams* __stdcall GenNTiParams();