// NInterface.h: interface for the NInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NINTERFACE_H__05E12622_C944_11D6_B64D_0050BF5C3BA4__INCLUDED_)
#define AFX_NINTERFACE_H__05E12622_C944_11D6_B64D_0050BF5C3BA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PartArray.h"
#include "MBSPForest.h"
#include "..\Surface\surEnums.h"
#include "..\Surface\Plane\NMeshArray.h"

class NSurFMComTool;
class NCadrsProcessing;
class NToolCombined;
class NCadrGeom;

typedef CTypedPtrArray <CPtrArray,NCadrGeom*> CagrArray;

class AFX_EXT_CLASS NInterface  
{
public:
//	void Build(MBSPForest &OutTree, PartArray& OutArray);
	void Build();
	void NInterfaceBuild();
	enum JobState SendOneCadr(NCadrGeom *pCadr);
	enum JobState BeginIssue( NToolCombined *pT);
	enum JobState GetStatus();
	void SetType(LPARAM type);
	LPARAM GetType(void);
	void SetViewType(LPARAM type);
	void Draw();
	void Reset(void);

	NInterface();
	virtual ~NInterface();

protected:
	NSurFMComTool *pBuildSurf;
	NCadrsProcessing  *pBuildPlane;
	LPARAM job_type;
	LPARAM view_type;
	int ViewTreeIndex;
	bool IsSound;

	PartArray		*pSurfArray;
	MeshArr			PlaneArray;
public:
	// Задание отображаемого дерева
	void SetSound(bool sound);
};

#endif // !defined(AFX_NINTERFACE_H__05E12622_C944_11D6_B64D_0050BF5C3BA4__INCLUDED_)
