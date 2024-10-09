#pragma once

#include "NObjEnums.h"

class AFX_EXT_CLASS NOMScaner
{
public:
	NOMScaner(class BObjectManager *pOM, class NActiveObject *pObj);
	~NOMScaner(void);
	void Next(void);
	bool IsEnded(void);
protected:
	class NPassiveObject *pPassiveObject;
	PObjectGroup PassiveGroup;
	unsigned int ActiveInd;
	int PassiveInd;
	unsigned int RelationInd;
	class BObjectManager *pObjectManager;
	enum PObjectStatus Status;
public:
	void SetPassiveObject(class NPassiveObject *pVal){ pPassiveObject = pVal;}
	void SetPassiveObjectGroup(PObjectGroup Group){ PassiveGroup = Group;}
	void SetActiveInd(unsigned int Val){ ActiveInd = Val;}
	void SetPassiveInd(int Val){ PassiveInd = Val;}
	void SetRelationInd(unsigned int Val){ RelationInd = Val;}
	void SetStatus(PObjectStatus Val){ Status = Val;}
	class NPassiveObject *GetPassiveObject(void){ return pPassiveObject;}
	unsigned int GetRelationInd(void){ return RelationInd;}
	unsigned int GetActiveInd(void){ return ActiveInd;}
	int GetPassiveInd(void){ return PassiveInd;}
	PObjectStatus GetStatus(void){ return Status;}
	PObjectGroup GetPassiveGroup(void){ return PassiveGroup;}
};
