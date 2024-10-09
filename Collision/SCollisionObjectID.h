#pragma once
#include "SOLID.h"
#include <vector>
#include "ncollisionobjectid.h"

class CollisionObjectInfo
{
	friend class SCollisionObjectID;
public:
	CollisionObjectInfo();
	~CollisionObjectInfo();
	void Clear(void);
	void SetSolidObject(class DT_Object* pObject) { pSolidObject = pObject; }
	DT_Object* GetSolidObject(void) const { return pSolidObject; }
	class COShape* GetShape() const { return pShape; }
	void SetShape(class COShape* pS) { pShape = pS; }
	const void* GetOldMesh(int i) const { return Meshs[i]; }
	void SetOldMesh(void* in) { memcpy_s(Meshs, sizeof(Meshs), in, sizeof(Meshs)); }
protected:
	class DT_Object* pSolidObject;
	class COShape* pShape;
	void* Meshs[NCollisionObjectID::IndStep * NCollisionObjectID::IndStep * NCollisionObjectID::IndStep] = { nullptr };
};

class SCollisionObjectID :
	public NCollisionObjectID
{
public:
	SCollisionObjectID(void);
	SCollisionObjectID(const NCollisionObjectID &NID);
	virtual ~SCollisionObjectID(void);
	void SetInfo0(DT_Object* pSolidObject, class COShape* pShape);
	CollisionObjectInfo& SetInfo(int ix, int iy, int iz, DT_Object* pSolidObject, class COShape* pShape);
	const auto& GetInfoVec() const { return InfoVec; }
	auto& GetInfoVec() { return InfoVec; }
	void SetRespNum(DT_ResponseClass RespN) { RespNum = RespN; }
	DT_ResponseClass GetRespNum(void) const { return RespNum; }
	void SetDimensions(int Nx, int Ny, int Nz) override;
	void ClearSolids();

protected:
	std::vector<CollisionObjectInfo*> InfoVec;
	DT_ResponseClass RespNum;
};
