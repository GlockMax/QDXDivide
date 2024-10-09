#pragma once
#include "Octree.h"
#include "NToolID.h"
class IDost
{
	friend class OctreeSkelTool;
public:
	IDost() { SetEmpty(); }
	~IDost() {}
	IDost(int inToolInd, int inCadrInd, bool inActive) { ToolInd = inToolInd; CadrInd = inCadrInd; Active = inActive; }
	bool IsEmpty() const { return CadrInd < 0; }
	void SetEmpty() { ToolInd = -1; CadrInd = -1; Active = false; }
	int GetCadrInd() const { return CadrInd; }
	int GetToolInd() const { return ToolInd; }
	bool GetActive() const { return Active; }
protected:
	int ToolInd;
	int CadrInd;
	bool Active;
};
class OctreeSkelTool : public Octree<IDost>
{
public:
	OctreeSkelTool();
	virtual ~OctreeSkelTool();
	void Initialize(class CNCMDoc* pDoc, bool StockView);
protected:
	const NCadrGeom* GetCadrGeom(IDost ID, int StPos) override;
};

