#pragma once
#include "OPENGLRENDER_API.h"
#include "BRotSolid.h"
#include "BMatr.h"
#include "BBox.h"
#include <list>

class OPENGLRENDER_API RSpinning
{
public:
	RSpinning();
	~RSpinning();
	RSpinning(const RSpinning&) = delete;
	RSpinning& operator=(const RSpinning&) = delete;
	void CreateRenders();
	void Draw(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool WFrame) const;
	void StorePrevState(const BMatr& RelMatr);
	bool CheckPrevState(const BMatr& RelMatr);
	void MakeTriMesh(const std::list<BClrContour>& Contours, size_t SectorsNum);
	static void StorePoint(float* Dest, const BPoint& P, const BPoint& N);
	const BBox& GetGabar() const { return Gabar; }
	const BPoint RayCasting(const BPoint& P, const BPoint& V, double MinW, BPoint& N) const;
	const float* GetTriangles() { return Triangles; }
	auto& GetContours() { return Contours; }

protected:
	std::list<BClrContour> Contours;
	float* Triangles;
	BBox Gabar;

	BMatr PrevMatr;
};

