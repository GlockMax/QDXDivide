// NInterface.cpp: implementation of the NInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "surEnums.h"

#include "NInterface.h"
#include "Plane\NCadrsProcessing.h"
#include "NSurFMComTool.h"
#include "NPartSurface.h"
#include "MBSPForest.h"
#include "ninterface.h"
#include "..\common\ninterface.h"
#include "Plane\NMeshArray.h"
#include "Plane\NParamMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NInterface::NInterface()
{
	pBuildSurf = NULL;
	pBuildPlane = NULL;
	job_type = bNOTSET;
	view_type = bVIEW_MESH;
	ViewTreeIndex = 0;
	IsSound = false;

	pSurfArray = NULL;
}

NInterface::~NInterface()
{
	if (pBuildSurf)
		delete pBuildSurf;

	if (pBuildPlane)
		delete pBuildPlane;

	if (pSurfArray)
	{
		pSurfArray->ClearAll();	
		pSurfArray->RemoveAll();
		delete pSurfArray;
	}

	for (int i=0; i<PlaneArray.GetSize(); i++)
		delete PlaneArray.GetAt(i);
}

// начать передачу данных
// возвращает: Status
enum JobState NInterface::BeginIssue(NToolCombined *pT)
{	
	enum JobState Status;
	if (job_type & bMESH)
	{
		if (!pBuildPlane)
			pBuildPlane = new NCadrsProcessing;
		Status = pBuildPlane->BeginIssue(pT);
	}
	if ((job_type & bSURFACE) && Status == POSSIBLEMORE)
	{
		if(!pBuildSurf)
			pBuildSurf = new NSurFMComTool;
		Status = pBuildSurf->BeginIssue(pT);
	}
	
	return Status;
}

// передать один кадр
// возвращает: Status
enum JobState NInterface::SendOneCadr(NCadrGeom *pCadr)
{
	enum JobState Status ;
	Status = EMPTY;
	if (job_type & bMESH)
	{
		if (pBuildPlane)
			Status = pBuildPlane->SendOneCadr(pCadr);
	}	
	if ((job_type & bSURFACE) && Status == POSSIBLEMORE)
	{
		if (pBuildSurf)
			Status = pBuildSurf->SendOneCadr(pCadr);
	}
	return Status;
}

// СБРОСИТЬ ВСЕ ПАРАМЕТРЫ
void NInterface::Reset(void)
{
	if(pBuildSurf)
	{
		delete pBuildSurf;
		pBuildSurf = NULL;
		pSurfArray->ClearAll();	
		pSurfArray->RemoveAll();
	}

	if (pBuildPlane)
	{
		delete pBuildPlane;
		pBuildPlane = NULL;
		for (int i=0; i<PlaneArray.GetSize(); i++)
			delete PlaneArray[i];
		PlaneArray.RemoveAll();
		MBSPForest::GetActive().ClearAll();
	}	
	job_type = bNOTSET;
}

// ПОСТРОЕНИЕ 
// построение дерева и/или поверхностей
void NInterface::Build()
{
	if (job_type | bMESH)
	{
		if (IsSound)
			Beep(2000,100);

		enum JobState res;
		
		if (!pBuildPlane)
			return;
		pBuildPlane->BuildPlane(PlaneArray);
		// построить законченное дерево
		res = pBuildPlane->GetStatus();
		
		if (IsSound)
			Beep(1000,100);
	}
	if (job_type & bSURFACE)
	{
		if (IsSound)
			Beep(3000,100);

		if (!pBuildSurf)
			return;
		if (pSurfArray)
			delete pSurfArray;
		pSurfArray = new PartArray;
		pBuildSurf->BuildSurface(*pSurfArray);
		if (IsSound)
			Beep(1000,100);
	}
}

// задать тип построния (поверхности/дерево)
void NInterface::SetType(LPARAM type)
{
	job_type = type;
}

// определить тип построния (поверхности/дерево)
LPARAM NInterface::GetType()
{
	return job_type;
}

// определить состояние потрония
enum JobState NInterface::GetStatus()
{
	enum JobState Status;
	
	if (job_type & bNOTSET) 
		Status = EMPTY;
	else	
	{
		if (job_type & bMESH)
		{
			if (!pBuildPlane)
				Status = EMPTY;
			else
				Status = pBuildPlane->GetStatus();
		}
		if (job_type & bSURFACE)
		{
			if (!pBuildSurf)
				Status = EMPTY;
			else
				Status = pBuildSurf->GetStatus();
		}
	}
	return Status;
}

// задать тип отображения
void NInterface::SetViewType(LPARAM type)
{
	view_type = type;
}

// задание звуковой	сигнализации
void NInterface::SetSound(bool sound)
{
	IsSound = sound;
}
// рисование построенного дерева или поверхности
void NInterface::Draw()
{
	if (job_type & bMESH)
	{
		// отображение BSP в начальном виде (до объёдинения)
		bool bVPoint = false, bVNum = false, bVNorm = false;
		if (view_type & bVIEW_POINT)
			bVPoint = true;
		if (view_type & bVIEW_NUM)
			bVNum = true;
		if (view_type & bVIEW_NORM)
			bVNorm = true;
		
		MBSPForest::GetActive().Draw(true); 
		for (int i=0; i<PlaneArray.GetSize(); i++)
			PlaneArray.GetAt(i)->Draw(bVPoint, bVNum, bVNorm);
	}
	if ((job_type & bSURFACE) &&
		(view_type & bVIEW_SUR))
	{
		if (!pSurfArray)
			return;
		for(int l=0;l<pSurfArray->GetSize();l++)
			pSurfArray->GetAt(l)->Draw();
	}	
}

int compare_double(const void *a, const void *b)
{
	if(*(double*)a>*(double*)b)
		return -1;
	return (*(double*)a==*(double*)b) ? 0 : 1;
}
