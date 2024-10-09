// MDisjoinFace.h: interface for the MDisjoinFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDISJOINFACE_H__AE91E391_0B60_11D7_B70C_0050BF5C3BA4__INCLUDED_)
#define AFX_BDISJOINFACE_H__AE91E391_0B60_11D7_B70C_0050BF5C3BA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MFace.h"

// грань между кадрами программы
class AFX_EXT_CLASS MDisjoinFace : public MFace  
{
public:
	MDisjoinFace(BPoint n, BPoint b);
	int AddFollow(MFace *fc);
	int AddPrev(MFace *fc);
	int AddFirst(MFace *f);
		
	void Draw();
	void Draw(Text *glText, CString str, bool bDrawText);

	MDisjoinFace& operator =(const MDisjoinFace &sfc);

	MDisjoinFace();
	MDisjoinFace(const MDisjoinFace &sf);
	virtual ~MDisjoinFace();
};

#endif // !defined(AFX_BDISJOINFACE_H__AE91E391_0B60_11D7_B70C_0050BF5C3BA4__INCLUDED_)
