#pragma once
#include "mbspmerge.h"

class AFX_EXT_CLASS MBSPInter :
	public MBSPMerge
{
public:
	MBSPInter(void);
	~MBSPInter(void);
	MERGERESULT MergeCells(MBSPNode *& pV1, MBSPNode * pV2) const override;

};
