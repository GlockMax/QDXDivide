#pragma once

#include "BSPEnums.h"
typedef int MERGERESULT;
const MERGERESULT T1CHANGED = 0x01;
const MERGERESULT T2CHANGED = 0x02;
const MERGERESULT UNCHANGED = 0x00;
const int MAX_STACK_LEN = 10240;

class MBSPNode;

class AFX_EXT_CLASS MBSPMerge
{
	class Stack
	{
	public:
		Stack();
		~Stack();

	protected:
		int Top;
		MBSPNode **M1[MAX_STACK_LEN];
		MBSPNode *M2[MAX_STACK_LEN];

	public:
		bool Push(MBSPNode **p1, MBSPNode *p2);
		bool Pop(MBSPNode **&p1, MBSPNode *&p2);
		void Clear(void);
	};
public:
	MBSPMerge(void);
	~MBSPMerge(void);
	virtual MERGERESULT MergeCells(MBSPNode *& pV1, MBSPNode * pV2) const = 0;
	MERGERESULT Merge(MBSPNode & V1, const MBSPNode & V2) const;
	MERGERESULT CopyMerge(MBSPNode *& pV1, MBSPNode *& pV2) const;
	MERGERESULT SafeMerge(MBSPNode *& pV1, MBSPNode *& pV2) const;
};
