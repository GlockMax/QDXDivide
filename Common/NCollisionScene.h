#pragma once

#include "Collision_API.h"
#include "ResLocal.h"
#include <set>
#include "NCollisionObjectID.h"
#include "NCollisionID.h"
#include "cadrID.h"

typedef int(ResponseCallback)(void* client_data,
	void* client_object1,
	void* client_object2,
	const void* coll_data);

class COLLISION_API NCollisionScene
{
protected:
	NCollisionScene();
public:
	static NCollisionScene *GenCollisionScene(void* pCollMan, ResponseCallback RespCallBack);
	virtual ~NCollisionScene(void);
	virtual int AddStlObject(const NCollisionObjectID &ID, const float *Triangles, int Stride, int TriNum, float ObjectMargin, bool Owner, bool Active = false) = 0;// Type should be GLfloat *
	virtual class CollisionObjectInfo& StartCell(int ix, int iy, int iz, const float* Triangles, int Stride, int PtsNum) = 0; // Type should be GLfloat *
	virtual NCollisionObjectID& StartComplexObject(const NCollisionObjectID& ID, int Nx, int Ny, int Nz) = 0;
	virtual void ProcCell(const NCollisionObjectID& ID, int ix, int iy, int iz, class RTriRandMesh** Meshs, float ObjectMargin) = 0;
	virtual NCollisionObjectID& GetComplexObject(const NCollisionObjectID& ID) = 0;
	virtual int AddToolObject(const NCollisionObjectID& ID, float* ToolTriangles, float ObjectMargin = 0., bool Active = false) = 0;
	virtual int AddToolObject(const NCollisionObjectID& ID, const std::vector<float*>& VectOfTriSets, float ObjectMargin = 0., bool Active = false) = 0;
	virtual int Add1SensitivePair(int IDIndex1, int IDIndex2, bool Remove = false) = 0;
	virtual int RunDetection() = 0;
	virtual int SetMatrix(int ObjNum, const class BMatr& Matr) = 0;
	virtual int ReplaceStlObject(const NCollisionObjectID& NID, const float* Triangles, int Stride, int TriNum, float ObjectMargin, bool Owner) = 0;
	virtual int ReplaceStlObjectIfChanged(int ObjNum, const float* Triangles, size_t SRVersion) = 0;
	virtual NCollisionObjectID *GetObjID(size_t i) = 0;
	virtual bool ChangeTool(const void *pTool, int ID) = 0;
	virtual bool ChangeActiveToolResp(bool IsFast5X, bool Restore) = 0;
	virtual int GetActiveToolID() const = 0;

	int AddSensitivePair(int IDIndex1, int IDIndex2);
	void SetCadrIDHist(const ProgTPoint& Cadr) {CadrIDAbs = Cadr;}
	void ClearActiveCollisions(void) { ActiveCollisions.clear(); Diskr = 0;}
	int AddActiveCollision(const NCollisionID &CollID);
	void SetActiveCollisionsEnabled(bool Val) { ActiveCollisionsEnabled = Val;}
	void SetFixStock(bool Val) { FixStock = Val;}
	const std::set<int>& GetActiveCollisions() const { return ActiveCollisions;} 
	const ProgTPoint& GetCadrIDAbs() const { return CadrIDAbs; }
	bool GetFixStock() const { return FixStock; }
	int GetDiskr() const { return Diskr; }
protected:
	ProgTPoint CadrIDAbs;
	std::set<int> ActiveCollisions;
	bool ActiveCollisionsEnabled;
	bool FixStock;
	int Diskr; // 1 - have non stock collision, 2 - have stock collision, 3 - both
};
