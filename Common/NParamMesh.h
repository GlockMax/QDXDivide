#pragma once
#include "MBSPForest.h"
#include "MElemIDProc.h"
#include "CadrID.h"

class MBody;
class BBox;

class AFX_EXT_CLASS NParamMesh
{
public: 
	NParamMesh(void);
	virtual ~NParamMesh(void);
	virtual void StoreMesh(MBody &Body, ElemID ElID, cadrID CaID, bool ToAux) const = 0;
	virtual void StoreMesh2Aux(class DXSubModel5X &Mod5, ElemID ElID, cadrID CaID) const = 0;
	ElemID GetMeshID() const { return MeshID; };
	void SetMeshID(ElemID ID){ MeshID = ID;}
	virtual const BBox & Expand(BBox & Gab) const = 0;
	virtual NParamMesh *CopyTransform(class CopyMap &Map) const = 0;
	void FillCopy(NParamMesh &Mesh, CopyMap &Map) const;


public:
	int	m_CadrNum;
	int	m_iSurf;
	int	m_sgn;
	bool CuttingTool;
	bool Shank;

protected:
	ElemID MeshID;
};
