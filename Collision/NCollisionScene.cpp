#include "StdAfx.h"
#include "SCollisionSceneSolid.h"
#include "NCollisionID.h"
#include "NCollisionScene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCollisionScene::NCollisionScene()
{
	ActiveCollisionsEnabled = false;
	FixStock = true;
	Diskr = 0;
}

NCollisionScene::~NCollisionScene(void)
{
}

NCollisionScene *NCollisionScene::GenCollisionScene(void* pCollMan, ResponseCallback RespCallBack)
{
	return new SCollisionSceneSolid(pCollMan, (DT_ResponseCallback)RespCallBack);
}

int NCollisionScene::AddSensitivePair(int IDIndex1, int IDIndex2)
{
	const int Res1 = Add1SensitivePair(IDIndex1, IDIndex2);
	const int Res2 = Add1SensitivePair(IDIndex1 + 1, IDIndex2);
	const int Res3 = Add1SensitivePair(IDIndex1, IDIndex2 + 1);
	const int Res4 = Add1SensitivePair(IDIndex1 + 2, IDIndex2);
	const int Res5 = Add1SensitivePair(IDIndex1, IDIndex2 + 2);
	const int Res6 = Add1SensitivePair(IDIndex1 + 2, IDIndex2 + 2);
	return Res1 * Res2 * Res3 * Res4 * Res5 * Res6;
}

int NCollisionScene::AddActiveCollision(const NCollisionID &CollID)
{
	if(ActiveCollisionsEnabled)
	{
		bool F1 = CollID.IDObj1.GetSelfNum() >= 0;
		if (F1)
			ActiveCollisions.insert(CollID.IDObj1.GetSelfNum());
		bool F2 = CollID.IDObj2.GetSelfNum() >= 0;
		if (F2)
			ActiveCollisions.insert(CollID.IDObj2.GetSelfNum());
		// 1 - have non stock collision, 2 - have stock collision, 3 - both	
		if (CollID.CollType == LE_COLLISION)
		{
			int LocDiskr = 0;
			if (F1)
				if (CollID.IDObj1.GetGroup() == OG_STOCKS)
					LocDiskr |= 2;
			if (LocDiskr == 0 && F2)
				if (CollID.IDObj2.GetGroup() == OG_STOCKS)
					LocDiskr |= 2;
			if (LocDiskr == 0)
				LocDiskr = 1;
			Diskr |= LocDiskr;
		}
	}
	return 0;
}

