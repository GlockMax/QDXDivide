#pragma once
#include "mbspmerge.h"

class AFX_EXT_CLASS MBSPSubtr :
	public MBSPMerge
{
public:
	MBSPSubtr(void);
	~MBSPSubtr(void);
	MERGERESULT MergeCells(MBSPNode*& pV1, MBSPNode* pV2) const override;
};
