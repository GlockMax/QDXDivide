#pragma once
#include "DT_Response.h"
#include "DT_Scene.h"
#include "DT_RespTable.h"
#include "SCollisionObjectID.h"
#include "ncollisionscene.h"


class SCollisionSceneSolid : public NCollisionScene
{
	typedef std::vector<const COConv*> T_PolyList;
public:
	SCollisionSceneSolid(void* pCollMan, DT_ResponseCallback SCallBack);
	~SCollisionSceneSolid(void);
protected:
	DT_Scene SScene;
	DT_RespTable SRespTable;
	DT_Response Response;
	std::vector<SCollisionObjectID> ObjectIDs;
	std::map<int, DT_ResponseClass> Behav2RespClass;
	int ActiveToolNum;
	int ActiveToolID;
protected:
	static COShape * AddStlMesh(const float *Triangles, int Stride, int TriNum, bool Owner = false);// Type should be GLfloat *
	COShape * AddTriRandMesh(const float* Points, int Stride, int PtsNum, bool Owner = false);// Type should be GLfloat *
	COShape* AddToolMesh(const float* Triangles);// Type should be GLfloat *
	COShape* AddToolMeshs(const std::vector<float*>& VectOfTriSets);// Type should be GLfloat *
	int RunDetection();
	bool GenRespNum(SCollisionObjectID& SID);
public:
	int AddStlObject(const NCollisionObjectID& ID, const float* Triangles, int Stride, int TriNum, float ObjectMargin, bool Owner, bool Active = false) override; // Type should be GLfloat *
	CollisionObjectInfo& StartCell(int ix, int iy, int iz, const float* Triangles, int Stride, int TriNum) override;
	int AddToolObject(const NCollisionObjectID &ID, float *ToolTriangles, float ObjectMargin = 0., bool Active = false) override;
	int AddToolObject(const NCollisionObjectID& ID, const std::vector<float*>& VectOfTriSets, float ObjectMargin = 0., bool Active = false) override;
	int Add1SensitivePair(int IDIndex1, int IDIndex2, bool Remove = false) override;
	int SetMatrix(int ObjNum, const BMatr &Matr) override;
	int ReplaceStlObject(const NCollisionObjectID& NID, const float* Triangles, int Stride, int TriNum, float ObjectMargin, bool Owner) override;
	NCollisionObjectID& StartComplexObject(const NCollisionObjectID& ID, int Nx, int Ny, int Nz) override;
	void ProcCell(const NCollisionObjectID& ID, int ix, int iy, int iz, class RTriRandMesh** Meshs, float ObjectMargin) override;
	NCollisionObjectID& GetComplexObject(const NCollisionObjectID& ID) override;
	int ReplaceStlObjectIfChanged(int ObjNum, const float* Triangles, size_t SRVersion) override;
	SCollisionObjectID *GetObjID(size_t i) override;
	bool ChangeTool(const void *pTool, int ID) override;
	bool ChangeActiveToolResp(bool IsFast5X, bool Restore) override;
	void RemoveSolidObject(CollisionObjectInfo* pInfoCell);
	int GetActiveToolID() const override { return ActiveToolID; }
};
