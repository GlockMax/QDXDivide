#pragma once
#include <vector>
#include "nparammesh.h"

typedef int TriElem[3];

class AFX_EXT_CLASS NTriMesh :
	public NParamMesh
{
	friend class MVRDS;

	class TriElem 
	{
		int TE[3];
	public:
		int operator[] (int i) const { return TE[i]; }
		int & operator[] (int i) { return TE[i]; }
	};
public:
	NTriMesh(void);
	virtual ~NTriMesh(void);
	virtual void StoreMesh(MBody & Body, ElemID ElID, cadrID CaID, bool ToAux) const override;
	virtual void StoreMesh2Aux(class DXSubModel5X &Mod5, ElemID ElID, cadrID CaID) const override;
	virtual const BBox & Expand(BBox & Gab) const override;

	int AddTri(int i0, int i1, int i2);
	void Invert(void);
	void SetSize(INT_PTR nNewSize) { Triangles.resize(nNewSize); }
	void Reserve(INT_PTR nNewSize) { Triangles.reserve(nNewSize); }
	NParamMesh *CopyTransform(class CopyMap &Map) const override;
protected:
	std::vector <TriElem> Triangles;
};
