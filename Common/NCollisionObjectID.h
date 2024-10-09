#pragma once

#ifdef COLLISION_EXPORTS
#define COLLISION_API __declspec(dllexport)
#else
#define COLLISION_API __declspec(dllimport)
#endif

#include "NObjEnums.h"

class COLLISION_API NCollisionObjectID
{
public:
	NCollisionObjectID(void);
	~NCollisionObjectID(void);
	void SetGroup(PObjectGroup Gr) { Group = Gr;}
	void SetID(int id) { ID = id; }
	int GetID(void) const { return ID; }
	void SetBehav(int Val) { Behav = Val; }
	int GetBehav(void) const { return Behav; }
	void SetSelfNum(int Num) { SelfNum = Num;}
	int GetSelfNum(void) const { return SelfNum;}
	PObjectGroup GetGroup(void) const { return Group;}
	void SetCollidableObj(class NCollidableObject* pCollObject) { pCollidableObj = pCollObject;}
	NCollidableObject* GetCollidableObj(void) const { return pCollidableObj;}
	size_t GetSpinRendVersion() const { return SpinRendVersion; }
	void SetSpinRendVersion(size_t Val) { SpinRendVersion = Val; }
	virtual void SetDimensions(int Nx, int Ny, int Nz);
	size_t MakeKey(int i, int j, int k) const noexcept;
	bool operator == (const NCollisionObjectID& tst) const;
protected:
	PObjectGroup Group;
	int ID; // The number of the corresponding axis in the machine tool
	int Behav; // Index of the group with the same behavior with respect to collision (moving axis index << 2 + stock(0/1) << 1 + cutting tool(0/1)
	int SelfNum; // The number of this object in the NCollisionScene
	class NCollidableObject* pCollidableObj;
	size_t SpinRendVersion;
	int ColsNum;
	int RowsNum;
	int LevelsNum;
public:
	static constexpr int IndStep = 16;
public:
	static int MakeBehav(int AxisInd, bool Stock, bool CutTool);
	bool IsBehavCutting() const;
	void MakeBehavCutting(bool Cutting);
};
