#pragma once
#include <list>
#include <vector>
#include "BBox.h"
#include "NParamMesh.h"
#include "BStockGrID.h"

typedef CTypedPtrArray <CPtrArray, NParamMesh *> BMeshArr;
class BSP_MODEL_API MeshArr: public BMeshArr
{
public:
	MeshArr::MeshArr(void) : BMeshArr() { Convex = false; }
	bool IsConvex() const { return Convex; }
	void SetConvex(bool Conv) { Convex = Conv; }
	auto& GetStockGroups() const { return StockGroups; }
	void SetStockPos(const std::vector<BStockGrID>& Groups) { StockGroups = Groups; }
	int GetSize(void) const { return int(BMeshArr::GetSize()); }
	const BBox GetGabar() const
	{
		BBox Gabar;
		for (int i = 0; i < GetSize(); ++i)
		{
			auto pMesh = GetAt(i);
			if (pMesh)
				pMesh->Expand(Gabar);
		}
		return Gabar;
	}

	void SetFastColor()
	{
		int FastColNum = FAST_COLOR_NUM;
		for (int j = 0; j < GetSize(); ++j)
		{
			NParamMesh *pMesh = GetAt(j);
			if (!pMesh)
				continue;
			ElemID ElID = pMesh->GetMeshID();
			MElemIDProc::SetPCn(&ElID, FastColNum);
			pMesh->SetMeshID(ElID);
		}
	}

	MeshArr * CopyTransform(const BMatr &M) const;
	void CrMesh4Turn(const std::list<class ClrPoint>& Contour, int CadrNum);

protected:
	bool Convex;
	std::vector<BStockGrID> StockGroups;
};
typedef CTypedPtrArray <CPtrArray, MeshArr *> BMeshsArr;
class BSP_MODEL_API MeshsArr: public BMeshsArr
{
public:
	MeshsArr( int Size )
	{
		SetSize(Size);
		for (int i = 0; i < Size; ++i)
			SetAt(i, new MeshArr);
	}
	MeshsArr(void) {}
	int GetSize(void) const { return int(BMeshsArr::GetSize());}
	void SetConvex(bool Conv) { for (int i = 0; i < GetSize(); ++i) GetAt(i)->SetConvex(Conv); }
	const BBox GetGabar() const
	{
		BBox Gabar;
		for (int i = 0; i < GetSize(); ++i)
		{
			auto pMeshs = GetAt(i);
			if (pMeshs)
				Gabar.Expand(pMeshs->GetGabar());
		}
		return Gabar;
	}
};

