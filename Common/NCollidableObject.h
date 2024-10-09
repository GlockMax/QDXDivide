#pragma once
#include "NCMB_API.h"
#include "NColor.h"

class NCMB_API NCollidableObject
{
public:
	NCollidableObject(void);
	virtual ~NCollidableObject(void);
	void SetCollided(bool HL) { Collided = HL;}
	bool IsCollided(void) const { return Collided;}
	static const NColor& GetCollidedColor();
	static void SetDefaults(class NTiParams& Par);
protected:
	bool Collided;
	static NColor CollidedColor;
};
