#pragma once
#include "NCMB_API.h"
#include "NColor.h"
class NCMB_API BSelectAttrs
{
public:
	BSelectAttrs();
	BSelectAttrs(class NTiParams& Par);
	~BSelectAttrs();
	void SetMain(const NColor& Col) { Main = Col; }
	void SetBox(const NColor& Col) { Box = Col; }
	void SetEdge(const NColor& Col) { Edge = Col; }
	const NColor& GetMain() const { return Main; }
	const NColor& GetBox() const { return Box; }
	const NColor& GetEdge() const { return Edge; }
	void SetEdgeWidth(double Val) { EdgeWidth = Val; }
	void SetBoxWidth(double Val) { BoxWidth = Val; }
	double GetEdgeWidth() const { return EdgeWidth; }
	double GetBoxWidth() const { return BoxWidth; }

protected:
	NColor Main;
	NColor Box;
	NColor Edge;
	double EdgeWidth;
	double BoxWidth;
};

