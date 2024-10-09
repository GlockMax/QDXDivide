#pragma once
#include "MPlane.h"
#include "BPoint.h"
#include "MFace.h"
#include "BSPEnums.h"

const int MAX_DIV_LEN = 1024;

class MDivideTree
{

	class Start
	{
		friend class MDivideTree;
	public:
		Start();
		~Start();
	protected:
		MBSPNode **ppS;
		MBSPNode *Type;
	public:

		void Check(MBSPNode **GrPoint, MBSPNode * pIn);
		void CheckEnd(MBSPNode * pIn);
		void Init(MBSPNode ** InitPtr);
	};
public:
	~MDivideTree(void);
protected:
	BPoint P;// Base point of the plane
	BPoint N;// Normal vector 
public:
	double CalcSign(BPoint & P);
protected:
	// The root of the tree to be divided
	MBSPNode *pRoot;
	// The growing-point of the left part
	MBSPNode **pV1;
	// The growing-point of the right part
	MBSPNode **pV2;
	// Dividing face
	MFace DFace;
	MFace F1, F2;
public:
	MDivideTree(const MFace * pF, MBSPNode * pR, MBSPNode *&p1, MBSPNode *&p2);
	void Init(const BPoint & P, const BPoint & N);
	void Init(const MPlane & Plane);
	M_POSITION DoDivide(void);
protected:
	// Positive chain start
	Start Start1;
	// Negative chain start
	Start Start2;
public:
	int BUF;// Is used for surf priority only
	bool DetPosition(const MFace & F, M_POSITION & Pos12, M_POSITION & Pos21, int& temp1, int& temp);
	bool CrParts(const MPlane &Pl, MFace *& pA1, MFace *& pA2, int temp1, int temp);
	void Reset(const MFace * pF, MBSPNode * pR, MBSPNode *&p1, MBSPNode *&p2);
	void OutputDebug() const;
};
