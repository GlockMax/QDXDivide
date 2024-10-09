#include "stdafx.h"
#include "NTiParams.h"
#include "NCollidableObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NColor NCollidableObject::CollidedColor(0.9f, 0.f, 0.f, 1.f);

NCollidableObject::NCollidableObject()
{
	Collided = false;
}

NCollidableObject::~NCollidableObject(void)
{
}

const NColor& NCollidableObject::GetCollidedColor()
{
	return CollidedColor;
}

void NCollidableObject::SetDefaults(NTiParams& Par)
{
	CString Key(_T("Defaults/View/Collision/"));
	CollidedColor.R() = float(Par.GetDouble((Key + "Color@R").GetBuffer(), 0.9));
	CollidedColor.G() = float(Par.GetDouble((Key + "Color@G").GetBuffer(), 0.));
	CollidedColor.B() = float(Par.GetDouble((Key + "Color@B").GetBuffer(), 0.));
	CollidedColor.A() = float(Par.GetDouble((Key + "Color@A").GetBuffer(), 1.));
}
