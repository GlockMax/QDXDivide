#include "stdafx.h"
#include "NCMDoc.h"
#include "MTMashineTool.h"
#include "OctreeSkelTool.h"

OctreeSkelTool::OctreeSkelTool()
{
}

OctreeSkelTool::~OctreeSkelTool()
{
}

void OctreeSkelTool::Initialize(CNCMDoc* pDoc, bool StockView)
{
	const auto& ToolList = *pDoc->NGetToolList();
	if (ToolList.empty())
		return;
	MTMachineTool& Mach = pDoc->GetMachTool();
	Mach.MakeActual();
	BBox sceneAABB;
	std::list<BBox> BoxList;
	std::list<IDost> OstList;
	for (int i = 0; i < ToolList.GetSize(); ++i)
	{
		NToolCombined* pComTool = nullptr;
		CString key;
		ToolList.GetAt(i, key, (CObject*&)pComTool);
		if (pComTool == nullptr)
			continue;
		BMatr Matr;
		bool ToolFound = pDoc->GetFullToolMatr(pComTool, StockView, Matr);
		if (!ToolFound)
			continue;
		pComTool->RemakeSkeleton();
		const BGeomArray& GeomArray = pComTool->GetSkeleton();
		for (int cadrNum = 0; cadrNum < GeomArray.GetSize(); cadrNum++)
		{
			NCadrGeom cCadr(*GeomArray[cadrNum]);
			cCadr.Tr(Matr);
			BoxList.emplace_back();
			cCadr.ExpandBox(&BoxList.back());
			sceneAABB.Expand(BoxList.back());
			OstList.emplace_back(i, cadrNum, false);
		}
	}
	Octree::Initialize(sceneAABB, pDoc);
	auto itID = OstList.begin();
	for (auto it = BoxList.begin(); it != BoxList.end(); ++it, ++itID)
		InsertObject(*itID, *it);
}

const NCadrGeom* OctreeSkelTool::GetCadrGeom(IDost ID, int StPos)
{
	return pDoc->GetCadrGeomIDost(ID);
}
