// NTrimPiece.h: interface for the NTrimPiece class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTRIMPIECE_H__A1998FA0_35A1_11D5_AB10_008048A96FFB__INCLUDED_)
#define AFX_NTRIMPIECE_H__A1998FA0_35A1_11D5_AB10_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NSquarePiece.h"
#include "NUVPoint.h"	// Added by ClassView

class NTrimPiece : public NSquarePiece  
{
public:
	int AddUVPoint(NUVPoint uv);
	bool RemoveUVPoint(int index);
	void ClearUVPoint();

public:
	void Draw();
	NTrimPiece();
	virtual ~NTrimPiece();

protected:
	int num;
	UVPointArray uvps;
};

#endif // !defined(AFX_NTRIMPIECE_H__A1998FA0_35A1_11D5_AB10_008048A96FFB__INCLUDED_)
