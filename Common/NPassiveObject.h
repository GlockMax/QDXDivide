#pragma once
#include "NCMB_API.h"
#include "BEdgesAttrs.h"
#include "NCollidableObject.h"
#include "BBox.h"
#include "nobject.h"

class AFX_EXT_CLASS NPassiveObject : public NObject, public NCollidableObject // only this order can work!!!
{
public:
	NPassiveObject(void);
	virtual ~NPassiveObject(void);
	void SetGroup(PObjectGroup Gr);
	void Changed();
	void Deleted();
	virtual void SetEdgesAttrs(const class BEdgesAttrs &Attr);
	const BEdgesAttrs &GetEdgesAttrs() const { return EdgesAttrs; }
protected:
	void SetPassiveGroup(PObjectGroup Gr);
protected:
	BEdgesAttrs EdgesAttrs;
};
