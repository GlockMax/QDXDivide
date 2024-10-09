#pragma once
#include "mbspmerge.h"

class AFX_EXT_CLASS MBSPUnion :
	public MBSPMerge
{
public:
	MBSPUnion(void);
	~MBSPUnion(void);
	MERGERESULT MergeCells(MBSPNode*& pV1, MBSPNode* pV2) const override;
};
