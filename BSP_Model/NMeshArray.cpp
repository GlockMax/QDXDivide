#include "StdAfx.h"
#include <list>
#include "ClrPoint.h"
#include "CopyMap.h"
#include "NRectMesh.h"
#include "TBladeCont.h"
#include "NMeshArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MeshArr * MeshArr::CopyTransform(const BMatr & M) const
{
	CopyMap CMap(M);
	MeshArr *pNewMeshArr = new MeshArr;
	pNewMeshArr->SetSize(GetSize());
	for (int i = 0; i < GetSize(); ++i)
	{
		auto pEl = GetAt(i);
		if (pEl != nullptr)
			pNewMeshArr->SetAt(i, pEl->CopyTransform(CMap));
	}
	return pNewMeshArr;
}

void MeshArr::CrMesh4Turn(const std::list<class ClrPoint>& Contour, int CadrNum)
{
	// CadrNum < 0 означает создание заготовки и, следовательно, необходимость
	// устанавливать противоположные направления нормалей к поверхностям	
	if (Contour.size() < 3)
		return;
	BMatr Rot90;
	Rot90.rotg(-90., BPoint(0., 0., 0., 1.), BPoint(0., 0., 1., 0.));// Matrix to get external normal
	// Create meshs for one tool
	BPoint N2 = BPoint(0., 0., 2., 0.);	
	NRectMesh* pPrevMesh = nullptr;
	for (auto itn = Contour.begin(), it = itn++; itn != Contour.end(); ++itn, ++it)
	{
		TBladeEdge E0;
		E0.SetPoint(*it);
		TBladeEdge E1;
		E1.SetPoint(*itn);
		E0.SetNext(&E1);
		PackedColor Col;
		Col = it->GetColor();
		const int ColorNum = MElemIDProc::AddPC(Col) + RESERVED_PCS;
		NRectMesh* pMesh = TBladeCont::CrMesh4OneElem(Rot90, ColorNum, ColorNum, CadrNum, &E0, pPrevMesh, N2, N2 * 0.5);
		pPrevMesh = pMesh;
		Add(pMesh);
	}
	SetConvex(false);
}
