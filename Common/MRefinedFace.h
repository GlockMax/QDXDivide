// MRefinedFace.h: interface for the MRefinedFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BREFINEDFACE_H__FF480702_081B_11D7_B6FF_0050BF5C3BA4__INCLUDED_)
#define AFX_BREFINEDFACE_H__FF480702_081B_11D7_B6FF_0050BF5C3BA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MFace.h"

// грань допускающая уточнение
class AFX_EXT_CLASS MRefinedFace : public MFace
{
public:
		
	void Draw();
	void Draw(Text *glText, CString str, bool bDrawText);

	MRefinedFace& operator =(const MRefinedFace &sfc);

	MRefinedFace();
	MRefinedFace(int i_n, int i_b);
	MRefinedFace(BPoint n, BPoint b);
	MRefinedFace(const MRefinedFace &sf);
	virtual ~MRefinedFace();
	// уточнить грань
	void RefinedFace(MBSPTree *pTree, MBSPNode *pNode);
	void RefinedFace1(MBSPTree *pTree, MBSPNode *pNode, bool bNeedTest, int &count);
	MBSPNode * RefinedFace2(MBSPTree *pTree, MBSPNode *pRoot, MBSPNode *pNode, bool bLeft);
	void SetState(bool f);
	int GetRefInd(int index);
	void SetRefPoint(int index, int n);
	//void SetRefPoint(int index, const BPoint&p);
	void SetRefContour(int it0, int it1, int it2, int it3, int it4, int it5);

protected:
	// уточнить грань
	bool LocRefinedFace(MBSPTree *pTree, MBSPNode *pNode);
	void RefContFace(MBSPTree *pTree, MBSPNode *pNode);
	void RefNotContFace(MBSPTree *pTree, MBSPNode *pNode);
	bool m_IsRefined;	// флаг - выполнено ли уточнение или нет
	int  m_pts[6];		// номера точек используемых для уточнения
	BPoint m_dir;		// вектор направления
public:
	// определить состояние
	bool GetState(void);
};

#endif // !defined(AFX_BREFINEDFACE_H__FF480702_081B_11D7_B6FF_0050BF5C3BA4__INCLUDED_)
