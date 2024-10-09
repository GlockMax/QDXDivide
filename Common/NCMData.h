// NCMData.h: interface for the NCMData class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "InfoStock.h"	// Added by ClassView
class AFX_EXT_CLASS NCMData 
{
	class NPtrArray : public CPtrArray 
	{
	public:
		void CopyAndAdd(const void *p,const int size);
		void Serialize(CArchive &ar);
		NPtrArray();
		virtual ~NPtrArray();

	};

public:
	CStringA UnitFile;
	CStringA UnitName;
	CStringA UnitType;
	CStringA UnitManufacturer;
	CStringA UnitOperation;
	CStringA UnitAxes;
	InfoStock Stock;
	NPtrArray ProgArray;
	NPtrArray ToolArray;
public:
	void Serialize(CArchive& ar);
	NCMData();
	virtual ~NCMData();
	bool LoadNCMFile(const CStringA& fname);
	bool GetText(int FormatIDAll, CStringA &TextAll, int FormatIDProgs, CStringA &TextProgs,int FormatIDTools, CStringA &TextTools);
	static const CStringA GetTimeFromDouble(double Val);

protected:
};
