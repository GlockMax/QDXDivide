#include "stdafx.h"
#include "NCEnums.h"
#include "NCollidableObject.h"
#include "NCollisionID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCollisionID::NCollisionID(ProgTPoint HistC, const NCollisionObjectID * pIDObj1, const NCollisionObjectID * pIDObj2, CollLogElemType inType)
{
	PTPoint = HistC;
	if(pIDObj1)
		IDObj1 = *pIDObj1;
	if(pIDObj2)
		IDObj2 = *pIDObj2;
	CollType = inType;
}

NCollisionID::NCollisionID(void)
{
}

NCollisionID::~NCollisionID(void)
{
}

bool NCollisionID::HasObj(void) const
{
	return (IDObj1.GetGroup() != OG_GR_EMPTY || IDObj2.GetGroup() != OG_GR_EMPTY);
}

bool NCollisionID::HasStock() const
{
	return (CollType == LE_COLLISION) && (IDObj1.GetGroup() == OG_STOCKS || IDObj2.GetGroup() == OG_STOCKS);
}

bool NCollisionID::NeedPlaceInTimeLine() const
{
	if(CollType == LE_COLLISION || CollType == LE_OUTOFRANGE || CollType == LE_NOCOLL)
		return true;
	return false;
}

bool NCollisionID::IsReason4Pause() const
{
	if (CollType == LE_NOCOLL)
		return false;
	return true;
}

bool NCollisionID::IsEquivalent(const NCollisionID* prev, const NCollisionID* next) const
{
	const auto nextCollType = next == nullptr ? LE_NOTDEFINED : next->CollType;
	const auto prevCollType = prev->CollType;
	return CollType == LE_NOCOLL && prevCollType == LE_NOCOLL && nextCollType == LE_NOCOLL 
		|| ((prevCollType == CollType && nextCollType == CollType && prev->IDObj1.GetSelfNum() == IDObj1.GetSelfNum() && prev->IDObj2.GetSelfNum() == IDObj2.GetSelfNum())
			&& ((next == nullptr) ? false : (next->IDObj1.GetSelfNum() == IDObj1.GetSelfNum() && next->IDObj2.GetSelfNum() == IDObj2.GetSelfNum())))
		|| prev->PTPoint.GetTime() == PTPoint.GetTime();
}
