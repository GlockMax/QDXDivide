// ServerData.h: Definition of the ServerData class
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"       // main symbols
#include "NCMData.h"
/////////////////////////////////////////////////////////////////////////////
// ServerData

class ServerData : 
	public IServerData,
	public CComObjectRoot,
	public CComCoClass<ServerData,&CLSID_ServerData>
{
public:
	ServerData()
	{
		pNData = NULL;
	}
BEGIN_COM_MAP(ServerData)
	COM_INTERFACE_ENTRY(IServerData)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(ServerData) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_ServerData)

// IServerData
public:
	STDMETHOD(GetToolPos)(long ToolNum, long *pNum);
	STDMETHOD(GetToolVolume)(long ToolName, double *pVal);
	STDMETHOD(GetToolType)(long ToolNum, BSTR *pText);
	STDMETHOD(GetToolName)(long ToolNum, BSTR *pText);
	STDMETHOD(GetToolTime)(long ToolNum, double *pVal);
	STDMETHOD(GetToolFLength)(long ToolNum, double *pVal);
	STDMETHOD(GetToolWLength)(long ToolNum, double *pVal);
	STDMETHOD(GetStockBase)(long *pNum);
	STDMETHOD(GetStockX)(double *pCoord);
	STDMETHOD(GetStockY)(double *pCoord);
	STDMETHOD(GetStockZ)(double *pCoord);
	STDMETHOD(GetStockRotX)(double *pCoord);
	STDMETHOD(GetStockRotY)(double *pCoord);
	STDMETHOD(GetStockRotZ)(double *pCoord);
	STDMETHOD(GetStockLength)(double *pSize);
	STDMETHOD(GetStockHeight)(double *pSize);
	STDMETHOD(GetStockWidth)(double *pSize);
	STDMETHOD(GetStockType)(BSTR * pText);
	STDMETHOD(GetStockStlName)(BSTR *pText);
	STDMETHOD(GetUnitAxes)(BSTR *pText);
	STDMETHOD(GetUnitOper)(BSTR *pText);
	STDMETHOD(GetUnitMan)(BSTR *pText);
	STDMETHOD(GetUnitType)(BSTR *pText);
	STDMETHOD(GetUnitFile)(BSTR *pText);
	STDMETHOD(GetToolsNum)(long *pNum);
	STDMETHOD(GetProgSteps)(long ProgNum, long *Count);
	STDMETHOD(GetProgLines)(long ProgNum, long *Count);
	STDMETHOD(GetProgTime)(long ProgNum, double *Time);
	STDMETHOD(GetProgFLength)(long ProgNum, double *Length);
	STDMETHOD(GetProgComment)(long ProgNum, BSTR* Text);
	STDMETHOD(GetProgName)(long ProgNum, BSTR *Name);
	STDMETHOD(Clear)();
	STDMETHOD(GetProgWLength)(long ProgNum, double *Length);

	STDMETHOD (GetProgramsNum)  (long* pNum);
	STDMETHOD (GetUnitName) (BSTR* ppbstrMessage);
	STDMETHOD (LoadFile) (BSTR* ppbstrMessage);
protected:
	NCMData * pNData;
};
