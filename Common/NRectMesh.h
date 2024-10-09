#pragma once
#include "nparammesh.h"

class AFX_EXT_CLASS NRectMesh :
	public NParamMesh
{
public:
	NRectMesh(void);
	virtual ~NRectMesh(void);
	void SetRowsNum(int r);
	void SetSize(int r, int c);
	void Draw(bool is_point = false, bool view_num = false, bool view_norm = false);
	CUIntArray* operator [](int r); 
	void JoinEdge(const NRectMesh * pMesh);
	void JoinEdgeRev(const NRectMesh * pMesh);

	CUIntArray	*m_arr;
	int			m_IsDeg;
	int			m_r;
	bool		m_Closed;

	virtual void StoreMesh(MBody & Body, ElemID ElID, cadrID CaID, bool ToAux) const override;
	virtual void StoreMesh2Aux(class DXSubModel5X &Mod5, ElemID ElID, cadrID CaID) const override;
	virtual const BBox & Expand(BBox & Gab) const override;
	float * MakeTriangles(const class BMatr * pMatr = NULL);
	NParamMesh *CopyTransform(class CopyMap &Map) const override;
	void MakeEquid(double Dist);
	BPoint CalcNormal(int i, int j) const;
	NParamMesh* Convert2TriMesh();
	void SetClosed() { m_Closed = true; }
	bool IsClosed() const { return m_Closed; }
};
