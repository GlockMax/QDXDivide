// MTPartArray.h: interface for the MTPartArray class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "StdAfx.h"
#include "BBox.h"
#include <array>
#include <vector>

class MTPart;

/// Список деталей
/**
\sa MTPart
*/
class AFX_EXT_CLASS MTPartArray 
{
	friend class MTMachine;
public:
	BBox GetGabar();
	MTPart* operator [] (int i) const;
	void Draw(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSharpEdges = true, bool DrawSurf = true,  bool DrawSmoothSurf = true);
	bool HasTranslucent(void);
	void RemoveAll();
	int GetSize() const;
	void RemovePart(int n);
	void AddPart(MTPart *NP);
	MTPartArray(const class MTMachine& MT);
	typedef CTypedPtrArray<CObArray,MTPart*> PartArray;
	
	virtual ~MTPartArray();

	/// Установка выделяемой при отображении детали.
	/** Если не нужно ничего выделять, то передать -1 */
	void SelectPart(int PartIndex);
	void SelectPart(const CString& PartName);
	int GetSelected() const;
protected:
	const class MTMachine& MT;
	PartArray Part;
	int Size;
	/// Номер детали, которая при отображении будет выделяться габаритным параллелепипедом
	int SelectedPart;
public:
	const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N, class RPickID& PickID) const;
    bool FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrs);
};

