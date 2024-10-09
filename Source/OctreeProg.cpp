#include "stdafx.h"
#include "NCMDoc.h"
#include "OctreeProg.h"


OctreeProg::OctreeProg()
{
}


OctreeProg::~OctreeProg()
{
}

const NCadrGeom * OctreeProg::GetCadrGeom(cadrID ID, int StPos)
{
	cadrID Cur = ID;
	int ChInd = Cur.Prog % pDoc->GetChannelsCount();
	Cur.Prog /= pDoc->GetChannelsCount();
	Cur.Cadr /= 2;
	const NCadrGeom *pCadr = pDoc->GetCadrIfVisible(Cur, (ID.Cadr % 2 != 0), ChInd, StPos);
	return pCadr;
}

void OctreeProg::Initialize(CNCMDoc * pDoc, int StPos)
{
	BBox sceneAABB;
	int ChanCount = pDoc->GetChannelsCount();
	for (int ChInd = 0; ChInd < ChanCount; ++ChInd)
	{
		const auto pChannel = pDoc->GetChannel(ChInd);
		CMapStringToNProgram* programList = pChannel->NGetProgramListCn();
		sceneAABB.Expand(GetSceneOverallDimensions(programList));
	}
	Octree::Initialize(sceneAABB, pDoc);

	cadrID id;
	for (int ChInd = 0; ChInd < ChanCount; ++ChInd)
	{
		const auto pChannel = pDoc->GetChannel(ChInd);
		CMapStringToNProgram* programList = pChannel->NGetProgramListCn();
		for (int progNum = 0; progNum < programList->GetSize(); progNum++)
		{
			id.Prog = progNum * ChanCount + ChInd;
			CString Key;
			NProgram* pProg = nullptr;
			programList->GetAt(progNum, Key, (CObject*&)pProg);
			if (pProg)
			{
				if (pProg->GetIndexByStPos(StPos) >= 0)
				{
					const auto Size = pProg->GetGeomArraySize();
					if (programList->HaveProgTraj())
					{
						if (Size >= 0)
						{
							const auto& GArray = pProg->GetGeomArrayByStPos(StPos);
							for (int cadrNum = 0; cadrNum < Size; cadrNum++)
							{
								BBox AABB;
								NCadrGeom cCadr(*(GArray[cadrNum]));
								cCadr.ApplyMatr();
								cCadr.ExpandBox(&AABB);
								id.Cadr = cadrNum * 2;
								InsertObject(id, AABB);
							}
						}
					}
					if (programList->HaveCompTraj())
					{
						if (Size >= 0)
						{
							const auto& CGArray = pProg->GetGeomArrayByStPos(StPos);
							for (int cadrNum = 0; cadrNum < Size; cadrNum++)
							{
								BBox AABB;
								NCadrGeom cCadr(*(CGArray[cadrNum]));
								cCadr.ApplyMatr();
								cCadr.ExpandBox(&AABB);
								id.Cadr = cadrNum * 2 + 1;
								InsertObject(id, AABB);
							}
						}
					}
				}
			}
		}
	}
}
