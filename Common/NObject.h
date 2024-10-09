#pragma once
#include "BSelectAttrs.h"

#define P_OBJECT_MANAGER BObjectManager *pBUF_OM = GetObjectManager();if(pBUF_OM)pBUF_OM
#define C_OBJECT_MANAGER(CLASS) BObjectManager *pBUF_OM = CLASS.GetObjectManager();if(pBUF_OM)pBUF_OM

#include "NObjEnums.h"

class AFX_EXT_CLASS NObject : public CObject
{
public:
	NObject();
	virtual ~NObject();
	DECLARE_SERIAL(NObject)
public:
	const class BObjectManager *GetObjectManager() const;
	void Serialize(CArchive & ar) override;
	BObjectManager *GetObjectManager();
	PObjectGroup GetGroup() const { return Group; }
//	virtual void SetGroup(PObjectGroup Gr) { Group = Gr; }
	static void SetSelectAttrs(BSelectAttrs SAttrs);
	const BSelectAttrs& GetSelectAttrs() const;
protected:
	PObjectGroup Group;
	static BSelectAttrs SelectAttrs;
};


