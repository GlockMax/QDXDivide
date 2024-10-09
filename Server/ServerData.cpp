// ServerData.cpp : Implementation of CServerApp and DLL registration.

#include "stdafx.h"
#include "server.h"
#include "InfoProgram.h"
#include "InfoTool.h"
#include "ServerData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP ServerData::GetProgramsNum(long* pNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(pNData == NULL)
		return E_FAIL;

   *pNum = (long)pNData->ProgArray.GetSize();
   return S_OK;
}
STDMETHODIMP ServerData::GetUnitName(BSTR* pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->UnitName.AllocSysString();

   return S_OK;
}

STDMETHODIMP ServerData::LoadFile(BSTR* ppbstrMessage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	Clear();
	CString sMessage;
	sMessage = *ppbstrMessage;
	::SysFreeString(*ppbstrMessage);
	if(!(pNData = new NCMData))
		return E_FAIL;
	if(pNData->LoadNCMFile(sMessage))
		return S_OK;
	else
	{
		delete pNData;
		pNData = NULL;
		return E_FAIL;
	}
}


STDMETHODIMP ServerData::GetProgWLength(long ProgNum, double *Length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Length = ((InfoProgram *)pNData->ProgArray[ProgNum])->WorkLength;
	return S_OK;
}

STDMETHODIMP ServerData::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData != NULL)
	{
		delete pNData;
		pNData = NULL;
	}

	return S_OK;
}

STDMETHODIMP ServerData::GetProgName(long ProgNum, BSTR *Name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Name = ((InfoProgram *)pNData->ProgArray[ProgNum])->FileName.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetProgComment(long ProgNum, BSTR *Text)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Text = ((InfoProgram *)pNData->ProgArray[ProgNum])->Comment.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetProgFLength(long ProgNum, double *Length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Length = ((InfoProgram *)pNData->ProgArray[ProgNum])->FastLength;
	return S_OK;
}

STDMETHODIMP ServerData::GetProgTime(long ProgNum, double *Time)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Time = ((InfoProgram *)pNData->ProgArray[ProgNum])->Time;

	return S_OK;
}

STDMETHODIMP ServerData::GetProgLines(long ProgNum, long *Count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Count = ((InfoProgram *)pNData->ProgArray[ProgNum])->LinesNumber;

	return S_OK;
}

STDMETHODIMP ServerData::GetProgSteps(long ProgNum, long *Count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ProgNum < 0 || ProgNum >= pNData->ProgArray.GetSize())
		return E_FAIL;

	*Count = ((InfoProgram *)pNData->ProgArray[ProgNum])->StepsNumber;

	return S_OK;
}

STDMETHODIMP ServerData::GetToolsNum(long *pNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

   *pNum = (long)pNData->ToolArray.GetSize();

	return S_OK;
}

STDMETHODIMP ServerData::GetUnitFile(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->UnitFile.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetUnitType(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->UnitType.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetUnitMan(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->UnitManufacturer.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetUnitOper(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->UnitOperation.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetUnitAxes(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->UnitAxes.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockStlName(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->Stock.StlName.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockType(BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pText = pNData->Stock.Type.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockWidth(double *pSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pSize = pNData->Stock.Gabar.dX();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockHeight(double *pSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pSize = pNData->Stock.Gabar.dY();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockLength(double *pSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pSize = pNData->Stock.Gabar.dZ();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockX(double *pCoord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pCoord = -pNData->Stock.Gabar.Xmin;

	return S_OK;
}
STDMETHODIMP ServerData::GetStockY(double *pCoord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pCoord = -pNData->Stock.Gabar.Ymin;

	return S_OK;
}
STDMETHODIMP ServerData::GetStockZ(double *pCoord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pCoord = -pNData->Stock.Gabar.Zmin;

	return S_OK;
}
STDMETHODIMP ServerData::GetStockRotX(double *pCoord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pCoord = pNData->Stock.RotBase.GetX();

	return S_OK;
}
STDMETHODIMP ServerData::GetStockRotY(double *pCoord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pCoord = pNData->Stock.RotBase.GetY();

	return S_OK;
}
STDMETHODIMP ServerData::GetStockRotZ(double *pCoord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pCoord = pNData->Stock.RotBase.GetZ();

	return S_OK;
}

STDMETHODIMP ServerData::GetStockBase(long *pNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	*pNum = pNData->Stock.BasePoint;

	return S_OK;
}

STDMETHODIMP ServerData::GetToolWLength(long ToolNum, double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pVal = ((InfoTool *)pNData->ToolArray[ToolNum])->WorkLength;

	return S_OK;
}

STDMETHODIMP ServerData::GetToolFLength(long ToolNum, double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pVal = ((InfoTool *)pNData->ToolArray[ToolNum])->FastLength;

	return S_OK;
}

STDMETHODIMP ServerData::GetToolTime(long ToolNum, double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pVal = ((InfoTool *)pNData->ToolArray[ToolNum])->Time;

	return S_OK;
}

STDMETHODIMP ServerData::GetToolName(long ToolNum, BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pText = ((InfoTool *)pNData->ToolArray[ToolNum])->Name.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetToolType(long ToolNum, BSTR *pText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pText = ((InfoTool *)pNData->ToolArray[ToolNum])->Type.AllocSysString();

	return S_OK;
}

STDMETHODIMP ServerData::GetToolVolume(long ToolNum, double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pVal = ((InfoTool *)pNData->ToolArray[ToolNum])->CutVolume;

	return S_OK;
}

STDMETHODIMP ServerData::GetToolPos(long ToolNum, long *pNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pNData == NULL)
		return E_FAIL;

	if(ToolNum < 0 || ToolNum >= pNData->ToolArray.GetSize())
		return E_FAIL;

	*pNum = ((InfoTool *)pNData->ToolArray[ToolNum])->Pos;

	return S_OK;
}
