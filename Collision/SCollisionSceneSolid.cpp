#include "StdAfx.h"
#include "BMatr.h"
#include "NCollisionLog.h"
#include "COTriangle.h"
#include "RTriRandMesh.h"
#include "RTriangles.h"
#include "SCollisionSceneSolid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SCollisionSceneSolid::SCollisionSceneSolid(void* pCollMan, DT_ResponseCallback SCallBack) : 
Response(SCallBack, DT_SIMPLE_RESPONSE, pCollMan)
{
	ActiveToolNum = -1;
}

SCollisionSceneSolid::~SCollisionSceneSolid(void)
{
	for (int i = 0; i < ObjectIDs.size(); ++i)
	{
		for each (auto El in ObjectIDs[i].GetInfoVec())
		{
			if (El == nullptr)
				continue;
			RemoveSolidObject(El);
			delete El;
		}
	}
}

COShape* SCollisionSceneSolid::AddStlMesh(const float* Triangles, int Stride, int TriNum, bool Owner)
{
	if (Triangles == nullptr)
		return nullptr;
	if (TriNum < 0)
		TriNum = int(Triangles[0]);
	if (TriNum < 1)
		return nullptr;
	DT_VertexBase* pVBase = new DT_VertexBase(&Triangles[4], Stride, Owner);
	COConcave* pComplex = new COConcave(pVBase);
	T_PolyList polyList;
	for (int i = 0; i < TriNum; ++i)
	{
		COConv* poly = new COTriangle(pVBase, 3 * i, 3 * i + 1, 3 * i + 2);
		polyList.push_back(poly);
	}
	pComplex->finish(DT_Count(polyList.size()), &polyList[0]);

	return pComplex;
}

COShape* SCollisionSceneSolid::AddTriRandMesh(const float* Points, int Stride, int PtsNum, bool Owner)
{
	if (Points == nullptr)
		return nullptr;
	if (PtsNum < 3)
		return nullptr;
	DT_VertexBase* pVBase = new DT_VertexBase(Points, Stride, Owner);
	COConcave* pComplex = new COConcave(pVBase);
	return pComplex;
}

COShape* SCollisionSceneSolid::AddToolMesh(const float* Triangles)
{
	if (Triangles == nullptr)
		return nullptr;
	if (Triangles[0] < 1.f)
		return nullptr;
	const int TriNum = int(Triangles[0]);
	float* BTriangles = new float[TriNum * 6 * 3 + 1 - 4];
	const int Size = (TriNum * 6 * 3 + 1 - 4) * sizeof(Triangles[0]);
	memcpy_s(BTriangles, Size, &Triangles[4], Size);
	const DT_VertexBase* pVBase = new DT_VertexBase(BTriangles, sizeof(float) * 6, true);
	COConcave* pComplex = new COConcave(pVBase);
	T_PolyList polyList;
	for (int i = 0; i < TriNum; ++i)
	{
		COConv* poly = new COTriangle(pVBase, 3 * i, 3 * i + 1, 3 * i + 2);
		polyList.push_back(poly);
	}
	pComplex->finish(DT_Count(polyList.size()), &polyList[0]);

	return pComplex;
}

COShape* SCollisionSceneSolid::AddToolMeshs(const std::vector<float*>& VectOfTriSets)
{
	if (VectOfTriSets.empty())
		return nullptr;
	// Calc result size
	int WholeTriNum = 0;
	for each (float* pTriNum in VectOfTriSets)
	{
		if (pTriNum == nullptr)
			continue;
		WholeTriNum += int(*pTriNum);
	}
	// 
	float* pWhole = new float[WholeTriNum * 3 * 3];
	// copy
	int DestInd = 0;
	for each (float* pTriNum in VectOfTriSets)
	{
		if (pTriNum == nullptr)
			continue;
		const int TriNum = int(pTriNum[0]);
		for (int TriInd = 0; TriInd < TriNum; ++TriInd)
		{
			const int SourceInd = 4 + TriInd * 6 * 3;
			for (int k = 0; k < 3; ++k)// points
			{
				for (int c = 0; c < 3; ++c)// coordinates
					pWhole[DestInd + k * 3 + c] = pTriNum[SourceInd + k * 6 + c];
			}
			DestInd += 3 * 3;
		}
	}
	// make result
	const DT_VertexBase* pVBase = new DT_VertexBase(pWhole, sizeof(float) * 3, true);
	COConcave* pComplex = new COConcave(pVBase);
	T_PolyList polyList;
	for (int i = 0; i < WholeTriNum; ++i)
	{
		COConv* poly = new COTriangle(pVBase, 3 * i, 3 * i + 1, 3 * i + 2);
		polyList.push_back(poly);
	}
	pComplex->finish(DT_Count(polyList.size()), &polyList[0]);
	return pComplex;
}

int SCollisionSceneSolid::RunDetection()
{
	return SScene.handleCollisions(&SRespTable);
}

int SCollisionSceneSolid::AddStlObject(const NCollisionObjectID& NID, const float* Triangles, int Stride, int TriNum, float ObjectMargin
	, bool Owner, bool Active /*= false*/)// Type should be GLfloat *
{
	COShape* pNewShape = AddStlMesh(Triangles, Stride, TriNum, Owner);
	if (!pNewShape)
		return -1;

	ObjectIDs.emplace_back(NID);
	SCollisionObjectID& SID = ObjectIDs.back();
	DT_Object* pNewObject = new DT_Object((void*)(ObjectIDs.size() - 1), *pNewShape);
	pNewObject->setMargin(ObjectMargin);
	SID.SetSelfNum(int(ObjectIDs.size() - 1));
	GenRespNum(SID);
	if (!Active)
	{
		SRespTable.setResponseClass(pNewObject, SID.GetRespNum());
		SScene.addObject(*pNewObject);
	}
	SID.SetInfo0(pNewObject, pNewShape);

	return int(ObjectIDs.size() - 1);
}

int SCollisionSceneSolid::ReplaceStlObject(const NCollisionObjectID& NID, const float* Triangles, int Stride, int TriNum, float ObjectMargin, bool Owner)
{
	auto ObjNum = NID.GetSelfNum();
	if (ObjNum < 0 || ObjNum >= ObjectIDs.size())
		return -1;
	SCollisionObjectID& SID = ObjectIDs[ObjNum];

	assert(SID.GetInfoVec().size() == 1);
	auto pInfoCell = SID.GetInfoVec().at(0);

	if (SID.GetRespNum() != -1)
	{
		RemoveSolidObject(pInfoCell);
		pInfoCell->SetSolidObject(nullptr);
		pInfoCell->SetShape(nullptr);
	}

	COShape* pNewShape = AddStlMesh(Triangles, Stride, TriNum, Owner);
	if (!pNewShape)
		return -1;
	pInfoCell->SetShape(pNewShape);
	DT_Object* pNewObject = new DT_Object((void*)static_cast<size_t>(SID.GetSelfNum()), *pNewShape);
	pNewObject->setMargin(ObjectMargin);
	SScene.addObject(*pNewObject);
	pInfoCell->SetSolidObject(pNewObject);
	SRespTable.setResponseClass(pNewObject, SID.GetRespNum());

	return 0;
}

NCollisionObjectID& SCollisionSceneSolid::StartComplexObject(const NCollisionObjectID& NID, int Nx, int Ny, int Nz)
{
	auto& ID = ObjectIDs.emplace_back(NID);
	ID.SetDimensions(Nx, Ny, Nz);
	ID.SetSelfNum(int(ObjectIDs.size() - 1));
	GenRespNum(ID);
	return ID;
}

void SCollisionSceneSolid::ProcCell(const NCollisionObjectID& ID, int ix, int iy, int iz, RTriRandMesh** Meshs, float ObjectMargin)
{
	const auto Ind = ID.MakeKey(ix, iy, iz);
	auto& SID = ObjectIDs[ID.GetSelfNum()];
	auto pInfoCell = SID.GetInfoVec().at(Ind);
	// Check Meshs for emptyness
	size_t WholePtsNum = 0;
	for (int i = 0; i < NCollisionObjectID::IndStep * NCollisionObjectID::IndStep * NCollisionObjectID::IndStep; ++i)
	{
		auto pMesh = Meshs[i];// pMesh is joined already by Open4Read
		if (pMesh == nullptr)
			continue;
		size_t PtsNum = pMesh->GetActSizeP();
		const float* pC = pMesh->GetpC0();
		if (PtsNum <= 0)
			continue;
		WholePtsNum += PtsNum;
	}
	if (WholePtsNum <= 0)
	{
		if (pInfoCell == nullptr)
			return;
		RemoveSolidObject(pInfoCell);
		delete pInfoCell;
		SID.GetInfoVec().at(Ind) = nullptr;
		return;
	}
	// Meshs is not empty
	bool MeshChanged = false;
	if (pInfoCell == nullptr)
	{
		pInfoCell = new CollisionObjectInfo;
		SID.GetInfoVec().at(Ind) = pInfoCell;
		MeshChanged = true;
	}
	else
	{
		// Check Meshs
		for (int i = 0; i < NCollisionObjectID::IndStep * NCollisionObjectID::IndStep * NCollisionObjectID::IndStep; ++i)
		{
			if (Meshs[i] != pInfoCell->GetOldMesh(i))
				MeshChanged = true;
		}
		if (MeshChanged)
			RemoveSolidObject(pInfoCell);
	}
	if (!MeshChanged)
		return;
	// store meshs 
	pInfoCell->SetOldMesh(Meshs);
	// combine meshes
	T_PolyList PolyList;
	float* buf = new float[WholePtsNum * 3];
	int IndShift = 0;
	const DT_VertexBase* pVBase = new DT_VertexBase(buf, 0, true);
	for (int k = 0; k < NCollisionObjectID::IndStep * NCollisionObjectID::IndStep * NCollisionObjectID::IndStep; ++k)
	{
		auto pMesh = Meshs[k];// pMesh is joined already by Open4Read
		if (pMesh == nullptr)
			continue;
		size_t PtsNum = pMesh->GetActSizeP();
		const float* pC = pMesh->GetpC0();
		if (PtsNum <= 0)
			continue;
		const int TriNum = pMesh->GetTriNum();
		if (TriNum <= 0)
			continue;
		for (size_t i = 0; i < PtsNum; ++i)
		{
			memcpy_s(buf + 3 * i, sizeof(float) * 3, pC + 3 + (pMesh->GetStride() / sizeof(float)) * i, sizeof(float) * 3);
		}
		for (auto l = 0; l < TriNum; ++l)
		{
			int i0, i1, i2;
			if (pMesh->ExtractTriangleInds(l, &i0, &i1, &i2) == 0)
			{
				COConv* poly = new COTriangle(pVBase, i0 + IndShift, i1 + IndShift, i2 + IndShift);
				PolyList.push_back(poly);
			}
		}
		buf += 3 * PtsNum;
		IndShift += int(PtsNum);
	}
	if (PolyList.empty())
	{
		delete pVBase;
		if (pInfoCell != nullptr)
			RemoveSolidObject(pInfoCell);
		delete pInfoCell;
		SID.GetInfoVec().at(Ind) = nullptr;
		return;
	}
	COConcave* pComplex = new COConcave(pVBase);
	pComplex->finish(static_cast<DT_Count>(PolyList.size()), PolyList.data());

	auto pNewObject = new DT_Object((void*)static_cast<size_t>(ID.GetSelfNum()), *pComplex);
	pNewObject->setMargin(ObjectMargin);
	SScene.addObject(*pNewObject);
	SRespTable.setResponseClass(pNewObject, ObjectIDs[ID.GetSelfNum()].GetRespNum());
	pInfoCell->SetShape(pComplex);
	pInfoCell->SetSolidObject(pNewObject);
}

NCollisionObjectID& SCollisionSceneSolid::GetComplexObject(const NCollisionObjectID& NID)
{
	return ObjectIDs.at(NID.GetSelfNum());
}

CollisionObjectInfo& SCollisionSceneSolid::StartCell(int ix, int iy, int iz, const float* Triangles, int Stride, int PtsNum)
{
	float* buf = new float[3 * PtsNum];
	for (size_t i = 0; i < PtsNum; ++i)
	{
		memcpy_s(buf + 3 * i, sizeof(float) * 3, Triangles + 3 + (Stride / sizeof(float)) * i, sizeof(float) * 3);
	}
	DT_VertexBase* pVBase = new DT_VertexBase(buf, 0, true);
	COConcave* pComplex = new COConcave(pVBase);
	return ObjectIDs.back().SetInfo(ix, iy, iz, nullptr, pComplex);
}

int SCollisionSceneSolid::AddToolObject(const NCollisionObjectID& NID, float* ToolTriangles, float ObjectMargin, bool Active)
{
	if (ToolTriangles == NULL)
		return -1;

	COShape* pNewShape = AddToolMesh(ToolTriangles);
	if (!pNewShape)
		return -1;

	ObjectIDs.emplace_back(NID);
	SCollisionObjectID& SID = ObjectIDs.back();
	DT_Object* pNewObject = new DT_Object((void*)(ObjectIDs.size() - 1), *pNewShape);
	pNewObject->setMargin(ObjectMargin);
	//	SScene.addObject(*pNewObject);
	SID.SetInfo0(pNewObject, pNewShape);
	SID.SetSelfNum(int(ObjectIDs.size() - 1));
	GenRespNum(SID);
	if (!Active)
		SRespTable.setResponseClass(pNewObject, SID.GetRespNum());

	return SID.GetSelfNum();
}

int SCollisionSceneSolid::AddToolObject(const NCollisionObjectID& NID, const std::vector<float*>& VectOfTriSets, float ObjectMargin, bool Active)
{
	if (VectOfTriSets.empty())
		return -1;

	COShape* pNewShape = AddToolMeshs(VectOfTriSets);
	if (!pNewShape)
		return -1;

	ObjectIDs.emplace_back(NID);
	SCollisionObjectID& SID = ObjectIDs.back();
	DT_Object* pNewObject = new DT_Object((void*)(ObjectIDs.size() - 1), *pNewShape);
	pNewObject->setMargin(ObjectMargin);
	SID.SetInfo0(pNewObject, pNewShape);
	SID.SetSelfNum(int(ObjectIDs.size() - 1));
	GenRespNum(SID);
	if (!Active)
	{
		SRespTable.setResponseClass(pNewObject, SID.GetRespNum());
		SScene.addObject(*pNewObject);
	}


	return SID.GetSelfNum();
}

int SCollisionSceneSolid::Add1SensitivePair(int IDIndex1, int IDIndex2, bool Remove /*= false*/)
{
	if (IDIndex1 == IDIndex2)
		return -1;
	int r1 = -1;
	int r2 = -1;
	auto it1 = Behav2RespClass.find(IDIndex1);
	if (it1 == Behav2RespClass.end())
		return -1;
	r1 = it1->second;
	auto it2 = Behav2RespClass.find(IDIndex2);
	if (it2 == Behav2RespClass.end())
		return -1;
	r2 = it2->second;
	if (r1 == -1 || r2 == -1)
		return -1;

	if (Remove)
	{
		DT_Response buf;
		SRespTable.removePair(r1, r2, buf);
	}
	else
		SRespTable.addPair(r1, r2, Response);
	return 0;
}

int SCollisionSceneSolid::SetMatrix(int ObjNum, const BMatr& Matr)
{
	if (ObjNum < 0 || ObjNum >= ObjectIDs.size())
		return -1;
	for each (auto & El in ObjectIDs[ObjNum].GetInfoVec())
	{
		if (El != nullptr)
		{
			auto pObj = El->GetSolidObject();
			if (pObj != nullptr)
				pObj->setMatrix(Matr.GetArray());
		}
	}
	return 0;
}

int SCollisionSceneSolid::ReplaceStlObjectIfChanged(int ObjNum, const float* Triangles, size_t SRVersion)
{
	if (ObjNum < 0 || ObjNum >= ObjectIDs.size())
		return -1;
	SCollisionObjectID& SID = ObjectIDs[ObjNum];
	if (SID.GetSpinRendVersion() == SRVersion)
		return 0;
	// Triangles changed
	SID.SetSpinRendVersion(SRVersion);
	return ReplaceStlObject(SID, Triangles, 6 * sizeof(float), -1, 0., false);
}

SCollisionObjectID* SCollisionSceneSolid::GetObjID(size_t i)
{
	if (i < 0 || i >= ObjectIDs.size())
		return nullptr;

	return &ObjectIDs[i];
}
bool SCollisionSceneSolid::ChangeTool(const void* pTool, int ID)
{
	// Find pTool
	int l = 0;
	for (; l < ObjectIDs.size(); ++l)
	{
		if (ObjectIDs[l].GetCollidableObj() == pTool && ObjectIDs[l].GetID() == ID)
			break;
	}
	if (l == ActiveToolNum)
		return true; // tool is the same 
	if (ActiveToolNum >= 0) // remove old tool from SRespTable and add old tool in place pos. (if exist)
	{
		for (int i = 0; i < ObjectIDs.size(); ++i)
		{
			if (ObjectIDs[i].GetCollidableObj() == ObjectIDs[ActiveToolNum].GetCollidableObj())
			{
				assert(ObjectIDs[i].GetInfoVec().size() == 1);
				const auto pObj = ObjectIDs[i].GetInfoVec().at(0)->GetSolidObject();
				if (ObjectIDs[i].GetID() == ActiveToolID)
				{
					SRespTable.clearResponseClass(pObj);
					SScene.removeObject(*pObj);
				}
				else
					SRespTable.setResponseClass(pObj, ObjectIDs[i].GetRespNum());
			}
		}
	}
	if (l == ObjectIDs.size())
	{
		ActiveToolNum = -1;
		return false;
	}
	if (!pTool)
		return false;
	if (ID < 0)
	{// New tool is absent
		ActiveToolNum = -1;
		return true;
	}
	else
	{
		ActiveToolNum = l;
		ActiveToolID = ID;
	}

	for (int i = 0; i < ObjectIDs.size(); ++i)// add new tool to SRespTable and remove it in place pos. (if exist)
	{
		if (ObjectIDs[i].GetCollidableObj() == ObjectIDs[ActiveToolNum].GetCollidableObj())
		{
			assert(ObjectIDs[i].GetInfoVec().size() == 1);
			const auto pObj = ObjectIDs[i].GetInfoVec().at(0)->GetSolidObject();
			if (ObjectIDs[i].GetID() == ActiveToolID)
			{
				SRespTable.setResponseClass(pObj, ObjectIDs[i].GetRespNum());
				SScene.addObject(*pObj);
			}
			else
				SRespTable.clearResponseClass(pObj);
		}
	}
	return true;
}

bool SCollisionSceneSolid::ChangeActiveToolResp(bool IsFast5X, bool Restore)
{
	if (!IsFast5X)
		return false;
	if (ActiveToolNum < 0)
		return false;
	bool Res = false;
	for (int i = 0; i < ObjectIDs.size(); ++i)
	{
		const SCollisionObjectID& CutToolID = ObjectIDs[i];
		if (CutToolID.GetCollidableObj() == ObjectIDs[ActiveToolNum].GetCollidableObj())
		{
			if ((CutToolID.GetBehav() & 1) != 0)
			{// This id represents active and cutting tool

				for (int k = 0; k < ObjectIDs.size(); ++k)
				{
					const SCollisionObjectID& StockID = ObjectIDs[k];
					if (StockID.GetGroup() == OG_STOCKS)
					{
						if (Restore)
							SRespTable.removePair(CutToolID.GetRespNum(), StockID.GetRespNum(), Response);
						else
							SRespTable.addPair(CutToolID.GetRespNum(), StockID.GetRespNum(), Response);
						// active cutting tool may (Restore == false) or may not (Restore == true) collide with stocks now
						Res = true;
					}
				}
				break;// all cutting tools have same behav and response num so above cycle once
			}
		}
	}
	return Res;
}

void SCollisionSceneSolid::RemoveSolidObject(CollisionObjectInfo* pInfoCell)
{
	if (pInfoCell == nullptr)
		return;
	DT_Object* pObj = pInfoCell->GetSolidObject();
	if (pObj != nullptr)
	{
		SRespTable.clearResponseClass(pObj);
		SScene.removeObject(*pObj);
		delete pObj;
		pInfoCell->SetSolidObject(nullptr);
	}
	delete pInfoCell->GetShape();
	pInfoCell->SetShape(nullptr);
}


bool SCollisionSceneSolid::GenRespNum(SCollisionObjectID& SID)
{
	// Generate response class if it is absent
	if (SID.GetBehav() < 0)
		return false;
	auto it = Behav2RespClass.find(SID.GetBehav());
	if (it != Behav2RespClass.end())
	{
		SID.SetRespNum(it->second);
		return true;
	}
	auto NewRespNum = SRespTable.genResponseClass();
	Behav2RespClass.insert(std::map<int, DT_ResponseClass>::value_type(SID.GetBehav(), NewRespNum));
	SID.SetRespNum(NewRespNum);
	return true;
}
