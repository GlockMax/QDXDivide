#pragma once

#ifdef COLLISION_EXPORTS
#define COLLISION_API __declspec(dllexport)
#else
#define COLLISION_API __declspec(dllimport)
#endif

#include "cadrID.h"
#include "NCollisionObjectID.h"

class COLLISION_API NCollisionID
{
public:
	NCollisionID(ProgTPoint PTPoint, const NCollisionObjectID * pIDObj1, const NCollisionObjectID * pIDObj2, enum CollLogElemType inType);
	NCollisionID(void);
	~NCollisionID(void);
	bool HasObj(void) const;
	bool HasStock() const;
	bool NeedPlaceInTimeLine() const;
	bool IsReason4Pause() const;
	bool IsEquivalent(const NCollisionID* prev, const NCollisionID* next) const;
	ProgTPoint PTPoint;
	NCollisionObjectID IDObj1;
	NCollisionObjectID IDObj2;
	CollLogElemType CollType;
};
