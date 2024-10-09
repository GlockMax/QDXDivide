#pragma once
#include "MPlane.h"
#include "BPoint.h"
#include "MFace.h"
#include "BSPEnums.h"
#include "MElemIDProc.h"
#include "ppl.h"
#include "mstack.h"
#include "CadrID.h"

class MInsertFace
{
protected:
	// The root of the tree
	MBSPNode **pRoot;
	// Face to be inserted
	const MFace *pFace;
	// The number of the face to be inserted
	int nFace;
	const ElemID ElementID;
	const cadrID HistID;
public:
	~MInsertFace(void);
protected:
	thread_local static MStack Stack;
	MInsertFace(void);
	M_POSITION DetPosition(MFace* pP1, MFace* pP2);
	int Insert(void);
public:
	MInsertFace(MBSPNode ** pR, int iF, int ID, cadrID cID);
	int DoInsert(void);
	void Init(void);
	void PlacePriElemID(MBSPNode * pNode);
};
