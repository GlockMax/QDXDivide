#include "StdAfx.h"
#include "DT_Object.h"
#include "SCollisionObjectID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SCollisionObjectID::SCollisionObjectID(void) :
NCollisionObjectID()
{
	RespNum = -1;
	InfoVec.resize(1);
	InfoVec[0] = new CollisionObjectInfo;
}
SCollisionObjectID::SCollisionObjectID(const NCollisionObjectID &NID)
: NCollisionObjectID(NID) 
{
	RespNum = -1;
	InfoVec.resize(1);
	InfoVec[0] = new CollisionObjectInfo;
}

SCollisionObjectID::~SCollisionObjectID(void)
{
} 

void SCollisionObjectID::SetInfo0(DT_Object* pSolidObject, COShape* pShape)
{
	CollisionObjectInfo* pb = InfoVec[0];
	if (pb == nullptr)
		pb = new CollisionObjectInfo;
	pb->pSolidObject = pSolidObject;
	pb->pShape = pShape;

	InfoVec[0] = pb;
}

CollisionObjectInfo& SCollisionObjectID::SetInfo(int ix, int iy, int iz, DT_Object* pSolidObject, COShape* pShape)
{
	auto Ind = MakeKey(ix, iy, iz);
	CollisionObjectInfo* pb = InfoVec[Ind];
	if(pb == nullptr)
		pb = new CollisionObjectInfo;
	pb->pSolidObject = pSolidObject;
	pb->pShape = pShape;

	InfoVec[Ind] = pb;
	return *InfoVec.at(Ind);
}

CollisionObjectInfo::CollisionObjectInfo()
{
	Clear();
}

CollisionObjectInfo::~CollisionObjectInfo()
{
}

void CollisionObjectInfo::Clear()
{
	pSolidObject = nullptr;
	pShape = nullptr;
}

void SCollisionObjectID::SetDimensions(int Nx, int Ny, int Nz)
{
	NCollisionObjectID::SetDimensions(Nx, Ny, Nz);
	const size_t Size = size_t(ColsNum) * RowsNum * LevelsNum;
	InfoVec.resize(Size);
	InfoVec.assign(Size, nullptr);
}

void SCollisionObjectID::ClearSolids()
{
	for each (auto El in GetInfoVec())
	{
		if (El == nullptr)
			continue;
		delete El->GetSolidObject();
		delete El->GetShape();
		delete El;
	}
}
