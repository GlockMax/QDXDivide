#include "stdafx.h"
#include "MBSPForest.h"
#include "MElemIDProc.h"
#include "CopyMap.h"
#include "NParamMesh.h"

NParamMesh::NParamMesh(void)
{
	m_iSurf = ID_EMPTY;
	m_sgn = 1;
	m_CadrNum = 0;
	MeshID = 0;
	CuttingTool = true;
	Shank = false;
}
NParamMesh::~NParamMesh(void)
{
}

void NParamMesh::FillCopy(NParamMesh & Mesh, CopyMap &Map) const
{
	Mesh.m_iSurf = Map.AddOldIndOr(m_iSurf);
	auto Buf = Map.AddOldIndOr(MFOREST.GetIDProc()->GetSurfID(MeshID));
	if(Mesh.MeshID != 0)
		MFOREST.GetIDProc()->SetSurfID(&Mesh.MeshID, Buf);
}


