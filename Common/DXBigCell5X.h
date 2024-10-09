#pragma once
#include <vector>
#include "DXEdge.h"
#include "DXBound.h"
#include "BPoint.h"
#include "DXFinalCycle.h"
#include "RTriRandMesh.h"

class DXBigCell5X
{
public:
	static const int MAX_FinalCycles = 5;
	class GroupData
	{
	public:
		GroupData(void) { PrevPCNum = -1;}
		DXFinalCycle FinalCycles[DXBigCell5X::MAX_FinalCycles]; // Temporary array for MakeFinalCycle
		DXFinalCycle &FinalCycle = FinalCycles[0]; // Temporary array for MakeFinalCycle
		int PrevPCNum; // for StoreNPC
		PackedColor PrevColor; // for StoreNPC
		static BPoint MinPointShift;
		static double StepX;
		static double StepY;
		static double StepZ;
		static const class DXMemoryPt *pActMemory;
	};
public:
	DXBigCell5X();
	DXBigCell5X(const DXBigCell5X &inBigCell5X);
	~DXBigCell5X();
	RTriRandMesh &GetTriangleArea(void) { return TriangleArea; }
	void DrawSharpEdges(const NColor& EdgesColor, double EdgesWidth) const;
	GroupData *GetGData(void) { return &Data; }
	void Transform(const BMatr &Tr) { TriangleArea.Transform(Tr); }
	const auto& GetSharpEdgesIndexes() const { return SharpEdgesIndexes; }
	const auto& GetSharpEdges() const { return SharpEdges; }
	auto& GetSharpEdges() { return SharpEdges; }
	std::vector <DXEdge> &GetMapEdges() { return MapEdges; }
	std::vector <DXBound> &GetMapBounds() { return MapBounds; }	
	bool AddSharpEdge(int ip0, int ip1, class DXSharpEdge& SharpEdge);
	bool AddSharpEdge(int ip0, const int ip1[2], class DXSharpEdge& SharpEdge);
	bool AddSharpEdge(const int ip0[2], const int ip1[2], class DXSharpEdge& SharpEdge);
	bool CheckEdgeAngle(const int ip[2]) const;
	void AddSharpInds2Bounds(const DXSharpEdge& SharpEdge);
	UINT GetAnotherSEdge(UINT BoundsInd, UINT SharpInd) const;

protected:
	RTriRandMesh TriangleArea;
	std::vector<unsigned int> SharpEdgesIndexes;
	std::vector<DXSharpEdge> SharpEdges;
	std::vector <DXEdge> MapEdges;
	std::vector <DXBound> MapBounds;	
	GroupData Data;
};

